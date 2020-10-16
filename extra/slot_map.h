#pragma once
/**********************************************************************
    class: SlotMap

    author: S. Hau
    date: December 31, 2018

    Similar to a std::map but about ~10-100x faster.
    [+] Data stored contigously in memory.
    [+] Fast. O(1) insert, erase and lookup.
       Insert: 10'000'000 elements: 0.350s  | std::map: 2.077s
       Lookup: 10'000'000 elements: 0.007s  | std::map: 0.775s
       Erase : 10'000'000 elements: 0.096s  | std::map: 1.523s
    [-] Additional jump in memory and potential cache miss when accessing elements.
    [+-] 8 - 12 additional bytes per entry. (4 for erase-table and 4 - 8 for index-table)

    In general the slot-map can be used whenever a collection of objects
    is needed and these can be deleted, created, retrieved in an arbitrary order.
    GameObjects for example would be a good use case for this.
    Another point of view is whenever std::vector is not sufficient, consider the slot-map ;)

    Inner workings:
     It works by using an handle as a key for every type of object in the slot-map.
     The key itself encodes an index and a generation counter (to fight the ABA problem).
     The index is used to index into an array, which contains the real data-index.
     Apart from that there is another array needed for each entry (m_Erase) in order to
     keep the data contigously in memory whenever an element gets erased.

     For a better and visual description see https://www.youtube.com/watch?v=SHaAR7XPtNU&t=1s
**********************************************************************/

// Commit 0de987d

/*      using Key = int;

        SlotMap<int, Key, 24, 8> slotMap;
        slotMap.reserve(loopCount + 1);

        std::vector<Key> keys(loopCount + 1);

        printf("Adding %d elements...\n", loopCount);
        {
            AutoClock clock;
            for (int i = 0; i < loopCount; i++)
                keys[i] = slotMap.push_back(i);
        }

        printf("Retrieving %d elements...\n", loopCount);
        {
            AutoClock clock;
            uint64_t sum = 0;
            for (int i = 0; i < loopCount; i++)
                sum += *slotMap[keys[i]];
        }

        printf("Removing %d elements...\n", loopCount);
        {
            AutoClock clock;
            for (int i = 0; i < loopCount; i++)
                slotMap.erase(keys[i]);
        }
    }    */

#include <type_traits>
#include <cstring>
#include <assert.h>

// Slot-Map data-structure with O(1) insert, erase and fast iteration/lookup
// @Value: Type of data contained in this slot map.
// @Key: Type is used as a key and contains the Index for the data array and a generation counter (to solve ABA problem)
// @INDEX_BIT_COUNT: Number of bits for the index into the index map. This specifies an upper limit for the entry count.
// @GENERATION_BIT_COUNT: Number of bits for the generation. If this value is too low, ABA problem is more likely to happen.
// @INVALID_KEY: It is guaranteed that this key will never be generated and can safely be used to represent something like INVALID.
// @SizeType: Type for the erase-table. The slot-map can not contain more entries than the number of elements this number can represent.
// NOTE: INDEX_BIT_COUNT + GENERATION_BIT_COUNT MUST be equal to the amount of bits in Key. (Ensured by static assert)
template<typename Value, typename Key = uint64_t, unsigned int INDEX_BIT_COUNT = 32, unsigned int GENERATION_BIT_COUNT = 32, Key INVALID_KEY = ~0, typename SizeType = unsigned int>
class SlotMap
{
    // Compile time pow function
    template<typename T>
    static inline constexpr T POW(const T base, const unsigned int exponent)
    {
        return exponent == 0 ? 1 : (base * POW(base, exponent - 1));
    }

public:
    using IndexGenType = Key;
    using GenType      = SizeType;
    using IndexType    = SizeType;

    static const Key INVALID_KEY = INVALID_KEY;

    // Generation | Index (Into index array or data array. Into data array if slot is unused to point to next free slot)
    static const unsigned int GENERATION_BIT_COUNT = GENERATION_BIT_COUNT;
    static const unsigned int INDEX_BIT_COUNT      = INDEX_BIT_COUNT;

    // Masks to retrieve the generation or index from a key
    static const IndexGenType GENERATION_BIT_MASK = POW<IndexGenType>(2, GENERATION_BIT_COUNT) - 1;
    static const IndexGenType INDEX_BIT_MASK      = POW<IndexGenType>(2, INDEX_BIT_COUNT) - 1;

