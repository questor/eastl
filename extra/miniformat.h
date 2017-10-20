//////////////////////////////////// JJ ///////////////////////////////////
// -------------------------------------------------------------------------
//  File Name        : miniformat.h
//  Author           : Jaewon Jung
//  Description      : A very simple type-safe string formatting library
//    - A header-only library
//    - Minimal dependency
//    - Take an output parameter as an instance of a 'string' class
//     -- Not necessarily std::string
//     -- Any string class with following interfaces will do:
//      --- Get a length of the string
//      --- Append a character or a string to it
//      --- Reserve a specific amount of memory for it
//      --- Get a non-const char pointer to a specific location in the string  
//     -- MFC CString adapter is also provided by default
//    - Faster than sprintf
//     -- Try to minimize the memory allocation
//    - Position-based format specifier
//    - Try to be as similar as possible to familiar printf format specifiers
//    - Support only a (arguably most useful) subset among a plethora of printf format specifiers 
//     -- Integer(signed/unsigned,32bit/64bit), floating-point number, c-string are supported as an argument
//     -- Width, precision options supported
//  References        : 
//    - http://code.google.com/p/stringencoders/wiki/NumToA
//    - http://altdevblogaday.com/2011/07/12/abusing-the-c-preprocessor/
//    - https://github.com/c42f/tinyformat
//    -  https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920
// -------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////

#pragma  once

#include <cstdio>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <algorithm>

#define _FORMAT_JOIN(x,y)		_FORMAT_JOIN2(x,y)
#define _FORMAT_JOIN2(x,y)	x##y

#define _FORMAT_LIST_0(x)
#define _FORMAT_LIST_1(x)		x##1
#define _FORMAT_LIST_2(x)		_FORMAT_LIST_1(x), x##2
#define _FORMAT_LIST_3(x)		_FORMAT_LIST_2(x), x##3
#define _FORMAT_LIST_4(x)		_FORMAT_LIST_3(x), x##4
#define _FORMAT_LIST_5(x)		_FORMAT_LIST_4(x), x##5
#define _FORMAT_LIST_6(x)		_FORMAT_LIST_5(x), x##6
#define _FORMAT_LIST_7(x)		_FORMAT_LIST_6(x), x##7
#define _FORMAT_LIST_8(x)		_FORMAT_LIST_7(x), x##8
#define _FORMAT_LIST_9(x)		_FORMAT_LIST_8(x), x##9

#define _FORMAT_LIST(cnt,x)		_FORMAT_JOIN(_FORMAT_LIST_,cnt)(x)

#define _FORMAT_ARG_0(P,p)
#define _FORMAT_ARG_1(P,p)		P##1 p##1
#define _FORMAT_ARG_2(P,p)		_FORMAT_ARG_1(P,p), P##2 p##2
#define _FORMAT_ARG_3(P,p)		_FORMAT_ARG_2(P,p), P##3 p##3
#define _FORMAT_ARG_4(P,p)		_FORMAT_ARG_3(P,p), P##4 p##4
#define _FORMAT_ARG_5(P,p)		_FORMAT_ARG_4(P,p), P##5 p##5
#define _FORMAT_ARG_6(P,p)		_FORMAT_ARG_5(P,p), P##6 p##6
#define _FORMAT_ARG_7(P,p)		_FORMAT_ARG_6(P,p), P##7 p##7
#define _FORMAT_ARG_8(P,p)		_FORMAT_ARG_7(P,p), P##8 p##8
#define _FORMAT_ARG_9(P,p)		_FORMAT_ARG_8(P,p), P##9 p##9

#define _FORMAT_ARG(cnt,P,p)		_FORMAT_JOIN(_FORMAT_ARG_,cnt)(P,p)

namespace miniformat {}
namespace mini = miniformat;

