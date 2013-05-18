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

#ifdef CINDER_GLES2
#include "Vbo.h"
#else
#include "cinder/gl/Vbo.h"
#endif

#include "cinder/gl/GlslProg.h"

#include "cinder/gl/Material.h"
#include "cinder/gl/Texture.h"

namespace MeshFu
{
  class Drawable
  {
    public:
      Drawable() {}
      ~Drawable() {}
      virtual void        draw() = 0;
  };
  
  class DrawableMesh : public Drawable
  {
  public:
    DrawableMesh();
    ~DrawableMesh();
    virtual void          draw();
    virtual void          setNodeTransform(const cinder::Matrix44f& transform) { mNodeTransform = transform; }
    
    cinder::Matrix44f                 *mBoneMatrices;
    
  protected:
#ifdef CINDER_GLES2
    cinder::gl::VboRef                mVbo;
#else
    cinder::gl::VboMesh               mVbo;
#endif
    cinder::gl::GlslProg              mMeshShader;
    ci::gl::Texture                   mTexture;
    bool                              mTwoSided;
    int                               mNVerts;
    int                               mNNormals;
    cinder::Matrix44f                 mNodeTransform;
    
    friend class Geometry;
  };
  
  typedef std::shared_ptr< DrawableMesh > DrawableMeshRef;
  
} //namespace MeshFu