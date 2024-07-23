# Intructions for compiling nyan-launcher for MacOS

Tested with a M1 MacBookPro running MacOS 11.3 (Big Sur)

___

### Tools needed:

## Qt - https://www.qt.io/download
```
Tested with Qt 6.2.2

You can install it with "brew" or the Qt online installer
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
I don't think that Qt comes with "make" installed, if you have then you can start compiling, if not, run:

xcode-select --install
```
___
## Compiling

1. Download the .zip file from this repository

2. Extract it & open the folder
 
3. Put the **nyan-doom** executable and **nyan-doom.wad** (not included in this repo) inside the nyan-launcher folder

      ^^^ Very important

4. Create a "build" folder
```
mkdir build & cd build
```

Your nyan-launcher folder should look like this: 

![Screenshot 2021-07-07 at 10 22 17](https://user-images.githubusercontent.com/82064173/124734657-40395480-df0d-11eb-81c1-fd19231c5035.jpg)


5. Run qmake from inside of the build folder
```
qmake ..

If you installed Qt with the online installer, careful with qmake not being in the path


// qmake .. "CONFIG+=deploy"
// ^ this will compile for both x64 and arm64 archs

```
6. Run make
```
make
```
There you go!
You can now use this launcher

7. (optional) macdeployqt

This is only for the people that want to distribute this launcher.
macdeployqt is a tool to copy the required libraries into the .app

It comes installed with Qt. Its in the same folder as qmake
```
macdeployqt /path/to/nyan-launcher.app
```
