# Top down shooter in C
A very simple game written in C using the SDL2 library.

## Video demo
when done...
<!--
TODO: replace ... with yt str
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/.../maxresdefault.jpg)](https://www.youtube.com/watch?v=...)
-->

## Requirments
I have only built this game with gcc on Linux. It requires SDL2 to build.
```sh
# Requirments
sudo apt-get update
sudo apt-get install gcc
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
sudo apt-get libsdl2-mixer-dev
# Build
make -C src
# Run
./src/main.exe
```

## Flags
```sh
# Set screen width [min is 400, max is 0.9*ScreenRes]
./src/main.exe -w 800

# Set screen height [min is 400, max is 0.9*ScreenRes]
./src/main.exe -h 600

# Set number of enemies [min is 1, max is 1000]
./src/main.exe -z 100

# Example: 400 enemies in a 450x999 window
./src/main.exe -z 400 -w 450 -h 999
```


## TODO:
* Bullets
    projectile vs hit scan?
    - if projectile...
        - linked list with pre-allocated mem larger than |max lifetime of a bullet| * |bullets/sec|
        - motion math...
        - something to render...
        - last_fram_pos ---> this_frame_pos line segment circle coll
    - hitscan easy...
        - line circ coll
    - sound
* Blood Animation? Store blood struct in each enemy with seperate position...
* Death (game over mechanic)
