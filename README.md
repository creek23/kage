# kage
https://kage.sourceforge.io/
[![screenshot](https://i.ytimg.com/vi/bironRFlZ7M/hqdefault.jpg)](https://www.youtube.com/watch?v=bironRFlZ7M)

**Kage Studio** is a cross-platform 2D Vector Animation Software for Windows, Mac, and Ubuntu GNU/Linux written with [GTKmm 3.0](https://www.gtkmm.org/en/) that can export to HTML5 (canvas) or AVI video format (via [FFmpeg](https://ffmpeg.org/)) and [KonsolScript](http://konsolscript.sourceforge.net/).

Goal is to have a full-fledged, free and open source 2D animation software that is light weight, intuitive, and easy-to-use. Think of MS Paint to Photoshop (or [GIMP](https://www.gimp.org/)); or Notepad to Visual Studio (or Android Studio).

Interested individuals may join this project. Join the forums and get involved!

**Kage Studio** is written by *Mj Mendoza IV* <mj.mendoza.iv at gmail dot com>

Installer:
==========
*Current version is **v0.6.220622***

All installers and precompiled binaries are located in [Kage Studio's SourceForge page](https://sourceforge.net/projects/kage/files/).

To compile source code, change directory inside ```/kage/studio{here}```
 * **Windows** -- choose among Installer or ZIP file
![Kage Studio on Windows](https://scontent.fyvr4-1.fna.fbcdn.net/v/t1.6435-9/122459092_108196437748637_981599371383616234_n.png?_nc_cat=111&ccb=1-7&_nc_sid=8024bb&_nc_ohc=OYtnrDj6tKcAX_cqnUK&_nc_ht=scontent.fyvr4-1.fna&oh=00_AT-oAA1GNHOJ9BUjAyRIVujtK_O6LBRRod2vBYyATCxJbQ&oe=62EB49C9)
 	* **Installer** -- you can choose to install kagestudio along with FFMPEG and Gtkmm-3.0 runtime

 	* **ZIP** -- simply extract and run kagestudio.exe

 * **GNU/Linux** -- choose among DEB file or SRC file
![Kage Studio on Linux](https://a.fsdn.com/con/app/proj/kage/screenshots/v191017%5B1%5D.png/max/max/1)
 	* **DEB** -- install DEB file and will ask you to install FFMPEG and Gtkmm-3.0 runtime:
```
sudo apt install -f ./kagestudio_0.6-220622_amd64.deb
```
 	
 	* **SRC** -- compile source code using:
```
sudo apt install make libgtkmm-3.0-dev build-essential
make -f Makefile.gnu resource
make -f Makefile.gnu
```

 * **Mac** -- choose among ZIP file or SRC file
![Kage Studio on Mac](https://kage.sourceforge.io/images/kage_0_6.png)
 	* **ZIP** -- requires that you have homebrew installed along with FFMPEG and Gtkmm-3.0 runtime

 	* **SRC** -- compile source code using:
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install gcc gtkmm3 pkg-config make ffmpeg
make -f Makefile.mac resource
make -f Makefile.mac
```


Demo:
=====
Open file "demo.ksf" or, check out these links:
 * [![Powerpuff Girls](https://i.ytimg.com/vi/RJ6j1g49arU/hqdefault.jpg)](https://www.youtube.com/watch?v=RJ6j1g49arU)
 * [![Kimi No Nawa](https://i.ytimg.com/vi/mb4geCP4NSM/hqdefault.jpg)](https://www.youtube.com/watch?v=mb4geCP4NSM)
 * [![Le Monkey Monster the Animation](https://i.ytimg.com/vi/CELOVDCIoQ4/hqdefault.jpg)](https://www.youtube.com/watch?v=CELOVDCIoQ4)
 * [![Combatron on Kage Studio](https://i.ytimg.com/vi/MogqL4ue3AA/hqdefault.jpg)](https://www.youtube.com/watch?v=MogqL4ue3AA)
 * [![Revising Combatron's face](https://i.ytimg.com/vi/8NTx-fIn0h0/hqdefault.jpg)](https://www.youtube.com/watch?v=8NTx-fIn0h0)
 
Tutorials:
==========
Visit https://sourceforge.net/p/kage/wiki/Tutorials/

For Kage Studio help, contact author (email address indicated above).

License:
========
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.  Or, see <https://www.gnu.org/licenses/>.
