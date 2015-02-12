
#ifndef __EASTL_QUEUE_H__
#define __EASTL_QUEUE_H__

namespace eastl {

template<typename T, typename _Container = eastl::deque<T> > class queue;
template<typename T, typename _Container> inline bool operator==(const queue<T, _Container>&,
                                                                 const queue<T, _Container>&);
template<typename T, typename _Container> inline bool operator< (const queue<T, _Container>&,
                                                                 const queue<T, _Container>&);
template<typename T, typename _Container> class queue {
   friend bool operator==(const queue&, const queue&);
   friend bool operator< (const queue&, const queue&);
public:
   typedef _Container container_type;
   typedef typename container_type::value_type value_type;
   typedef typename container_type::size_type size_type;
   typedef typename container_type::reference reference;
   typedef typename container_type::const_reference const_reference;

protected:
   container_type c;

public:
   explicit queue(const container_type &_c = container_type())
      : c(_c) {}
   bool empty() const {
      return c.empty();
   }
   size_type size() const {
      return c.size();
   }
   reference front() {
      return c.front();
   }
   const_reference front() const {
      return c.front();
   }
   reference back() {
      return c.back();
   }
   const_reference back() const {
      return c.back();
   }
   void push(const value_type &_x) {
      c.pushBack(_x);
   }
   void pop() {
      c.popFront();
   }
};
template<typename T, typename _Container> inline bool operator==(const queue<T, _Container>&x,
                                                                 const queue<T, _Container>&y) {
   return x.c == y.c;
}
template<typename T, typename _Container> inline bool operator< (const queue<T, _Container>&x,
                                                                 const queue<T, _Container>&y) {
   return x.c < y.c;
}
template<typename T, typename _Container> inline bool operator!=(const queue<T, _Container>&x,
                                                                 const queue<T, _Container>&y) {
   return !(x == y);
}
template<typename T, typename _Container> inline bool operator> (const queue<T, _Container>&x,
                                                                 const queue<T, _Container>&y) {
   return y < x;
}
template<typename T, typename _Container> inline bool operator>=(const queue<T, _Container>&x,
                                                                 const queue<T, _Container>&y) {
   return !(x < y);
}
template<typename T, typename _Container> inline bool operator<=(const queue<T, _Container>&x,
                                                                 const queue<T, _Container>&y) {
   return !(y < x);
}

}  //namespace eastl

#endif
