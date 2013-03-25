/**
 * 	Copyright (c) 2002-2005 Igor Chalenko - igor.chalenko@gmail.com
 * 	All rights reserved.
 *
 * 	This code is in public domain.
 *
 * 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * 	FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * 	SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * 	FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * 	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * 	DEALINGS IN THE SOFTWARE.
 */

// HASH ARRAY MAPPED TREE
// http://hamt.sourceforge.net/

#ifndef __AEON_STDLIB_HAMT_HPP_INCLUDED__
#define __AEON_STDLIB_HAMT_HPP_INCLUDED__

#include <functional>
#include <utility>
#include <cstddef>
#include <cassert>
#include <cstring>
#include <string>
#include <algorithm>

#define LOWER(n, i) ( ~((~0UL)<<i) & n)

template <typename _Type> _Type CTPop(_Type val) {
   const size_t SK5  = 0x55555555;
   const size_t SK3  = 0x33333333;
   const size_t SKF0 = 0xF0F0F0F;

   val -= ((val >> 1) & SK5);
   val = (val & SK3) + ((val >> 2) & SK3);
   val = (val & SKF0) + ((val >> 4) & SKF0);
   val += val >> 8;
   return (val + (val >> 16)) & 0x3F;
}


template<class _Char> size_t _rehash_mix(_Char k, size_t level, const size_t length) {
#define mix(a,b,c) \
   { \
   a -= b; a -= c; a ^= (c>>13); \
   b -= c; b -= a; b ^= (a<<8); \
   c -= a; c -= b; c ^= (b>>13); \
   }

   size_t a, b, c;

   // Set up the internal state
   size_t len = length;
   a = b = 0x9e3779b9;  // the golden ratio; an arbitrary value
   c = 0x9e3779b9 * level;

   // ---------------------------------------- handle most of the key
   while (len >= 12) {
      a += (k[0]+((size_t)k[1]<<8)+((size_t)k[2]<<16) +((size_t)k[3]<<24));
      b += (k[4]+((size_t)k[5]<<8)+((size_t)k[6]<<16) +((size_t)k[7]<<24));
      c += (k[8]+((size_t)k[9]<<8)+((size_t)k[10]<<16)+((size_t)k[11]<<24));
      mix(a,b,c);
      k += 12; len -= 12;
   }

   //------------------------------------- handle the last 11 bytes
   c += length;

   switch (len) {
      case 11: c+=((size_t)k[10]<<24);
      case 10: c+=((size_t)k[9]<<16);
      case 9 : c+=((size_t)k[8]<<8);

      case 8 : b+=((size_t)k[7]<<24);
      case 7 : b+=((size_t)k[6]<<16);
      case 6 : b+=((size_t)k[5]<<8);
      case 5 : b+=k[4];
      case 4 : a+=((size_t)k[3]<<24);
      case 3 : a+=((size_t)k[2]<<16);
      case 2 : a+=((size_t)k[1]<<8);
      case 1 : a+=k[0];

      // case 0: nothing left to add
   }
   mix(a,b,c);
   return c;
#undef mix
}

template<class _Char> size_t _hash_mix(_Char k, size_t length) {
   return _rehash_mix(k, 0, length);
}

static const size_t T[256] = {
   251, 175, 119, 215, 81, 14, 79, 191, 103, 49, 181, 143, 186, 157,  0,
   232, 31, 32, 55, 60, 152, 58, 17, 237, 174, 70, 160, 144, 220, 90, 57,
   223, 59,  3, 18, 140, 111, 166, 203, 196, 134, 243, 124, 95, 222, 179,
   197, 65, 180, 48, 36, 15, 107, 46, 233, 130, 165, 30, 123, 161, 209, 23,
   97, 16, 40, 91, 219, 61, 100, 10, 210, 109, 250, 127, 22, 138, 29, 108,
   244, 67, 207,  9, 178, 204, 74, 98, 126, 249, 167, 116, 34, 77, 193,
   200, 121,  5, 20, 113, 71, 35, 128, 13, 182, 94, 25, 226, 227, 199, 75,
   27, 41, 245, 230, 224, 43, 225, 177, 26, 155, 150, 212, 142, 218, 115,
   241, 73, 88, 105, 39, 114, 62, 255, 192, 201, 145, 214, 168, 158, 221,
   148, 154, 122, 12, 84, 82, 163, 44, 139, 228, 236, 205, 242, 217, 11,
   187, 146, 159, 64, 86, 239, 195, 42, 106, 198, 118, 112, 184, 172, 87,
   2, 173, 117, 176, 229, 247, 253, 137, 185, 99, 164, 102, 147, 45, 66,
   231, 52, 141, 211, 194, 206, 246, 238, 56, 110, 78, 248, 63, 240, 189,
   93, 92, 51, 53, 183, 19, 171, 72, 50, 33, 104, 101, 69, 8, 252, 83, 120,
   76, 135, 85, 54, 202, 125, 188, 213, 96, 235, 136, 208, 162, 129, 190,
   132, 156, 38, 47, 1, 7, 254, 24, 4, 216, 131, 89, 21, 28, 133, 37, 153,
   149, 80, 170, 68, 6, 169, 234, 151
};

template<class _Char> size_t _rehash_pearson_lookup(_Char k, size_t level, size_t length) {
   size_t h1 = 0;
   size_t h2 = 0;
   size_t h3 = 0;
   size_t h4 = 0;

   while (length >= 4) {
      h1 = T[(h1 ^ *k++ + level) % 256];
      h2 = T[(h2 ^ *k++ + level) % 256];
      h3 = T[(h3 ^ *k++ + level) % 256];
      h4 = T[(h4 ^ *k++ + level) % 256];
      length -= 4;
   }

   switch (length) {
      case 3:
         h1 = T[(h1 ^ *k++ + level) % 256];
      case 2:
         h2 = T[(h2 ^ *k++ + level) % 256];
      case 1:
         h3 = T[(h3 ^ *k++ + level) % 256];
   }
   return h1 | (h2 << 8) | (h3 << 16) | (h4 << 24);
}

template<class _Char> size_t _hash_pearson_lookup(_Char k, size_t length) {
   size_t h1 = 0;
   size_t h2 = 0;
   size_t h3 = 0;
   size_t h4 = 0;

   while (length >= 4) {
      h1 = T[h1 ^ *k++];
      h2 = T[h2 ^ *k++];
      h3 = T[h3 ^ *k++];
      h4 = T[h4 ^ *k++];

      length -= 4;
   }
   switch (length) {
      case 3:
         h1 = T[h1 ^ *k++];
      case 2:
         h2 = T[h2 ^ *k++];
      case 1:
         h3 = T[h3 ^ *k++];
   }

   return h1 | (h2 << 8) | (h3 << 16) | (h4 << 24);
}

template<class _Char> size_t _hash_fast(_Char k) {
   size_t __h = 0;
   for ( ; *k; ++k) __h = 5 * __h + *k;

   return __h;
}

template<class _Char> size_t _rehash_fast(_Char k, size_t level) {
   size_t __h = 0;
   for ( ; *k; ++k) __h = 5 * __h + (*k + level);

   return __h;
}

template<class _Char> struct equal_to {};

template <>
struct equal_to<const char*> : public std::binary_function<const char*, const char*, bool> {
   bool operator()(const char* const & s1, const char* const & s2) const {
      return (s1 == s2) || (s1 != NULL && s2 != NULL && strcmp(s1, s2) == 0);
   }
};

template <>
struct equal_to<const wchar_t*> : public std::binary_function<const wchar_t*, const wchar_t*, bool> {
   bool operator()(const wchar_t* const & s1, const wchar_t* const & s2) const {
      return (s1 == s2) || (s1 != NULL && s2 != NULL && wcscmp(s1, s2) == 0);
   }
};

template <>
struct equal_to<std::string> : public std::binary_function<std::string, std::string, bool> {
   bool operator()(const std::string & s1, const std::string & s2) const {
      return (s1 == s2);
   }
};

template <>
struct equal_to<std::basic_string<wchar_t> > : public std::binary_function<std::basic_string<wchar_t>, std::basic_string<wchar_t>, bool> {
   bool operator()(const std::basic_string<wchar_t> & s1, const std::basic_string<wchar_t> & s2) const {
      return (s1 == s2);
   }
};

template <class _Key> struct hash_mix {
   size_t operator()(_Key key) const {
      return (size_t)key;
   }
   size_t operator()(_Key key, size_t level) const {
      return (size_t)key * (level + 1);
   }
};

template <class _Key> struct hash_fast {
   size_t operator()(_Key key) const {
      return (size_t)key;
   }
   size_t operator()(_Key key, size_t level) const {
      return (size_t)key * (level + 1);
   }
};

template <class _Key> struct hash_pearson_lookup {
   size_t operator()(_Key key) const {
      return (size_t)key;
   }
   size_t operator()(_Key key, size_t level) const {
      return (size_t)key * (level + 1);
   }
};

//template <class _Key> struct hash_fast { };
//template <class _Key> struct hash_pearson_lookup { };

template<> struct hash_mix<char*> {
   size_t operator()(const char* s) const {
      return _hash_mix<const char*>(s, strlen(s));
   }
   size_t operator()(const char* s, size_t level) const {
      return _rehash_mix<const char*>(s, level, strlen(s));
   }
};

template<> struct hash_mix<const char*> {
   size_t operator()(const char* s) const {
      return _hash_mix<const char*>(s, strlen(s));
   }
   size_t operator()(const char* s, size_t level) const {
      return _rehash_mix<const char*>(s, level, strlen(s));
   }
};

template<> struct hash_mix<wchar_t*> {
   size_t operator()(const wchar_t* s) const {
      return _hash_mix<const wchar_t*>(s, wcslen(s));
   }
   size_t operator()(const wchar_t* s, size_t level) const {
      return _rehash_mix<const wchar_t*>(s, level, wcslen(s));
   }
};

