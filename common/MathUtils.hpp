#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include <type_traits>
#include <limits>

#include <configure.h>


namespace PROGRAM_NAMESPACE {

class MathUtils {

private:
    static constexpr int ULP_DEFAULT = 2;

public:

    template<class T>
    static inline typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type almostEqual(T x, T y, int ulp = ULP_DEFAULT) {
        // NOTE NIC 20/05/2019 - vedere link en.cppreference - numeric limits - epsilon
        return std::abs(x-y) <= std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp
            || std::abs(x-y) < std::numeric_limits<T>::min();
    }

    template<class T>
    static inline typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type almostEqual(T x, T y) {
        return x == y;
    }

};

}

#endif // MATHUTILS_HPP
