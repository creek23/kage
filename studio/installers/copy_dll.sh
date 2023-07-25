#!/usr/bin/sh
#this will:
# 1. get a copy of kagestudio inside windows directory,
# 2. get the GTKMM DLLs
# 3. and copy it into windows directory
# 4. finally, will clean-up
cp ./windows/kagestudio.exe ./
rm ./windows/*.dll
ldd ./kagestudio.exe | sed -n 's/\([^ ]*\) => \/mingw64/cp \/c\/msys64\/mingw64/p' | sed -n 's/ (0x[a-f0-9)]*/ .\/windows/p' | sort > copy_gtkmm_dll.sh
chmod +x ./copy_gtkmm_dll.sh
./copy_gtkmm_dll.sh
rm copy_gtkmm_dll.sh
rm ./kagestudio.exe
echo GTKMM DLLs
ls -la ./windows/*.dll
