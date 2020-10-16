
#ifndef __UTILS_PHANTOMTYPE_H__
#define __UTILS_PHANTOMTYPE_H__

// Type Phantoming:
// https://blog.demofox.org/2015/02/05/getting-strongly-typed-typedefs-using-phantom-types/

// struct TagType1 {};
// struct TagType2 {};
// typedef PhantomType<uint32_t, TagType1> Type1;
// typedef PhantomType<uint32_t, TagType2> Type2;


template<typename BaseType, typename PhantomType> struct PhantomType {
public:
   BaseType(BaseType value) : mValue(value) {}
   inline BaseType &value() { return &mValue; }
private:
   BaseType mValue;
};

#endif   //#ifndef __UTILS_PHANTOMTYPE_H__
