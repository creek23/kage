Current version is v0.3.200512
Kage Studio is a simple, Free and Open Source 2D Animation Software
written by Mj Mendoza IV <mj.mendoza.iv at gmail dot com>

Kage Studio uses FFMPEG to produce AVI
FFMPEG is by FFmpeg team.  see ffmpeg.org

Kage Studio uses GTKMM3 as User Interface
GTKMM3 is by GTK Team.  see gtkmm.org

Demo:
=====
Open file "demo.ksf" or, check out these links:
 * https://www.youtube.com/watch?v=CELOVDCIoQ4
 * https://www.youtube.com/watch?v=MogqL4ue3AA
 * https://www.youtube.com/watch?v=8NTx-fIn0h0

Tutorials:
==========
Visit https://sourceforge.net/p/kage/wiki/Tutorials/


For Kage Studio help, contact author (email address indicated above).


HOW TO COMPILE Kage Studio (on Windows)
=======================================
1 install MSYS2 if you haven't already
2 open MSYS and install 'gtkmm3', 'gcc', 'pkg-config', and 'make' via pacman
	MSYS2: pacman -S mingw-w64-x86_64-gtkmm3 mingw-w64-x86_64-gcc mingw-w64-x86_64-pkg-config mingw-w64-x86_64-make
3 cd to this path
4 then type: 'make -f Makefile.win resource'
5 finally type: 'make -f Makefile.win'


HOW TO COMPILE Kage Studio (on GNU/Linux)
=========================================
1 open Terminal and install 'gtkmm3', 'g++' and 'make' via apt
	Ubuntu: sudo apt install make libgtkmm-3.0-dev build-essential
2 cd to this path
3 then type: 'make -f Makefile.gnu resource'
4 finally type: 'make -f Makefile.gnu'
