wxWabbitemu README
===================
Updated: 06/11/2012

What is wxWabbitemu?
---------------------
wxWabbitemu is a TI-8x emulator that uses the wxWidgets toolkit.
It is based on a decent TI-8x emulator, Wabbitemu.
Hence the name: wxWabbitemu!

It is designed to mainly run on Linux, but it can be run on
other platforms as well.

wxWabbitemu supports loading any TI-8x variables, programs,
apps, etc. It also supports loading and saving calculator
states, capturing screenshots, debugging programs, and
much more!

Who wrote wxWabbitemu?
-----------------------
This project was originally started by Albert Huang (alberthdev)
as an attempt to port a good TI-8x emulator to Linux. (At the
time, there was TilEm, a GTK+ TI-8X emulator, but it was
buggy, slow, and rarely updated.**) Thankfully, a Wabbitemu
developer, "Buckeye", joined in to do most of the porting.
Later, David Gomes (Scout/ephan) joined development and
helped develop the GUI. (He later left to work on the
prestigous Experience desktop. We wish him good luck!)
Nowadays, Jon "Jonimus" Storm has joined the project as
a "cleanup" guy to ensure code and build system quality.

** At the time of writing, TilEm2, written by new developers
Thibault "contra-sh" Duponchelle and Benjamin "FloppusMaximus" Moody,
was released. This version sports a new emulation code, skinning
support, and many, many improvements. If you find that
wxWabbitemu is causing you too many problems,
development/bug fixes are too slow, and/or you want to play
with something new, give their emulator a try!
http://lpg.ticalc.org/prj_tilem/

Building wxWabbitemu
---------------------
You need the following:
	* A working C compiler. This includes, but is not limited to,
	  GCC and Clang.
	* The wxWidgets development files. Basically, the headers and
	  linking libraries for wxWidgets. And of course, the wxWidgets
	  library itself.
	* The GNU Make program. This program will build wxWabbitemu
	  for you. BSD make might work as well, but no guarantees.

In all of the commands that you are instructed to type in, you should
follow each command with pressing the return key.
	* Debian, Ubuntu, Mint Linux, and friends
		sudo apt-get update && sudo apt-get upgrade
		
		Reboot if necessary.
		
		Then open a terminal again (if you rebooted), and type:
		sudo apt-get install build-essential libwxgtk2.8-dev

	* Fedora, RHEL, and friends ==
		su -c "yum update"
		
		Reboot if necessary. 
		
		Then open a terminal again (if you rebooted), and type:
		su -c "yum install gcc gcc-c++ make wxGTK wxGTK-devel"

Once you have installed the dependencies, you can go ahead and
build wxWabbitemu!

In a terminal type:
	cd build
	make

To run it, type:
	../bin/wxwabbitemu

Note, though, that wxwabbitemu is a portable file and it can
be copied to anywhere in your computer and ran from any location.

To install, you can type:
	make install
Make sure you do this as root.

If you want to build on another platform, or use a different
build system, you can use premake to generate a different file!
In the premake directory, run the premake program (with the
options you want) to create files that can build wxWabbitemu!
(If you're running Linux x86, we've included the premake4 binary.)

For more information and/or building instructions, visit the
Building page on our website:
https://github.com/alberthdev/wxwabbitemu/wiki/Building

That's it! We hope you enjoy wxWabbitemu! Should it fail to do
something, or crash, please help us by reporting bugs in our 
issue tracker, located at https://github.com/alberthdev/wxwabbitemu/issues.
