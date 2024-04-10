#ifndef FROGWM_WINDOW_MANAGER_H_
#define FROGWM_WINDOW_MANAGER_H_

#include <X11/X.h>
#include <unordered_map>
#include <unordered_set>
#include <memory>
extern "C" {
#include <X11/Xlib.h>
}
#include "window_utils.h"
#include "client.h"

namespace frogwm {

class WindowManager {
    public:
        static const char* Version();
        // Factory method for establishing a connection to an X server and creating a
        // WindowManager instance.
        static ::std::unique_ptr<WindowManager> Create();
        // Disconnects from the X server.
        ~WindowManager();
        // The entry point to this class. Enters the main event loop.
        void Run();

        // Xlib error handler. It must be static as its address is passed to Xlib.
        static int OnXError(Display* display, XErrorEvent* e);

        /* Xlib error handler used to determine whether another window manager is
         * running. It is set as the error handler right before selecting substructure
         * redirection mask on the root window, so it is invoked if and only if
         * another window manager is running. It must be static as its address is
         * passed to Xlib.
         */
        static int OnWMDetected(Display* display, XErrorEvent* e);
        void Frame(Window w, bool was_created_before_window_manager);
        void Unframe(Window w);

        // XEvent handlers
        void OnCreateNotify(const XCreateWindowEvent& e);
        void OnDestroyNotify(const XDestroyWindowEvent& e);
        void OnReparentNotify(const XReparentEvent& e);
        void OnConfigureRequest(const XConfigureRequestEvent& e);
        void OnConfigureNotify(const XConfigureEvent& e);
        void OnMapRequest(const XMapRequestEvent& e);
        void OnMapNotify(const XMapEvent& e);
        void OnUnmapNotify(const XUnmapEvent& e);
        void OnKeyPress(const XKeyEvent& e);
        void OnButtonPress(const XButtonEvent& e);
        void OnButtonRelease(const XButtonReleasedEvent& e);
        void OnMotionNotify(const XButtonEvent& e);
        void OnEnterNotify(const XEnterWindowEvent& e);
        void OnClientMessage(const XClientMessageEvent& e);

        static bool wm_detected_;
    private:
        // Invoked internally by Create().
        WindowManager(Display* display);

        // Handle to the underlying Xlib Display struct.
        Display* display_;
        // Handle to root window.
        const Window root_;
        ::std::unordered_map<Window, Window> clients_;

        /* Some programs (for example Stream) tend to work in background...
         * this is the structure to keep track of them so we won't destroy them
         * if those programs just want to work in the background
         */
        ::std::unordered_set<Client> hidden_clients_;
};

} // namespace frogwm

#endif // FROGWM_WINDOW_MANAGER_H_
