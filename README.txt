wxWabbitemu on Windows README
==============================
[This readme BEST viewed in a monospaced font!]

This is the fruit from my little experiment! :)

Many people have wondered - if wxWabbitemu has wxWidgets, couldn't it be
compiled and ran on Windows? To satisfy that curiosity, I've set off to do it myself.
In the end, after countless code modifications, it worked.... sort of.
The screen flickers to the point of causing seizures... :P The icon is also non-existent,
too.

If you wish to try it yourself:
1) Remove any WINVER #ifdef'd code. If you see a #elif WXVER at the end of it, change
   it to #ifdev WXVER.
2) Change all the build_wx COMPILEC commands to COMPILECPP.
3) Add in the typedefs for u_* - I think they are in the include/winsock2.h file.
   (u_char, etc.)
4) Comment out any broken code (I think there was some in the label.c file, but probably
   more... at LEAST two files need modifications via commenting out lines.)
5) Test - recompile, fix, recompile, fix.
6) Modify gui_wx.cpp to use a full frame (with caption/titlebar, minimize/maximize/close buttons, etc.).
6) Done!

If you are downloading this source code, this has already been done for you.
There's also a CodeBlocks project that can compile it too! :)

If you are using the build_wx script (build_run_wx is NOT updated for building on Windows),
you need to have MSYS installed, with the PATH set to include the MinGW compiler.
Note that BASH is pretty darn slow, so CodeBlocks might be the best option. ;)
For whatever reason, the build_wx script won't link due to some weird issues
(likely because ANSI and Unicode is mixed up somewhere in the build process.)

Have fun! :) If you can fix this up a bit, patches are very much welcome! :D
