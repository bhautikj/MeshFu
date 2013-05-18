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

#include "gzstream.h"
#include "MeshFuDrawableMesh.h"

namespace MeshFu
{
  
  class PointCloud;
  typedef std::shared_ptr< PointCloud > PointCloudRef;
  
  class PointCloud
  {
  public:
    PointCloud() :
    mName("")
    {
      
    }
    
    ~PointCloud()
    {
    }
    
    virtual void          write(ogzstream& stream);
    virtual void          read(igzstream& stream);
    
    std::string                   mName;
    std::vector<cinder::Vec3f>    mVertices;
    std::vector<cinder::Vec3f>    mNormals;
  };
  
  class MeshTri;
  typedef std::shared_ptr< MeshTri > MeshTriRef;
  
  class MeshTri : public PointCloud
  {
  public:
    MeshTri() :
    PointCloud(),
    mHasTexture(false)
    {
      
    }
    
    ~MeshTri()
    {
    }
    
    virtual void          write(ogzstream& stream);
    virtual void          read(igzstream& stream);
    
    struct Face
    {
    public:
      Face () {}
      Face (int a, int b, int c)
      {
        arr[0] = a; arr[1] = b; arr[2] = c;
      }
      
      const int &operator[](int i) const
      {
        return arr[i];
      }
      
    private:
      int arr[3];
    };
    
    bool                          mHasTexture;
    std::vector<cinder::Vec2f>    mTextureCoords;
    std::vector<cinder::ColorAf>  mColors;
    
    
    std::vector<Face>             mFaces;
    std::vector<PointCloudRef>    mMorphs;
  };
  
  class Bone;
  typedef std::shared_ptr< Bone > BoneRef;
  
  class Bone
  {
  public:
    Bone() {}
    ~Bone() {}
    
    struct VertexWeight
    {
      int   idx;
      float value;
    };
    
    std::string                 mName;
    
    //! Matrix that transforms from mesh space to bone space in bind pose
    cinder::Matrix44f           mOffsetMatrix;
    
    virtual void                write(ogzstream& stream);
    virtual void                read(igzstream& stream);
  };

  class Geometry;
  typedef std::shared_ptr< Geometry > GeometryRef;
 
  class Geometry : public MeshTri
  {
  public:
    Geometry() : MeshTri()
    {
      
    }
    
    ~Geometry()
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