template<> struct hash_mix<const wchar_t*> {
   size_t operator()(const wchar_t* s) const {
      return _hash_mix<const wchar_t*>(s, wcslen(s));
   }
   size_t operator()(const wchar_t* s, size_t level) const {
      return _rehash_mix<const wchar_t*>(s, level, wcslen(s));
   }
};

template<> struct hash_mix<std::string> {
   size_t operator()(const std::string & x) const {
      return _hash_mix<const char*>(x.data(), x.length());
   }
   size_t operator()(const std::string & x, size_t level) const {
      return _rehash_mix<const char*>(x.data(), level, x.length());
   }
};

template<> struct hash_mix<std::basic_string<wchar_t> > {
   size_t operator()(const std::basic_string<wchar_t> & x) const {
      return _hash_mix<const wchar_t*>(x.data(), x.length());
   }
   size_t operator()(const std::basic_string<wchar_t> & x, size_t level) const {
      return _rehash_mix<const wchar_t*>(x.data(), level, x.length());
   }
};

template<> struct hash_fast<char*> {
   size_t operator()(const char* s) const {
      return _hash_fast<const char*>(s);
   }
   size_t operator()(const char* s, size_t level) const {
      return _rehash_fast<const char*>(s, level);
   }
};

template<> struct hash_fast<const char*> {
   size_t operator()(const char* s) const {
      return _hash_fast<const char*>(s);
   }
   size_t operator()(const char* s, size_t level) const {
      return _rehash_fast<const char*>(s, level);
   }
};

template<> struct hash_fast<wchar_t*> {
   size_t operator()(const wchar_t* s) const {
      return _hash_fast<const wchar_t*>(s);
   }
   size_t operator()(const wchar_t* s, size_t level) const {
      return _rehash_fast<const wchar_t*>(s, level);
   }
};

template<> struct hash_fast<const wchar_t*> {
   size_t operator()(const wchar_t* s) const {
      return _hash_fast<const wchar_t*>(s);
   }
   size_t operator()(const wchar_t* s, size_t level) const {
      return _rehash_fast<const wchar_t*>(s, level);
   }
};

template<> struct hash_fast<std::string> {
   size_t operator()(const std::string & x) const {
      return _hash_fast<const char*>(x.data());
   }
   size_t operator()(const std::string & x, size_t level) const {
      return _rehash_fast<const char*>(x.data(), level);
   }
};

template<> struct hash_fast<std::basic_string<wchar_t> > {
   size_t operator()(const std::basic_string<wchar_t> & x) const {
      return _hash_fast<const wchar_t*>(x.data());
   }
   size_t operator()(const std::basic_string<wchar_t> & x, size_t level) const {
      return _rehash_fast<const wchar_t*>(x.data(), level);
   }
};

template<> struct hash_pearson_lookup<char*> {
   size_t operator()(const char* s) const {
      return _hash_pearson_lookup<const char*>(s, strlen(s));
   }
   size_t operator()(const char* s, size_t level) const {
      return _rehash_pearson_lookup<const char*>(s, level, strlen(s));
   }
};

template<> struct hash_pearson_lookup<const char*> {
   size_t operator()(const char* s) const {
      return _hash_pearson_lookup<const char*>(s, strlen(s));
   }
   size_t operator()(const char* s, size_t level) const {
      return _rehash_pearson_lookup<const char*>(s, level, strlen(s));
   }
};

template<> struct hash_pearson_lookup<wchar_t*> {
   size_t operator()(const wchar_t* s) const {
      return _hash_pearson_lookup<const wchar_t*>(s, wcslen(s));
   }
   size_t operator()(const wchar_t* s, size_t level) const {
      return _rehash_pearson_lookup<const wchar_t*>(s, level, wcslen(s));
   }
};

template<> struct hash_pearson_lookup<const wchar_t*> {
   size_t operator()(const wchar_t* s) const {
      return _hash_pearson_lookup<const wchar_t*>(s, wcslen(s));
   }
   size_t operator()(const wchar_t* s, size_t level) const {
      return _rehash_pearson_lookup<const wchar_t*>(s, level, wcslen(s));
   }
};

template<> struct hash_pearson_lookup<std::basic_string<char> > {
   size_t operator()(const std::basic_string<char> & x) const {
      return _hash_pearson_lookup<const char*>(x.data(), x.length());
   }
   size_t operator()(const std::basic_string<char> & x, size_t level) const {
      return _rehash_pearson_lookup<const char*>(x.data(), level, x.length());
   }
};

template<> struct hash_pearson_lookup<std::basic_string<wchar_t> > {
   size_t operator()(const std::basic_string<wchar_t> & x) const {
      return _hash_pearson_lookup<const wchar_t*>(x.data(), x.length());
   }
   size_t operator()(const std::basic_string<wchar_t> & x, size_t level) const {
      return _rehash_pearson_lookup<const wchar_t*>(x.data(), level, x.length());
   }
};


namespace hamt_ns {

template <class _Value> struct hamt_trie;

/**
 * A HAMT trie node. Each node have one of the following states:
 * -# empty, or not taken. Empty nodes may only appear in a root trie. 
 * -# non-empty. In this case, attribute 'bitmap' contains the first-level
 * hash of the key, stored in the node. Value of the 'subtrie' attribute is
 * NULL.
 * -# sub-trie reference. 'subtrie' in this case contains the pointer to the
 * sub-trie object, and bitmap encodes the subtrie layout as HAMT dictates.
 *
 * This class is not for public use.
 */
template <class _Value> struct hamt_node {
	typedef hamt_trie<_Value> trie_type;
	typedef typename _Value::first_type key_type;
	typedef typename _Value::second_type value_type;

	//! (key, value) pair.
	_Value node;

	//! Bitmap of the sub-trie, if the node is a sub-trie reference.
	size_t bitmap;

	//! Subtrie pointer, or NULL if the node is not a sub-trie reference.
	trie_type * subtrie;

	/**
	 * Returns true <==> the given node is a sub-trie reference.
	 */
	size_t is_subtrie() const
	{
		//return ((address_t)m_offset & 0x80000000) == 0x80000000;
		return subtrie != NULL;
	}

	/**
	 * Returns true <==> the given node is empty.
	 * a key-value pair bound.
	 */
	size_t is_empty() const
	{
		return (node.first == key_type() && !is_subtrie());
	}

	/**
	 * Returns the number of nodes in the node's sub-trie. It's not legal to call this
	 * operation for non sub-trie nodes.
	 */
	size_t size() const
	{
		assert(is_subtrie());
		return CTPop(this->bitmap);
	}

	/**
	 * Changes the state of the given node to 'sub-trie reference'.
	 * @param offset the pointer to the subtrie.
	 */
	void set_subtrie(trie_type * offset)
	{
		node.first = key_type();
		node.second = value_type();
		subtrie = offset;
	}

	/**
	 * Changes the state of the given node to 'empty'.
	 */
	void set_empty()
	{
		node.first = key_type();
		node.second = value_type();
		subtrie = NULL;
	}

	trie_type & offset() const
	{
		return *subtrie;
	}

	/**
	 * Returns the key bound to this node.
	 */
	const key_type & key() const
	{
		return node.first;
	}

	/**
	 * Returns the value bound to the given node.
	 */
	const value_type value() const
	{
		return node.second;
	}

	/**
	 * A default constructor creates an empty node.
	 */
	hamt_node() : node(), bitmap(0), subtrie(NULL)
	{ 
	}
	
	/**
	 * Constructs a non-empty node from the key-value pair.
	 * 
	 * @param key    node key.
	 * @param value  node value.
	 */
	hamt_node(key_type const & key, value_type value)
	: node(key, value), bitmap(0), subtrie(NULL)
	{
	}

	/**
	 * Compares two nodes. Note that key uniqueness is considered to be
	 * enough for the node comparing.
	 * 
	 * @param node   the second comparison argument.
	 * 
	 * @return true if the nodes are equal, false otherwise.
	 */
	bool operator == (const hamt_node & node) const
	{
		return (node.bitmap == bitmap && node.key() == key());
	}

};

/**
 * HAMT trie is an array of nodes, all sharing a common parent.
 * Trie nodes are accessible by the index in a constant time.
 * Trie may be resized during its lifetime; the number of nodes
 * allocated by a trie, may be bigger than the value, reported by
 * the size() method of the node which is the root of the given 
 * subtrie. This may happen when trie is shrunk.
 *
 * This class is not for public use.
 */
template <class _Value>
struct hamt_trie
{
	typedef hamt_node<_Value> * node_ptr;
	typedef hamt_trie<_Value> * trie_ptr;

//protected:
	//! Storage
	node_ptr m_nodes;

	//! Number of nodes allocated by this trie
	size_t m_length;

	//! Parent node reference
	trie_ptr m_parent;

public:
	/**
	 * Default constructor creates a trie with empty storage.
	 */
	hamt_trie()
	: m_nodes(NULL), m_length(NULL), m_parent(NULL)
	{
	}

	/**
	 * Creates a trie with the given nodes and the length.
	 */
	hamt_trie(node_ptr nodes, size_t length)
	: m_nodes(nodes), m_length(length), m_parent(NULL)
	{
	}

	/**
	 * Returns the number of nodes in the trie storage.
	 */
	size_t length() const
	{
		return m_length;
	}

	/**
	 * Returns the node at position index.
	 */
	node_ptr operator[](size_t index) const
	{
		assert(index < m_length);

		return &m_nodes[index];
	}

	/**
	 * Sets the trie parent.
	 */
	void set_parent(trie_ptr ptr)
	{
		m_parent = ptr;
	}

