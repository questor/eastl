
#ifndef __EASTL_STACK_H__
#define __EASTL_STACK_H__

namespace eastl {

template<typename T, typename _Container = eastl::vector<T> > class stack;
template<typename T, typename _Container> bool operator==(const stack<T, _Container>&,
                                                          const stack<T, _Container>&);
template<typename T, typename _Container> bool operator< (const stack<T, _Container>&,
                                                          const stack<T, _Container>&);

template<typename T, typename _Container> class stack {
   friend bool operator==<T,_Container>(const stack&, const stack&);
   friend bool operator< <T,_Container>(const stack&, const stack&);
public:
   typedef _Container container_type;
   typedef typename container_type::value_type value_type;
   typedef typename container_type::size_type size_type;
   typedef typename container_type::reference reference;
   typedef typename container_type::const_reference const_reference;

protected:
   container_type c;

public:
   explicit stack(const container_type &_c = container_type())
      : c(_c) {}
   bool empty() const {
      return c.empty();
   }
   size_type size() const {
      return c.size();
   }
   reference top() {
      return c.back();
   }
   const_reference top() const {
      return c.back();
   }
   void push(const value_type &_x) {
      c.pushBack(_x);
   }
   void pop() {
      c.popBack();
   }
};
template<typename T, typename _Container> inline bool operator==(const stack<T, _Container>&x,
                                                                 const stack<T, _Container>&y) {
   return x.c == y.c;
}
template<typename T, typename _Container> inline bool operator< (const stack<T, _Container>&x,
                                                                 const stack<T, _Container>&y) {
   return x.c < y.c;
}
template<typename T, typename _Container> inline bool operator!=(const stack<T, _Container>&x,
                                                                 const stack<T, _Container>&y) {
   return !(x == y);
}
template<typename T, typename _Container> inline bool operator> (const stack<T, _Container>&x,
                                                                 const stack<T, _Container>&y) {
   return y < x;
}
template<typename T, typename _Container> inline bool operator>=(const stack<T, _Container>&x,
                                                                 const stack<T, _Container>&y) {
   return !(x < y);
}
template<typename T, typename _Container> inline bool operator<=(const stack<T, _Container>&x,
                                                                 const stack<T, _Container>&y) {
   return !(y < x);
}

}  //namespace eastl

#endif