namespace miniformat
{
#if __cplusplus > 199711L || (defined(_MSC_VER) && _MSC_VER >= 1800) // if this is a C++11-compliant compiler or VS2013 and above
    template <typename String, typename... Args>
    void format(String& outputText, const char *formatText, Args&&... args);
#else
    template <typename String>
    void format(String& outputText, const char *formatText);
    template <typename String, _FORMAT_LIST(1, typename P)>
    void format(String& outputText, const char *formatText, _FORMAT_ARG(1, P, p));
    template <typename String, _FORMAT_LIST(2, typename P)>
    void format(String& outputText, const char *formatText, _FORMAT_ARG(2, P, p));
    template <typename String, _FORMAT_LIST(3, typename P)>
    void format(String& outputText, const char *formatText, _FORMAT_ARG(3, P, p));
    template <typename String, _FORMAT_LIST(4, typename P)>
    void format(String& outputText, const char *formatText, _FORMAT_ARG(4, P, p));
    template <typename String, _FORMAT_LIST(5, typename P)>
    void format(String& outputText, const char *formatText, _FORMAT_ARG(5, P, p));
    template <typename String, _FORMAT_LIST(6, typename P)>
    void format(String& outputText, const char *formatText, _FORMAT_ARG(6, P, p));
    template <typename String, _FORMAT_LIST(7, typename P)>
    void format(String& outputText, const char *formatText, _FORMAT_ARG(7, P, p));
    template <typename String, _FORMAT_LIST(8, typename P)>
    void format(String& outputText, const char *formatText, _FORMAT_ARG(8, P, p));
    template <typename String, _FORMAT_LIST(9, typename P)>
    void format(String& outputText, const char *formatText, _FORMAT_ARG(9, P, p));
#endif // __cplusplus > 199711L || (defined(_MSC_VER) && _MSC_VER >= 1800)

    namespace string_adaptor
    {
        // Function overloads for std::string and similiars 
        template <typename String>
        void reserve(String& self, int size) { self.reserve(size); }
        template <typename String>
        int length(const String& self) { return static_cast<int>(self.length()); }
        template <typename String>
        int append(String& self, int, const char *str) { self.append(str); return length(self); }
        template <typename String>
        int append(String& self, int, const char *str, int count) { self.append(str, count); return length(self); }
        template <typename String>
        int append(String& self, int, int count, char c) { self.append(count, c); return length(self); }
        template <typename String>
        char * at(String& self, int index) { return &self[index]; }
        template <typename String>
        void copy(String& self, const char *str) { self = str; }

        // Function overloads for fixed-size char arrays
        template <int N>
        void reserve(char (&self)[N], int size) {}
        template <int N>
        int append(char (&self)[N], int currentLength, const char *str)
        {
            assert(self[currentLength] == 0);
            assert(currentLength+strlen(str)<N);
            while(currentLength < N-1 && *str)
            {
                self[currentLength] = *str;
                ++str;
                ++currentLength;
            }
            self[currentLength] = 0;
            return currentLength;
        }
        template <int N>
        int append(char (&self)[N], int currentLength, const char *str, int count)
        {
            assert(self[currentLength] == 0);
            assert(currentLength+count<N);
            int end = std::min(N-1, count+currentLength);
            while(currentLength < end && *str)
            {
                self[currentLength] = *str;
                ++str;
                ++currentLength;
            }
            self[currentLength] = 0;
            return currentLength;
        }
        template <int N>
        int append(char (&self)[N], int currentLength, int count, char c)
        { 
            assert(self[currentLength] == 0);
            assert(currentLength+count<N);
            int end = std::min(N-1, count+currentLength);
            while(currentLength < end)
            {
                self[currentLength] = c;
                ++currentLength;
            }
            self[currentLength] = 0;
            return currentLength;
        }
        template <int N>
        int length(const char (&self)[N]) { return static_cast<int>(strlen(self)); }
        template <int N>
        char * at(char (&self)[N], int index)
        {
            assert(index<N);
            return &self[index];
        }
        template <int N>
        void copy(char (&self)[N], const char *str)
        {
#if defined(_MSC_VER)
            strcpy_s(self, N-1, str);
#else
            strncpy(self, str, N-1);
            self[N-1] = '\0';
#endif
        }
    }

    namespace detail
    {
        const int kGranulity = 32;	// This determines the reservation size for non-string arguments.
        static const double pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000,
                                                                        10000000, 100000000, 1000000000 };
        // Integer render functions are inspired(optimized) from this: 
        // https://www.facebook.com/notes/facebook-engineering/three-optimization-tips-for-c/10151361643253920
        int digits10(uint32_t v);
        int digits10(uint64_t v);

        template <typename String>
        int render(String& wstr, int currentLength, int32_t value, int width, int precision);
        template <typename String>
        int render(String& wstr, int currentLength,  uint32_t value, int width, int precision);
        template <typename String>
        int render(String& wstr, int currentLength,  int64_t value, int width, int precision);
        template <typename String>
        int render(String& wstr, int currentLength,  uint64_t value, int width, int precision);
        template <typename String>
        int render(String& wstr, int currentLength,  double value, int width, int precision);
        template <typename String>
        int render(String& wstr, int currentLength,  char value, int width, int precision);
        template <typename String>
        int render(String& wstr, int currentLength,  const char *value, int width, int precision);
        template <typename String, typename T>
        int render(String& wstr, int currentLength,  T *value, int width, int precision);
        template <typename T>
        int size_enough(T value);
        template <>
        int size_enough(const char *value);
        template <typename T>
        int size_enough(T *value);
        void strreverse(char *begin, char *end);
    }
}

