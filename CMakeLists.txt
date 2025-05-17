cmake_minimum_required(VERSION 3.6)
project(HDKFaceExplosionProject)

add_definitions(
    -DSIZEOF_VOID_P=8
    -D__APPLE__
    -D__MACH__
    -D__arm64__
)

set(HOUDINI_SDK_PATH
/Applications/Houdini/Houdini20.5.550/Frameworks/Houdini.framework/Versions/Current/Resources)
list( APPEND CMAKE_PREFIX_PATH "${HOUDINI_SDK_PATH}/toolkit/cmake" )
find_package(Houdini REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_PATH ./source)

set(SOURCES 
    ${SOURCE_PATH}/faceexplosion.h
    ${SOURCE_PATH}/faceexplosion.cpp
)

add_library(HDKFaceExplosion SHARED ${SOURCES})
target_link_libraries(HDKFaceExplosion Houdini)
houdini_configure_target(HDKFaceExplosion)