	/**
	 * Returns the trie parent.
	 */
	trie_ptr parent() const
	{
		return m_parent;
	}

	/**
	 * Returns true if the given pointer points to the first node.
	 */
	bool is_first(node_ptr node) const
	{
		return node == &m_nodes[0];
	}

	/**
	 * Returns true if the given pointer points to the last node.
	 */
	bool is_last(node_ptr node) const
	{
		if (node == &m_nodes[m_length - 1]) return true;

/*
		for (size_t i = m_length - 1; i >= 0; i--) {
			if (!m_nodes[i].is_empty()) return (node == &m_nodes[i]);
		}
*/
		return false;
	}

	/**
	 * Returns the pointer to the node that is the root of the given sub-trie.
	 * @param p a sub-trie to find a reference to.
	 */
	node_ptr find(trie_ptr p)
	{
		for (size_t i = 0; i < m_length; i++) {
			//if (m_nodes[i].is_subtrie()) {
			if (m_nodes[i].subtrie) {
				if (p == m_nodes[i].subtrie) {
					return &m_nodes[i];
				}
			}
		}
		assert(false);
		return NULL;
	}

	/**
	 * Copies the contents of the trie to the given location, removing a node 
	 * at the given position. Moves the resting nodes to preserve array contiguity.
	 * @param dst the memory location to copy nodes to.
	 * @param index storage position to skip during copying.
	 * @param src_size number of nodes to copy.
	 */
	void shrink(const node_ptr dst, size_t index, size_t src_size)
	{
		if (!index) {
			std::copy(m_nodes + 1, m_nodes + src_size, dst);
		}
		else {
			if (m_nodes != dst) {
				std::copy(m_nodes, m_nodes + index, dst);
			}
			
			if (src_size  > index + 1) {
				std::copy(m_nodes + index + 1, m_nodes + src_size, dst + index);
			}
		}
	}

	/**
	 * Copies the contents of the trie to the given location, inserting an empty node 
	 * at the given position.
	 * @param dst the memory location to copy nodes to.
	 * @param index storage position to insert a node at.
	 * @param src_size number of nodes to copy.
	 */
	node_ptr grow(const node_ptr dst, size_t index, size_t src_size)
	{
		size_t res = 0;
		if (!index) {
			std::copy_backward(m_nodes, m_nodes + src_size, dst + src_size + 1);
		}
		else {
			std::copy_backward(m_nodes, m_nodes + index, dst + index);
			if (src_size - index > 0) {
				std::copy_backward(m_nodes + index, m_nodes + src_size, dst + src_size + 1);
			}
			res = index;
		}
		return &dst[res];
	}
};

/**
 * Implements storage-related HAMT operations: caching, memory allocation, 
 * and trie resizing.
 *
 * This class may be inhereted by the clients willing to override 
 * the default storage behavior.
 */
template <class _Allocator>
struct hamt_storage_2
{
public:
	typedef _Allocator allocator_type;
protected:
	typedef hamt_node<typename allocator_type::value_type> node_type;
	typedef node_type * node_ptr;
	typedef hamt_trie<typename allocator_type::value_type> trie_type;
	typedef trie_type * trie_ptr;
	//typedef typename _Traits::node_type node_type;
	//typedef typename _Traits::node_ptr node_ptr;
	//typedef typename _Traits::trie_type trie_type;
	//typedef typename _Traits::trie_ptr trie_ptr;

	typedef hamt_storage_2<_Allocator> self_type;

	//! allocator object for nodes
	typename allocator_type::template rebind<node_type>::other m_node_alloc;

	//! allocator object for tries
	typename allocator_type::template rebind<trie_type>::other m_trie_alloc;

	//! allocator object for tries
	typename allocator_type::template rebind<node_ptr>::other m_ptr_alloc;

	//! maximum number of nodes in a non-root sub-trie.
	const static size_t TRIE_LENGTH = sizeof(size_t) * 8;

	typedef typename allocator_type::template rebind<node_ptr>::other::pointer trie_array;

	//! Array of sub-trie pointers. Sub-tries of size n are at index n - 1.
	trie_array m_tries[TRIE_LENGTH];

	//! Count of free subtries of size n is at position n-1.
	size_t m_counts[TRIE_LENGTH];

	//! A hint for the trie cache sizes.
	size_t m_init_size;

	//! Number of nodes currently allocated.
	size_t m_nodes;

	/**
	 * Returns a number of cached tries depending on the needed trie size.
	 * Override this method to provide your own caching strategy.
	 */
	virtual size_t cache_size(size_t subtrie_size)
	{
		assert(subtrie_size > 0);

		switch (subtrie_size) {
			case 1: return m_init_size * 4;
			case 2: return m_init_size * 2;
			case 3: return m_init_size;
			case 4: return m_init_size / 2 + 1;
			case 5: return m_init_size / 4 + 1;
			default: return 2;
		}
	}

	/**
	 * Wrapper around allocate(size_t) to handle allocation errors.
	 */
	void allocate(size_t size, size_t to_alloc)
	{
		size_t i = 0;

		try {
			for (i = 0; i < to_alloc; i++) {
				m_tries[size - 1][m_counts[size-1]++] = allocate(size);
			}
			m_nodes += size * to_alloc;
		}
		catch (std::bad_alloc & e) {
			while (i > 0) {
				m_node_alloc.deallocate(m_tries[size - 1][--m_counts[size - 1]], size);
				i--;
			}
			throw e;
		}
	}

public:
	/**
	 * Constructs an HAMT storage with the given cache size hint.
	 */
	hamt_storage_2(size_t init_size = TRIE_LENGTH) : m_init_size(init_size), m_nodes(0)
	{
		assert(init_size >= 1);

		memset(&m_counts[0], 0, sizeof(size_t)*TRIE_LENGTH);
		memset(&m_tries[0], 0, sizeof(size_t)*TRIE_LENGTH);

		for (size_t i = 1; i <= TRIE_LENGTH; i++) {
			size_t tries = cache_size(i);
			try {
				m_tries[i - 1] = m_ptr_alloc.allocate(tries);
				allocate(i, tries);
			}
			catch (std::bad_alloc & e) {
				clean();
				throw e;
			}
		}
	}

	/**
	 * Frees any memory consumed by the trie cache.
	 */
	void clean()
	{
		for (size_t i = 0; i < TRIE_LENGTH; i++) {
			for (size_t j = 0; j < m_counts[i]; j++) {
				node_ptr ptr = m_tries[i][j];
				deallocate(ptr, i + 1);
			}
			size_t trie_size = cache_size(i + 1);
			if (m_tries[i]) {
				m_ptr_alloc.deallocate(m_tries[i], trie_size);
			}
		}
	}

	/**
	 * Destroys HAMT storage.
	 */
	virtual ~hamt_storage_2()
	{
		clean();
	}

	/**
	 * Allocates a new trie of the given size.
	 */
	virtual node_ptr allocate(size_t size)
	{
		node_type node;

		node_ptr nodes = m_node_alloc.allocate(size);
		node_ptr ptr = nodes;
		for (size_t j = 0; j < size; j++) {
			m_node_alloc.construct(ptr++, node);
		}
		return nodes;
	}

	/**
	 * Allocates a new root trie of the given size.
	 */
	trie_ptr root_allocate(size_t size)
	{
		node_ptr nodes = allocate(size);
		try {
			trie_ptr trie = m_trie_alloc.allocate(1);
			m_trie_alloc.construct(trie, trie_type(nodes, size));
			return trie;
		}
		catch (std::bad_alloc & e) {
			deallocate(nodes, size);
			throw e;
		}
	}

	/**
	 * Frees memory associated with the given array of nodes.
	 */
	void deallocate(node_ptr trie, size_t length)
	{
		m_node_alloc.deallocate(trie, length);
	}

	/**
	 * Frees memory associated with the given trie.
	 */
	void deallocate(trie_ptr trie, size_t length)
	{
		m_node_alloc.deallocate(trie->m_nodes, length);
		m_trie_alloc.deallocate(trie, 1);
	}

	/**
	 * Returns the pointer to a usable trie of the given size.
	 */
	trie_ptr consume(size_t size)
	{
		assert(size > 0);

		if (m_counts[size-1] == 0) {
			size_t to_alloc = cache_size(size);
			allocate(size, to_alloc);
		}
		trie_array & tries = m_tries[size - 1];
		node_ptr nodes = tries[--m_counts[size-1]];
		trie_ptr trie = new trie_type(nodes, size);

		return trie;
	}

	/**
	 * Frees the given trie or puts its storage in the trie cache.
	 */
	void discard(trie_ptr trie)
	{
		discard(trie->m_nodes, trie->length());
		delete trie;
	}

	/**
	 * Frees the given array of nodes or puts it in the trie cache.
	 * @param nodes pointer to the array to free.
	 * @param size number of elements in the array.
	 */
	void discard(node_ptr nodes, size_t size)
	{
		assert(size > 0);

		bool retain = m_counts[size-1] < cache_size(size);

		if (retain) {
			//trie.clear();
			for (size_t i = 0; i < size; i++) {
				nodes[i].set_empty();
				//m_node_alloc.destroy(&nodes[i]);
			}
			m_tries[size - 1][m_counts[size-1]++] = nodes;
		}
		else {
			deallocate(nodes, size);
			m_nodes -= size;
		}
	}

