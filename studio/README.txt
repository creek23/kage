Current version is v0.2.200307
Kage Studio is a simple, Free and Open Source 2D Animation Software
written by Mj Mendoza IV <mj.mendoza.iv at gmail dot com>


HOW TO COMPILE Kage Studio (on Windows)
=======================================
1 install MSYS2 if you haven't already
2 open MSYS and install 'gtkmm3', 'gcc', 'pkg-config', and 'make' via pacman
	MSYS2: pacman -S mingw-w64-x86_64-gtkmm3 mingw-w64-x86_64-gcc mingw-w64-x86_64-pkg-config mingw-w64-x86_64-make
3 cd to this path
4 then type: 'make -f Makefile.win'


HOW TO COMPILE Kage Studio (on GNU/Linux)
=========================================
1 open Terminal and install 'gtkmm3', 'cairo'
	Ubuntu: libgtkmm-3.0-dev libcairomm-1.0-dev
2 cd to this path
3 type: 'make -f Makefile.gnu'
