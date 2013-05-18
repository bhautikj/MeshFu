#ifndef _MESHASSIMP_H_
#define _MESHASSIMP_H_

#include "MeshFuRiggedGeometry.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"

namespace MeshFu
{


class RiggedGeometryAssimp : public RiggedGeometry
{
  public:
    RiggedGeometryAssimp(const std::string &file);
    ~RiggedGeometryAssimp() {}

  protected:
    void                                  loadAllMeshes();
    GeometryRef                      convertAiMesh( const aiMesh *mesh, std::string nameToUse );
    NodeRef                               loadNodes( const aiNode *nd, NodeRef parentRef = NodeRef()  );

    std::shared_ptr< Assimp::Importer >   mImporterRef;
    const aiScene*                        mScene;
};


}//namespace MeshFu

#endif