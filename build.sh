mkdir build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cd ..