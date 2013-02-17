include(LibFindMacros)

find_path(assimp_DIR "include/assimp/mesh.h " DOC "Root directory of libassimp")

if(EXISTS "${assimp_DIR}")
  find_path(assimp_INCLUDE_DIR
    NAMES assimp/mesh.h 
    PATHS ${assimp_DIR}/include 
  )
  
  find_library(assimp_LIBRARY
    NAMES assimp
    PATHS ${assimp_DIR}/build/code
  )
  
  REQUIRE_HEADER(assimp_INCLUDE_DIR assimp/mesh.h )
  
  libfind_process(assimp)
else(EXISTS "${assimp_DIR}")
        set(ERR_MSG "Please specify assimp directory using assimp_DIR env. variable")
endif(EXISTS "${assimp_DIR}")