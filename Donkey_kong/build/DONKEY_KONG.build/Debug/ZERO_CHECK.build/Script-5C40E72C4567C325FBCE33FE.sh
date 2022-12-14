#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/Final_Project/Graphics_final_project/Donkey_kong/build
  make -f /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/Final_Project/Graphics_final_project/Donkey_kong/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/Final_Project/Graphics_final_project/Donkey_kong/build
  make -f /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/Final_Project/Graphics_final_project/Donkey_kong/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/Final_Project/Graphics_final_project/Donkey_kong/build
  make -f /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/Final_Project/Graphics_final_project/Donkey_kong/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/Final_Project/Graphics_final_project/Donkey_kong/build
  make -f /Users/chriscallahan/Desktop/2022_Fall_classes/Computer_Graphics/Final_Project/Graphics_final_project/Donkey_kong/build/CMakeScripts/ReRunCMake.make
fi

