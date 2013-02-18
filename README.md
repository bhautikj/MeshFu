MeshFu
======

MeshFu a library that uses assimp to generate compressed, optimised meshes. These lightweight (small size, preprocessed) meshes can then be loaded by libMeshfu at runtime without having to link against assimp and the memory and computational overhead that entails. On the display end, only VBOs are used, with an abstract wrapper that supports both native cinder VBOs and bloomtimes excellent cinder-glkit VBOs.

It is important to note that a fork of assimp is needed for the mesh packing utility (I make use of morph targets, which are not in the master build for assimp) and that the example code require forked versions of cinder/cinder_es2 as well (small changes related to how the shader is interfaced with). See *Building base libraries* below for more info.

Mesh packing? Why?
------------------

Assimp is superb, but in my test cases I found it unsuitable for use on iOS. It generates a large binary which is statically linked against, and it provides many features that are not needed if you just need to load a mesh and display it. Also, assimp provides many excellent mesh optimisation routines - however, there is no need to evaluate these at runtime, and can be expensive for larger meshes. Also, raw mesh data - especially in collada format - is large.

The MeshPacker uses assimp to load and optimise a mesh, then writes it out as an easily parsable, gzip compressed text stream that can be read by libMeshfu, which is lightweight and has no dependency on Assimp.

GPU-based skinning
------------------

The example projects provided demo GPU-based skinning. Here, geometry, bone indices and weights are loaded into a VBO, and at runtime the GPU evaluates vertex positions by examining the node matrices only - all the vertex computation happens in the shader. The DesktopApp example uses native cinder VBOs for the job, and the GlkitApp uses the bloomtime glkit VBOs.


Building
--------

*MeshPacker*

There is an xcode project in MeshPacker for building the mesh packing utility. Alternativey, you can use the cmake script for building it along with an example app:

    export Boost_DIR = <your cinder root>/boost
    mkdir build
    cd build
    ccmake ../src
    <fill in your cinder_ROOT, assimp_ROOT values here>
    make

*Demo apps*

The desktop version of the app is here: meshFuDesktopAppExample - open up the corresponding xcode file, fill in your cinder root and build

The glkit version of the app is here: meshFuGlkitAppExample - open up the corresponding xcode file, fill in your cinder root, update the location of the glkit block to wherever you have it and build

How to use
----------

*Converting Meshes*

MeshPacker will convert a mesh like this:

    MeshPacker inputMesh.dae

and will spit out

    inputMesh.dae.cmf

Under the hood, all we are doing is:

    #include "ModelWranglerAssimp.h"

    std::string filename("inputMesh.dae");
    std::string target = filename + ".cmf";

    MeshFu::ModelWranglerAssimp loader(fileName);
      
    ogzstream out;
    out.open(target.c_str());
    loader.write(out);
    out.close();

*Loading and displaying meshes*

In your init/setup routine:

    #include "MeshFuModelWrangler.h"

    std::string modelPath("inputMesh.dae.cmf")
    MeshFu::ModelWrangler mLoader = MeshFu::ModelWrangler( modelPath );
    igzstream testIn;
    testIn.open(modelPath.c_str());
    mLoader.read(testIn);
    testIn.close();
    mLoader.buildAll();

And in your draw routine:

    mLoader.draw();

It is important to note that at the moment, we are assuming that the loaded meshes are completely skinned. Any vertices that are not bound to a bone will be rendered in the rest position. Fixing this is on the TODO list :)

Whats in the kit
----------------

Core:

    blocks/MeshFu : this is the core lib for representing meshes and textures and converting them to shader-friendly VBOs
    src/MeshFuUtil : this primarily provides MeshPacker, a utility that uses Assimp to load a mesh and spit out a compressed version that can be  quickly and simply read by MeshFu
    src/cmake : cmake scripts for cinder
    MeshPacker : xcode project to build MeshPacker
    
Example projects - mesh loading & GPU-based skinning:

    src/*App* : these are interfaces that abstract away kit specific app implementations (i.e. vanilla cinder vs. glkit) and provide a kit-agnostic wrapper (AppCore.*) that is common to both.
    meshFuDesktopAppExample - GPU-based skinning for the desktop
    meshFuGlkitAppExample - GPU-based skinning in OpenGL ES2 using bloomtime-glkit

Asset & shader data:

    src/assets : sample collada mesh and compressed mesh version of it
    src/skinMorph.* : shaders used for the GPU-based skinning

    
Building base libraries
-----------------------

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
    
TODO
----

* Permit loading of unskinned meshes
* Put model loading into a worker thread, and put in support for callbacks to let UI know what % the models are loaded
* 2D UI library that works on ES2
* Font rendering library that works on ES2

