#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/markbiju/Documents/GitHub/Graphics_final_project/Asteroids!_HD_copy/build
  make -f /Users/markbiju/Documents/GitHub/Graphics_final_project/Asteroids!_HD_copy/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/markbiju/Documents/GitHub/Graphics_final_project/Asteroids!_HD_copy/build
  make -f /Users/markbiju/Documents/GitHub/Graphics_final_project/Asteroids!_HD_copy/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/markbiju/Documents/GitHub/Graphics_final_project/Asteroids!_HD_copy/build
  make -f /Users/markbiju/Documents/GitHub/Graphics_final_project/Asteroids!_HD_copy/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/markbiju/Documents/GitHub/Graphics_final_project/Asteroids!_HD_copy/build
  make -f /Users/markbiju/Documents/GitHub/Graphics_final_project/Asteroids!_HD_copy/build/CMakeScripts/ReRunCMake.make
fi