#if __cplusplus > 199711L || (defined(_MSC_VER) && _MSC_VER >= 1800) // if this is a C++11-compliant compiler or VS2013 and above
// Variadic template from C++11 is supported. Let's use it!
namespace miniformat
{
    namespace detail
    {
        int sumSizes();

        template <typename H, typename... Ts>
        int sumSizes(H&& h, Ts&&... ts)
        {
            return detail::size_enough(std::forward<H>(h)) + sumSizes(std::forward<Ts>(ts)...);
        }

        template <typename String>
        int renderByToken(int, String&, char, int, int, int)
        {
            return -1;
        }
        template <typename String, typename H, typename... Ts>
        int renderByToken(int cnt, String& outputText, char c, int currentLength, int w, int p, H&& h, Ts&&... ts)
        {
            if (c == '0' + cnt - sizeof...(Ts) - 1)
                return detail::render(outputText, currentLength, std::forward<H>(h), w, p);
            else
                return renderByToken(cnt, outputText, c, currentLength, w, p, std::forward<Ts>(ts)...);
        }
    }
}

template <typename String, typename... Args>
void miniformat::format(String& outputText, const char *formatText, Args&&... args)
{
    int newLength = static_cast<int>(strlen(formatText)) + detail::sumSizes(args...);
    string_adaptor::reserve(outputText, newLength);
    string_adaptor::copy(outputText, "");
    const char *itr = formatText;
    int currentLength = 0;
    char c = 0;

    while ((c = *itr++))
    {
        if (c == '%')
        {
            if (*itr == '%')										/* "%%" */
            {
                currentLength = string_adaptor::append(outputText, currentLength, 1, '%');
                ++itr;
            }
            else if (*itr >= '0' && *itr <= '9')					/* "%n" */
            {
                int const newLength = detail::renderByToken(sizeof...(Args), outputText, *itr, currentLength, 0, 6, std::forward<Args>(args)...);
                if (newLength == -1)
                {
                    currentLength = string_adaptor::append(outputText, currentLength, 1, c);
                    currentLength = string_adaptor::append(outputText, currentLength, 1, *itr);
                    assert(!"An out-of-range format specifier given!");
                }
                else
                {
                    currentLength = newLength;
                }
                ++itr;
            }
            else if (*itr == '(' &&
                (*(itr + 1) >= '0' && *(itr + 1) <= '9') &&
                *(itr + 2) == ')' &&
                (*(itr + 3) >= '0' && *(itr + 3) <= '9'))			/* %(w)n */
            {
                int const newLength = detail::renderByToken(sizeof...(Args), outputText, *(itr + 3), currentLength, *(itr + 1) - '0', 6, std::forward<Args>(args)...);
                if (newLength == -1)
                {
                    currentLength = string_adaptor::append(outputText, currentLength, 1, c);
                    currentLength = string_adaptor::append(outputText, currentLength, itr, 4);
                    assert(!"An out-of-range format specifier given!");
                }
                else
                {
                    currentLength = newLength;
                }
                itr += 4;
            }
            else if (*itr == '(' &&
                *(itr + 1) == '.' &&
                (*(itr + 2) >= '0' && *(itr + 2) <= '9') &&
                *(itr + 3) == ')' &&
                (*(itr + 4) >= '0' && *(itr + 4) <= '9'))			/* "%(.p)n" */
            {
                int const newLength = detail::renderByToken(sizeof...(Args), outputText, *(itr + 4), currentLength, 0, *(itr + 2) - '0', std::forward<Args>(args)...);
                if (newLength == -1)
                {
                    currentLength = string_adaptor::append(outputText, currentLength, 1, c);
                    currentLength = string_adaptor::append(outputText, currentLength, itr, 5);
                    assert(!"An out-of-range format specifier given!");
                }
                else
                {
                    currentLength = newLength;
                }
                itr += 5;
            }
            else if (*itr == '(' &&
                (*(itr + 1) >= '0' && *(itr + 1) <= '9') &&
                *(itr + 2) == '.' &&
                (*(itr + 3) >= '0' && *(itr + 3) <= '9') &&
                *(itr + 4) == ')' &&
                (*(itr + 5) >= '0' && *(itr + 5) <= '9'))			/* %(w.p)n */
            {
                int const newLength = detail::renderByToken(sizeof...(Args), outputText, *(itr + 5), currentLength, *(itr + 1) - '0', *(itr + 3) - '0', std::forward<Args>(args)...);
                if (newLength == -1)
                {
                    currentLength = string_adaptor::append(outputText, currentLength, 1, c);
                    currentLength = string_adaptor::append(outputText, currentLength, itr, 6);
                    assert(!"An out-of-range format specifier given!");
                }
                else
                {
                    currentLength = newLength;
                }
                itr += 6;
            }
            else
            {
                currentLength = string_adaptor::append(outputText, currentLength, 1, c);
                assert(!"An invalid format specifier given!");
            }
        }
        else
        {
            currentLength = string_adaptor::append(outputText, currentLength, 1, c);
        }
    }
}
#else // No luck. We should resort to the old preprocessor tricks without variadic template.
#define _FORMAT_GET_NEW_LENGTH_0		static_cast<int>(strlen(formatText))
#define _FORMAT_GET_NEW_LENGTH_1		_FORMAT_GET_NEW_LENGTH_0 + detail::size_enough(p1)
#define _FORMAT_GET_NEW_LENGTH_2		_FORMAT_GET_NEW_LENGTH_1 + detail::size_enough(p2)
#define _FORMAT_GET_NEW_LENGTH_3		_FORMAT_GET_NEW_LENGTH_2 + detail::size_enough(p3)
#define _FORMAT_GET_NEW_LENGTH_4		_FORMAT_GET_NEW_LENGTH_3 + detail::size_enough(p4)
#define _FORMAT_GET_NEW_LENGTH_5		_FORMAT_GET_NEW_LENGTH_4 + detail::size_enough(p5)
#define _FORMAT_GET_NEW_LENGTH_6		_FORMAT_GET_NEW_LENGTH_5 + detail::size_enough(p6)
#define _FORMAT_GET_NEW_LENGTH_7		_FORMAT_GET_NEW_LENGTH_6 + detail::size_enough(p7)
#define _FORMAT_GET_NEW_LENGTH_8		_FORMAT_GET_NEW_LENGTH_7 + detail::size_enough(p8)
#define _FORMAT_GET_NEW_LENGTH_9		_FORMAT_GET_NEW_LENGTH_8 + detail::size_enough(p9)

