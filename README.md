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

## TODO:
1. Render enemies
2. Move enemies
3. Animate enemies
4. Respawn enemies
5. Collision enemy+player
6. Data structure for bullets (pre-allocated hashset?)
7. Render Bullets
8. Move bullets
9. Collision bullets+enemies
10. Sound effect for bullets
11. Data structure for blood
12. Render blood
13. Animate blood
14. Death (game over)
