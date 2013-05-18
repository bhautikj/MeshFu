#include "ModelWranglerAssimp.h"
#include "MeshFuUtil.h"

#include "assimp/postprocess.h"
#include "assimp/IOSystem.hpp"
#include <iostream>
#include <sstream>
#include <vector>

namespace 
{
  class WeightPacker
  {
    public:
      WeightPacker()
      {
      }
      ~WeightPacker() {}
    
      void addWeight(float boneIndex, float boneWeight)
      {
        if (weights.size()==0)
        {
          indices.push_back(boneIndex);
          weights.push_back(boneWeight);
          return;
        }
        else
        {
          for(int i=0; i<weights.size(); i++)
          {
            if (boneWeight>weights[i])
            {
                //insert at position i, shift everything else down
                weights.insert(weights.begin()+i+1, boneWeight);
                indices.insert(indices.begin()+i+1, boneIndex);
                if (weights.size()>4)
                {
                  weights.resize(4);
                  indices.resize(4);
                }
                return;
            }
          }
          indices.push_back(boneIndex);
          weights.push_back(boneWeight);
        }
      }
    
      void getIndexAndWeights(ci::Vec4f &index, ci::Vec4f &weight)
      {
        weights.resize(4, 0.0);
        indices.resize(4, 0.0);
        for(int i=0 ;i<4;i++)
        {
          weight[i]  = weights[i];
          index[i]   = indices[i];
        }
      }
    
      std::vector<float> indices;
      std::vector<float> weights;
  };
}

