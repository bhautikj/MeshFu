MeshFu
======

MeshFu a library that uses assimp to generate compressed meshes. These lightweight meshes can then be loaded by libMeshfu at runtime without having to link against assimp and the memory and computational overhead that entails. On the display end, only VBOs are used, with an abstract wrapper that supports both native cinder VBOs and bloomtimes excellent cinder-glkit VBOs.

This kit provides:

blocks/MeshFu : this is the core lib for representing meshes and textures and converting them to shader-friendly VBOs
src/MeshFuUtil : this primarily provides MeshPacker, a utility that uses Assimp to load a mesh and spit out a compressed version that can be quickly and simply read by MeshFu
src/*App* : these are wrappers that abstract away kit specific app implementations (i.e. vanilla cinder vs. glkit) and then 
src/cmake
meshFuDesktopAppExample
meshFuGlkitAppExample

Build prerequisites:

get boost

    http://sourceforge.net/projects/boost/files/boost/1.48.0/boost_1_48_0.tar.gz/download
    tar zxf boost_1_48_0.tar.gz

get assimp, build

    git clone https://github.com/bhautikj/assimp assimp
    cd assimp
    mkdir build
    mkdir lib
    cd build
    cmake -DENABLE_BOOST_WORKAROUND=ON -DASSIMP_BUILD_STATIC_LIB=ON -D"CMAKE_OSX_ARCHITECTURES=i386;x86_64" -DCMAKE_CXX_FLAGS="-fvisibility-inlines-hidden" ..
    make

get cinder, build

    git clone https://github.com/bhautikj/Cinder/ cinder
    cd cinder
    ln -s ../boost_1_48_0 boost
    cd  xcode
    ./fullbuild.sh
    cd ..

get cinder_es2

    git clone https://github.com/bhautikj/Cinder -b es2 cinder_es2 
    cd cinder_es2
    ln -s ../boost_1_48_0 boost
    cd  xcode
    ./fullbuild.sh
    cd ..

get cinder-glkit

    git clone https://github.com/bloomtime/cinder-glkit cinder-glkit
