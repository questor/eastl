
#ifndef __EASTL_EXTRA_PHANTOM_TYPE_H__
#define __EASTL_EXTRA_PHANTOM_TYPE_H__

//taken from here:
// http://blog.demofox.org/2015/02/05/getting-strongly-typed-typedefs-using-phantom-types/

/* How to use:
   // some "tag types" to use as phantom types
   struct SModelID {};
   struct SAnimationID {};

   // our strongly typed typedefs, which both are really just unsigned ints
   typedef SStronglyTypedType<unsigned int, SModelID> TModelID;
   typedef SStronglyTypedType<unsigned int, SAnimationID> TAnimationID;

   TModelID GetModelID () {
      TModelID ret(3);
      return ret;
   }
 
   void SetAnimationID (TAnimationID animId) {
      // do something with the animation id
   }
 
   int main(int argc, char **argv) {
      TModelID id = GetModelID();
      SetAnimationID(id);
      return 0;
   }
*/

template <typename T, typename PHANTOM_TYPE>
struct StronglyTypedType {
public:
    StronglyTypedType (T value) 
    : m_value(value) { }
    inline T& Value () { return &m_value; }
private:
    T m_value;
};

#endif	//#ifndef __EASTL_EXTRA_PHANTOM_TYPE_H__