namespace MeshFu
{

inline std::string fromAssimp( const aiString &s )
{
  return std::string( s.data );
}

inline ci::ColorAf fromAssimp( const aiColor4D &c )
{
    return ci::ColorAf( c.r, c.g, c.b, c.a );
}

inline ci::Vec3f fromAssimp( const aiVector3D &v )
{
    return ci::Vec3f( v.x, v.y, v.z );
}

inline ci::Quatf fromAssimp( const aiQuaternion &q )
{
    return ci::Quatf( q.w, q.x, q.y, q.z );
}

inline ci::Matrix44f fromAssimp( const aiMatrix4x4 &m )
{
  return ci::Matrix44f( &m.a1, true );
}

inline Bone::VertexWeight fromAssimp( const aiVertexWeight& vw)
{
  Bone::VertexWeight ret;
  ret.idx   = vw.mVertexId;
  ret.value = vw.mWeight;
  return ret;
}

inline std::string intToString(int i)
{
    std::stringstream ss;
    std::string s;
    ss << i;
    s = ss.str();

    return s;
}

void fromAssimp( const aiMesh *aim, MeshContainerRef cim )
{
  // copy vertices
  for ( unsigned i = 0; i < aim->mNumVertices; ++i )
  {
    cim->mVertices.push_back( fromAssimp( aim->mVertices[i] ) );
  }

  if( aim->HasNormals() )
  {
    for ( unsigned i = 0; i < aim->mNumVertices; ++i )
    {
      cim->mNormals.push_back( fromAssimp( aim->mNormals[i] ) );
    }
  }

  // aiVector3D * mTextureCoords [AI_MAX_NUMBER_OF_TEXTURECOORDS]
  // just one for now
  if ( aim->GetNumUVChannels() > 0 )
  {
    for ( unsigned i = 0; i < aim->mNumVertices; ++i )
    {
      cim->mTextureCoords.push_back( ci::Vec2f( aim->mTextureCoords[0][i].x,
                    aim->mTextureCoords[0][i].y ) );
    }
  }

  //aiColor4D *mColors [AI_MAX_NUMBER_OF_COLOR_SETS]
  if ( aim->GetNumColorChannels() > 0 )
  {
    for ( unsigned i = 0; i < aim->mNumVertices; ++i )
    {
      cim->mColors.push_back( fromAssimp( aim->mColors[0][i] ) );
    }
  }

  for ( unsigned i = 0; i < aim->mNumFaces; ++i )
  {
//     if ( aim->mFaces[i].mNumIndices > 3 )
//     {
//       throw MeshFuExc( "non-triangular face found: model " +
//           std::string( aim->mName.data ) + ", face #" +
//           toString< unsigned >( i ) );
//     }

    cim->mFaces.push_back(MeshTri::Face(aim->mFaces[ i ].mIndices[ 0 ],
               aim->mFaces[ i ].mIndices[ 1 ],
               aim->mFaces[ i ].mIndices[ 2 ] ));
  }
}

ModelWranglerAssimp::ModelWranglerAssimp(const std::string &file) :
  ModelWrangler( file )
{
  
    unsigned flags = aiProcess_Triangulate |
          aiProcess_FlipUVs |
          aiProcess_FindInstances |
          aiProcess_ValidateDataStructure |
          aiProcess_OptimizeMeshes |
          aiProcess_CalcTangentSpace        |
          aiProcess_GenSmoothNormals        |  
          aiProcess_JoinIdenticalVertices     |
          aiProcess_ImproveCacheLocality      |  
          aiProcess_LimitBoneWeights        |  
          aiProcess_RemoveRedundantMaterials      |  
          aiProcess_SplitLargeMeshes        |  
          aiProcess_Triangulate         |  
          aiProcess_GenUVCoords                   |  
          aiProcess_SortByPType                   |  
//           aiProcess_FindDegenerates               |  
          aiProcess_FindInvalidData               |  0;

  mImporterRef = std::shared_ptr< Assimp::Importer >(new Assimp::Importer());
  mImporterRef->SetPropertyInteger( AI_CONFIG_PP_SBP_REMOVE,
      aiPrimitiveType_LINE | aiPrimitiveType_POINT );
  mImporterRef->SetPropertyInteger( AI_CONFIG_PP_PTV_NORMALIZE, true );

  mScene = mImporterRef->ReadFile( file, flags );
  if ( !mScene )
  {
    std::cout<<mImporterRef->GetErrorString() <<std::endl;
    throw Util::MeshFuExc( mImporterRef->GetErrorString() );
  }
  
  loadAllMeshes();
  mRootNode = loadNodes( mScene->mRootNode );
  mRootNodeName = mRootNode->getName();
}

void ModelWranglerAssimp::loadAllMeshes()
{

  for ( unsigned i = 0; i < mScene->mNumMeshes; ++i )
  {
    std::string name = fromAssimp( mScene->mMeshes[ i ]->mName );
    if (std::find(mMeshNames.begin(), mMeshNames.end(), name )!= mMeshNames.end())
    {
      name = name + "_" + intToString(i);
    }
    MeshContainerRef meshFuRef = convertAiMesh( mScene->mMeshes[ i ], name );
    mModelMeshes.push_back( meshFuRef );
    mMeshMap[ name ] = meshFuRef;
    mMeshNames.push_back( name );
  }
}


MeshContainerRef ModelWranglerAssimp::convertAiMesh( const aiMesh *mesh, std::string nameToUse )
{
  
  // the current AssimpMesh we will be populating data into.
  MeshContainerRef meshFuRef = MeshContainerRef( new MeshContainer() );

  meshFuRef->mName = nameToUse;//fromAssimp( mesh->mName );

  // Handle material info
  aiMaterial *mtl = mScene->mMaterials[ mesh->mMaterialIndex ];

  aiString name;
  mtl->Get( AI_MATKEY_NAME, name );

  // Culling
  int twoSided;
  if ( ( AI_SUCCESS == mtl->Get( AI_MATKEY_TWOSIDED, twoSided ) ) && twoSided )
  {
    meshFuRef->mTwoSided = true;
    meshFuRef->mMaterialData.face = GL_FRONT_AND_BACK;
  }
  else
  {
    meshFuRef->mTwoSided = false;
    meshFuRef->mMaterialData.face = GL_FRONT;
  }


  aiColor4D dcolor(1,1,1,1), scolor(1,1,1,1), acolor(1,1,1,1), ecolor(1,1,1,1);
  if ( AI_SUCCESS == mtl->Get( AI_MATKEY_COLOR_DIFFUSE, dcolor ) )
  {
    meshFuRef->mMaterialData.dcolor = fromAssimp( dcolor );
//     meshFuRef->mMaterialData.hasDcolor = true;
  }
//   else
//     meshFuRef->mMaterialData.hasDcolor = false;

  if ( AI_SUCCESS == mtl->Get( AI_MATKEY_COLOR_SPECULAR, scolor ) )
  {
    meshFuRef->mMaterialData.scolor = fromAssimp( scolor );
//     meshFuRef->mMaterialData.hasScolor = false;
  }
//   else
//     meshFuRef->mMaterialData.hasScolor = false;

  if ( AI_SUCCESS == mtl->Get( AI_MATKEY_COLOR_AMBIENT, acolor ) )
  {
    meshFuRef->mMaterialData.acolor = fromAssimp( acolor );
//     meshFuRef->mMaterialData.hasAcolor = false;
  }
//   else
//     meshFuRef->mMaterialData.hasAcolor = false;

  if ( AI_SUCCESS == mtl->Get( AI_MATKEY_COLOR_EMISSIVE, ecolor ) )
  {
    meshFuRef->mMaterialData.ecolor = fromAssimp( ecolor );
//     meshFuRef->mMaterialData.hasEcolor = false;
  }
//   else
//     meshFuRef->mMaterialData.hasEcolor = false;

  // Load Textures
  int texIndex = 0;
  aiString texPath;

  // TODO: handle other aiTextureTypes
  if ( AI_SUCCESS == mtl->GetTexture( aiTextureType_DIFFUSE, texIndex, &texPath ) )
  {
    ci::fs::path texFsPath( texPath.data );
    ci::fs::path modelFolder = mFilePath.parent_path();
    ci::fs::path relTexPath = texFsPath.parent_path();
    ci::fs::path texFile = texFsPath.filename();
    ci::fs::path realPath = modelFolder / relTexPath / texFile;

    ci::fs::path relTexLoc = relTexPath / texFile;
    //std::cout<<relTexLoc<<std::endl;
    meshFuRef->mTextureData.relativeTexPath = relTexLoc.string();

    // texture wrap
    int uwrap;
    if ( AI_SUCCESS == mtl->Get( AI_MATKEY_MAPPINGMODE_U_DIFFUSE( 0 ), uwrap ) )
    {
      switch ( uwrap )
      {
        case aiTextureMapMode_Wrap:
          meshFuRef->mMaterialFormatData.wrapS = GL_REPEAT;
          break;

        case aiTextureMapMode_Clamp:
          //format.setWrapS( GL_CLAMP );
          meshFuRef->mMaterialFormatData.wrapS = GL_CLAMP_TO_EDGE;
          //iOS compatibility
          break;

        case aiTextureMapMode_Decal:
          // If the texture coordinates for a pixel are outside [0...1]
          // the texture is not applied to that pixel.
          meshFuRef->mMaterialFormatData.wrapS = GL_CLAMP_TO_EDGE;
          break;

        case aiTextureMapMode_Mirror:
          // A texture coordinate u|v becomes u%1|v%1 if (u-(u%1))%2
          // is zero and 1-(u%1)|1-(v%1) otherwise.
          // TODO
          meshFuRef->mMaterialFormatData.wrapS = GL_REPEAT;
          break;
      }
    }

    int vwrap;
    if ( AI_SUCCESS == mtl->Get( AI_MATKEY_MAPPINGMODE_V_DIFFUSE( 0 ), vwrap ) )
    {
      switch ( vwrap )
      {
        case aiTextureMapMode_Wrap:
          meshFuRef->mMaterialFormatData.wrapT = GL_REPEAT;
          break;

        case aiTextureMapMode_Clamp:
          //format.setWrapT( GL_CLAMP );
          //iOS compatibility
          meshFuRef->mMaterialFormatData.wrapT = GL_CLAMP_TO_EDGE;
          break;

        case aiTextureMapMode_Decal:
          // If the texture coordinates for a pixel are outside [0...1]
          // the texture is not applied to that pixel.
          meshFuRef->mMaterialFormatData.wrapT = GL_CLAMP_TO_EDGE;
          break;

        case aiTextureMapMode_Mirror:
          // A texture coordinate u|v becomes u%1|v%1 if (u-(u%1))%2
          // is zero and 1-(u%1)|1-(v%1) otherwise.
          // TODO
          meshFuRef->mMaterialFormatData.wrapT = GL_REPEAT;
          break;
      }
    }    
  }

  fromAssimp( mesh, meshFuRef );
  meshFuRef->mValidCache = true;
  
//   assimpMeshRef->mAnimatedPos.resize( mesh->mNumVertices );
//   if ( mesh->HasNormals() )
//   {
//     assimpMeshRef->mAnimatedNorm.resize( mesh->mNumVertices );
//   }
// 
// 
//   meshFuRef->mIndices.resize( meshFuRef->mCachedMeshTri->mFaces.size()* 3 );
//   unsigned j = 0;
//   for ( unsigned x = 0; x < mesh->mNumFaces; ++x )
//   {
//     for ( unsigned a = 0; a < mesh->mFaces[x].mNumIndices; ++a)
//     {
//       meshFuRef->mIndices[ j++ ] = mesh->mFaces[ x ].mIndices[ a ];
//     }
//   }

  int nMorphs = mesh->mNumAnimMeshes;
  for (int nm = 0; nm < nMorphs; nm++)
  {
    aiAnimMesh* aiMesh = mesh->mAnimMeshes[nm];
    PointCloudRef morph = PointCloudRef(new PointCloud());
    morph->mName = aiMesh->mName.data;

    // copy vertices
    for ( unsigned i = 0; i < aiMesh->mNumVertices; ++i )
    {
      morph->mVertices.push_back( fromAssimp( aiMesh->mVertices[i] ) );
    }

    //TODO: something goes horribly wrong writing normals. investigate.
//     if( aiMesh->HasNormals() )
//     {
//       for ( unsigned i = 0; i < aiMesh->mNumVertices; ++i )
//       {
//         morph->mNormals.push_back( fromAssimp( aiMesh->mNormals[i] ) );
//       }
//     }
    
    meshFuRef->mMorphs.push_back(morph);
    meshFuRef->mMorphWeights.push_back(0.0);
  }

  int nVerts = meshFuRef->mVertices.size();
  std::vector<WeightPacker> packList;
  packList.resize(nVerts);
  meshFuRef->mBoneIndices.resize(nVerts);
  meshFuRef->mBoneWeights.resize(nVerts);
    
  int nBones = mesh->mNumBones;
  for (int nb=0; nb<nBones; nb++)
  {

    
    aiBone* bone = mesh->mBones[nb];
    BoneRef boneRef = BoneRef (new Bone());
    boneRef->mName = fromAssimp(bone->mName);
    
    int numWeights = bone->mNumWeights;
    for (int i=0; i<numWeights;i++)
    {
      int vertexIdx = bone->mWeights[i].mVertexId;
      float weight = bone->mWeights[i].mWeight;
      packList[vertexIdx].addWeight(nb, weight);
      //boneRef->mWeights.push_back(fromAssimp(bone->mWeights[i]));
    }
    
    boneRef->mOffsetMatrix = fromAssimp(bone->mOffsetMatrix);
    meshFuRef->mBones.push_back(boneRef);
  }

  for (int i=0; i<nVerts;i++)
  {
    packList[i].getIndexAndWeights(meshFuRef->mBoneIndices[i], meshFuRef->mBoneWeights[i]);
  }
  
  // Assume mesh is visible
//   meshFuRef->mVisible = true;

  return meshFuRef;
  
}

NodeRef ModelWranglerAssimp::loadNodes( const aiNode *nd, NodeRef parentRef )
{
  NodeRef nodeRef = NodeRef( new Node() );
  nodeRef->setParent( parentRef );
  std::string nodeName = fromAssimp( nd->mName );
  nodeRef->setName( nodeName );
  mNodeMap[ nodeName] = nodeRef;
  mNodeNames.push_back( nodeName );

  // store transform
  aiVector3D scaling;
  aiQuaternion rotation;
  aiVector3D position;
  nd->mTransformation.Decompose( scaling, rotation, position );
  nodeRef->setScale( fromAssimp( scaling ) );
  nodeRef->setOrientation( fromAssimp( rotation ) );
  nodeRef->setPosition( fromAssimp( position ) );

  // meshes
  for ( unsigned i = 0; i < nd->mNumMeshes; ++i )
  {
    unsigned meshId = nd->mMeshes[ i ];
//     if ( meshId >= mModelMeshes.size() )
//       throw MeshFuExc( "node " + nodeRef->getName() + " references mesh #" +
//           toString< unsigned >( meshId ) + " from " +
//           toString< size_t >( mModelMeshes.size() ) + " meshes." );
    nodeRef->addMesh( mModelMeshes[ meshId ] );
    nodeRef->addMeshName( mModelMeshes[ meshId ]->mName );
  }
  

  // store the node with meshes for rendering
  if ( nd->mNumMeshes > 0 )
  {
    mMeshNodes.push_back( nodeRef );
  }

  // process all children
  for ( unsigned n = 0; n < nd->mNumChildren; ++n )
  {
    NodeRef childRef = loadNodes( nd->mChildren[ n ], nodeRef );
    nodeRef->addChild( childRef );
    nodeRef->addChildName ( childRef->getName() );
  }
  return nodeRef;
}


}//namespace MeshFu