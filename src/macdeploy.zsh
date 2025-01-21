#!/bin/zsh

# Run this from src/

mkdir build
cd build

/Users/pedro/Qt/6.6.1/macos/bin/qmake .. "CONFIG+=deploy" "CONFIG+=release"
make -j6

rm ./nyan-launcher.app/Contents/Resources/nyan-doom
rm ./nyan-launcher.app/Contents/Resources/nyan-doom.wad

cp ~/Documents/Github/nyan-doom/prboom2/build/nyan-doom ./nyan-launcher.app/Contents/Resources/nyan-doom
cp ~/Documents/Github/nyan-doom/prboom2/build/nyan-doom.wad ./nyan-launcher.app/Contents/Resources/nyan-doom.wad
cp -r ~/Documents/Github/nyan-doom/prboom2/build/libs_arm64 ./nyan-launcher.app/Contents/Resources/
cp -r ~/Documents/Github/nyan-doom/prboom2/build/libs_x64 ./nyan-launcher.app/Contents/Resources/

/Users/pedro/Qt/6.6.1/macos/bin/macdeployqt  ./nyan-launcher.app

create-dmg --app-drop-link 10 10 ./nyan-launcher.dmg ./nyan-launcher.app