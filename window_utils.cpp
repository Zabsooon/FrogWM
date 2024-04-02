#include "window_utils.hpp"
#include <X11/Xlib.h>
#include <ostream>
#include <vector>

template<typename T>
::std::ostream& operator <<(::std::ostream& out, Size<T>& size) {
    return out << size.ToString();
}


template<typename T>
::std::string Vector2D<T>::ToString() const {
    ::std::ostringstream out;
    out << "(x:" << x_ << ", y:" << y_ << ")";
    return out.str();
}

template<typename T>
::std::ostream& operator <<(::std::ostream& out, Vector2D<T>& vector) {
    return out << vector.ToString();
}


template<typename T>
::std::string Position<T>::ToString() const {
    ::std::ostringstream out;
    out << "(x:" << x_ << ", y:" << y_ << ")";
    return out.str();
}

template<typename T>
::std::ostream& operator <<(::std::ostream& out, Position<T>& position) {
    return out << position.ToString();
}

template<typename T>
::std::string ToString(T& val) {
    ::std::ostringstream out;
    out << val;
    return out.str();
}

::std::string ToString(XEvent& e) {

    using ::std::string;
    using ::std::vector;
    using ::std::pair;

    /* There are a lot of events we have to bare in mind.
     * We store every property depending on type of event.
     */
    vector<pair<string, string>> properties;

    switch(e.type) {
        case KeyPress:
        case KeyRelease:
            properties.reserve(5);
            properties.emplace_back("window", ToString(e.xkey.window));
            properties.emplace_back("position_root", Position<int>(e.xkey.x_root, e.xkey.y_root).ToString());
            properties.emplace_back("state", ToString(e.xkey.state));
            properties.emplace_back("keycode", ToString(e.xkey.keycode));
            break;
        case ButtonPress:
        case ButtonRelease:
            properties.reserve(5);
            properties.emplace_back("window", ToString(e.xbutton.window));
            properties.emplace_back("position_root", Position<int>(e.xbutton.x_root, e.xbutton.y_root).ToString());
            properties.emplace_back("state", ToString(e.xbutton.state));
            properties.emplace_back("button", ToString(e.xbutton.button));
            break;
        case MotionNotify:
            properties.reserve(5);
            properties.emplace_back("window", ToString(e.xmotion.window));
            properties.emplace_back("position_root", Position<int>(e.xmotion.x_root, e.xmotion.y_root).ToString());
            properties.emplace_back("state", ToString(e.xmotion.state));
            properties.emplace_back("time", ToString(e.xmotion.time));
            properties.emplace_back("is_hint", ToString(e.xmotion.is_hint));
            break;
        case EnterNotify:
        case LeaveNotify:
            break;
        case FocusIn:
        case FocusOut:
            break;
        case KeymapNotify:
            break;
        case Expose:
        case GraphicsExpose:
        case NoExpose:
            break;
        case VisibilityNotify:
            break;
        case CreateNotify:
        case DestroyNotify:
            break;
        case UnmapNotify:
        case MapNotify:
            break;
        case MapRequest:
            break;
        case ReparentNotify:
            break;
        case ConfigureNotify:
        case ConfigureRequest:
            break;
        case GravityNotify:
            break;
        case ResizeRequest:
            break;
        case CirculateNotify:
        case CirculateRequest:
            break;
        case PropertyNotify:
            break;
        case SelectionClear:
        case SelectionRequest:
        case SelectionNotify:
            break;
        case ColormapNotify:
            break;
        case ClientMessage:
            break;
        case MappingNotify:
            break;
        case GenericEvent:
            break;
        case LASTEvent:
            break;
        default:
            // no properties are printed...
            break;
    }
    return "";
}