	/**
	 * Removes a trie node at the given position.
	 * This operation is never called for 1-subtries.
	 * 
	 * @param root   a subtrie to shrink.
	 * @param index  the index of the element that will be removed.
	 * @param leftmost a pointer to the leftmost HAMT node
	 * @param rightmost a pointer to the rightmost HAMT node
	 */
	void shrink(node_ptr root, size_t index, node_ptr * leftmost, node_ptr * rightmost)
	{
		trie_type & trie = root->offset();
		trie[index]->set_empty();

		// 1) consume the new trie of size 'old size' - 1
		size_t last_size = root->size();
		assert(last_size > 1);

		node_ptr new_nodes = trie.m_nodes;

		// 2) copy old subtrie to the new location
		subtrie_moved(root, last_size, index, new_nodes, leftmost, rightmost, false);
		//node_ptr res = new_subtrie->grow(old_subtrie, index, last_size);
		trie.shrink(new_nodes, index, last_size);
		trie[last_size - 1]->set_empty();
	}

	/**
	 * Changes the number of nodes in the given trie to the specified one.
	 * @param trie a trie to resize.
	 * @param desired_size the size of the trie upon completion.
	 */
	node_ptr resize(trie_type & trie, size_t desired_size)
	{
		node_type node;

		if (trie.length() >= desired_size) return trie.m_nodes;
		
		if (m_counts[desired_size - 1] == 0) {
			size_t to_alloc = cache_size(desired_size);
			allocate(desired_size, to_alloc);
		}
		trie_array & tries = m_tries[desired_size - 1];
		node_ptr nodes = tries[--m_counts[desired_size - 1]];

		return nodes;
	}

	/**
	 * Moves contents of the n-subtrie to the (n+1)-subtrie.
	 * An empty node is inserted at the specified position.
	 * This operation is never called for TRIE_LENGTH-subtries.
	 * 
	 * @param root   the node referencing the sub-trie to enlarge.
	 * @param index  the position at which an empty node is to be placed.
	 * @param leftmost a pointer to the leftmost HAMT node
	 * @param rightmost a pointer to the rightmost HAMT node
	 * 
	 * @return a pointer to the new array of nodes.
	 */
	node_ptr grow(node_ptr root, size_t index, node_ptr * leftmost, node_ptr * rightmost)
	{
		trie_type & trie = root->offset();
		size_t last_size = root->size();
		assert(last_size > 0);
		assert(last_size <= trie.length());

		node_ptr new_nodes = this->resize(trie, last_size + 1);

		// 2) copy old subtrie to the new location
		subtrie_moved(root, last_size, index, new_nodes, leftmost, rightmost, true);
		//node_ptr res = new_subtrie->grow(old_subtrie, index, last_size);
		node_ptr res = trie.grow(new_nodes, index, last_size);

		if (trie.m_nodes != new_nodes) {
			discard(trie.m_nodes, trie.m_length);
		}
		//for (size_t i = 0; i < trie.length(); i++) {
		//	m_node_alloc.destroy(&trie.m_nodes[i]);
		//}
		//m_node_alloc.deallocate(trie.m_nodes, trie.length());

		trie.m_nodes = new_nodes;
		if (last_size + 1 > trie.m_length) trie.m_length = last_size + 1;
		return res;
	}

	/**
	 * Exchanges contents of two storage instances.
	 */
	void swap(self_type & other)
	{
		//trie_array m_tries[TRIE_LENGTH];
		for (size_t i = 0; i < TRIE_LENGTH; i++) {
			std::swap(m_tries[i], other.m_tries[i]);
/*
			for (size_t j = 0; j < max; j++) {
				node_ptr trie = m_tries[i][j];
				m_tries[i][j] = other.m_tries[i][j];
				other.m_tries[i][j] = trie;
			}
*/
			std::swap(m_counts[i], other.m_counts[i]);
		}
		std::swap(m_init_size, other.m_init_size);
		std::swap(m_nodes, other.m_nodes);
		std::swap(m_nodes, other.m_nodes);
	}

protected:

	void adjust_ptr(node_ptr * ptr, trie_type & trie, size_t i, size_t index, bool grow)
	{
		if (grow) {
			if (i < index) {
				*ptr = trie[i];
			}
			else {
				*ptr = trie[i + 1];
			}
		}
		else {
			if (i <= index) {
				*ptr = trie[i];
			}
			else {
				*ptr = trie[i - 1];
			}
		}
	}

	void adjust_ptr(node_ptr * ptr, node_ptr trie, size_t i, size_t index, bool grow)
	{
		if (grow) {
			if (i < index) {
				*ptr = trie + i;
			}
			else {
				*ptr = trie + i + 1;
			}
		}
		else {
			if (i <= index) {
				*ptr = trie + i;
			}
			else {
				*ptr = trie + i - 1;
			}
		}
	}

	void subtrie_moved(node_ptr root, size_t size, size_t index, node_ptr new_subtrie, node_ptr * leftmost, node_ptr * rightmost, bool grow)
	{
		// update parent for children
		for (size_t i = 0; i < size; i++) {
			node_ptr at = root->offset()[i];
			// update the leftmost and rightmost pointers if needed
			if (at == *leftmost) {
				adjust_ptr(leftmost, new_subtrie, i, index, grow);
			}
			if (at == *rightmost) {
				adjust_ptr(rightmost, new_subtrie, i, index, grow);
			}
		}
	}

};

/**
 * HAMT traits control a number of aspects in HAMT behavior.
 * Root trie size, resizing behavior, storage cache size is controlled.
 */
//template <typename _Allocator>
struct hamt_traits
{
	//typedef _Allocator allocator_type;
	//typedef hamt_storage_2< hamt_traits<_Allocator> > storage_type;
	//typedef hamt_node<typename allocator_type::value_type> node_type;
	//typedef typename node_type::trie_type trie_type;
	//typedef _Node node_type;

	//! Controls root trie size.
	/* 
	 * How many bits are used for the root trie addressing. 
	 * The size of the root trie is 2 ^ root_trie_size_bits.
	 */
	static const size_t ROOT_TRIE_SIZE_BITS = 7;

	//! Controls whether resizing is enabled.
	/**
	 * HAMT insert operations may resize the root trie by a factor of 32.
	 * This behavior may be enabled (true) or diasabled (false) by the value
	 * of this attribute.
	 */
	static const bool RESIZE_ON_INSERT = true;

	static bool resize_on_insert()
	{
		return RESIZE_ON_INSERT;
	}

	//! Controls the resizing strategy.
	/**
	 * Resizing occurs if the ratio of the number of keys to the root trie size
	 * is bigger than the value of this attribute.
	 */
	static const size_t RESIZE_THRESHOLD = 32;

	//! Controls the trie cache size.
	/**
	 * Value of this attribute should always be bigger than 1.
	 */
	static const size_t CACHE_SIZE = 16;

	//typedef typename _Allocator::template rebind<node_type>::other::pointer node_ptr;
	//typedef typename _Allocator::template rebind<trie_type>::other::pointer trie_ptr;
};

/**
 * The Hash Array Mapped Trie (HAMT) is based on the simple notion of hashing
 * a key and storing the key in a trie based on this hash value.
 * Behavior of HAMT is controlled by the traits template parameter.
 *
 * @see Phil Bagwell "Ideal Hash Trees".
 * @see Alexander Stepanov, Meng Lee "The Standard Template Library". This document is also
 * a part of ISO 14882 standard "Programming languages - C++".
 */
template <	typename _Key, 
		typename _Value, 
		class _Hasher = hamt_ns::hash_mix<_Key>, 
		class _KeyEq = hamt_ns::equal_to<_Key>,
		class _Traits = hamt_traits,
		class _Storage = hamt_storage_2< std::allocator<std::pair<_Key, _Value> > > >
class hamt
{
protected:
	typedef _Traits traits_type;
	typedef _Storage storage_type;
public:
	typedef typename storage_type::allocator_type allocator_type;
protected:
	//typedef typename traits_type::storage_type storage_type;
	//typedef hamt_storage_2<_Traits, _Allocator> storage_type;
	typedef hamt_node<typename allocator_type::value_type> node_type;
	typedef node_type * node_ptr;
	typedef hamt_trie<typename allocator_type::value_type> trie_type;
	typedef trie_type * trie_ptr;

        template <class _Tp>
        struct hamt_nonconst_traits;

        template <class _Tp>
        struct hamt_const_traits {
		typedef _Tp value_type;
		typedef const _Tp&  reference;
		typedef const _Tp*  pointer;
		typedef hamt_nonconst_traits<_Tp> hamt_non_const_traits;
        };

        template <class _Tp>
        struct hamt_nonconst_traits {
		typedef _Tp value_type;
		typedef _Tp& reference;
		typedef _Tp* pointer;
		typedef hamt_nonconst_traits<_Tp> hamt_non_const_traits;
        };
        
        /**
         * HAMT bidrectional iterator implementation. The operation end()
         * returns a pointer to the node [rightmost + 1].
         */
        template <class _IterTraits>
        class hamt_iterator
        {
        	//typedef hamt_node<typename _Traits::value_type> node_type;
        	//typedef hamt_node<typename _Traits::value_type> * node_ptr;
        	//typedef hamt_trie<typename _Traits::value_type> trie_type;
        	//typedef hamt_trie<typename _Traits::value_type> * trie_ptr;
        	//typedef typename _NodeTraits::node_type node_type;
        	//typedef typename _NodeTraits::node_ptr node_ptr;
        	//typedef typename _NodeTraits::trie_type trie_type;
        	//typedef typename _NodeTraits::trie_ptr trie_ptr;

        public:
        	typedef std::bidirectional_iterator_tag iterator_category;
        	typedef ptrdiff_t difference_type;
        	typedef typename _IterTraits::reference reference;
        	typedef typename _IterTraits::pointer pointer;
        	typedef typename _IterTraits::value_type value_type;

        	typedef hamt_iterator<_IterTraits> self;

        	/**
        	 * Constructs an iterator pointing at the given HAMT node.
        	 * 
        	 * @param p1      node pointer. The node may have
        	 * any state (empty, bound or a subtrie).
        	 * @param p2      trie containing p1.
        	 * @param at_end equals true iff *this == end().
        	 */
        	hamt_iterator(node_ptr p1, trie_ptr p2, bool at_end)
        	: m_pointer(p1), m_trie(p2), m_at_end(at_end)
        	{
        	}