    static_assert(INDEX_BIT_COUNT <= sizeof(IndexType) * 8, "Index bit-count is too high, because SizeType can not represent it. Consider another type for SizeType or decrease the index bit-count.");
    static_assert(GENERATION_BIT_COUNT >= 2, "Generation bit-count is too low. Minimum must be 2!");
    static_assert(INDEX_BIT_COUNT >= 8, "Index bit-count is too low. Minimum must be 8!");
    static_assert((GENERATION_BIT_COUNT + INDEX_BIT_COUNT) == (sizeof(Key) * 8), "Key bit-count MUST match the INDEX_BIT_COUNT + GENERATION_BIT_COUNT!");

    ///////////////////////////////////////////////////////////////////////////
    SlotMap() = default;
    ~SlotMap() { _Destroy(); }

    SlotMap(const SlotMap& other) { _CopyFrom(other); }
    SlotMap& operator= (const SlotMap& other)
    {
        _Destroy();
        _CopyFrom(other);
        return *this;
    }

    SlotMap(SlotMap&& other) { _MoveFrom(std::move(other)); }
    SlotMap& operator= (SlotMap&& other)
    {
        _Destroy();
        _MoveFrom(std::move(other));
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////
    Key push_back(const Value& _Val)
    {
        return emplace_back(_Val);
    }

    Key push_back(Value&& _Val)
    {
        return emplace_back(std::move(_Val));
    }

    template<class... Values>
    Key emplace_back(Values&&... vals)
    {
        // Map is full, allocate more memory
        if (size() == capacity())
            _Grow();

        assert(size() < capacity());

        // Calculate position in "indices"-array
        IndexType index = static_cast<IndexType>(m_Head - m_IndicesAndGeneration);
        assert(index <= INDEX_BIT_MASK);

        Key newKey = static_cast<Key>(index);

        // Set correct generation for the key. (Gen gets incremented in erase())
        GenType latestGen = _GetGeneration(m_IndicesAndGeneration[newKey]);
        _SetGeneration(newKey, latestGen);

        if (newKey == INVALID_KEY) // Simply increase generation by one manually
        {
            latestGen++;
            _SetGeneration(newKey, latestGen);
            _SetGeneration(m_IndicesAndGeneration[newKey], latestGen);
        }

        // Set head to next free available index
        // The index table itself is aliased to contain the next free index
        IndexType nextFreeIndex = _GetIndex(m_IndicesAndGeneration[index]);
        m_Head = &m_IndicesAndGeneration[nextFreeIndex];

        // New index for given data is simply the size of this container
        IndexType dataIndex = static_cast<IndexType>(m_Size);

        // Update the key to contain the right index
        _SetIndex(m_IndicesAndGeneration[index], dataIndex);

        new (std::addressof(m_Data[dataIndex])) Value(std::forward<Values>(vals)...);
        m_Erase[dataIndex] = index;

        m_Size++;

        return newKey;
    }

    ///////////////////////////////////////////////////////////////////////////
    bool erase(Key key)
    {
        IndexType index = _GetIndex(key);

        // Check if generation matches. If not element was already deleted or key is invalid
        GenType currentGen = _GetGeneration(m_IndicesAndGeneration[index]);
        GenType gen = _GetGeneration(key);
        if (currentGen != gen)
            return false;

        IndexType dataIndex = _GetIndex(m_IndicesAndGeneration[index]);

        // Mark new index slot as free by setting it as the next head and alias memory to set it to the next free index
        IndexType nextFreeIndex = static_cast<IndexType>(m_Head - m_IndicesAndGeneration);
        _SetIndex(m_IndicesAndGeneration[index], nextFreeIndex);
        m_Head = std::addressof(m_IndicesAndGeneration[index]);

        // Increment generation so key above can not be used twice
        _SetGeneration(m_IndicesAndGeneration[index], currentGen + 1);

        // Deconstruct object if not trivially deconstructable
        _DeconstructObject(m_Data, dataIndex);

        // Move last element into the slot where the element was deleted
        IndexType indexToMove = static_cast<IndexType>(size() - 1);
        _MoveConstructObject(m_Data, dataIndex, m_Data, indexToMove);
        _DeconstructObject(m_Data, indexToMove);

        // Null out bits in last slot (just for error checking)
        memset(std::addressof(m_Data[size() - 1]), 0, sizeof(Value));

        // Update indices-array to point to the new location
        _SetIndex(m_IndicesAndGeneration[m_Erase[indexToMove]], dataIndex);
        m_Erase[dataIndex] = indexToMove;

        // Null out bits in last slot (just for error checking)
        memset(std::addressof(m_Erase[size() - 1]), 0, sizeof(SizeType));

        m_Size--;

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    Value* at(Key key)
    {
        IndexType index = _GetIndex(key);

        assert((SizeType)index < capacity());

        // Check if generation matches
        GenType generation = _GetGeneration(key);
        GenType currentGeneration = _GetGeneration(m_IndicesAndGeneration[index]);
        if (generation != currentGeneration)
            return nullptr;

        // Generation did match, return object
        IndexType dataIndex = _GetIndex(m_IndicesAndGeneration[index]);
        return std::addressof(m_Data[dataIndex]);
    }
    const Value* at(Key key) const { return at(key); }

    Value*       operator[] (Key key)       { return at(key); }
    const Value* operator[] (Key key) const { return at(key); }

    ///////////////////////////////////////////////////////////////////////////
    SizeType     size()     const { return m_Size; }
    SizeType     capacity() const { return m_Capacity; }
    bool         empty()    const { return size() == 0; }

    const Value* data()     const { return m_Data; }
    const Value* begin()    const { return m_Data; }
    const Value* end()      const { return m_Data + m_Size; }

    ///////////////////////////////////////////////////////////////////////////
    void reserve(const SizeType count)
    {
        if (count > m_Capacity)
            _Create(count);
    }

    ///////////////////////////////////////////////////////////////////////////
    void clear()
    {
        for (unsigned int i = 0; i < size(); i++)
            m_Data[i].~Value();
        memset(m_Data, 0, m_Capacity * sizeof(Value));
        memset(m_Erase, 0, m_Capacity * sizeof(SizeType));

        for (unsigned int i = 0; i < m_Capacity; i++)
            m_IndicesAndGeneration[i] = i + 1;

        m_Size = 0;
        m_Head = m_IndicesAndGeneration;
    }

    ///////////////////////////////////////////////////////////////////////////
    void shrink_to_fit()
    {
        assert(m_Capacity >= m_Size);
        if (m_Capacity > m_Size)
        {
            m_Capacity = m_Size;
            m_Size > 0 ? _Create(m_Capacity) : _Destroy();
        }
    }

private:
    // Data is always stored contiguously in memory
    Value*          m_Data = nullptr;

    // Index stored in first N-Bits, generation stored in last M-Bits
    IndexGenType*   m_IndicesAndGeneration = nullptr;

    // Erase table needed to move the last element into a deleted element slot and patch the index table
    SizeType*       m_Erase = nullptr;

    // Points to next free element
    IndexGenType*   m_Head = nullptr;

    // How much memory was allocated in terms of objects
    SizeType        m_Capacity = 0;

    // How many objects this map contains
    SizeType        m_Size = 0;

    //////////////////////////////////////////////////////////////////////////////
    GenType     _GetGeneration(Key val) const { return val >> INDEX_BIT_COUNT; }
    IndexType   _GetIndex(Key val)      const { return val & INDEX_BIT_MASK; }

    void _SetGeneration(IndexGenType& val, GenType gen)
    {
        val &= ~(GENERATION_BIT_MASK << INDEX_BIT_COUNT); // erases all generation bits
        val |= gen << INDEX_BIT_COUNT;
    }

    void _SetIndex(IndexGenType& val, IndexType index)
    {
        val &= ~INDEX_BIT_MASK; // erases all index bits
        val |= index;
    }

    // Grow container
    void _Grow()
    {
        const SizeType oldSize = size();
        const SizeType newSize = oldSize + 1;
        const SizeType newCapacity = _Calculate_growth(newSize);

        _Create(newCapacity);
    }

    // Allocate enough memory for the new capacity and copy data over if any
    void _Create(const SizeType newCapacity)
    {
        assert(newCapacity > 0);
        const SizeType oldCapacity = capacity();

        m_Capacity = newCapacity <= INDEX_BIT_MASK ? newCapacity : INDEX_BIT_MASK;
        {
            Value* newData = (Value*)malloc(newCapacity * sizeof(Value));
            memset(newData, 0, newCapacity * sizeof(Value));

            // Move construct old objects into new mem if move is possible, otherwise copy
            for (unsigned int i = 0; i < size(); i++)
            {
                _MoveConstructObject(newData, i, m_Data, i);
                _DeconstructObject(m_Data, i);
            }

            free(m_Data);
            m_Data = newData;
        }
        {
            IndexGenType* newIndicesAndGen = new IndexGenType[newCapacity];

            if (m_IndicesAndGeneration)
                memcpy(newIndicesAndGen, m_IndicesAndGeneration, oldCapacity * sizeof(IndexGenType));

            for (SizeType i = oldCapacity; i < newCapacity; i++)
                newIndicesAndGen[i] = static_cast<IndexGenType>(i + 1);

            IndexType headIndex = m_Head ? static_cast<IndexType>(m_Head - m_IndicesAndGeneration) : 0;
            m_Head = std::addressof(newIndicesAndGen[headIndex]);

            delete[] m_IndicesAndGeneration;
            m_IndicesAndGeneration = newIndicesAndGen;
        }
        {
            SizeType* erase = new SizeType[newCapacity];
            memset(erase, 0, newCapacity * sizeof(SizeType));

            if (m_Erase)
                memcpy(erase, m_Erase, oldCapacity * sizeof(SizeType));

            delete[] m_Erase;
            m_Erase = erase;
        }
    }

    // From std::vector
    SizeType _Calculate_growth(const SizeType newSize) const
    {   // given oldCapacity and newSize, calculate geometric growth
        const SizeType oldCapacity = capacity();
        if (oldCapacity > (INDEX_BIT_MASK + 1) - oldCapacity / 2)
            return newSize; // geometric growth would overflow

        const SizeType geometric = oldCapacity + oldCapacity / 2;
        if (geometric < newSize)
            return newSize; // geometric growth would be insufficient

        return geometric; // geometric growth is sufficient
    }

    //////////////////////////////////////////////////////////////////////////////
    void _Destroy()
    {
        for (unsigned int i = 0; i < size(); i++)
            _DeconstructObject(m_Data, i),

        free(m_Data);
        delete[] m_IndicesAndGeneration;
        delete[] m_Erase;

        m_Data = nullptr;
        m_IndicesAndGeneration = m_Head = nullptr;
        m_Erase = nullptr;
        m_Size = m_Capacity = 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    void _CopyFrom(const SlotMap& other)
    {
        assert(m_Data == nullptr);

        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;

        /////////////////////////////////////////////////////
        m_Data = (Value*)malloc(m_Capacity * sizeof(Value));
        std::uninitialized_copy(other.begin(), other.end(), m_Data);

        /////////////////////////////////////////////////////
        m_IndicesAndGeneration = new IndexGenType[m_Capacity];
        memcpy(m_IndicesAndGeneration, other.m_IndicesAndGeneration, m_Capacity * sizeof(IndexGenType));
        m_Head = m_IndicesAndGeneration;

        /////////////////////////////////////////////////////
        m_Erase = new SizeType[m_Capacity];
        memcpy(m_Erase, other.m_Erase, m_Capacity * sizeof(SizeType));
    }

    //////////////////////////////////////////////////////////////////////////////
    void _MoveFrom(SlotMap&& other)
    {
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_Data = other.m_Data;
        m_IndicesAndGeneration = other.m_IndicesAndGeneration;
        m_Erase = other.m_Erase;
        m_Head = other.m_Head;

        other.m_Size = 0;
        other.m_Capacity = 0;
        other.m_Data = nullptr;
        other.m_IndicesAndGeneration = other.m_Head = nullptr;
        other.m_Erase = nullptr;
    }

    //////////////////////////////////////////////////////////////////////////////
    template<typename T>
    inline typename std::enable_if<std::is_move_constructible<T>::value>::type _MoveConstructObject(T* dst, IndexType indexToStore, T* src, IndexType indexToMove)
    {
        new (std::addressof(dst[indexToStore])) T(std::move(src[indexToMove]));
    }

    template<typename T>
    inline typename std::enable_if<!std::is_move_constructible<T>::value>::type _MoveConstructObject(T* dst, IndexType indexToStore, T* src, IndexType indexToMove)
    {
        new (std::addressof(dst[indexToStore])) T(src[indexToMove]);
    }

    //////////////////////////////////////////////////////////////////////////////
    template <typename T>
    inline typename std::enable_if<std::is_trivially_destructible<T>::value>::type _DeconstructObject(T* src, IndexType index)
    {
        // Do nothing
    }

    template <typename T>
    inline typename std::enable_if<!std::is_trivially_destructible<T>::value>::type _DeconstructObject(T* src, IndexType index)
    {
        src[index].~Value();
    }
};