#define _FORMAT_GET_NEW_LENGTH(cnt)		_FORMAT_JOIN(_FORMAT_GET_NEW_LENGTH_,cnt)

#define _FORMAT_RENDER_CASES_1(w,p)		case '0': currentLength = detail::render(outputText, currentLength, p1, w, p); break;
#define _FORMAT_RENDER_CASES_2(w,p)		_FORMAT_RENDER_CASES_1(w,p) case '1': currentLength = detail::render(outputText, currentLength,  p2, w, p); break;
#define _FORMAT_RENDER_CASES_3(w,p)		_FORMAT_RENDER_CASES_2(w,p) case '2': currentLength = detail::render(outputText, currentLength,  p3, w, p); break;
#define _FORMAT_RENDER_CASES_4(w,p)		_FORMAT_RENDER_CASES_3(w,p) case '3': currentLength = detail::render(outputText, currentLength,  p4, w, p); break;
#define _FORMAT_RENDER_CASES_5(w,p)		_FORMAT_RENDER_CASES_4(w,p) case '4': currentLength = detail::render(outputText, currentLength,  p5, w, p); break;
#define _FORMAT_RENDER_CASES_6(w,p)		_FORMAT_RENDER_CASES_5(w,p) case '5': currentLength = detail::render(outputText, currentLength,  p6, w, p); break;
#define _FORMAT_RENDER_CASES_7(w,p)		_FORMAT_RENDER_CASES_6(w,p) case '6': currentLength = detail::render(outputText, currentLength,  p7, w, p); break;
#define _FORMAT_RENDER_CASES_8(w,p)		_FORMAT_RENDER_CASES_7(w,p) case '7': currentLength = detail::render(outputText, currentLength,  p8, w, p); break;
#define _FORMAT_RENDER_CASES_9(w,p)		_FORMAT_RENDER_CASES_8(w,p) case '8': currentLength = detail::render(outputText, currentLength,  p9, w, p); break;

#define _FORMAT_RENDER_CASES(cnt,w,p)		_FORMAT_JOIN(_FORMAT_RENDER_CASES_,cnt)(w,p)

template <typename String>
void miniformat::format(String& outputText, const char *formatText)
{
    outputText = formatText;
}