        	/**
        	 * Constructs an iterator pointing at the given HAMT node.
        	 * Used to construct iterators not equal to end().
        	 * 
        	 * @param p1      node pointer. The node may have
        	 * any state (empty, bound or a subtrie).
        	 * @param p2      trie containing p1.
        	 */
        	hamt_iterator(node_ptr p1, trie_ptr p2)
        	: m_pointer(p1), m_trie(p2), m_at_end(false)
        	{
        	}

        	/**
        	 * Constructs an iterator from the existing non-const iterator.
        	 */
        	hamt_iterator(const hamt_iterator<hamt_nonconst_traits<typename _IterTraits::value_type> > & it)
        	{ 
        		m_pointer = it.m_pointer;
        		m_trie = it.m_trie;
        		m_at_end = it.m_at_end;
        	}

        	/**
        	 * Dereference operator.
        	 */
        	reference operator*() const 
        	{ 
        		return m_pointer->node; 
        	}

        	/**
        	 * Arrow operator.
        	 */
        	pointer operator->() const 
        	{ 
        		return &(operator*()); 
        	}

        	/**
        	 * Returns the hash value for the bound nodes, bitmap for the sub-tries.
        	 */
        	size_t hash() const
        	{
        		return m_pointer->bitmap;
        	}

        	/**
        	 * Postfix increment operator.
        	 */
        	self operator++(int) 
        	{
        		self tmp = *this;
        		increment();

        		//if (m_at_end) {
        		//	m_pointer = tmp.m_pointer + 1;
        		//}

        		return tmp;
        	}

        	/**
        	 * Prefix increment operator.
        	 */
        	self operator++()
        	{
        		//node_ptr prev = m_pointer;
        		increment();

        		//if (m_at_end) {
        		//	m_pointer = ++prev;
        		//}

        		return *this;
        	}

        	/**
        	 * Postfix decrement operator.
        	 */
        	self operator--(int) 
        	{
        		self tmp = *this;
        		decrement();
        		return tmp;
        	}

        	/**
        	 * Prefix decrement operator.
        	 */
        	self operator--()
        	{
        		decrement();

        		return *this;
        	}

        	/**
        	 * Inequality comparison operator.
        	 */
        	bool operator != (const hamt_iterator & right) const
        	{
        		return (m_pointer != right.m_pointer);
        	}

        	/**
        	 * Equality comparison operator.
        	 */
        	bool operator == (const hamt_iterator & right) const
        	{
        		return (m_pointer == right.m_pointer);
        	}

        protected:

        	/**
        	 * Implementation of increment operators.
        	 */
        	void increment()
        	{
        		do {
        			if (m_pointer->is_subtrie()) {
        				m_trie = m_pointer->subtrie;
        				m_pointer = (*m_trie)[0];
        			}
        			else {
        				trie_ptr prev = m_trie;
        				while (m_trie != NULL && m_trie->is_last(m_pointer)) {
        					prev = m_trie;
        					m_trie = m_trie->parent();
        					if (m_trie == NULL) break;
        					m_pointer = m_trie->find(prev);
        				}
        				if (m_trie != NULL) {
        					//m_pointer = m_trie->find(prev);
        					m_pointer++;
        				}
        				else {
        					// == end()
        					++m_pointer;
        					m_trie = prev;
        					m_at_end = true;
        					return;
        				}
        			}
        		} while (m_pointer->is_empty() || m_pointer->is_subtrie());
        	}

        	/**
        	 * Implementation of decrement operators.
        	 */
        	void decrement()
        	{
        		bool end = m_at_end;
        		m_at_end = false;
        /*
        		if (end) {
        			m_pointer--;
        		}
        */
        		do {
        			if (m_trie->is_first(m_pointer) && !end) {
        				trie_ptr trie = m_trie->parent();
        				if (trie == NULL) {
        					//m_at_start = true;
        					return;
        				}
        				m_pointer = trie->find(m_trie);
        				m_trie = trie;
        			}
        			else {
        				--m_pointer;
        				if (m_pointer->is_subtrie()) {
        					node_ptr prev = m_pointer;
        					while (m_pointer->is_subtrie()) {
        						prev = m_pointer;
        						//m_pointer = m_pointer->offset + m_pointer->size() - 1;
        						m_trie = m_pointer->subtrie;
        						m_pointer = (*m_trie)[m_pointer->size() - 1];
        					}
        				}
        			}
        		} while (m_pointer->is_empty() || m_pointer->is_subtrie());
        	}

        public:
        	//! The current position of the iterator in HAMT storage.
        	node_ptr m_pointer;
        	trie_ptr m_trie;
        	//! Equals true iff end() == *this
        	bool m_at_end;
        };

public: 
// typedefs: 
	typedef _Key key_type;
	typedef _Hasher hasher;
	typedef _KeyEq key_equal;

	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type difference_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::value_type value_type;

	//typedef _Allocator allocator_type;

	typedef hamt<_Key, _Value, _Hasher, _KeyEq, _Traits, _Storage> self_type;

	typedef	hamt_iterator<hamt_nonconst_traits<value_type> > iterator;
	typedef hamt_iterator<hamt_const_traits<value_type> > const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	//! Bitness of the value of maximum trie size.
	const static size_t TRIE_BITNESS = 5 + sizeof(size_t) / 64;

	//! The maximum size of the trie.
	const static size_t TRIE_LENGTH = sizeof(size_t) * 8;

	//! Bitness of the value of the root trie size.
	const static size_t INIT_BITS = traits_type::ROOT_TRIE_SIZE_BITS;

	//! Initial size of the root trie.
	const static size_t INIT_SIZE = 1 << INIT_BITS;

// allocation/deallocation: 
protected:

	/**
	 * Performs initialization at construct stage.
	 */
	void init()
	{
		m_bitness = INIT_BITS;
		m_size = INIT_SIZE;
		m_node_count = 0;

		//m_root = new node_type[m_size];
		m_root = m_storage.root_allocate(m_size);

		//m_root[0].set_first();
		//m_root[m_size-1].set_last();
		m_leftmost = m_rightmost = &m_root->m_nodes[m_size];
		m_leftmost_trie = m_rightmost_trie = m_root;
#ifdef _DEBUG
		memset(m_levels, 0, TRIE_LENGTH * sizeof(size_t));
#endif
	}
public:

	/**
	 * Default constructor with optionally specified hasher, key comparator, and traits.
	 *
	 * @param hasher hash function object to use to construct new HAMT.
	 * @param eq key comparison object to use to construct new HAMT.
	 * @param traits traits object to use to construct new HAMT.
	 */
	hamt(const _Hasher & hasher = _Hasher(), const _KeyEq & eq = _KeyEq(), const traits_type & traits = traits_type())
	: m_storage(traits.CACHE_SIZE), m_hasher(hasher), m_eq(eq), m_traits(traits)
	{
		init();
	}

	/**
	 * Constructs HAMT given the range to insert upon constructing and optionally
	 * hasher, key comparator, and traits.
	 *
	 * @param first iterator pointing to the beginning of the range to insert.
	 * @param last iterator pointing to the end of the range to insert.
	 * @param hasher hash function object to use to construct new HAMT.
	 * @param eq key comparison object to use to construct new HAMT.
	 * @param traits traits object to use to construct new HAMT.
	 */
	template <class InputIterator> hamt(InputIterator first, 
						InputIterator last, 
						const _Hasher & hasher = _Hasher(), 
						const _KeyEq & eq = _KeyEq(),
						const traits_type & traits = traits_type())
	: m_storage(traits.CACHE_SIZE), m_hasher(hasher), m_eq(eq), m_traits(traits)
	{
		init();

		try {
			insert(first, last);
		}
		catch (std::bad_alloc & e) {
			clear();
			destroy();
			throw e;
		}
	}

	/**
	 * Copy constructor.
	 *
	 * @param x HAMT instance to clone.
	 */
	hamt(const self_type & x)
	: m_storage(x.m_traits.CACHE_SIZE), m_hasher(x.m_hasher), m_eq(x.m_eq), m_traits(x.m_traits)
	{
		init();

		try {
			insert(x.begin(), x.end());
		}
		catch (std::bad_alloc & e) {
			clear();
			destroy();
			throw e;
		}
	}

	/**
	 * Destructor.
	 */
	~hamt()
	{
		clear();
		destroy();
	}

	/**
	 *  Assignment operator.
	 */
	self_type & operator=(const self_type & x)
	{
		clear();
		m_hasher = x.m_hasher;
		m_eq = x.m_eq;
		m_traits = x.m_traits;
		insert(x.begin(), x.end());

		return *this;
	}

	/**
	 * Exchanges contents of the 'this' and the given trie.
	 *
	 * @param x HAMT to swap contents with.
	 */
	void swap(self_type & x)
	{

		if (this->get_allocator() == x.get_allocator()) {
			// same allocator, swap control information
			m_storage.swap(x.m_storage);

			std::swap(m_leftmost, x.m_leftmost);
			std::swap(m_leftmost_trie, x.m_leftmost_trie);
			std::swap(m_rightmost, x.m_rightmost);
			std::swap(m_rightmost_trie, x.m_rightmost_trie);
			std::swap(m_root, x.m_root);
			std::swap(m_bitness, x.m_bitness);
			std::swap(m_size, x.m_size);
			std::swap(m_node_count, x.m_node_count);
			std::swap(m_hasher, x.m_hasher);
			std::swap(m_eq, x.m_eq);
			std::swap(m_traits, x.m_traits);

#ifdef _DEBUG
			for (size_t i = 0; i < TRIE_LENGTH; i++) {
				std::swap(m_levels[i], x.m_levels[i]);
			}
#endif
		}
		else {
			// different allocator, do multiple assigns
			self_type tmp = *this; *this = x, x = tmp;
		}
	}

// accessors: 

