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

#include "MeshFuDrawableMesh.h"

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "MeshFuUtil.h"

namespace MeshFu
{
  
  DrawableMesh::DrawableMesh() : Drawable()
  {
    mBoneMatrices = new cinder::Matrix44f[MeshFu::Util::MAXBONES];
    
    for (int i=0; i<MeshFu::Util::MAXBONES; i++)
    {
      mBoneMatrices[i] = cinder::Matrix44f::identity();
    }
    
    try {
      mMeshShader = cinder::gl::GlslProg(cinder::app::loadResource("skinMorph.vsh"),
                                         cinder::app::loadResource("skinMorph.fsh"));
//       std::cout<<"shader log: "<<mMeshShader.getShaderLog(mMeshShader.getHandle())<<std::endl;
    } catch (cinder::gl::GlslProgCompileExc exc) {
      std::cout<<exc.what()<<std::endl;
      assert(0);
    }
  }
  
  DrawableMesh::~DrawableMesh()
  {
  }
  
  void DrawableMesh::draw()
  {
    mTexture.enableAndBind();
    
    if ( mTwoSided )
      cinder::gl::enable( GL_CULL_FACE );
    else
      cinder::gl::disable( GL_CULL_FACE );
  
    glEnable(GL_DEPTH_TEST);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // binding should always go first!
    mMeshShader.bind();
#ifndef CINDER_GLES2
    mMeshShader.uniform("texture", 0);
    mMeshShader.uniform("u_mvp_matrix", Util::AppState::getAppState()->mCameraMvpMat);
    mMeshShader.uniform("u_model_matrix", cinder::Matrix44f::identity());
    mMeshShader.uniform("boneMatrices", mBoneMatrices, MeshFu::Util::MAXBONES );
    //cinder::gl::draw(mVbo);
    cinder::gl::drawRange(mVbo, 0, mVbo.getNumIndices()*3);
    mMeshShader.unbind();
#else
    mVbo->assignLocations(mMeshShader);
    mMeshShader.uniform("texture", 0);
    mMeshShader.uniform("u_mvp_matrix", Util::AppState::getAppState()->mCameraMvpMat);
    mMeshShader.uniform("u_model_matrix", cinder::Matrix44f::identity());
    mMeshShader.uniform("boneMatrices", mBoneMatrices, MeshFu::Util::MAXBONES );
    mVbo->draw();
#endif


    mTexture.unbind();
  }
  
}//namespace MeshFu