wxWabbitemu
============

What is wxWabbitemu?
---------------------
wxWabbitemu is a TI-8x emulator that uses the wxWidgets toolkit.
It is based on a decent TI-8x emulator, Wabbitemu.
Hence the name: wxWabbitemu!

It is designed to mainly run on Linux, but it can be run on other
platforms as well.

wxWabbitemu supports loading any TI-8x variables, programs, apps, etc.
It also supports loading and saving calculator states, capturing
screenshots, debugging programs, and much more!

wxWabbitemu History
--------------------
This project was originally started by [Albert Huang (alberthdev)][albert]
as an attempt to port a good TI-8x emulator to Linux.

(At the time, there was TilEm, a GTK+ TI-8X emulator, but it was buggy,
slow, and rarely updated.**)

Thankfully, a Wabbitemu developer, [Chris Shappel (Buckeye)][buckeye],
joined in to do most of the porting. Later, [David Gomes][dgomes] joined
development and helped develop the GUI.

Nowadays, these contributers are part of the project:

 * [Jon "Jonimus" Storm][jonimus]
 * [geekboy][geekboy]

** TilEm2:

At the time of writing, TilEm2, written by new developers
Thibault "contra-sh" Duponchelle and Benjamin "FloppusMaximus" Moody,
was released.

This version sports new emulation code, skinning support, and many
improvements. If you find that wxWabbitemu is causing you too many
problems, development/bug fixes are too slow, and/or you want to play
with something new, give their emulator a try!

http://lpg.ticalc.org/prj_tilem/

Installing wxWabbitemu
----------------------
wxWabbitemu can be found in the [eeems-linux](https://repo.eeems.codes/) pacman repository for arch based systems. Otherwise you will need to build it from source.

Building wxWabbitemu
---------------------
You need the following:

  * A working C compiler. This includes, but is not limited to,
    GCC and Clang.

  * The wxWidgets development files. Basically, the headers and linking
    libraries for wxWidgets. And of course, the wxWidgets library
    itself.

  * The GNU Make program. This program will build wxWabbitemu for you.
    BSD make might work as well, but no guarantees.

In all of the commands that you are instructed to type in, you should
follow each command with pressing the return key.

**Install Dependencies:**

  * Debian, Ubuntu, Mint Linux, and friends:
    
    Run:
    
        sudo apt-get update && sudo apt-get upgrade
    
    Reboot if necessary.
    
    Then open a terminal again (if you rebooted), and type:
    
        # If libwxgtk3.0-dev isn't available, try libwxgtk2.8-dev.
        # 2.8 is available on older distros, while 3.0 is available
        # on more recent releases.
        sudo apt-get install build-essential libwxgtk3.0-dev

  * Fedora, RHEL, and friends:
    
    Run:
    
        su -c "yum update"
    
    Reboot if necessary. 
    
    Then open a terminal again (if you rebooted), and type:
    
        su -c "yum install gcc gcc-c++ make wxGTK wxGTK-devel"

Once you have installed the dependencies, you can go ahead and
build wxWabbitemu!

**Build wxWabbitemu:**

In a terminal type:

    make

**Run wxWabbitemu:**

To run it, type:

    bin/wxwabbitemu

Note, though, that wxwabbitemu is a portable file and it can be copied
to anywhere in your computer and ran from any location.

**Install wxWabbitemu:**

To install, you can type:

    make install

Make sure you do this as root.

**Notes:**

If you want to build on another platform, or use a different build
system, you can use premake to generate a different file! In the premake
directory, run the premake program (with the options you want) to create
files that can build wxWabbitemu!

(If you're running Linux x86, we've included the premake4 binary.)

For more information and/or building instructions, visit the
Building page on our website:

https://github.com/alberthdev/wxwabbitemu/wiki/Building

The End
--------
That's it! We hope you enjoy wxWabbitemu! Should it fail to do
something, or crash, please help us by reporting bugs in our 
issue tracker, located at https://github.com/alberthdev/wxwabbitemu/issues.

License
--------
See [LICENSE](License)

[albert]: https://github.com/alberthdev
[buckeye]: https://github.com/BuckeyeDude
[dgomes]: https://github.com/davidgomes
[jonimus]: https://github.com/Jonimoose
[geekboy]: https://github.com/geekbozu
