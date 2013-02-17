include(LibFindMacros)

find_path(meshfu_BASE_DIR "include/MeshFu.h " DOC "Root directory of meshfu")


if(EXISTS "${meshfu_BASE_DIR}")

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE( ZLIB REQUIRED )

find_path(meshfu_INCLUDE_DIR
  NAMES MeshFuGeometry.h 
  PATHS ${meshfu_BASE_DIR}/include 
)


#SET(meshfu_INCLUDE_DIR ${meshfu_BASE_DIR}/include)
SET(meshfu_SRC_DIR ${meshfu_BASE_DIR}/src)

REQUIRE_HEADER(meshfu_INCLUDE_DIR MeshFu.h)


SET(MESHFU_INCLUDES
${meshfu_INCLUDE_DIR}/MeshFuGeometry.h
${meshfu_INCLUDE_DIR}/MeshFuContainer.h
${meshfu_INCLUDE_DIR}/MeshFuNode.h
${meshfu_INCLUDE_DIR}/MeshFuModelWrangler.h
${meshfu_INCLUDE_DIR}/MeshFuUtil.h
${meshfu_INCLUDE_DIR}/gzstream.h
${meshfu_INCLUDE_DIR}/MeshFuDrawableMesh.h
)

SET(MESHFU_SOURCES
${meshfu_SRC_DIR}/MeshFuGeometry.cpp
${meshfu_SRC_DIR}/MeshFuContainer.cpp
${meshfu_SRC_DIR}/MeshFuNode.cpp
${meshfu_SRC_DIR}/MeshFuModelWrangler.cpp
${meshfu_SRC_DIR}/MeshFuUtil.cpp
${meshfu_SRC_DIR}/gzstream.cpp
${meshfu_SRC_DIR}/MeshFuDrawableMesh.cpp
)

ADD_LIBRARY("meshfu" STATIC ${MESHFU_INCLUDES} ${MESHFU_SOURCES})

SET_TARGET_PROPERTIES("meshfu"
  PROPERTIES
  COMPILE_FLAGS "-fvisibility-inlines-hidden"
  OSX_ARCHITECTURES i386
  )

  TARGET_LINK_LIBRARIES("meshfu" ${cinder_LIBRARIES} )

else(EXISTS "${meshfu_BASE_DIR}")
        set(ERR_MSG "Please specify meshfu directory using meshfu_BASE_DIR env. variable")
endif(EXISTS "${meshfu_BASE_DIR}")