	/**
	 * Returns allocator object.
	 */
	allocator_type get_allocator()
	{
		return m_alloc;
	}

	/**
	 * Returns the hash function object.
	 */
	hasher hash_funct() const 
	{	
		return m_hasher; 
	}

	/**
	 * Returns the equality comparison object.
	 */
	key_equal key_eq() const 
	{ 
		return m_eq; 
	}

	/**
	 * Returns the reverse iterator past the last node. rbegin() == end().
	 */
	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}

	/**
	 * rbegin() version for the const object.
	 * @see rbegin
	 */
	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(end());
	}

	/**
	 * Returns the reverve iterator at the first node. rend() == begin().
	 */
	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}

	/**
	 * rend() version for the const object.
	 * @see rend
	 */
	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin());
	}

	/**
	 * Returns the value associated with the key k. If the
	 * given key is not in the trie, it's inserted with a 
	 * default value associated, which is then returned.
	 *
	 * @param k the key of the entry to find.
	 */
	_Value & operator[](const key_type & k)
	{
		node_ptr val;
		trie_ptr trie;

		size_t res = find(k, &val, &trie);
		if (!res) {
			return val->node.second;
		}
		else {
			iterator it = _insert(value_type(k, _Value()), m_traits.resize_on_insert());
			return it->second;
		}
	}

	/**
	 * Returns a forward iterator pointing at the first node.
	 */
	iterator begin()
	{ 
		return iterator(m_leftmost, m_leftmost_trie);
	}

	/**
	 * Version of begin() for the const object.
	 * @see begin
	 */
	const_iterator begin() const 
	{ 
		return const_iterator(m_leftmost, m_leftmost_trie);
	}
	
	/**
	 * Returns a forward iterator past the last node.
	 */
	iterator end() 
	{ 
		//return iterator(m_rightmost, m_rightmost_trie, true);
		return iterator(&m_root->m_nodes[m_size], m_root, true);
	}

	/**
	 * Version for the const object.
	 */
	const_iterator end() const 
	{ 
		return const_iterator(&m_root->m_nodes[m_size], m_root, true);
		//return const_iterator(m_rightmost, m_rightmost_trie, true);
	}

	/**
	 * Returns true if there are no keys in the trie. 
	 */
	bool empty() const 
	{ 
		return m_node_count == 0; 
	}

	/**
	 * Returns the count of keys in the trie.
	 */
	size_type size() const 
	{ 
		return m_node_count;
	}

	/**
	 * Returns the largest positive value of type difference_type.
	 */
	size_type max_size() const 
	{ 
		return size_type(-1); 
	}

// insert/erase: 

	/**
	 * Inserts x iff there is no element with key equal to the key of x
	 * @param x a (key, value) pair to insert.
	 * @return the (iterator, bool) pair. The iterator component points to 
	 * the element with key equal to the key of x. The bool component indicates
	 * whether the insert actually took place or the given key is already in the tree.
	 */
	std::pair<iterator, bool> insert(const value_type & x)
	{
		const iterator & res = _insert(x, m_traits.resize_on_insert());
		return std::pair<iterator, bool>(res, res != end());
	}

	std::pair<iterator, bool> insert_noresize(const value_type & x)
	{
		const iterator & res = _insert(x, false);
		return std::pair<iterator, bool>(res, res != end());
	}

	/**
	 * Inserts x iff there is no element with key equal to the key of x
	 * @param p is a hint pointing at where the insert should start to search.
	 * @param x a (key, value) pair to insert.
	 * @return the iterator pointing at the element with key equal to the key of x. 
	 */
	iterator insert(iterator p, const value_type & x)
	{
		p;
		return _insert(x, m_traits.resize_on_insert());
	}

	/**
	 * Inserts the elements in the range [first, last) to HAMT.
	 */
	template <class InputIterator> 
	void insert(InputIterator first, InputIterator last)
	{
		for ( ; first != last; ++first) {
			insert(*first);
		}
	}

	/**
	 * Erases the element pointed at by the given iterator.
	 */
	void erase(iterator q)
	{
		erase(q->first, q.hash());
	}

	/**
	 * Erases the node in HAMT with the key equal to k.
	 * 
	 */
	size_type erase(const key_type & k, size_t hash = 0)
	{
		if (hash == 0) hash = m_hasher(k);
	
		size_t root_ind = LOWER(hash, m_bitness);
		node_ptr first = (*m_root)[root_ind];
		if (first->is_empty()) return 0;

		size_t last_bit = m_bitness;
		size_t level = 0;

		trie_ptr trie = m_root;
		node_ptr second = first;
		size_t offset = 0;
		size_t index = 0;

		while (second->is_subtrie()) {
			index = (hash >> last_bit) & 0x1F;

			size_t one = second->bitmap & (1 << index);
			if (one) {
				last_bit += TRIE_BITNESS;

				if (last_bit >= TRIE_LENGTH) {
					level++;
					hash = m_hasher(k, level);
					last_bit = 0;
				}
				offset = CTPop( LOWER(second->bitmap, index));
				first = second;
				trie = second->subtrie;
				second = second->offset()[offset];
			}
			else {
				return 0;
			}
		}
		
		--m_node_count;

		// check leftmost and rightmost
		if (second == m_leftmost) {
			if (m_node_count == 0) {
				m_leftmost = m_rightmost = &m_root->m_nodes[m_size];
				m_leftmost_trie = m_rightmost_trie = m_root;
			}
			else {
				//iterator it = this->upper_bound(second->key());
				iterator it = iterator(second, trie);
				++it;
				m_leftmost = it.m_pointer;
				m_leftmost_trie = it.m_trie;
			}
		}
		if (second == m_rightmost) {
			if (m_node_count == 0) {
				m_leftmost = m_rightmost = &m_root->m_nodes[m_size];
				m_leftmost_trie = m_rightmost_trie = m_root;
			}
			else {
				iterator it = iterator(second, trie);
				--it;
				m_rightmost = it.m_pointer;
				m_rightmost_trie = it.m_trie;
			}
		}

		if (first == second) {
			second->set_empty();
			return 1;
		}
		
		size_t subtrie_size = first->size();
		if (subtrie_size == 2) {
			// find the node that will remain
			size_t ind = 1 - offset;
			node_ptr remain = first->offset()[ind];

			if (remain->is_subtrie()) {
				//second->m_key = remain;
				//second->m_value = remain;
				m_storage.shrink(first, offset, &m_leftmost, &m_rightmost);
				first->bitmap = first->bitmap & ~(1 << index);
			}
			else {
				// will not modify parent
				trie_ptr offset = first->subtrie;
				first->node = remain->node;
				first->bitmap = remain->bitmap;
				//first->subtrie.clear();
				if (m_leftmost == remain) {
					m_leftmost = first;
					m_leftmost_trie = trie->parent();

				}
				if (m_rightmost == remain) {
					m_rightmost = first;
					m_rightmost_trie = trie->parent();
				}
				m_storage.discard(offset);
				first->subtrie = NULL;
			}
			//second->set_empty();
		}
		else if (subtrie_size == 1) {
			key_type key_copy = second->key();
			//check_empty(second, key_copy, hash, last_bit, level);
			check_empty(first->subtrie, key_copy, hash, last_bit, level);
			//second->first.m_key = 0;
			//second->second.m_value = 0;
		}
		else {
			m_storage.shrink(first, offset, &m_leftmost, &m_rightmost);
			//second->first.m_key = 0;
			//second->second.m_value = 0;
			first->bitmap = first->bitmap & ~(1 << index);
		}
		return 1;
	}

	/**
	 * Erases all the elements in the range [first, last).
	 */
	void erase(iterator first, iterator last)
	{
		while (first != last) {
			erase(first++);
			//first++;
		}
	}

// hamt operations:

	/**
	 * Returns an iterator pointing at the element with the key equal to k, or end() if
	 * such an element is not found.
	 * 
	 * @param k      a key to find.
	 * 
	 * @return an iterator at the found node, or end() if the key was not found.
	 */
	iterator find(const key_type & k)
	{
		node_ptr p;
		trie_ptr t;

		size_t res = find(k, &p, &t);
		return !res ? iterator(p, t) : end();
	}

	/**
	 * Find operation on the const HAMT object.
	 */
	const_iterator find(const key_type& x) const
	{
		node_ptr p;
		trie_ptr t;

		size_t res = find(x, &p, &t);
		return !res ? const_iterator(p, t) : end();
	}

	/**
	 * Returns the number of elements with key equal to k.
	 * 
	 * @param k      a key to find.
	 * 
	 * @return 1, if a key was found, 0 otherwise.
	 */
	size_type count(const key_type & k) const
	{
		node_ptr p = NULL;
		trie_ptr t;

		find(k, &p, &t);
		return (p != NULL) ? 1 : 0;
	}

	/**
	 * Returns the iterator pair (find(k), ++find(k)). If the given 
	 * key is not in the tree, pair (end(), end()) is returned.
	 * 
	 * @param k      a key to search the range for
	 * 
	 * @return a pair of iterators denoting the equal range.
	 */
	std::pair<iterator, iterator> equal_range(const key_type& k)
	{
		iterator e = find(k);
		if (e != end()) {
			iterator first = e;
			return std::pair<iterator, iterator>(first, ++e);
		}
		else {
			return std::pair<iterator, iterator>(e, e);
		}
	}

	/**
	 * The equal_range version for the const tree.
	 * @see equal_range
	 */
	std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const
	{
		const_iterator e = find(x);
		if (e != end()) {
			return std::pair<const_iterator, const_iterator>(e, e++);
		}
		else {
			return std::pair<const_iterator, const_iterator>(e, e);
		}
	}

	/**
	 * Removes all the elements from the tree, freeing associated memory. 
	 */
	void clear()
	{
		while (m_node_count > 0) {
			erase(iterator(m_leftmost, m_leftmost_trie));
		}
	}

