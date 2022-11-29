#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/algorithms-module-halo-3/Asteroids!_HD/build
  make -f /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/algorithms-module-halo-3/Asteroids!_HD/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/algorithms-module-halo-3/Asteroids!_HD/build
  make -f /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/algorithms-module-halo-3/Asteroids!_HD/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/algorithms-module-halo-3/Asteroids!_HD/build
  make -f /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/algorithms-module-halo-3/Asteroids!_HD/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/algorithms-module-halo-3/Asteroids!_HD/build
  make -f /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/algorithms-module-halo-3/Asteroids!_HD/build/CMakeScripts/ReRunCMake.make
fi

