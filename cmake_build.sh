cmake --clean .
make clean
cmake -D OPTIMIZE=ON CMakeLists.txt
cmake .
make -j 4