protected:

	/**
	 * Implementation of the insert(...) operations.
	 * 
	 * @param x a (key, value) pair to insert.
	 * 
	 * @return the iterator pointing at the newly inserted node.
	 */
	iterator _insert(const value_type & x, bool _resize)
	{
		const key_type & key = x.first;
		
		// check threshold
		if (_resize && m_node_count / m_size > m_traits.RESIZE_THRESHOLD) resize();

		size_t hash = m_hasher(key);
		size_t first_hash = hash;

		// bitmap contains the hash for the bound nodes
		bool leftmost = false;
		bool rightmost = false;

		size_t root_ind = LOWER(hash, m_bitness);

		node_ptr node = (*m_root)[root_ind];
		trie_ptr trie = m_root;

		if (node->is_empty()) {
			node->node = x;
			node->bitmap = hash;
			++m_node_count;
			// here we are sure there won't be collision with l/r - most
			init_ends(leftmost, rightmost, hash);
			update_ends(leftmost, rightmost, node, trie);
			return iterator(node, trie);
		}

		size_t last_bit = m_bitness;
		size_t level = 0;
		size_t rehash_with_level = 0;
	
		for ( ; ; ) {
			if (!node->is_subtrie()) {
				// this is hash entry, compare keys (first hash compare)
				bool same_hash = level == 0 ? hash == node->bitmap : true;
				if (same_hash && m_eq(key, node->key())) {
					// do not replace
					//node->node.second = x.second;
					return iterator(node, trie);
				}
				else {
					size_t key2;
					
					//init_ends(leftmost, rightmost, key, first_hash);
					//key2 = m_hasher(first->key());
					key2 = node->bitmap;
					if (rehash_with_level != 0) {
						key2 = m_hasher(node->key(), rehash_with_level);
						rehash_with_level = 0;
					}

					for ( ; ; ) {
						size_t keypart = (hash >> last_bit) & 0x1F;
						size_t keypart2 = (key2 >> last_bit) & 0x1F;
						if (keypart == keypart2) {
							// Still equal, build one-node subtrie and continue downward
							trie_ptr newtrie = m_storage.consume(1);
							newtrie->set_parent(trie);
							assert (node->key() != key_type());
							*(*newtrie)[0] = *node;
							node->set_empty();
							node->set_subtrie(newtrie);
						
							if (m_leftmost == node) {
								m_leftmost = (*newtrie)[0];
								m_leftmost_trie = newtrie;
							}
							if (m_rightmost == node) {
								m_rightmost = (*newtrie)[0];
								m_rightmost_trie = newtrie;
							}
							node->bitmap = 1 << keypart;
							//node->set_subtrie(newtrie);
							node = (*newtrie)[0];
							trie = newtrie;

							last_bit += TRIE_BITNESS;
							if (last_bit >= TRIE_LENGTH) {
								// Exceeded TRIE_LENGTH bits: rehash
								level++;
								hash = m_hasher(key, level);
								key2 = m_hasher(node->key(), level);
								rehash_with_level = level;
								last_bit = 0;
							}
							
						} else {
							// partitioned: allocate two-node subtrie
							trie_ptr newtrie = m_storage.consume(2);
							node_ptr newnode1 = (*newtrie)[0];
							node_ptr newnode2 = (*newtrie)[1];
							node_ptr res = NULL;

							// Copy nodes into subtrie based on order 
							if ( keypart2 < keypart ) {
								*newnode1 = *node;
								newnode1->subtrie = NULL;
								newnode2->subtrie = NULL;
								newnode2->node = x;
								newnode2->bitmap = first_hash;
								res = newnode2;
								if (m_leftmost == node) {
									m_leftmost = newnode1;
									m_leftmost_trie = newtrie;
								}
								if (m_rightmost == node) {
									m_rightmost = newnode1;
									m_rightmost_trie = newtrie;
								}
								//newnode2->m_key = key;
								//newnode2->m_value = x.m_value;
								//newnode2->m_hash = hash;
							} else {
								//newnode1->m_key = key;
								//newnode1->m_value = value;
								//newnode1->m_hash = hash;
								newnode1->node = x;
								newnode1->bitmap = first_hash;
								newnode1->subtrie = NULL;
								*newnode2 = *node;
								newnode2->subtrie = NULL;
								//newnode2->node = first->node;
								res = newnode1;
								if (m_leftmost == node) {
									m_leftmost = newnode2;
									m_leftmost_trie = newtrie;
								}
								if (m_rightmost == node) {
									m_rightmost = newnode2;
									m_rightmost_trie = newtrie;
								}
							}

							// Set bits in bitmap corresponding to keys
							node->bitmap = (1UL<<keypart) | (1UL<<keypart2);
							node->set_empty();
							node->set_subtrie(newtrie);
							newtrie->set_parent(trie);

							++m_node_count;
							init_ends(leftmost, rightmost, key, first_hash);
							update_ends(leftmost, rightmost, res, newtrie);
							return iterator(res, newtrie);
						}
					}
				}
			}
		
			// subtrie - need to find non-taken node

			size_t index = (hash >> last_bit) & 0x1F;
			if (!(node->bitmap & (1 << index))) {
				// add node here
				size_t off = CTPop( LOWER(node->bitmap, index));
				//size_t size = CTPop<size_t>(node->bitmap);
				// underlying node storage must ensure there's enough space
				node_ptr newnode = m_storage.grow(node, off, &m_leftmost, &m_rightmost);
				newnode->node = x;
				newnode->bitmap = first_hash;
				newnode->subtrie = NULL;
				node->subtrie->set_parent(trie);
				//if (off == 0) newnode->set_first();
				//if (size == off) newnode->set_last();
				//newnode->m_key = key;
				//newnode->m_value = value;
				//newnode->m_hash = hash;
				
				//first->m_subtrie_size++;
				node->bitmap |= (1 << index);
				++m_node_count;
				//first->m_offset = newnode;
				//first->set_subtrie(true);
				init_ends(leftmost, rightmost, key, first_hash);
				update_ends(leftmost, rightmost, newnode, node->subtrie);
				return iterator(newnode, node->subtrie);
			}
			size_t offset = CTPop( LOWER(node->bitmap, index));
	#ifdef _DEBUG
			assert(offset < TRIE_LENGTH);
	#endif		
			trie = node->subtrie;
			node = node->offset()[offset];

			last_bit += TRIE_BITNESS;
			if (last_bit >= TRIE_LENGTH) {
				level++;
				hash = m_hasher(key, level);
				rehash_with_level = level;
				last_bit = 0;
				//init_ends(leftmost, rightmost, hash, level);
			}
		}
	}

	/**
	 * Updates data about leftmost/rightmost node upon insert.
	 */
	void init_ends(bool & left, bool & right, size_t hash)
	{
		if (m_leftmost == &m_root->m_nodes[m_size]) {
			left = true;
			right = true;
			return;
		}

		size_t last_bit = m_bitness;

		size_t min_hash = m_leftmost->bitmap;
		size_t max_hash = m_rightmost->bitmap;
		bool first = true;
		bool need1 = true;
		bool need2 = true;

		// no rehashing needed here - we know the hashes are different
		while (last_bit < TRIE_LENGTH) {
			size_t part = first ? m_bitness : TRIE_BITNESS;
			size_t h1 = LOWER(hash, part);
			size_t h2 = LOWER(min_hash, part);
			size_t h3 = LOWER(max_hash, part);

			if (need1 && h1 < h2) {
				left = true;
				break;
			}

			if (need2 && h1 > h3) {
				right = true;
				break;
			}
			need1 = h1 == h2 && need1;
			need2 = h1 == h3 && need2;

			if (!need1 && !need2) break;

			hash >>= part;
			min_hash >>= part;
			max_hash >>= part;
			last_bit += TRIE_BITNESS;
			first = false;
		}
	}

	/**
	 * Updates data about leftmost/rightmost node upon insert.
	 */
	void init_ends(bool & left, bool & right, const key_type & key, size_t hash)
	{
		if (m_leftmost == &m_root->m_nodes[m_size]) {
			left = true;
			right = true;
			return;
		}

		size_t last_bit = m_bitness;

		size_t min_hash = m_leftmost->bitmap;
		size_t max_hash = m_rightmost->bitmap;
		bool first = true;
		bool need1 = true;
		bool need2 = true;

		size_t level = 0;
		for ( ; ; ) {
			size_t part = first ? m_bitness : TRIE_BITNESS;
			size_t h1 = LOWER(hash, part);
			size_t h2 = LOWER(min_hash, part);
			size_t h3 = LOWER(max_hash, part);

			if (need1 && h1 < h2) {
				left = true;
				break;
			}

			if (need2 && h1 > h3) {
				right = true;
				break;
			}
			need1 &= (h1 == h2);
			need2 &= (h1 == h3);

			if (!need1 && !need2) break;

			hash >>= part;
			min_hash >>= part;
			max_hash >>= part;
			last_bit += TRIE_BITNESS;
			first = false;

			if (last_bit >= TRIE_LENGTH) {
				level++;
				hash = m_hasher(key, level);
				min_hash = m_hasher(m_leftmost->key(), level);
				max_hash = m_hasher(m_rightmost->key(), level);
				last_bit = 0;
			}
		}
	}

	void update_ends(bool left, bool right, node_ptr node, trie_ptr trie)
	{
		if (left) {
			m_leftmost = node;
			m_leftmost_trie = trie;
		}
		if (right) {
			m_rightmost = node;
			m_rightmost_trie = trie;
		}
	}

	/**
	 * Implementation of the find operation.
	 * 
	 * @param key        a key to find.
	 * @param ptr [out]  a pointer to the searched node, on return contains a valid
	 *                   node pointer if the given key was found, or stays unchanged otherwise.
	 * @param trie [out] a pointer to the searched trie, on return contains a valid
	 *                   trie pointer if the given key was found, or stays unchanged otherwise.
	 * 
	 * @return A number of found elements (either 1 or 0).
	 */
	size_t find(const key_type & key, node_ptr * ptr, trie_ptr * trie) const
	{
		size_t hash = m_hasher(key);
	
		size_t root_ind = LOWER(hash, m_bitness);
		node_ptr first = (*m_root)[root_ind];
		if (first->is_empty()) return 1;

		size_t last_bit = m_bitness;
		size_t level = 0;
		*trie = m_root;
		
#ifdef _DEBUG
		size_t level2 = 0;
#endif

		while (first->is_subtrie()) {
#ifdef _DEBUG
			level2++;
#endif
			size_t index = (hash >> last_bit) & 0x1F;

			size_t one = first->bitmap & (1 << index);
			if (one) {
				last_bit += TRIE_BITNESS;

				if (last_bit >= TRIE_LENGTH) {
					level++;
					hash = m_hasher(key, level);
					last_bit = 0;
				}
				size_t offset = CTPop( LOWER(first->bitmap, index));
				first = first->offset()[offset];
				*trie = first->subtrie;
			}
			else {
				return 1;
			}
		}
		//if (!first->is_empty() && m_eq(key, first->key())) {
		if (!first->is_empty() && m_eq(key, first->key())) {
#ifdef _DEBUG
			subtrie_level(level2);
#endif
			*ptr = first;
			return 0;
		}
		else {
			return 1;
		}
	}

	/**
	 * Resizes the root storage of the given HAMT by the factor of TRIE_LENGTH.
	 * No full rehashing occurs during resize, only root subtrie collisions 
	 * are rehashed. Depending on the HAMT traits, this may or may not be
	 * called during insert operation.
	 */
	void resize()
	{
		node_type node;

		trie_ptr new_root = m_storage.root_allocate(TRIE_LENGTH * m_size);

		try {
			m_bitness += TRIE_BITNESS;
			
			for (size_t i = 0 ; i < m_size; i++) {
				node_ptr n = (*m_root)[i];
				if (!n->is_empty()) {
					if (!n->is_subtrie()) {
						insert2(n, new_root);
					}
					else {
						size_t size = CTPop<size_t>(n->bitmap);
						for (size_t j = 0 ; j < size; j++) {
							node_ptr node = n->offset()[j];
							insert2(node, new_root);
						}
						//m_storage.discard(n.offset, size);
					}
				}
			}
		}
		catch (std::bad_alloc & e) {
			m_bitness -= TRIE_BITNESS;
			m_storage.deallocate(new_root, TRIE_LENGTH * m_size);
			throw e;
		}

		for (size_t i = 0 ; i < m_size; i++) {
			node_ptr n = (*m_root)[i];
			if (n->is_subtrie()) {
				//size_t size = CTPop<size_t>(n->bitmap);
				m_storage.discard(n->subtrie);
			}
		}

		//for (size_t i = 0; i < m_size; i++) {
		//	m_storage.m_node_alloc.destroy(&m_root[i]);
		//}
		m_storage.deallocate(m_root, m_size);
		m_root = new_root;
		m_size *= TRIE_LENGTH;

		// get new l/r-most
		iterator i1 ((*m_root)[0], m_root);
		iterator i2 ((*m_root)[m_size - 1], m_root);

		if ( (*m_root)[0]->is_empty() || (*m_root)[0]->is_subtrie()) {
			i1++;
		}

		if ( (*m_root)[m_size - 1]->is_empty() || (*m_root)[m_size - 1]->is_subtrie()) {
			i2--;
		}

		if (i1 != end()) {
			m_leftmost = i1.m_pointer;
			m_leftmost_trie = i1.m_trie;
		}
		if (!i2.m_pointer->is_empty()) {
			m_rightmost = i2.m_pointer;
			m_rightmost_trie = i2.m_trie;
		}
	}

	/**
	 * Cleans empty 1-subtrie recursively until a non-empty subtrie is encountered.
	 */
	void check_empty(trie_ptr trie, const key_type & key, size_t hash, size_t last_bit, size_t level)
	{
		trie_ptr parent_trie = trie->parent();
		if (!parent_trie) return;

		node_ptr parent = parent_trie->find(trie);

		if (last_bit == 0) {
			level--;
			if (level) last_bit = (TRIE_LENGTH / TRIE_BITNESS) * TRIE_BITNESS;
			else last_bit = ((TRIE_LENGTH - 1 - m_bitness) / TRIE_BITNESS) * TRIE_BITNESS + m_bitness;
			hash = (level) ? m_hasher(key, level) : m_hasher(key);
		}
		else {
			if (level == 0 && last_bit == m_bitness) last_bit = 0;
			else last_bit -= TRIE_BITNESS;
		}

		size_t index = (hash >> last_bit) & 0x1F;
		size_t bitmap = parent->bitmap & ~(1 << index);

		if (bitmap == 0) {
			m_storage.discard(trie);
			parent->bitmap = 0;
			parent->subtrie = NULL;
			if (parent) check_empty(parent_trie, key, hash, last_bit, level);
		}
		else {
			size_t offset = CTPop( LOWER(parent->bitmap, index));
			m_storage.shrink(parent, offset, &m_leftmost, &m_rightmost);
			parent->bitmap = bitmap;
		}
	}

	/**
	 * Returns the index into the root subtrie, from which the given 
	 * node's insertion started.
	 */
	size_t get_root_ind(node_ptr node)
	{
		if (node->is_subtrie()) {
			node_ptr node2 = node->offset()[0];
			return get_root_ind(node2);
		}
		size_t hash = m_hasher(node->key());
		return LOWER(hash, m_bitness);
	}

	/**
	 * Inserts elements during resizing.
	 */
	void insert2(node_ptr node, trie_ptr new_root)
	{
		size_t root_ind = get_root_ind(node);
		node_ptr newnode = (*new_root)[root_ind];

		// must be free
		assert(newnode->is_empty());

       		//new_root[root_ind] = *node;
       		newnode->node = node->node;
       		newnode->bitmap = node->bitmap;
       		newnode->subtrie = node->subtrie;

       		if (node == m_leftmost) m_leftmost = newnode;
       		if (node == m_rightmost) m_rightmost = newnode;
       		// update the pointer to parent for second-level subtries
       		if (node->is_subtrie()) {
       			trie_ptr trie = node->subtrie;
       			trie->set_parent(new_root);
       		}
	}

	/**
	 * Deallocates root trie.
	 */
	void destroy()
	{
		m_storage.deallocate(m_root, m_size);
	}

	//! root trie
	trie_ptr m_root;
	
	//! associated storage
	storage_type m_storage;

	//! The number of bits needed to index into the root subtrie. 
	size_type m_bitness;

	//! The size of the root subtrie.
	size_type m_size;

	//! The count of inserted keys.
	size_type m_node_count;

	//hamt_keytraits_pearson_lookup<key_type> m_hasher;

	//! Hash functor.
	hasher m_hasher;

	//! Key equality comparison object.
	key_equal m_eq;

	//! The leftmost node.
	node_ptr m_leftmost;

	//! The trie where the leftmost node resides.
	trie_ptr m_leftmost_trie;

	//! The rightmost node.
	node_ptr m_rightmost;

	//! The trie where the rightmost node resides.
	trie_ptr m_rightmost_trie;

	//! HAMT traits.
	traits_type m_traits;

	//! Allocator object.
	allocator_type m_alloc;

