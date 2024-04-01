#include <X11/X.h>
#include <unordered_map>
extern "C" {
#include <X11/Xlib.h>
}
#include <memory>

class WindowManager {
    public:
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

        // Xlib window notify event handlers
        void OnCreateNotify(const XCreateWindowEvent& e);
        void OnDestroyNotify(const XDestroyWindowEvent& e);
        void OnReparentNotify(const XReparentEvent& e);
        void OnConfigureRequest(const XConfigureRequestEvent& e);
        void OnConfigureNotify(const XConfigureEvent& e);
        void OnMapRequest(const XMapRequestEvent& e);
        void OnUnmapNotify(const XUnmapEvent& e);

        static bool wm_detected_;
    private:
        // Invoked internally by Create().
       WindowManager(Display* display);

        // Handle to the underlying Xlib Display struct.
        Display* display_;
        // Handle to root window.
        const Window root_;
        ::std::unordered_map<Window, Window> clients_;
};
