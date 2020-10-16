
#pragma once

#include <cinttypes>
#include <stdio.h>
#include "debug.h"
#include <type_traits>
#include <string>
#include <string.h>
#include <vector>

#include "eventhandling/stringholder.h"

// based on https://yave.handmade.network/blogs/p/2723-how_media_molecule_does_serialization#13476


//these are helpers to be used in the generated code, contain version check already at 
// caller side (might be better for optimization) and is needed for counting the size of
// a message
#define SERIALIZATION_COUNT_POD(_serializer, _version, _sizeVar, _datatype) \
               if(_serializer.dataVersion>=_version) { \
                  _sizeVar+=sizeof(_datatype); \
               }
#define SERIALIZATION_COUNT_STRING(_serializer, _version, _sizeVar, _var) \
               if(_serializer.dataVersion>=_version) { \
                  _sizeVar+=sizeof(_var.length) + _var.length; \
               }

#define SERIALIZATION_ADD(_serializer, _version, _var, _defaultValue) \
               if(_serializer.dataVersion>=_version) { \
                  _serializer.addPreprocessed(_var); \
               } else { \
                  _var = _defaultValue; \
               }


// these are the helpers which ignore versioning (can be configured in the json of a service)
#define SERIALIZATION_COUNT_POD_NO_VERSIONING(_serializer, _version, _sizeVar, _datatype) \
                  _sizeVar+=sizeof(_datatype);

#define SERIALIZATION_COUNT_STRING_NO_VERSIONING(_serializer, _version, _sizeVar, _var) \
                  _sizeVar+=sizeof(_var.length) + _var.length;

#define SERIALIZATION_ADD_NO_VERSIONING(_serializer, _version, _var, _defaultValue) \
               _serializer.addPreprocessed(_var);

#define DEBUG_SERIALIZATION 1

// ============================================================================

struct FixedMemReadSerializer {
private:
   template<typename Type>void serialize(std::true_type, Type &datum) {
#if DEBUG_SERIALIZATION
      bytesCount += sizeof(Type);
#endif   
      memcpy(&datum, memPtr, sizeof(Type));
      memPtr += sizeof(Type);
   }
   template<>void serialize(std::true_type, StringHolder &datum) {
      uint32_t len;
      serialize(std::true_type(), len);
      datum.length = len;
#if DEBUG_SERIALIZATION
      bytesCount += len;
#endif
      datum.data = (char*)memPtr;
      memPtr += len;
   }
   template<typename Type>void serialize(std::false_type, Type &datum) {
      datum.serialize(*this);
   }
   template<>void serialize(std::false_type, std::string &datum) {
      uint32_t len;
      serialize(std::true_type(), len);
      datum.resize(len+1);                    //no +1 for termination needed in c++11 and later, but to be sure...
#if DEBUG_SERIALIZATION
      bytesCount += len;
#endif   
      memcpy(&datum[0], memPtr, len);
      memPtr += len;
   }
public:
   static const bool isReader = true;
   static const bool isWriter = false;

   bool setVersion(uint32_t dataVer) {
      dataVersion = dataVer;
//      serialize(std::true_type(), dataVersion);
//      if(dataVersion > latestVersion)
//         return false;
      return true;
   }
   void resetBuffers(uint8_t *ptr) {
      memPtr = ptr;
      integrityCounter = 0;
#if DEBUG_SERIALIZATION
      bytesCount = 0;
#endif
   }
   void checkIntegrity(uint32_t checkAdded) {
      if(dataVersion >= checkAdded) {
         uint32_t check;
         serialize(std::true_type(), check);
         ASSERT_MSG(check == integrityCounter, "Serialization integrity broken!");
         integrityCounter += 1;
      }
   }

   template<typename Type>void addPreprocessed(Type &datum) {
      serialize(std::is_trivial<Type>(), datum);
   }