#ifdef _DEBUG
	//! Signals that a node was found by find operation at the depth n.
	void subtrie_level(size_t n) const { m_levels[n]++; }

	//! Stats about nodes depth.
	mutable size_t m_levels[TRIE_LENGTH];

/*
	friend std::ostream & operator << (std::ostream & s, const self_type & x)
	{
		for (size_t i = 0 ; i < x.size(); i++) {
			node_ptr n = (*x.m_root)[i];
			s << "Entry #" << (unsigned int)i << ": ";
			s << *n;
		}
		return s;
	}
*/
#endif
}; 

template <class Key, class T, class Compare, class _Traits, class _Allocator> 
bool operator==(const hamt<Key, T, Compare, _Traits, _Allocator>& x, const hamt<Key, T, Compare, _Traits, _Allocator>& y)
{
	return (x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin()));
}

template <class Key, class T, class Compare, class _Traits, class _Allocator>
bool operator<(const hamt<Key, T, Compare, _Traits, _Allocator>& x, const hamt<Key, T, Compare, _Traits, _Allocator>& y)
{
	return (std::lexicographical_compare(x.begin(), x.end(), x.begin(), x.end()));
}

/*
#ifdef _DEBUG
template <typename _Key, typename _Value>
std::ostream & operator << (std::ostream & s, const hamt_node<_Key, _Value> & x)
{
	if (x.is_empty()) {
		s << "<empty>" << std::endl;
	}
	else {
		if (x.is_subtrie()) {
			size_t size = CTPop(x.bitmap);
			s << " -->(" << (unsigned int)size << ")" << std::endl;
			for (size_t i = 0 ; i < size; i++) {
				s << *(*x.subtrie)[i];
			}
		}
		else {
			s << "(" << x.key() << "," << x.value() << ")" << std::endl; 
		}
	}
	return s;
}
#endif
*/

#undef LOWER

}

#endif // __AEON_STDLIB_HAMT_HPP_INCLUDED__