#define _FORMAT_FUNCTION_TEMPLATE(cnt) \
    int newLength = _FORMAT_GET_NEW_LENGTH(cnt); \
    string_adaptor::reserve(outputText, newLength); \
    string_adaptor::copy(outputText, ""); \
    const char *itr = formatText; \
    int currentLength = 0; \
    char c = 0; \
    while((c=*itr++)) \
    { \
        if(c == '%') \
        { \
            if(*itr == '%')										/* "%%" */ \
            { \
                currentLength = string_adaptor::append(outputText, currentLength, 1, '%'); \
                ++itr; \
            } \
            else if(*itr >= '0' && *itr <= '9')					/* "%n" */ \
            { \
                switch(*itr) \
                { \
                    _FORMAT_RENDER_CASES(cnt, 0, 6); \
                default: \
                    currentLength = string_adaptor::append(outputText, currentLength, 1, c); \
                    currentLength = string_adaptor::append(outputText, currentLength, 1, *itr); \
                    assert(!"An out-of-range format specifier given!"); \
                    break; \
                } \
                ++itr; \
            } \
            else if(*itr == '(' && \
                (*(itr+1) >= '0' && *(itr+1) <= '9') && \
                *(itr+2) == ')' && \
                (*(itr+3) >= '0' && *(itr+3) <= '9'))			/* %(w)n */ \
            { \
                switch(*(itr+3)) \
                { \
                    _FORMAT_RENDER_CASES(cnt, *(itr+1)-'0', 6); \
                default: \
                    currentLength = string_adaptor::append(outputText, currentLength, 1, c); \
                    currentLength = string_adaptor::append(outputText, currentLength, itr, 4); \
                    assert(!"An out-of-range format specifier given!"); \
                    break; \
                } \
                itr += 4; \
            } \
            else if(*itr == '(' && \
                *(itr+1) == '.' && \
                (*(itr+2) >= '0' && *(itr+2) <= '9') && \
                *(itr+3) == ')' && \
                (*(itr+4) >= '0' && *(itr+4) <= '9'))			/* "%(.p)n" */ \
            { \
                switch(*(itr+4)) \
                { \
                    _FORMAT_RENDER_CASES(cnt, 0, *(itr+2)-'0'); \
                default: \
                    currentLength = string_adaptor::append(outputText, currentLength, 1, c); \
                    currentLength = string_adaptor::append(outputText, currentLength, itr, 5); \
                    assert(!"An out-of-range format specifier given!"); \
                    break; \
                } \
                itr += 5; \
            } \
            else if(*itr == '(' && \
                (*(itr+1) >= '0' && *(itr+1) <= '9') && \
                *(itr+2) == '.' && \
                (*(itr+3) >= '0' && *(itr+3) <= '9') && \
                *(itr+4) == ')' && \
                (*(itr+5) >= '0' && *(itr+5) <= '9'))			/* %(w.p)n */ \
            { \
                switch(*(itr+5)) \
                { \
                    _FORMAT_RENDER_CASES(cnt, *(itr+1)-'0', *(itr+3)-'0'); \
                default: \
                    currentLength = string_adaptor::append(outputText, currentLength, 1, c); \
                    currentLength = string_adaptor::append(outputText, currentLength, itr, 6); \
                    assert(!"An out-of-range format specifier given!"); \
                    break; \
                } \
                itr += 6; \
            } \
            else \
            { \
                currentLength = string_adaptor::append(outputText, currentLength, 1, c); \
                assert(!"An invalid format specifier given!"); \
            } \
        } \
        else \
        { \
            currentLength = string_adaptor::append(outputText, currentLength, 1, c); \
        } \
    }

/*[[[cog
import cog
format_method_template = '''template <typename String, LIST({0}, typename P)>
void miniformat::format(String& outputText, const char *formatText, ARG({0}, P, p))
{{ FORMAT_FUNCTION_TEMPLATE({0}) }}
'''
for i in range(1, 10):
    cog.outl(format_method_template.format(i))
]]]*/
template <typename String, _FORMAT_LIST(1, typename P)>
void miniformat::format(String& outputText, const char *formatText, _FORMAT_ARG(1, P, p))
{ _FORMAT_FUNCTION_TEMPLATE(1) }

template <typename String, _FORMAT_LIST(2, typename P)>
void miniformat::format(String& outputText, const char *formatText, _FORMAT_ARG(2, P, p))
{ _FORMAT_FUNCTION_TEMPLATE(2) }

template <typename String, _FORMAT_LIST(3, typename P)>
void miniformat::format(String& outputText, const char *formatText, _FORMAT_ARG(3, P, p))
{ _FORMAT_FUNCTION_TEMPLATE(3) }

template <typename String, _FORMAT_LIST(4, typename P)>
void miniformat::format(String& outputText, const char *formatText, _FORMAT_ARG(4, P, p))
{ _FORMAT_FUNCTION_TEMPLATE(4) }