   // template<typename Type>void add(uint32_t fieldAdded, Type &datum, Type defaultValue) {
   //    if(dataVersion >= fieldAdded)
   //       serialize(std::is_trivial<Type>(), datum);
   //    else
   //       datum = defaultValue;
   // }
   // template<typename Type>void addVector(uint32_t fieldAdded, std::vector<Type> &data) {
   //    uint32_t size;
   //    serialize(std::true_type(), size);
   //    data.resize(size);
   //    for(int i=0; i<size; ++i) {
   //       serialize(std::is_trivial<Type>(), data[i]);
   //    }
   // }
   template<typename Type>Type remove(uint32_t fieldAdded, uint32_t fieldRemoved, Type defaultValue) {
      Type field = defaultValue;
      if(dataVersion >= fieldAdded && dataVersion < fieldRemoved) {
         serialize(std::is_trivial<Type>(), field);
      }
      return field;
   }

   uint32_t dataVersion;
   uint32_t integrityCounter;
   uint8_t *memPtr;
#if DEBUG_SERIALIZATION
   uint32_t bytesCount;
#endif
};

struct FixedMemWriteSerializer {
private:
   template<typename Type>void serialize(std::true_type, Type &datum) {
#if DEBUG_SERIALIZATION
      bytesCount += sizeof(Type);
#endif
      memcpy(memPtr, &datum, sizeof(Type));
      memPtr += sizeof(Type);
   }
   template<>void serialize(std::true_type, StringHolder &datum) {
      uint32_t len = datum.length;
      serialize(std::true_type(), len);
#if DEBUG_SERIALIZATION
      bytesCount += len;
#endif
      memcpy(memPtr, datum.data, len);
      memPtr += len;
   }
   template<typename Type>void serialize(std::false_type, Type &datum) {
      datum.serialize(*this);
   }
   template<>void serialize(std::false_type, std::string &datum) {
      uint32_t len = datum.size();
      serialize(std::true_type(), len);
#if DEBUG_SERIALIZATION
      bytesCount += len;
#endif   
      memcpy(memPtr, datum.c_str(), len);
      memPtr += len;
   }

public:   
   static const bool isReader = false;
   static const bool isWriter = true;

   bool setVersion(uint32_t latestVersion) {
      dataVersion = latestVersion;
//      serialize(std::true_type(), dataVersion);
      return true;
   }
   void resetBuffers(uint8_t *ptr) {
      memPtr = ptr;
      integrityCounter = 0;
#if DEBUG_SERIALIZATION
      bytesCount = 0;
#endif
   }

   void checkIntegrity(uint32_t checkAdded) {
      if(dataVersion >= checkAdded) {
         uint32_t check = integrityCounter;
         serialize(std::true_type(), check);
         integrityCounter += 1;
      }
   }
   template<typename Type>void addPreprocessed(Type &datum) {
      serialize(std::is_trivial<Type>(), datum);
   }
   // template<typename Type>void add(uint32_t fieldAdded, Type &datum, Type defaultValue) {
   //    if(dataVersion >= fieldAdded)
   //       serialize(std::is_trivial<Type>(), datum);
   // }
   // template<typename Type>void addVector(uint32_t fieldAdded, std::vector<Type> &data) {
   //    uint32_t size = data.size();
   //    serialize(std::true_type(), size);
   //    for(int i=0; i<size; ++i) {
   //       serialize(std::is_trivial<Type>(), data[i]);
   //    }
   // }
   template<typename Type>Type remove(uint32_t fieldAdded, uint32_t fieldRemoved, Type defaultValue) {
      Type field = defaultValue;
      if(dataVersion >= fieldAdded && dataVersion < fieldRemoved) {
         serialize(std::is_trivial<Type>(), field);
      }
      return field;
   }

   uint32_t dataVersion;
   uint32_t integrityCounter;
   uint8_t *memPtr;   
#if DEBUG_SERIALIZATION
   uint32_t bytesCount;
#endif
};

