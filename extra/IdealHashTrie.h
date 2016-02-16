// ideal hash trie implementation by alex evans, 2011
// see http://altdevblogaday.org/?p=2311 for more info

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <sys/types.h>

/////////////////////////////// define some useful types and timing macros

typedef unsigned char u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef signed char s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#ifdef WIN32
#include <conio.h>
#include <windows.h>
u64 GetMicroTime() { static u64 hz=0; static u64 hzo=0;	if (!hz) { QueryPerformanceFrequency((LARGE_INTEGER*)&hz); QueryPerformanceCounter((LARGE_INTEGER*)&hzo); } u64 t; QueryPerformanceCounter((LARGE_INTEGER*)&t); return ((t-hzo)*1000000)/hz; }
#else
#include <sys/time.h>
u64 GetMicroTime() { timeval t;gettimeofday(&t,NULL); return t.tv_sec * 1000000ull + t.tv_usec; }
#endif

///////////////////////// some bit twiddling helpers we need to maintain our bitmask ///////////////////////// 
// CountSetBits adapted from http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
inline u32 CountSetBits(u32 v)
{
	v = v - ((v >> 1) & 0x55555555);                    // reuse input as temporary
	v = (v & 0x33333333) + ((v >> 2) & 0x33333333);     // temp
	return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
}
inline u32 CountSetBits(u64 v)
{
    u64 c;
    c = v - ((v >> 1) & 0x5555555555555555ull);
    c = ((c >> 2) & 0x3333333333333333ull) + (c & 0x3333333333333333ull);
    c = ((c >> 4) + c) & 0x0F0F0F0F0F0F0F0Full;
    c = ((c >> 8) + c) & 0x00FF00FF00FF00FFull;
    c = ((c >> 16) + c) & 0x0000FFFF0000FFFFull;
    return u32((c >> 32) + c)/* & 0x00000000FFFFFFFFull*/;
}
template <class T> inline T ClearNthSetBit(T v, int idx)
{
	for (T b=v;b;)
   {
		T lsb=b&~(b-1);
		if (--idx<0) return v^lsb;
		b^=lsb;
	}
	return v;
}
int NextPowerOf2(int x)
{
	x -= 1;
	x |= x >> 16;
	x |= x >> 8;
	x |= x >> 4;
	x |= x >> 2;
	x |= x >> 1;
	return x+1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/* this class defines a HashTrie - a type of hash table that works by having a trie of nodes,
   each of which has at most 64 children. each child is either a data value, or another trie
   node. the children are indexed by picking off 6 bits of the object's key at a time.

   the data structure is very cache friendly, fast, and has low memory overhead: 1 bit per entry.
   it should be faster than any binary tree, and a little slower than a big hash table, but without
   any need to ever re-hash (ie no lumpy performance)
   it should also use less memory than both the binary trees (only 1-2 bit per entry overhead) and
   the hash table (which has to be kept relatively un-loaded to be fast).
   delete is also efficient and doesn't require the use of tombstones and compaction.
   
   the API is simple
    T *Get(T &root, const T &k)
   T *Set(T &root, const T &k)
   void Delete(T &root, const T &k) // yes I implemented delete!

   this implementation has some particular features that users should be aware of:   
   * up to 30 bits of the hash are used to walk the trie; after that, collisions are handled with
     a brute force linear list. with a good hash function, collisions should be rare enough that
     this is fast.
   * as a side effect of this, at most 30/6 = 5 pointer hops are followed to find any key.
   * currently the HashTrie allocates memory using malloc() and free(), and grows its arrays from
     size 1 to 64 *in steps of 1*. This favours low memory waste over speed / avoiding fragmentation. 
     it should be trivial to replace the functions Resize() and Capacity() to change this behaviour.
   * the type T is expected to be a pointer, or other small POD type (for example a pair of ints)
     that can be safely & efficiently copied around.
   * the root of the tree is stored as a T. this is nice: a HashTrie with 0 or 1 entries entails
     no memory allocation, and is just stored in the root itself: memory cost is sizeof(T).
     this does however set some restrictions on T, detailed below.
   * the type T need not have any operators or base type, but should have these global functions 
     overloaded for it:

   HashTrieGetHash - return the hash of a given T (at least 30 bits with good distribution)
   HashTrieEqual   - compare the full keys of two Ts, not just the hashes.
   HashTrieReplace - copy new T over the top of an existing old T. should de-allocate old T.
         this is your chance to handle the case that Set() is called with duplicate keys.
   
   * the type T needs to be able to be aliased with a single pointer, in other words
     sizeof(T) must be >= sizeof(void*), and must have 1 bit of spare storage to indicate if 
     a given bit of memory is a T, or a pointer. if T is a pointer type, you need do nothing,
     and the 'is pointer' bit is stored in the bottom bit of the T.
     if T is something else, you must also overload these 3 additional global functions for T:

 void HashTrieSetNode(T&dst, HashTrie<T> *d); put pointer d into the memory for dst and mark it as a pointer
 HashTrie<T>* HashTrieGetNode(const T&src)    return previously stored pointer, or null if src is not marked
 bool HashTrieIsEmpty(const T&src)            return true if src is equal to the empty tree
 void HashTrieDelete(T &dst)		clear dst to the empty state (eg used to set the root to the empty tree)

   * for more info , read the code ;-)
   * this hasn't been tested in production code, it was just written for fun.
 */

#if 0	// this chooses between 64-way fanout and 32-way fanout. choose based on cache line size and ISA... and measurement ;)
typedef u64 FANOUT_T;
#define FANOUT_LOG2 6
#else
typedef u32 FANOUT_T;
#define FANOUT_LOG2 5
#endif
#define FANOUT_BITS (1<<FANOUT_LOG2)
#define FANOUT_MASK (FANOUT_BITS-1)
#define MAX_DEPTH (30/FANOUT_LOG2)

template <typename T> struct HashTrie
{
    typedef HashTrie<T> N;
	FANOUT_T used;  // each set bit in this bitmask corresponds to an entry in 'data'.
	T data[1];      // we store the children here, tightly packed according to 'used'.
                    // the HashTrie structure is malloced such that the data array is
                    // big enough to hold CountSetBits(used) entries.

   static inline T *Get(T &root, const T &k) { return Lookup(root,k,true); }
   static inline T *Set(T &root, const T &k) { return Lookup(root,k,false); }

    static T *Lookup(T &root, const T &k, bool get)
    {
		assert(sizeof(T) >= sizeof(void*)); // static assert, really...
        if (HashTrieIsEmpty(root)) // special case: empty trie
			return get ? 0 : HashTrieReplace(root, k);
        T*slot=&root; // slot holds the slot in the parent node, ie the place we are going to update if we do an insert
        N*n;    // the last internal node we visited
        u32 k_hash = HashTrieGetHash(k); // the hash we are looking for
        // walk through the hashed key FANOUT_LOG2 bits at a time
        for (int depth=0;;depth+=FANOUT_LOG2, k_hash>>=FANOUT_LOG2)
        {
            if (!(n = HashTrieGetNode(*slot)))  // is this an inner node or a value?
            {   // it's a leaf node
                if (HashTrieEqual(*slot,k)) return get ? slot : HashTrieReplace(*slot,k);
                if (get) return 0;
                // we have a hash collision - replace this leaf with an internal node to resolve the collision
                T oldval = *slot;
                u32 old_hash = HashTrieGetHash(oldval)>>depth;
                // as long as the hashes match, we have to create single element internal nodes. this loop is hopefully nearly always run 0 times
                for (;depth<30 && (old_hash&FANOUT_MASK)==(k_hash&FANOUT_MASK)
                     ;depth+=FANOUT_LOG2,k_hash>>=FANOUT_LOG2,old_hash>>=FANOUT_LOG2)
                    slot=Alloc1(k_hash,slot);
                // finally we create our collision-resolving internal node with just 2 slots: the old and the new value
                return (depth>=30)?Alloc2Linear(k,oldval,slot):Alloc2(k_hash,k,old_hash,oldval,slot);
            }
            // else internal node - find our child and continue the walk
            if (depth>=30) break; // nodes above bit 30 are a linear list walk - handled outside the main loop
            T *child_slot = n->Lookup(k_hash);
            if (!child_slot)
                return get ? 0 : Insert(n,k_hash,k,slot);
            slot=child_slot;
        }
        // we've run out of tree! switch to linear list search. 30 bit hash collisions are hopefully very, very rare.
		T *child_slot = n->LookupLinear(k);
		if (child_slot) return get ? child_slot : HashTrieReplace(*child_slot, k);
        return get ? 0 : Append(n,k,slot);
    }

	
	static bool Delete(T &root, const T &k)
	{
		T *slots[MAX_DEPTH+2]; slots[0]=&root;
		N *nodes[MAX_DEPTH+2]; nodes[0]=0;
		u32 k_hash = HashTrieGetHash(k);
		int depth; 
        for (depth=0;depth<=MAX_DEPTH;++depth,k_hash>>=FANOUT_LOG2)
		{
			if (!(nodes[depth]=HashTrieGetNode(*slots[depth])))
			{   // leaf node
				if (!HashTrieEqual(*slots[depth],k)) return false;
				break;
			} else    // inner node
            if (!(slots[depth+1] = ((depth>=MAX_DEPTH) ? nodes[depth]->LookupLinear(k) : nodes[depth]->Lookup(k_hash))))
					return false;
		}			
		// now we've found the leaf we want to delete, we have to go back up the tree deleting unnecessary nodes.
		assert(HashTrieEqual(*slots[depth],k));
		for (HashTrieDelete(*slots[depth]);--depth>=0;)
		{	// we are going to have to delete an entry from the internal node at nodes[depth]
			int oldsize=(depth>=MAX_DEPTH)?(int)(nodes[depth]->used):(int)(CountSetBits(nodes[depth]->used));
			int oldidx =(int)(slots[depth+1]-nodes[depth]->data);
			if (oldsize==2 && !HashTrieGetNode(nodes[depth]->data[!oldidx])) // the second condition is that the remaining entry is a leaf
			{	// we no longer need this node; just fold the remaining entry, which must be a leaf, into the parent and free this node								
				*slots[depth] = nodes[depth]->data[!oldidx];
				free(nodes[depth]);
				return true;
			}
			if (oldsize>1) 
			{	// resize this node down by a bit, and update the used bitfield
				N *n=Resize(nodes[depth],oldsize,-1,oldidx);				
				n->used =(depth>=MAX_DEPTH)?(n->used-1):ClearNthSetBit(n->used,oldidx);
				HashTrieSetNode(*slots[depth],n); // update the parent slot to point to the resized node
				return true;
			}			
			HashTrieDelete(*slots[depth]);
			free(nodes[depth]);	// oldsize==1. delete this node, and then loop to kill the parent too!
		}
		return true;
	}

    // helpers to search for a given entry
   T *Lookup(u32 index) // this function counts bits in order to return the correct slot for a given hash
	{ 	
		FANOUT_T bit=FANOUT_T(1)<<(index&FANOUT_MASK);
		if (!(used & bit)) return 0;
		return data+CountSetBits(used & (bit-1));
	}
	T *LookupLinear(const T&k) // this one treats the data as an unsorted array and scans.
	{
		for (u32 c1=0;c1<used;++c1) if (HashTrieEqual(data[c1],k)) return data+c1;
		return 0;
	}
	///////////////////////// helper functions - mainly memory allocation & shuffling memory around. joy
	static void DebugPrint(const T &root, int depth=0, u32 hash_so_far=0)
	{
		if (HashTrieIsEmpty(root)) return;
		N *n = HashTrieGetNode(root);
		u32 c2=0;
		if (n) 
		{
			if (depth>=30) for (u32 c1=0;c1<n->used;++c1) DebugPrint(n->data[c1],depth+FANOUT_LOG2);
			else   		   for (u32 c1=0;c1<64;++c1) 
			{   u64 bit=FANOUT_T(1)<<c1;
				if (n->used & bit) DebugPrint(n->data[c2++],depth+FANOUT_LOG2,hash_so_far + (c1<<depth));
			}
		}
		else  
		{
			u64 mask=(FANOUT_T(1)<<depth)-1;
			assert(hash_so_far == (HashTrieGetHash(root) & mask)); // this asserts that the tree is layed out as we expect
			::DebugPrint(root);
		}
	}
	static inline int Capacity(int c) { return c; }			// growth policy enshrined in this function. could eg round c up to next power of 2.
	//static inline int Capacity(int c) { return (c<4)?4:NextPowerOf2(c); }			// ...like this
	static N *Resize(N*n, int oldsize, int deltasize, int idx) // memory allocation all in this function. (re)allocates n, copies old data, and inserts space at index 'idx'
	{
		N *newn=n; int newsize=oldsize+deltasize;
		int oldcapacity=Capacity(oldsize);
		int newcapacity=Capacity(newsize);
		if (n==0 || oldcapacity!=newcapacity) newn=(N*)malloc(sizeof(N)+(newcapacity-1)*sizeof(T));
		if (n)
		{
			if (deltasize>0)
				memmove(newn->data+idx+deltasize,n->data+idx,sizeof(T)*(oldsize-idx)); // shuffle tail to make room
			else
				memmove(newn->data+idx,n->data+idx-deltasize,sizeof(T)*(newsize-idx)); // shuffle tail to make room
			if (n!=newn) 
			{   // copy over old unchanged data & free old version
				newn->used=n->used;
				memcpy(newn->data,n->data,sizeof(T)*idx);
				free(n); 
			}
		}		
		return newn;
	}
	// allocation helper functions that use Resize to insert new nodes in various ways; all return a pointer to the new slot
	static T *Insert(N*n, u32 index, const T &newval, T*slot_to_replace) 
	{
		FANOUT_T bit=FANOUT_T(1)<<(index&FANOUT_MASK);
		assert(0==(n->used&bit));
		u32 numbitsbelow = CountSetBits(n->used & (bit-1));
		n=Resize(n,CountSetBits(n->used), 1, numbitsbelow);
		n->used |= bit;
		n->data[numbitsbelow]=newval;
        HashTrieSetNode(*slot_to_replace,n);
		return n->data+numbitsbelow;
	}
	static T *Append(N*&n, T newval, T*slot_to_replace)
	{
		n=Resize(n,int(n->used),1, int(n->used));
		n->data[n->used]=newval;
        HashTrieSetNode(*slot_to_replace,n);
		return n->data+n->used++;
	}
	static T* Alloc1(u32 index, T*slot_to_replace) 
    {
        N*n=Resize(0,0,1,0);
        n->used=FANOUT_T(1)<<(index&FANOUT_MASK);
        HashTrieSetNode(*slot_to_replace,n);
        return n->data; // warning: does not set the data yet... caller must do that.
    }
	static T *Alloc2(u32 newindex, const T &newval, u32 oldindex, const T &oldval, T*slot_to_replace)
	{   // allocates a node with room for 2 elements, and sorts them by index.
		newindex&=FANOUT_MASK; oldindex&=FANOUT_MASK;
		assert(newindex!=oldindex);
		N*n=Resize(0,0,2,0); 
		n->used=(FANOUT_T(1)<<newindex)|(FANOUT_T(1)<<oldindex); 
		if (newindex<oldindex)
		{
			n->data[0]=newval;n->data[1]=oldval;
            HashTrieSetNode(*slot_to_replace,n);
			return n->data;
		}
		else
		{
			n->data[0]=oldval;n->data[1]=newval;
            HashTrieSetNode(*slot_to_replace,n);
            return n->data+1;
		}
	}
	static T *Alloc2Linear(const T &newval, const T &oldval, T*slot_to_replace)
	{   // allocates a node with room for 2 elements, to be used as a linear list.
		N*n=Resize(0,0,2,0);
		n->used=2;n->data[0]=newval;n->data[1]=oldval;
        HashTrieSetNode(*slot_to_replace,n);
		return n->data;
	}
};

// provide specialisations of these if your T is not pointer-like, or you dont want to use the bottom bit as a flag
template <class T> inline void HashTrieSetNode(T&dst, HashTrie<T> *d)  { dst = (T)(1|(size_t)d); }
template <class T> inline HashTrie<T>* HashTrieGetNode(const T&src)    { size_t p =(size_t)src; return (p&1)?(HashTrie<T>*)(p-1):0; }
template <class T> inline bool HashTrieIsEmpty(const T&src)                { return !src; }
template <class T> inline void HashTrieDelete(T &dst) { dst=T(); };
template <class T> inline T* HashTrieReplace(T &dst, const T &src) { HashTrieDelete(dst); dst=src; return &dst; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// example program, that also compares performance (ROUGHLY!) with stl set and unordered_set

#define TEST_SIZE 1000000
#define TEST_TRIE
//#define TEST_SET
//#define TEST_UNORDERED_SET

#ifdef TEST_SET
#include <set>
#endif
#ifdef TEST_UNORDERED_SET
#include <unordered_set>
#endif


// from http://www.cris.com/~Ttwang/tech/inthash.htm
#define ror64(x,k) (((x)>>(k)) | ((x)<<(64-(k))))
u32 hash6432shift(u64 key)
{
    key = (~key) + (key << 18); // key = (key << 18) - key - 1;
    key ^= ror64(key,31);
    key *= 21; // key = (key + (key << 2)) + (key << 4);
    key ^= ror64(key,11);
    key += (key << 6);
    key ^= ror64(key,22);
    return (u32)key;
}

// from murmurhash2
inline u64 murmurmix(u64 h, u64 k)
{
	const u64 m = 0xc6a4a7935bd1e995ull;
	const int r = 47;
	k*=m;
	k^=k>>r;
	k*=m;
	h^=k;
	h*=m;
	return h;
}

u32  HashTrieGetHash(u64 kv)                { return hash6432shift(kv); }
bool HashTrieEqual(u64 a, u64 b)            { return a==b; }
void DebugPrint(u64 a)						{ printf("%llx ",(long long unsigned int)(a&0xfff)); }

u32  HashTrieGetHash(void* kv)                { return 11; } // test a really bad hash function
bool HashTrieEqual(void*a, void*b)            { return a==b; }


struct hashseteq {   
	const static size_t bucket_size = FANOUT_BITS; 
	bool operator()(u64 a, u64 b) const { return HashTrieEqual(a,b); } 
	size_t operator()(u64 a) const { return HashTrieGetHash(a); } 
};

#undef max
namespace tracking_allocator
{
    size_t g_bytesAllocated = 0;
    size_t g_numAllocs = 0;

    template <class T> class allocator;

    // specialize for void:
    template <>
    class allocator<void>
    {
    public:
        typedef void*       pointer;
        typedef const void* const_pointer;
        // reference to void members are impossible.
        typedef void        value_type;

        template <class U>
        struct rebind
        {
            typedef allocator<U> other;
        };
    };

    template <class T>
    class allocator
    {
    public:
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
        typedef T*      pointer;
        typedef const T*    const_pointer;
        typedef T&      reference;
        typedef const T&    const_reference;
        typedef T       value_type;

        template <class U>
        struct rebind
        {
            typedef allocator<U> other;
        };

        allocator() throw()
        {
        }
        template <class U>
        allocator(const allocator<U>& u) throw()
        {
        }
        ~allocator() throw()
        {
        }

        pointer address(reference r) const
        {
            return &r;
        }
        const_pointer address(const_reference r) const
        {
            return &r;
        }
        size_type maxSize() const throw()
        {
            return std::numeric_limits<size_t>::max()/sizeof(T);
        }
        pointer allocate(size_type n, allocator<void>::const_pointer hint = 0)
        {
            g_bytesAllocated += n * sizeof(T);
            ++g_numAllocs;
            return reinterpret_cast<pointer>(::operator new(n * sizeof(T)));
        }
        void deallocate(pointer p, size_type n)
        {
            g_bytesAllocated -= n * sizeof(T);
            --g_numAllocs;
            ::operator delete(p);
        }

        void construct(pointer p, const_reference val)
        {
            ::new(p) T(val);
        }
        void destroy(pointer p)
        {
            p->~T();
        }
    };

    template <class T1, class T2>
    bool operator==(const allocator<T1>& a1, const allocator<T2>& a2) throw()
    {
        return true;
    }

    template <class T1, class T2>
    bool operator!=(const allocator<T1>& a1, const allocator<T2>& a2) throw()
    {
        return false;
    }

}

int main(int argc, char **argv)
{
   u64 root=0;
   u32 c1;
    u64 t0;
   //_getch();
#ifdef TEST_TRIE

   {
      // test hash collision edge case with size_t whose hash function is 'return 11';
      // so everything ends up in a linear list
      void* root=0;
      HashTrie<void*>::Set(root,(void*) 100);
      HashTrie<void*>::Set(root,(void*) 200);
      HashTrie<void*>::Set(root,(void*) 300);
      HashTrie<void*>::Set(root,(void*) 400);
      assert(HashTrie<void*>::Delete(root,(void*) 400));
      assert(HashTrie<void*>::Delete(root,(void*) 300));
      assert(HashTrie<void*>::Delete(root,(void*) 200));
      assert(HashTrie<void*>::Delete(root,(void*) 100));
      assert(root==0);
   }

    /////////////////////////////////////////////////////////////////////////
   for (t0=GetMicroTime(),c1=0;c1<TEST_SIZE;++c1)
   {
        u64 randy=murmurmix(12345,c1*2)*2+2; // we use the bottom bit to indicate internal nodes, and 0 means null.
        HashTrie<u64>::Set(root, randy);
      //HashTrie<u64>::DebugPrint(root); printf("\n");
    }
    printf("trie insert   %8d %8dusec\n",c1,int(GetMicroTime()-t0));
   //_getch();
    for (int iter=0;iter<3;++iter)
   {
      for (t0=GetMicroTime(),c1=0;c1<TEST_SIZE*2;++c1)
       {
           u64 randy=murmurmix(12345,c1)*2+2; // we use the bottom bit to indicate internal nodes, and 0 means null.
           HashTrie<u64>::Get(root, randy);
       }
       printf("trie get      %8d %8dusec\n",c1,int(GetMicroTime()-t0));
    }
   for (t0=GetMicroTime(),c1=0;c1<TEST_SIZE;++c1)
   {
        u64 randy=murmurmix(12345,c1*2)*2+2; // we use the bottom bit to indicate internal nodes, and 0 means null.
        HashTrie<u64>::Delete(root, randy);
      //HashTrie<u64>::DebugPrint(root); printf("\n");
    }
    assert(root==0);
   printf("trie delete   %8d %8dusec\n",c1,int(GetMicroTime()-t0));
#endif

#ifdef TEST_SET
   /////////////////////////////////////////////////////////////////////////
   std::set<u64> myset;
    for (t0=GetMicroTime(),c1=0;c1<TEST_SIZE;++c1)
   {
        u64 randy=murmurmix(12345,c1*2)*2+2; // we use the bottom bit to indicate internal nodes, and 0 means null.
        myset.insert(randy);
    }
    printf("set insert    %8d %8dusec\n",c1,int(GetMicroTime()-t0));
   //_getch();
    for (int iter=0;iter<3;++iter)
   {

       for (t0=GetMicroTime(),c1=0;c1<TEST_SIZE*2;++c1)
       {
           u64 randy=murmurmix(12345,c1)*2+2; // we use the bottom bit to indicate internal nodes, and 0 means null.
           std::set<u64>::iterator i=myset.find(randy);
       }
       printf("set get       %8d %8dusec\n",c1,int(GetMicroTime()-t0));
   }
   for (t0=GetMicroTime(),c1=0;c1<TEST_SIZE;++c1)
   {
        u64 randy=murmurmix(12345,c1*2)*2+2; // we use the bottom bit to indicate internal nodes, and 0 means null.
        myset.erase(myset.find(randy));
    }
   printf("set delete    %8d %8dusec\n",c1,int(GetMicroTime()-t0));
   assert(myset.empty());
#endif

#ifdef TEST_UNORDERED_SET
   /////////////////////////////////////////////////////////////////////////

    //standard allocator
    //typedef std::unordered_set<u64> hashsettype;

    //tracking_allocator
    typedef std::unordered_set<u64, std::tr1::hash<u64>, std::equal_to<u64>, tracking_allocator::allocator<u64> > hashsettype;

   hashsettype hashset;

    for (t0=GetMicroTime(),c1=0;c1<TEST_SIZE;++c1)
   {
        u64 randy=murmurmix(12345,c1*2)*2+2; // we use the bottom bit to indicate internal nodes, and 0 means null.
        hashset.insert(randy);
    }
    printf("hset insert   %8d %8dusec   memory used=%d, numAllocs=%d\n",c1,int(GetMicroTime()-t0), tracking_allocator::g_bytesAllocated, tracking_allocator::g_numAllocs);
   //_getch();
    for (int iter=0;iter<3;++iter)
   {
        int numFound = 0; //make sure to actually use the result of the find, don't want compiler to optimize it away
        for (t0=GetMicroTime(),c1=0;c1<TEST_SIZE*2;++c1)
       {
           u64 randy=murmurmix(12345,c1)*2+2; // we use the bottom bit to indicate internal nodes, and 0 means null.
           if (hashset.find(randy) != hashset.end())
                ++numFound;
       }
       printf("hset get      %8d %8dusec %8d\n",c1,int(GetMicroTime()-t0),numFound);
   }
   for (t0=GetMicroTime(),c1=0;c1<TEST_SIZE;++c1)
   {
        u64 randy=murmurmix(12345,c1*2)*2+2; // we use the bottom bit to indicate internal nodes, and 0 means null.
        hashset.erase(randy);
    }
    hashset.swap(hashsettype()); //full clear, make sure our tracking_allocator is working properly
    printf("hset delete   %8d %8dusec    memory used=%d, numAllocs=%d\n",c1,int(GetMicroTime()-t0), tracking_allocator::g_bytesAllocated, tracking_allocator::g_numAllocs);
   assert(hashset.empty());
#endif

   return 0;
}
