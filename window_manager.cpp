#include "window_manager.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <glog/logging.h>

using ::std::unique_ptr;

unique_ptr<WindowManager> WindowManager::Create() {
    // 1. Open X Display.
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        LOG(ERROR) << "Failed to open X display " << XDisplayName(nullptr);
        return nullptr;
    }
    // 2. Construct WindowManager instance.
    return unique_ptr<WindowManager>(new WindowManager(display));
}

WindowManager::WindowManager(Display* display)
    : display_(CHECK_NOTNULL(display)),
    root_(DefaultRootWindow(display_)) {}

WindowManager::~WindowManager() {
    XCloseDisplay(display_);
}

void WindowManager::Run() {
    // 1. Initiallization.
    //  a. Select events on root window. Use a special error handler so we can
    //  exit gracefully if another window manager is already running.
    wm_detected_ = false;
    XSetErrorHandler(&WindowManager::OnWMDetected);
    XSelectInput(
            display_,
            root_,
            SubstructureRedirectMask | SubstructureNotifyMask);
    XSync(display_, false);
    if (wm_detected_) {
        LOG(ERROR) << "Detected another window manager on display "
            << XDisplayString(display_);
        return;
    }
    //  b. set error handler.
    XSetErrorHandler(&WindowManager::OnXError);

    /*  c. Grab X server to prevent windows from changing under us,
     *  while we frame them.
     */
    XGrabServer(display_);

    /*  d. Frame existing Top-Level windows .
     *    i. Query existing Top-Level windows.
     */
    Window returned_root, returned_parent;
    Window* top_level_windows;
    unsinged int num_top_level_windows;
    CHECK(XQueryTree(
        display_,
        root_,
        &returned_root,
        &returned_parent,
        &top_level_windows,
        &num_top_level_windows));

    CHECK_EQ(returned_root, root_);

    //    ii. Frame each Top-Level window.
    for(unsinged int i = 0; i < num_top_level_windows; ++i) {
        Frame(top_level_windows[i], true /* was_created_before_window_manager */);
    }

    //    iii. Free Top-Level window array.
    XFree(top_level_windows);
    XUngrabServer(display_);


    // 2. Main event loop.
    while(true) {
        // 1. Get next event.
        XEvent e;
        XNextEvent(display_, &e);
        LOG(INFO) << "Received event: " << ToString(e);

        // 2. Dispatch event.
        switch(e.type) {
            case CreateNotify:
                OnCreateNotify(e.xcreatewindow);
                break;
            case DestroyNotify:
                OnDestroyNotify(e.xdestroywindow);
                break;
            case ReparentNotify:
                OnReparentNotify(e.xreparent);
                break;
            case ConfigureRequest:
                OnConfigureRequest(e.xconfigurerequest);
                break;
            case ConfigureNotify:
                OnConfigureNotfiy(e.xconfigure);
                break;
            case MapRequest:
                OnMapRequest(e.xmaprequest);
                break;
            case UnmapNotify:
                OnUnmapNotify(e.xunmap);
                break;

            default:
                LOG(WARNING) << "Ignored event";
        }
    }
}

int WindowManager::OnWMDetected(Display* display, XErrorEvent* e) {
    // In case of another window manager already running, the error code from
    // XSelectInput is BadAccess. We don't expect this handler to recive any
    // other errors.
    CHECK_EQ(static_cast<int>(e->error_code), BadAccess);
    // Set flag.
    wm_detected_ = true;
    // The return value is ignored.
    return 0;
}

void WindowManager::Frame(Window w, bool was_created_before_window_manager) {

    // Visual properties of the frame to create.
    const unsigned int BORDER_WIDTH = 3;
    const unsigned long BORDER_COLOR = 0xff0000;
    const unsigned long BG_COLOR = 0x0000ff;

    XWindowAttributes x_window_attrs;
    CHECK(XGetWindowAttributes(display_, w, &x_window_attrs));

    // Frameing Existing Top-Level Windows.
    if(was_created_before_window_manager) {
        if(x_window_attrs.override_redirect || x_window_attrs.map_state != IsViewable) {
            return;
        }
    }

    // Create Frame.
    const Window frame = XCreateSimpleWindow(
            display_,
            root_,
            x_window_attrs.x,
            x_window_attrs.y,
            x_window_attrs.width,
            x_window_attrs.height,
            BORDER_WIDTH,
            BORDER_COLOR,
            BG_COLOR);

    XSelectInput(
            display_,
            frame,
            SubstructureRedirectMask | SubstructureNotifyMask);

    XAddToSaveSet(display_, w);
    XReparentWindow(
            display_,
            w,
            frame,
            0, 0);

    XMapWindow(display_, frame);

    clients_[w] = frame;

    LOG(INFO) << "Framed window " << w << " [" << frame << "]";
}

void WindowManager::Unframe(Window w) {
    const Window frame = clients_[w];

    XUnmapWindow(display_, frame);
    XReparentWindow(
            display_,
            w,
            root_,
            0, 0);
    XRemoveFromSaveSet(display_, w);
    clients_.erase(w);

    LOG(INFO) << "Unframed window " << w << " [" << frame << "]";
}

int WindowManager::OnXError(Display* display, XErrorEvent* e) {
    // Print e
}

void WindowManager::OnCreateNotify(const XCreateWindowEvent& e) {

}

void WindowManager::OnDestroyNotify(const XDestroyWindowEvent& e) {

}

void WindowManager::OnReparentNotify(const XReparentEvent& e) {

}

void WindowManager::OnConfigureRequest(const XConfigureRequestEvent& e) {
    XWindowChanges changes;

    // Copy fields from e to changes.
    changes.x = e.x;
    changes.y = e.y;
    changes.width = e.width;
    changes.height = e.height;
    changes.border_width = e.border_width;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;

    if(clients_.count(e.window)) {
        const Window frame = clients_[e.window];
        XConfigureWindow(display_, frame, e.value_mask, &changes);
        LOG(INFO) << "Resize [" << frame << "] to " << Size<int>(e.width, e.height);
    }

    // Grant request by calling XConfigureWindow().
    XConfigureWindow(display_, e.window, e.value_mask, &changes);
    LOG(INFO) << "Resize " << e.window << " to " << Size<int>(e.width, e.height);
}

void WindowManager::OnConfigureNotify(const XConfigureEvent& e) {

}

void WindowManager::OnMapRequest(const XMapRequestEvent& e) {
    Frame(e.window, false);
    XMapWindow(display_, e.window);
}

void WindowManager::OnUnmapNotify(const XUnmapEvent& e) {
    /* If the window is a client window we manage, unframe it upon UnmapNotify.
     * We need the check because we will receive an UnmapNotify event for a frame
     * window we just destroyed ourselves
     */
    if(!clients_.count(e.window)) {
        LOG(INFO) << "Ignore UnmapNotify for non-client window " << e.window;
        return;
    }

    /* Ignore event if it is triggered by reparenting a window that was mapped
     * before the window manager started.
     *
     * Since we receive UnmapNotify events from the SubstructureNotify mask, the
     * event attribute specifies the parent window of the window that was
     * unmapped. This means that an UnmapNotify event from a normal client window
     * should have this attribute set to a frame window we maintain. Only an
     * UnmapNotify event triggered by reparenting a pre-existing window will have
     * this attribute set to the root window.
     */
    if(e.event == root_) {
        LOG(INFO) << "Ignore UnmapNotify for reparented pre-existing window "
            << e.window;
        return;
    }

    Unframe(e.window);
}
