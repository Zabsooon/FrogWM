#ifndef FROGWM_CLIENT_H_
#define FROGWM_CLIENT_H_

#include <unordered_map>
extern "C" {
#include <X11/Xlib.h>
}

namespace frogwm {

class Client {

public:

    static std::unordered_map<Window, Client*> mapper_; // maps Window to Client*

    Client(Display* display, Window window);
    ~Client();

    void Map() const;
    void Unmap();
    void Raise() const;
    void Move(int x, int y, bool is_absolute = true) const;
    void Resize(int width, int height, bool is_absolute = true) const;


    /* Getters */
    Window Get_window_() const;
    XWindowAttributes Get_attrs_() const;
    bool Get_has_wm_requested_unmap_() const;
    bool Get_is_mapped_() const;
    bool Get_is_fullscreen_() const;
    bool Get_is_floating_() const;

    /* Setters */
    void Set_window_(Window& window);
    void Set_attrs_(XWindowAttributes& attrs);
    void Set_has_wm_requested_unmap_(bool has_wm_requested_unmap);
    void Set_is_mapped_(bool is_mapped);
    void Set_is_fullscreen_(bool is_fullscreen);
    void Set_is_floating_(bool is_floating);

private:

    Display* display_;
    Window window_;
    XWindowAttributes attrs_;

    bool has_wm_requested_unmap_;

    bool is_mapped_;
    bool is_fullscreen_;
    bool is_floating_;

};

} // namespace frogwm

#endif // FROGWM_CLIENT_H_
