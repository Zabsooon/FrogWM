#!/bin/bash

# Replace 'window_manager_executable' with the actual name of your window manager executable
WINDOW_MANAGER_EXECUTABLE="./build/FrogWM"

# Check if the executable exists
if [ ! -f "$WINDOW_MANAGER_EXECUTABLE" ]; then
    echo "Window manager executable not found."
    exit 1
fi

# Start Xephyr nested X server with a virtual display
Xephyr :100 -ac -screen 1280x720 &

# Set the DISPLAY environment variable to the nested X server
export DISPLAY=:100

# Run your window manager in the nested X server
$WINDOW_MANAGER_EXECUTABLE &

# Give some time for the window manager to start
sleep 2

# Run some test applications or actions to see if the window manager behaves as expected
# For example:
 xterm &
 xeyes &
# firefox &
# etc.

# Optionally, you can keep the nested X server running after the tests are done for further inspection
# Uncomment the following line if you want to keep the nested X server running
# trap "killall Xephyr" EXIT

# You can also add cleanup code if needed
# trap "cleanup" EXIT
# cleanup() {
#     echo "Performing cleanup..."
#     # Any cleanup commands go here
# }

# Optionally, you can leave the script running indefinitely to keep the test environment active
# Comment out the following line if you want to leave the script running indefinitely
# wait

