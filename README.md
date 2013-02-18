MeshFu
======

MeshFu a library that uses assimp to generate compressed meshes. These lightweight meshes can then be loaded by libMeshfu at runtime without having to link against assimp and the memory and computational overhead that entails.


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
