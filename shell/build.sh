CWD=$PWD
BUILD_PATH=$CWD/build

rm -rf $BUILD_PATH
mkdir $BUILD_PATH
mkdir $BUILD_PATH/bin

cmake -B$BUILD_PATH -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build $BUILD_PATH -- -j$(sysctl -n hw.ncpu)

cd $CWD
