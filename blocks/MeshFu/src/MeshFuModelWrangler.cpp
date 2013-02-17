/*
 Copyright (C) 2013 Bhautik Joshi
 
 Based in part on https://github.com/gaborpapp/Cinder-Assimp/blob/master/src/AssimpLoader.cpp
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 
 */

#include "MeshFuModelWrangler.h"

#include <fstream>
#include "MeshFuUtil.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Texture.h"

namespace MeshFu
{
  
  void ModelWrangler::buildAll()
  {
    std::vector< MeshContainerRef >::iterator it =  mModelMeshes.begin();
    std::string modelDir = ci::fs::path(mFilePath).parent_path().string();
    
    while (it != mModelMeshes.end())
    {
      (*it)->build(modelDir);
      it++;
    }
    
    mRootNode = mNodeMap[mRootNodeName];
    // per node these need to bet set up once the node has been read
    mMeshToNodeMap.clear();
    std::vector< NodeRef >::iterator nit = mMeshNodes.begin();
    while (nit != mMeshNodes.end())
    {
      NodeRef node = (*nit);
      node->setParent(mNodeMap[node->mParentName]);
      node->mChildren.clear();
      for (int i=0; i<node->mChildNodeNames.size(); i++)
      {
        node->mChildren.push_back(mNodeMap[node->mChildNodeNames[i]]);
      }
      node->mMeshes.clear();
      for(int i=0; i<node->mMeshNames.size(); i++)
      {
        MeshContainerRef mesh = mMeshMap[node->mMeshNames[i]];
        node->mMeshes.push_back(mesh);
        if (mMeshToNodeMap.find(mesh) == mMeshToNodeMap.end())
        {
          std::vector<NodeRef> vec;
          vec.push_back(node);
          mMeshToNodeMap[mesh] = vec;
        }
        else
        {
          mMeshToNodeMap[mesh].push_back(node);
        }
      }
      nit++;
    }
  }
  
  void ModelWrangler::write(ogzstream& stream)
  {
    int nMeshes = mModelMeshes.size();
    stream<<nMeshes<<std::endl;
    for (int i=0; i<nMeshes; i++)
    {
      mModelMeshes[i]->write(stream);
    }
    std::cout<<"Wrote "<<nMeshes<<" meshes"<<std::endl;
    
    int nNodes = mNodeNames.size();
    stream<<nNodes<<std::endl;
    for (int i=0; i<nNodes; i++)
    {
      mNodeMap[mNodeNames[i]]->write(stream);
    }
    
    std::cout<<"Wrote "<<nNodes<<" nodes"<<std::endl;
    
    Util::writeString(stream, mRootNodeName);
    
    std::cout<<"writing complete"<<std::endl;
  }
  
  void ModelWrangler::read(igzstream& stream)
  {
    int nMeshes;
    stream>>nMeshes;
    mModelMeshes.clear();
    for (int i=0; i<nMeshes; i++)
    {
      MeshContainerRef meshFu = MeshContainerRef(new MeshContainer);
      meshFu->read(stream);
      
      mModelMeshes.push_back(meshFu);
    }
    
    int nNodes;
    stream>>nNodes;
    mMeshNodes.clear();
    for (int i=0; i<nNodes; i++)
    {
      NodeRef node = NodeRef(new Node);
      node->read(stream);
      mMeshNodes.push_back(node);
    }
    
    Util::readString(stream, mRootNodeName);
    
    //setup maps, nodes
    mMeshNames.clear();
    mMeshMap.clear();
    mNodeNames.clear();
    mNodeMap.clear();
    
    for(int i=0; i<nMeshes; i++)
    {
      std::string meshName = mModelMeshes[i]->mName;
      mMeshNames.push_back(meshName);
      mMeshMap[meshName] = mModelMeshes[i];
    }
    
    for(int i=0; i<nNodes; i++)
    {
      std::string nodeName = mMeshNodes[i]->getName();
      mNodeNames.push_back(nodeName);
      mNodeMap[nodeName] = mMeshNodes[i];
    }
  }
  
  void ModelWrangler::update()
  {
    updateSkinning();
    updateMeshes();
  }
  
  void ModelWrangler::draw()
  {
    //NOTE:
    //fixing alpha blending: https://forum.libcinder.org/topic/alpha-texture-setup
    //?
    std::vector< NodeRef >::const_iterator it = mMeshNodes.begin();
    for ( ; it != mMeshNodes.end(); ++it )
    {
      NodeRef nodeRef = *it;
      
      std::vector< MeshContainerRef >::const_iterator meshIt = nodeRef->mMeshes.begin();
      for ( ; meshIt != nodeRef->mMeshes.end(); ++meshIt )
      {
        MeshContainerRef meshContainerRef = *meshIt;
        meshContainerRef->mDrawable->draw();
      }
    }
  }
  
