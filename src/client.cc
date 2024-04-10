#include "client.h"
#include <X11/Xlib.h>

frogwm::Client::Client(Display* display, Window window) :
        display_(display),
        window_(window),
        is_mapped_(false),
        is_fullscreen_(false),
        has_wm_requested_unmap_(){
    ::frogwm::Client::mapper_[window] = this;
}

frogwm::Client::~Client() {
    ::frogwm::Client::mapper_.erase(window_);
}

void frogwm::Client::Map() const {
    XMapWindow(display_, window_);
}

void frogwm::Client::Unmap() {
    if(!this->is_mapped_ || this->has_wm_requested_unmap_) {
        return;
    }

    this->has_wm_requested_unmap_ = true;
    XUnmapWindow(display_, window_);
}

void frogwm::Client::Raise() const {
    XRaiseWindow(display_, window_);
}

void frogwm::Client::Move(int x, int y, bool is_absolute) const {
    if(is_absolute) {
        XMoveWindow(display_, window_, x, y);
        return;
    }

    XMoveWindow(display_, window_, attrs_.x + x, attrs_.y + y);
}

void frogwm::Client::Resize(int width, int height, bool is_absolute) const {
    if(is_absolute) {
        XResizeWindow(display_, window_, width, height);
        return;
    }

    XResizeWindow(display_, window_, attrs_.width + width, attrs_.height + height);
}

Window frogwm::Client::Get_window_() const {
    return this->window_;
}

XWindowAttributes frogwm::Client::Get_attrs_() const {
    return this->attrs_;
}

bool frogwm::Client::Get_has_wm_requested_unmap_() const {
    return this->has_wm_requested_unmap_;
}

bool frogwm::Client::Get_is_mapped_() const {
    return this->is_mapped_;
}

bool frogwm::Client::Get_is_fullscreen_() const {
    return this->is_fullscreen_;
}

bool frogwm::Client::Get_is_floating_() const {
    return this->is_floating_;
}

void frogwm::Client::Set_window_(Window& window) {
    this->window_ = window;
}

void frogwm::Client::Set_attrs_(XWindowAttributes& attrs) {
    this->attrs_ = attrs;
}

void frogwm::Client::Set_has_wm_requested_unmap_(bool has_wm_requested_unmap) {
    this->has_wm_requested_unmap_ = has_wm_requested_unmap;
}

void frogwm::Client::Set_is_mapped_(bool is_mapped) {
    this->is_mapped_ = is_mapped;
}

void frogwm::Client::Set_is_fullscreen_(bool is_fullscreen) {
    this->is_fullscreen_ = is_fullscreen;
}

void frogwm::Client::Set_is_floating_(bool is_floating) {
    this->is_floating_ = is_floating;
}
