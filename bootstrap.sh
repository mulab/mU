#!/bin/bash
echo
echo "*************************************************************************"
echo "installing building and version control tools..."
sudo apt-get install --quiet build-essential subversion cmake

echo
read -p "also install the QT GUI for cmake? (y/n) [press ENTER for y]" YN
if [ "y${YN}" != "yn" -a "y${YN}" != "yN" ]; then
    sudo apt-get install --quiet cmake-qt-gui
fi

echo
echo "*************************************************************************"
echo "installing required libraries..." 
sudo apt-get install --quiet libboost-dev libboost-system-dev libgmp3-dev libmpfr-dev

echo
echo "*************************************************************************"
echo "generating Makefiles..."
rm -f CMakeCache.txt
echo
read -p "also generate an eclipse cdt project? (y/n) [press ENTER for y]" YN1
echo
echo "please choose build type:"
echo "1) debug version"
echo "2) release version"
echo "3) release version with debug info"
echo "4) release version with minimum size"
read -p "please choose (1-4): [press ENTER for 1]" BT
case "${BT}" in
    2) TYPE="Release" ;;
    3) TYPE="RelWithDebInfo" ;;
    4) TYPE="MinSizeRel" ;;
    *) TYPE="Debug" ;;
esac

if [ "y${YN1}" != "yn" -a "y${YN1}" != "yN" ]; then
    cmake -G "Eclipse CDT4 - Unix Makefiles" -D "CMAKE_BUILD_TYPE:STRING=${TYPE}" .
    echo
    echo "now you can launch eclipse, select \"File -> Import... -> Existing Projects into Workspace\", then select this directory to start importing"
else
    cmake -G "Unix Makefiles" -D "CMAKE_BUILD_TYPE:STRING=${TYPE}" .
    echo
    echo "now you can type \"make\" to starting building"
fi

echo "enjoy hacking the mU kernel!"