template <typename String, _FORMAT_LIST(5, typename P)>
void miniformat::format(String& outputText, const char *formatText, _FORMAT_ARG(5, P, p))
{ _FORMAT_FUNCTION_TEMPLATE(5) }

template <typename String, _FORMAT_LIST(6, typename P)>
void miniformat::format(String& outputText, const char *formatText, _FORMAT_ARG(6, P, p))
{ _FORMAT_FUNCTION_TEMPLATE(6) }

template <typename String, _FORMAT_LIST(7, typename P)>
void miniformat::format(String& outputText, const char *formatText, _FORMAT_ARG(7, P, p))
{ _FORMAT_FUNCTION_TEMPLATE(7) }

template <typename String, _FORMAT_LIST(8, typename P)>
void miniformat::format(String& outputText, const char *formatText, _FORMAT_ARG(8, P, p))
{ _FORMAT_FUNCTION_TEMPLATE(8) }

template <typename String, _FORMAT_LIST(9, typename P)>
void miniformat::format(String& outputText, const char *formatText, _FORMAT_ARG(9, P, p))
{ _FORMAT_FUNCTION_TEMPLATE(9) }

//[[[end]]]
#endif // __cplusplus > 199711L || (defined(_MSC_VER) && _MSC_VER >= 1800)

inline void miniformat::detail::strreverse(char *begin, char *end)
{
    char aux;
    while(end > begin)
        aux = *end, *end-- = *begin, *begin++ = aux;
}

template <typename T>
int miniformat::detail::size_enough(T value)
{
    return kGranulity;
}

namespace miniformat
{
    namespace detail
    {
        template <>
        inline int size_enough(const char *value)
        {
            return static_cast<int>(strlen(value));
        }
    }
}

template <typename T>
int miniformat::detail::size_enough(T *value)
{
    // Pointer to hex digits. Each hex digit deals with 4 bits (a half byte). 
    return sizeof(T*) * 2;
}

inline int miniformat::detail::digits10(uint32_t v)
{
    static const uint32_t P01 = 10;
    static const uint32_t P02 = 100;
    static const uint32_t P03 = 1000;
    static const uint32_t P04 = 10000;
    static const uint32_t P05 = 100000;
    static const uint32_t P06 = 1000000;
    static const uint32_t P07 = 10000000;
    static const uint32_t P08 = 100000000;
    static const uint32_t P09 = 1000000000;

    if(v < P01) return 1;
    if(v < P02) return 2;
    if(v < P03) return 3;
    if(v < P08)
    {
        if(v < P06)
        {
            if(v < P04) return 4;
            return 5 + (v >= P05);
        }
        return 7 + (v >= P07);
    }
    return 9 + (v >= P09);
}

