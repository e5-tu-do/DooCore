#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CMAKE_BASE_DIR="$(dirname "$DIR")"
#CMAKE=/usr/local/bin/cmake

if [ -f CMakeLists.txt ]
then 
  echo "Directory contains CMakeLists.txt. You should use a clean out of source build. Exiting."
  exit 1
fi

if [ -f CMakeCache.txt ]
then 
  echo "Directory contains CMake files, will clean up."
  rm -rf bin/ cfg/ CMake* cmake_install.cmake Makefile lib/ main/ src/
fi

echo "Building CMake build system for installation to $DOOCORESYS."
if [ "$1" = "debug" ]
then
  $CMAKE_EXECUTABLE -DCMAKE_INSTALL_PREFIX=$DOOCORESYS -DCMAKE_BUILD_TYPE=Dbg $CMAKE_BASE_DIR
elif [ "$1" = "profiling" ]
then
  $CMAKE_EXECUTABLE -DCMAKE_INSTALL_PREFIX=$DOOCORESYS -DCMAKE_BUILD_TYPE=Profiling $CMAKE_BASE_DIR
else 
  $CMAKE_EXECUTABLE -DCMAKE_INSTALL_PREFIX=$DOOCORESYS -DCMAKE_BUILD_TYPE=Release $CMAKE_BASE_DIR
fi
