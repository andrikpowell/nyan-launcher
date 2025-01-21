# Intructions for compiling Nyan Launcher for Linux


## Qt - https://www.qt.io/download
```
All commits tested using qt6.2.0 on the latest ubuntu

You could also install qt using your prefered package manager
```

## Compiling

1. Clone this repo
```
git clone https://github.com/andrikpowell/nyan-launcher.git
```

2. Create a "build" folder inside src/
```
cd nyan-launcher
cd src

mkdir build
cd build
```
3. Run qmake from the build folder
```
qmake6 ..
```
4. Run make
```
make
```

A basic build+install script would be
```
#!/bin/bash

# building
	mkdir -p "./src/build"
	cd "./src/build"
	qmake6 ..
	make
	
#installing
	sudo mv ./nyan-launcher /bin
	cd ..
	sudo install -Dm644 ./icons/nyan-launcher.png "${pkgdir}"/usr/share/pixmaps/nyan-launcher.png
	sudo install -Dm644 ./icons/nyan-launcher.desktop "${pkgdir}"/usr/share/applications/nyan-launcher.desktop
```



## Distributing

For distribution of the binary, you can use `linuxdeployqt` which makes an appimage with everything the users will need to run in their system.
