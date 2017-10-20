
#include "miniformat.h"

#if __cplusplus > 199711L || (defined(_MSC_VER) && _MSC_VER >= 1800) // if this is a C++11-compliant compiler or VS2013 and above
// Variadic template from C++11 is supported. Let's use it!
namespace miniformat
{
    namespace detail
    {
        int sumSizes()
        {
            return 0;
        }
    }
}
#endif
