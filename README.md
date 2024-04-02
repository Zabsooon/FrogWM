# FrogWM

## How to build

```
cmake -S. -Bbuild
cd build
make
```

To run and test the FrogWM we are can use Xephyr

```
./run.sh
```

OR

To run wm on Xephyr
```
Xephyr :100 -ac -screen 800x600 -host-cursor &
DISPLAY=:100 ./FrogWM
```