inline int miniformat::detail::digits10(uint64_t v)
{
    static const uint64_t P01 = 10;
    static const uint64_t P02 = 100;
    static const uint64_t P03 = 1000;
    static const uint64_t P04 = 10000;
    static const uint64_t P05 = 100000;
    static const uint64_t P06 = 1000000;
    static const uint64_t P07 = 10000000;
    static const uint64_t P08 = 100000000;
    static const uint64_t P09 = 1000000000;
    static const uint64_t P10 = 10000000000;
    static const uint64_t P11 = 100000000000;
    static const uint64_t P12 = 1000000000000;

    if(v < P01) return 1;
    if(v < P02) return 2;
    if(v < P03) return 3;
    if(v < P12)
    {
        if(v < P08)
        {
            if(v < P06)
            {
                if(v < P04) return 4;
                return 5 + (v >= P05);
            }
            return 7 + (v >= P07);
        }
        if(v < P10)
        {
            return 9 + (v >= P09);
        }
        return 11 + (v >= P11);
    }

    return 12 + digits10(v / P12);
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, int32_t value, int width, int)
{
    static const char digits[201] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";

    // Take care of sign.
    unsigned int uvalue = (value < 0) ? -value : value;

    int next = currentLength;
    const int length = digits10(uvalue);

    // Handle the 'width' parameter.
    int spaceCnt = width - length - (value < 0);
    if(spaceCnt > 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');
        next += spaceCnt;
    }

    if(value < 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, 1, '-');
        ++next;
    }

    currentLength = string_adaptor::append(wstr, currentLength, length, '0');
    next += length-1;

    while(uvalue >= 100)
    {
        const uint32_t i = (uvalue % 100) * 2;
        uvalue /= 100;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
        next -= 2;
    }

    // Handle last 1-2 digits.
    if(uvalue < 10)
    {
        *string_adaptor::at(wstr, next) = '0'+static_cast<char>(uvalue);
    }
    else
    {
        const uint32_t i = static_cast<uint32_t>(uvalue) * 2;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
    }

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, uint32_t value, int width, int)
{
    static const char digits[201] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";

    int next = currentLength;
    const int length = digits10(value);

    // Handle the 'width' parameter.
    int spaceCnt = width - length;
    if(spaceCnt > 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');
        next += spaceCnt;
    }

    currentLength = string_adaptor::append(wstr, currentLength, length, '0');
    next += length-1;

    while(value >= 100)
    {
        const uint32_t i = (value % 100) * 2;
        value /= 100;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
        next -= 2;
    }

    // Handle last 1-2 digits.
    if(value < 10)
    {
        *string_adaptor::at(wstr, next) = '0'+static_cast<char>(value);
    }
    else
    {
        const uint32_t i = static_cast<uint32_t>(value) * 2;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
    }

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, int64_t value, int width, int)
{
    static const char digits[201] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";

    // Take care of sign.
    uint64_t uvalue = (value < 0) ? -value : value;

    int next = currentLength;
    const int length = digits10(uvalue);

    // Handle the 'width' parameter.
    int spaceCnt = width - length - (value < 0);
    if(spaceCnt > 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');
        next += spaceCnt;
    }

    if(value < 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, 1, '-');
        ++next;
    }

    currentLength = string_adaptor::append(wstr, currentLength, length, '0');
    next += length-1;

    while(uvalue >= 100)
    {
        const uint64_t i = (uvalue % 100) * 2;
        uvalue /= 100;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
        next -= 2;
    }

    // Handle last 1-2 digits.
    if(uvalue < 10)
    {
        *string_adaptor::at(wstr, next) = '0'+static_cast<char>(uvalue);
    }
    else
    {
        const uint32_t i = static_cast<uint32_t>(uvalue) * 2;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
    }

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, uint64_t value, int width, int)
{
    static const char digits[201] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";

    int next = currentLength;
    const int length = digits10(value);

    // Handle the 'width' parameter.
    int spaceCnt = width - length;
    if(spaceCnt > 0)
    {
        currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');
        next += spaceCnt;
    }

    currentLength = string_adaptor::append(wstr, currentLength, length, '0');
    next += length-1;

    while(value >= 100)
    {
        const uint64_t i = (value % 100) * 2;
        value /= 100;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
        next -= 2;
    }

    // Handle last 1-2 digits.
    if(value < 10)
    {
        *string_adaptor::at(wstr, next) = '0'+static_cast<char>(value);
    }
    else
    {
        const uint32_t i = static_cast<uint32_t>(value) * 2;
        *string_adaptor::at(wstr, next) = digits[i+1];
        *string_adaptor::at(wstr, next-1) = digits[i];
    }

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, double value, int width, int precision)
{
    int beginIndex = currentLength;
    /* Hacky test for NaN
     * under -fast-math this won't work, but then you also won't
     * have correct nan values anyways.  The alternative is
     * to link with libmath (bad) or hack IEEE double bits (bad)
     */
    if(!(value == value))		
    {
        // Handle the 'width' parameter.
        int spaceCnt = width - 3;
        if(spaceCnt > 0)
            currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');

        currentLength = string_adaptor::append(wstr, currentLength, 1, 'n'); 
        currentLength = string_adaptor::append(wstr, currentLength, 1, 'a'); 
        currentLength = string_adaptor::append(wstr, currentLength, 1, 'n');
        return currentLength;
    }

    /* if input is larger than thres_max, revert to exponential */
    const double thres_max = (double)(0x7FFFFFFF);

    double diff = 0.0;

    if(precision < 0) 
    {
        precision = 0;
    } 
    else if(precision > 9) 
    {
        /* precision of >= 10 can lead to overflow errors */
        precision = 9;
    }

    /* we'll work in positive values and deal with the
        negative sign issue later */
    int neg = 0;
    if(value < 0) 
    {
        neg = 1;
        value = -value;
    }

    int whole = (int)value;
    double tmp = (value - whole) * pow10[precision];
    uint32_t frac = (uint32_t)(tmp);
    diff = tmp - frac;

    if(diff > 0.5) 
    {
        ++frac;
        /* handle rollover, e.g.  case 0.99 with precision 1 is 1.0  */
        if(frac >= pow10[precision]) 
        {
            frac = 0;
            ++whole;
        }
    }
    else if(diff == 0.5 && ((frac == 0) || (frac & 1)))
    {
        /* if halfway, round up if odd, OR
            if last digit is 0.  That last part is strange */
        ++frac;
    }

    /* for very large numbers switch back to native sprintf for exponentials.
        anyone want to write code to replace this? */
    /*
         normal printf behavior is to print EVERY whole number digit
        which can be 100s of characters overflowing your buffers == bad
    */
    if(value > thres_max)
    {
        const int kSomeEnoughSpace = 128;
        char buffer[kSomeEnoughSpace+1];
#if defined(_MSC_VER)
        sprintf_s(buffer, kSomeEnoughSpace, "%*e", width, neg ? -value : value);
#else
        snprintf(buffer, kSomeEnoughSpace, "%*e", width, neg ? -value : value);
#endif
        buffer[kSomeEnoughSpace] = 0;
        currentLength = string_adaptor::append(wstr, currentLength, buffer);
        return currentLength;
    }

    if(precision == 0)
    {
        diff = value - whole;
        if(diff > 0.5) 
        {
            /* greater than 0.5, round up, e.g. 1.6 -> 2 */
            ++whole;
        } 
        else if(diff == 0.5 && (whole & 1)) 
        {
            /* exactly 0.5 and ODD, then round up */
            /* 1.5 -> 2, but 2.5 -> 2 */
            ++whole;
        }
    } 
    else 
    {
        int count = precision;
        // now do fractional part, as an unsigned number
        do 
        {
            --count;
            currentLength = string_adaptor::append(wstr, currentLength, 1, (char)(48 + (frac % 10)));
        } 
        while((frac /= 10));
        // add extra 0s
        currentLength = string_adaptor::append(wstr, currentLength, count, '0');
        // add decimal
        currentLength = string_adaptor::append(wstr, currentLength, 1, '.');
    }

    // do whole part.
    // Take care of sign.
    // Conversion. Number is reversed.
    do 
    {
        currentLength = string_adaptor::append(wstr, currentLength, 1, (char)(48 + (whole % 10))); 
    }
    while((whole /= 10));
    if(neg) 
    {
        currentLength = string_adaptor::append(wstr, currentLength, 1, '-');
    }
    
    // Handle the 'width' parameter.
    int spaceCnt = width - (currentLength-beginIndex);
    if(spaceCnt > 0)
        currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');

    int endIndex = currentLength-1;
    strreverse(string_adaptor::at(wstr, beginIndex), string_adaptor::at(wstr, endIndex));

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, char value, int, int)
{
    currentLength = string_adaptor::append(wstr, currentLength, 1, value);

    return currentLength;
}

