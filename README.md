# About app:
Image taking / monitoring for enclosed environment 3D printers printing process.
Read setup.md for instruction on running headless with nomachine.

# Hardware:
1. Raspberrypi 4b 4gb
2. EPL 4K autofocus USB camera
3. raspbian os 64bit

# Requires:
1. Opencv 4.0
2. Qt 6.5.3
3. Boost
4. FFmpeg
5. date and timezone by howard

# Build: 
```
mkdir build && cd build 
cmake ../ 
```
then run 
```
cmake --build . 

 * optional install at /usr/local *
$ cmake --install .
```
# License
MIT. If you enjoyed this app, and found any bugs/issues please let me know!

# Current known issue:
sept24
1. IANA tzdata in 2024b has changed to 'April' instead of 'Apr'. Awaiting fix from IANA maintainer.
work around is manually to change northamerica file in tzdata at line 2634 to Apr so the tz parser works.

