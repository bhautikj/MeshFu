#ifndef _MESHASSIMP_H_
#define _MESHASSIMP_H_

#include "MeshFuModelWrangler.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"

namespace MeshFu
{


class ModelWranglerAssimp : public ModelWrangler
{
  public:
    ModelWranglerAssimp(const std::string &file);
    ~ModelWranglerAssimp() {}

  protected:
    void                                  loadAllMeshes();
    MeshContainerRef                      convertAiMesh( const aiMesh *mesh, std::string nameToUse );
    NodeRef                               loadNodes( const aiNode *nd, NodeRef parentRef = NodeRef()  );

    std::shared_ptr< Assimp::Importer >   mImporterRef;
    const aiScene*                        mScene;
};


}//namespace MeshFu

#endif