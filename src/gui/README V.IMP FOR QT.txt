steps to run:
1- create a folder build: mkdir build  (Delete it if exists)
2- cd build
3- cmake -G "MinGW Makefiles" -DQt6_DIR="E:/DSA/Qt/AppFiles/6.10.1/mingw_64/lib/cmake/Qt6" ..
[or equivalent path in ur pc likely C:/Qt/6.10.1/mingw_64/lib/cmake/Qt6] !! better go check your intallation path

4- cmake --build .

then press f5 it should run normally


!!!!! NOTE : YOU Should install Qt first from here [ https://www.qt.io/development/download ]


# update "miDebuggerPath": "E:/DSA/Qt/AppFiles/Tools/mingw1310_64/bin/gdb.exe" in launch.json according to ur pc
## update tasks in launch.json