  void ModelWrangler::updateMeshes()
  {
    std::vector< NodeRef >::iterator it = mMeshNodes.begin();
    for ( ; it != mMeshNodes.end(); ++it )
    {
      NodeRef nodeRef = *it;
      
      std::vector< MeshContainerRef >::iterator meshIt = nodeRef->mMeshes.begin();
      for ( ; meshIt != nodeRef->mMeshes.end(); ++meshIt )
      {
        MeshContainerRef meshContainerRef = *meshIt;
        
        if ( meshContainerRef->mValidCache )
          continue;
        
        meshContainerRef->mValidCache = true;
      }
    }
  }
  
  void ModelWrangler::updateSkinning()
  {
    std::vector< NodeRef >::const_iterator it = mMeshNodes.begin();
    for ( ; it != mMeshNodes.end(); ++it )
    {
      NodeRef nodeRef = *it;
      
      std::vector< MeshContainerRef >::const_iterator meshIt = nodeRef->mMeshes.begin();
      for ( ; meshIt != nodeRef->mMeshes.end(); ++meshIt )
      {
        MeshContainerRef meshContainerRef = *meshIt;
        // current mesh we are introspecting
        const MeshTri mesh = meshContainerRef->mMeshTri;
        
        meshContainerRef->mDrawable->setNodeTransform(nodeRef->getDerivedTransform());
        
        // calculate bone matrices
        //std::vector< cinder::Matrix44f > boneMatrices( meshContainerRef->mBones.size() );
        
        for ( unsigned a = 0; a < meshContainerRef->mBones.size() ; ++a )
        {
          const BoneRef bone = meshContainerRef->mBones[ a ];
          
          // find the corresponding node by again looking recursively through
          // the node hierarchy for the same name
          NodeRef nodeRef = getNodeByName( bone->mName );
          assert( nodeRef );
          // start with the mesh-to-bone matrix
          // and append all node transformations down the parent chain until
          // we're back at mesh coordinates again
          cinder::Matrix44f transformMat = nodeRef->getDerivedTransform() * bone->mOffsetMatrix;
          
          meshContainerRef->mDrawable->mBoneMatrices[ a ] = transformMat;
        }
        
        
        meshContainerRef->mValidCache = false;
        
#ifndef CINDER_GLES2
        /*
         meshContainerRef->mDrawable->preUpdate();
         
         unsigned nVerts = meshContainerRef->mMeshTri.mVertices.size();
         for (unsigned a=0; a < nVerts; a++)
         {
         cinder::Vec3f srcPos = mesh.mVertices[a];
         for(int i=0;i<meshContainerRef->mMorphWeights.size(); i++)
         {
         float wt = meshContainerRef->mMorphWeights[i];
         if (wt>0.0)
         {
         srcPos = srcPos + (mesh.mMorphs[i]->mVertices[a] - srcPos) * wt;
         }
         }
         
         int x = int(meshContainerRef->mDrawable->mBoneIndices[a*4+0]);
         int y = int(meshContainerRef->mDrawable->mBoneIndices[a*4+1]);
         int z = int(meshContainerRef->mDrawable->mBoneIndices[a*4+2]);
         int w = int(meshContainerRef->mDrawable->mBoneIndices[a*4+3]);
         float wtx = meshContainerRef->mDrawable->mBoneWeights[a*4+0];
         float wty = meshContainerRef->mDrawable->mBoneWeights[a*4+1];
         float wtz = meshContainerRef->mDrawable->mBoneWeights[a*4+2];
         float wtw = meshContainerRef->mDrawable->mBoneWeights[a*4+3];
         
         cinder::Vec3f p = meshContainerRef->mDrawable->mBoneMatrices[x] * srcPos * wtx +\
         meshContainerRef->mDrawable->mBoneMatrices[y] * srcPos * wty +\
         meshContainerRef->mDrawable->mBoneMatrices[z] * srcPos * wtz +\
         meshContainerRef->mDrawable->mBoneMatrices[w] * srcPos * wtw;
         
         meshContainerRef->mDrawable->setVertex(p, a);
         }
         
         meshContainerRef->mDrawable->postUpdate();
         */
#endif
      }
    }
  }
  
}