/*
 Copyright (C) 2013 Bhautik Joshi
 
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

#pragma once

#include "cinder/Cinder.h"
#include "cinder/Color.h"

#include "cinder/app/App.h"

#include "cinder/gl/Material.h"
#include "cinder/gl/Texture.h"

#include <vector>
#include <iostream>

#include "MeshFuGeometry.h"

namespace MeshFu
{
  class MeshContainer;
  typedef std::shared_ptr< MeshContainer > MeshContainerRef;
 
  class MeshContainer
  {
  public:
    MeshContainer() {}
    ~MeshContainer()
    {
    }
    
    void                  build(std::string modelDir);
    virtual void          write(ogzstream& stream);
    virtual void          read(igzstream& stream);
    
    
    struct textureStruct
    {
      std::string relativeTexPath;
    };
    
    struct materialStruct
    {
      GLenum face;
      ci::ColorAf dcolor;
      ci::ColorAf scolor;
      ci::ColorAf acolor;
      ci::ColorAf ecolor;
    };
    
    struct materialFormatStruct
    {
      GLenum wrapS;
      GLenum wrapT;
    };
    
    MeshTri               mMeshTri;
    std::string           mName;
    textureStruct         mTextureData;
    materialStruct        mMaterialData;
    materialFormatStruct  mMaterialFormatData;
    bool                  mTwoSided;
    
    
    std::vector< float >  mMorphWeights;
    bool                  mValidCache;
    std::vector<BoneRef>  mBones;
    
    std::vector<ci::Vec4f>        mBoneIndices;
    std::vector<ci::Vec4f>        mBoneWeights;
    
    //Non-cached elements
    DrawableMeshRef       mDrawable;
  };
  
} //namespace MeshFu
