#!/bin/bash

# building
	mkdir -p "./src/build"
	cd "./src/build"
	qmake6 ..
	make
	
#installing
	sudo mv ./src /bin
	cd ..
	sudo install -Dm644 ./icons/nyan-launcher.png "${pkgdir}"/usr/share/pixmaps/nyan-launcher.png
	sudo install -Dm644 ./icons/nyan-Launcher.desktop "${pkgdir}"/usr/share/applications/nyan-Launcher.desktop
	
# if you want to vomit from this install script blame this fool: Zse#0330
