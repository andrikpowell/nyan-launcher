# Intructions for compiling nyan-launcher for Windows


___

### Tools needed:

## Qt - https://www.qt.io/download
```
Tested with Qt 6.2.2
```
<img width="300" alt="Screenshot 2021-06-09 at 12 44 33" src="https://user-images.githubusercontent.com/82064173/121386518-fbd48c00-c941-11eb-8abc-bc2e1e7e5a69.png"><img width="250" alt="Screenshot 2021-06-09 at 12 46 06" src="https://user-images.githubusercontent.com/82064173/121386962-33433880-c942-11eb-9528-a3923dc4ee52.png">
```
When running the installer, I recommend that you install the "Qt for desktop environment"
```
<img width="400" alt="Screenshot 2021-06-09 at 10 56 12" src="https://user-images.githubusercontent.com/82064173/121387513-a351be80-c942-11eb-9962-536a66c03689.png">

## qmake
```
This comes installed with Qt, no need to do anything
```

## make
```
I don't think that Qt comes with "make" installed, if you have then you can start compiling,
if not, try to install it using chocolatry:
```
___
## Compiling

1. Download the .zip file from this repository
2. Extract it & open the folder
4. Create a "build" folder
```
mkdir build & cd build
```
5. Run qmake from the build folder
```
qmake ..

If you installed Qt with the online installer, careful with qmake not being in the path
```
6. Run make
```
make
```
Make sure that this launcher is in the same folder as **nyan-doom**
There you go!
You can now use this launcher

