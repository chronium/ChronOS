#ifndef _INITIALIZER_LIST_H
#define _INITIALIZER_LIST_H 1

#include <sys/types.h>

extern "C++" {

namespace std {

template <typename T>
struct initializer_list {
    const T* first;
    size_t _size;

    constexpr initializer_list (const T* __b, size_t __s) noexcept : first (__b), _size (__s) {

    }

public:
    using value_type        = T;
    using reference         = const T&;
    using const_reference   = const T&;
    using size_type         = size_t;
    using iterator          = const T*;
    using const_iterator    = const T*;

    constexpr initializer_list () noexcept : first (nullptr), _size (0) {

    } 

    constexpr size_t size () const noexcept {
        return _size;
    }

    constexpr const T* begin () const noexcept {
        return first;
    }

    constexpr const T* end () const noexcept {
        return first + _size;
    }
};

template <typename T>
inline constexpr const T* begin (initializer_list<T> list) noexcept {
    return list.begin ();
}

template <typename T>
inline constexpr const T* end (initializer_list<T> list) noexcept {
    return list.end ();
}

}

}

#endif
