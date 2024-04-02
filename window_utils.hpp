#ifndef FrogWM_utils
#define FrogWM_utils

extern "C" {
#include <X11/Xlib.h>
}
#include <ostream>
#include <sstream>
#include <string>

/* This file contains the declarations of structures needed to describe windows,
 * such as their position on screen, size etc.
 */

template<typename T>
struct Size {
    T width_;
    T height_;

    Size() = default;
    Size(T width, T height) :
        width_(width), height_(height) {}

    ::std::string ToString() const;
};

template<typename T>
std::string Size<T>::ToString() const {
    std::ostringstream out;
    out << width_ << 'x' << height_;
    return out.str();
}

template<typename T>
::std::ostream& operator <<(::std::ostream& out, Size<T>& size);


template<typename T>
struct Vector2D {
    T x_;
    T y_;

    Vector2D() = default;
    Vector2D(T x, T y) :
        x_(x), y_(y) {}

    ::std::string ToString() const;
};

template<typename T>
::std::ostream& operator <<(::std::ostream& out, Vector2D<T>& vector);


template<typename T>
struct Position {
    T x_;
    T y_;

    Position() = default;
    Position(T x, T y) :
        x_(x), y_(y) {}

    ::std::string ToString() const;
};

template<typename T>
::std::ostream& operator <<(::std::ostream& out, Position<T>& position);

template<typename T>
::std::string ToString(T& val);

::std::string ToString(XEvent& e);

#endif // FrogWM_utils
