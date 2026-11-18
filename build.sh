cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build

mkdir -p bin/resources
rsync -a --delete resources/ bin/resources/