template <typename String>
int miniformat::detail::render(String& wstr, int currentLength, const char *value, int width, int)
{
    // Handle the 'width' parameter.
    int spaceCnt = width - static_cast<int>(strlen(value));
    if(spaceCnt > 0)
        currentLength = string_adaptor::append(wstr, currentLength, spaceCnt, ' ');

    currentLength = string_adaptor::append(wstr, currentLength, value);

    return currentLength;
}

template <typename String, typename T>
int miniformat::detail::render(String& wstr, int currentLength,  T *value, int, int)
{
    static const char hexDigits[513]=
        "000102030405060708090A0B0C0D0E0F"
        "101112131415161718191A1B1C1D1E1F"
        "202122232425262728292A2B2C2D2E2F"
        "303132333435363738393A3B3C3D3E3F"
        "404142434445464748494A4B4C4D4E4F"
        "505152535455565758595A5B5C5D5E5F"
        "606162636465666768696A6B6C6D6E6F"
        "707172737475767778797A7B7C7D7E7F"
        "808182838485868788898A8B8C8D8E8F"
        "909192939495969798999A9B9C9D9E9F"
        "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
        "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
        "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
        "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
        "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
        "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

    const int size = sizeof(T*) * 2;
    char buffer[size];
    uint16_t const* pHexDigit = reinterpret_cast<uint16_t const*>(hexDigits);
    uint16_t *pDest = reinterpret_cast<uint16_t*>(buffer + size - 2);
    uintptr_t v = reinterpret_cast<uintptr_t>(value);

    // Assume sizeof(T*) is a multiple of 4
    for(; (void*)pDest >= (void*)buffer; pDest -= 4)
    {
        *(pDest+0) = pHexDigit[v & 0xff];
        *(pDest-1) = pHexDigit[(v >> 8) & 0xff];
        *(pDest-2) = pHexDigit[(v >> 16) & 0xff];
        *(pDest-3) = pHexDigit[(v >> 24) & 0xff];
        v >>= 32;
    }

    currentLength = string_adaptor::append(wstr, currentLength, buffer, size);

    return currentLength;
}
