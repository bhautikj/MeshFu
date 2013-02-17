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

#include "MeshFuContainer.h"
#include "MeshFuUtil.h"

#include "cinder/ImageIo.h"
#include "cinder/gl/gl.h"

namespace
{
  const bool debugOut = false;
}

namespace MeshFu
{
  
  void MeshContainer::build(std::string modelDir)
  {
    DrawableMesh* vboMesh = new DrawableMesh();
    int nVerts = mMeshTri.mVertices.size();
    GLfloat* positions = new GLfloat[nVerts*3];
    // copy vertices
    for ( unsigned i = 0; i < nVerts ; ++i )
    {
      positions[i*3+0] = mMeshTri.mVertices[i][0];
      positions[i*3+1] = mMeshTri.mVertices[i][1];
      positions[i*3+2] = mMeshTri.mVertices[i][2];
    }
    
    int nNormals = mMeshTri.mNormals.size();
    GLfloat* normals =  new GLfloat[nNormals*3];
    for ( unsigned i = 0; i < nNormals ; ++i )
    {
      normals[i*3+0] = mMeshTri.mNormals[i][0];
      normals[i*3+1] = mMeshTri.mNormals[i][1];
      normals[i*3+2] = mMeshTri.mNormals[i][2];
    }
    
    int nTexCoords = mMeshTri.mTextureCoords.size();
    GLfloat* texcoords =  new GLfloat[nTexCoords*2];
    for ( unsigned i = 0; i < nTexCoords; ++i )
    {
      texcoords[i*2+0] = mMeshTri.mTextureCoords[i][0];
      texcoords[i*2+1] = mMeshTri.mTextureCoords[i][1];
    }
    
    
    int nFaces = mMeshTri.mFaces.size();
    GLushort* indices =  new GLushort[nFaces*3];
#ifndef CINDER_GLES2
    std::vector<uint32_t> idx;
#endif
    for ( unsigned i = 0; i < nFaces; ++i )
    {
      indices[i*3+0] = mMeshTri.mFaces[i][0];
      indices[i*3+1] = mMeshTri.mFaces[i][1];
      indices[i*3+2] = mMeshTri.mFaces[i][2];
#ifndef CINDER_GLES2
      idx.push_back(mMeshTri.mFaces[i][0]);
      idx.push_back(mMeshTri.mFaces[i][1]);
      idx.push_back(mMeshTri.mFaces[i][2]);
#endif
      
    }
    
    GLfloat *boneIndices = new GLfloat[nVerts*4];
    GLfloat *boneWeights = new GLfloat[nVerts*4];
    
    for(int i=0; i<nVerts; i++)
    {
      for(int j=0; j<4; j++)
      {
        boneIndices[i*4 + j] = mBoneIndices[i][j];
        boneWeights[i*4 + j] = mBoneWeights[i][j];
      }
    }
    
    ci::gl::Texture::Format format;
    format.setWrapS(mMaterialFormatData.wrapS);
    format.setWrapT(mMaterialFormatData.wrapT);
    //needed to draw on ios correctly(?)
    format.setMinFilter(GL_NEAREST);
    format.setMagFilter(GL_NEAREST);
    
    ci::fs::path texPath = ci::fs::path (modelDir) / ci::fs::path (mTextureData.relativeTexPath);
    
    if (debugOut)
      std::cout<<"Loading tex: "<<texPath<<std::endl;
    vboMesh->mTexture = ci::gl::Texture( ci::loadImage( texPath ), format );
    
    vboMesh->mTwoSided = mTwoSided;
    vboMesh->mNVerts = nVerts;
    vboMesh->mNNormals = nNormals;
    
    
    mDrawable = DrawableMeshRef(vboMesh);
    
#ifndef CINDER_GLES2
    cinder::gl::VboMesh::Layout layout;
    
    layout.setStaticIndices();
    //positions
    layout.mCustomStatic.push_back(std::make_pair(cinder::gl::VboMesh::Layout::CUSTOM_ATTR_FLOAT3, 0 ));
    //normals
    layout.mCustomStatic.push_back(std::make_pair(cinder::gl::VboMesh::Layout::CUSTOM_ATTR_FLOAT3, 0 ));
    //texcoords
    layout.mCustomStatic.push_back(std::make_pair(cinder::gl::VboMesh::Layout::CUSTOM_ATTR_FLOAT2, 0 ));
    //boneweights
    layout.mCustomStatic.push_back(std::make_pair(cinder::gl::VboMesh::Layout::CUSTOM_ATTR_FLOAT4, 0 ));
    //boneindices
    layout.mCustomStatic.push_back(std::make_pair(cinder::gl::VboMesh::Layout::CUSTOM_ATTR_FLOAT4, 0 ));
    
    vboMesh->mVbo = cinder::gl::VboMesh( nVerts, nFaces, layout, GL_TRIANGLES );
    vboMesh->mVbo.bufferIndices(idx);
    vboMesh->mVbo.unbindBuffers();
    
    vboMesh->mVbo.getStaticVbo().bind();
    
    //sub buffers here
    size_t offset = 0;
    vboMesh->mVbo.getStaticVbo().bufferSubData(offset, sizeof(GLfloat)*3*nVerts, positions);
    offset += sizeof(GLfloat)*3*nVerts;
    vboMesh->mVbo.getStaticVbo().bufferSubData(offset, sizeof(GLfloat)*3*nVerts, normals);
    offset += sizeof(GLfloat)*3*nVerts;
    vboMesh->mVbo.getStaticVbo().bufferSubData(offset, sizeof(GLfloat)*2*nVerts, texcoords);
    offset += sizeof(GLfloat)*2*nVerts;
    vboMesh->mVbo.getStaticVbo().bufferSubData(offset, sizeof(GLfloat)*4*nVerts, boneWeights);
    offset += sizeof(GLfloat)*4*nVerts;
    vboMesh->mVbo.getStaticVbo().bufferSubData(offset, sizeof(GLfloat)*4*nVerts, boneIndices);
    offset += sizeof(GLfloat)*4*nVerts;
    vboMesh->mVbo.getStaticVbo().unbind();
    
    vboMesh->mMeshShader.bind();
    GLint loc0 = vboMesh->mMeshShader.getAttribLocation( "position" );
    vboMesh->mVbo.setCustomStaticLocation( 0, loc0 );
    GLint loc1 = vboMesh->mMeshShader.getAttribLocation( "normal" );
    vboMesh->mVbo.setCustomStaticLocation( 1, loc1 );
    GLint loc2 = vboMesh->mMeshShader.getAttribLocation( "texcoord" );
    vboMesh->mVbo.setCustomStaticLocation( 2, loc2 );
    GLint loc3 = vboMesh->mMeshShader.getAttribLocation( "boneWeights" );
    vboMesh->mVbo.setCustomStaticLocation( 3, loc3 );
    GLint loc4 = vboMesh->mMeshShader.getAttribLocation( "boneIndices" );
    vboMesh->mVbo.setCustomStaticLocation( 4, loc4 );
    vboMesh->mMeshShader.unbind();
    
#else
    vboMesh->mVbo = cinder::gl::Vbo::create(GL_TRIANGLES);
    vboMesh->mVbo->set(cinder::gl::Vbo::Attribute::create("position", 3, GL_FLOAT, GL_STATIC_DRAW)->setData(positions, nVerts*3));
    vboMesh->mVbo->set(cinder::gl::Vbo::Attribute::create("normal", 3, GL_FLOAT, GL_STATIC_DRAW)->setData(normals, nNormals*3));
    vboMesh->mVbo->set(cinder::gl::Vbo::Attribute::create("texcoord", 2, GL_FLOAT, GL_STATIC_DRAW)->setData(texcoords, nTexCoords*2));
    vboMesh->mVbo->set(cinder::gl::Vbo::Attribute::create("boneWeights", 4, GL_FLOAT, GL_STATIC_DRAW)->setData(boneWeights, nVerts*4));
    vboMesh->mVbo->set(cinder::gl::Vbo::Attribute::create("boneIndices", 4, GL_FLOAT, GL_STATIC_DRAW)->setData(boneIndices, nVerts*4));
    
    vboMesh->mVbo->set(cinder::gl::Vbo::Attribute::createIndex()->setData(indices, nFaces*3));
#endif
    
    delete [] positions;
    delete [] normals;
    delete [] texcoords;
    delete [] boneWeights;
    delete [] boneIndices;
    delete [] indices;
  }
  
  void MeshContainer::write(ogzstream& stream)
  {
    mMeshTri.write(stream);
    Util::writeString(stream, mName);
    Util::writeString(stream, mTextureData.relativeTexPath);
    Util::writeGLenum(stream, mMaterialData.face);
    Util::writeColorAf(stream, mMaterialData.dcolor);
    Util::writeColorAf(stream, mMaterialData.scolor);
    Util::writeColorAf(stream, mMaterialData.acolor);
    Util::writeColorAf(stream, mMaterialData.ecolor);
    Util::writeGLenum(stream, mMaterialFormatData.wrapS);
    Util::writeGLenum(stream, mMaterialFormatData.wrapT);
    
    int numMorphWeights = mMorphWeights.size();
    stream<<numMorphWeights<<std::endl;
    for (int i=0; i<numMorphWeights; i++)
    {
      stream<<mMorphWeights[i]<<std::endl;
    }
    
    stream<<mValidCache<<std::endl;
    
    int numBones = mBones.size();
    stream<<numBones<<std::endl;
    if (debugOut)
      std::cout<<numBones<<" bones for this mesh"<<std::endl;
    for(int i=0; i<numBones; i++)
    {
      mBones[i]->write(stream);
    }
    
    int nBoneIndices = mBoneIndices.size();
    stream<<nBoneIndices<<std::endl;
    for(int i=0; i<nBoneIndices; i++)
      Util::writeVec4f(stream, mBoneIndices[i]);
    
    int nBoneWeights = mBoneWeights.size();
    stream<<nBoneWeights<<std::endl;
    for(int i=0; i<nBoneWeights; i++)
      Util::writeVec4f(stream, mBoneWeights[i]);
  }
  
  void MeshContainer::read(igzstream& stream)
  {
    mMeshTri.read(stream);
    Util::readString(stream, mName);
    if (debugOut)
      std::cout<<"reading mesh name "<<mName<<std::endl;
    
    Util::readString(stream, mTextureData.relativeTexPath);
    Util::readGLenum(stream, mMaterialData.face);
    Util::readColorAf(stream, mMaterialData.dcolor);
    Util::readColorAf(stream, mMaterialData.scolor);
    Util::readColorAf(stream, mMaterialData.acolor);
    Util::readColorAf(stream, mMaterialData.ecolor);
    Util::readGLenum(stream, mMaterialFormatData.wrapS);
    Util::readGLenum(stream, mMaterialFormatData.wrapT);
    
    int numMorphWeights;
    stream>>numMorphWeights;
    for (int i=0; i<numMorphWeights; i++)
    {
      float data;
      stream>>data;
      mMorphWeights.push_back(data);
    }
    
    stream>>mValidCache;
    
    int numBones = mBones.size();
    stream>>numBones;
    if (debugOut)
      std::cout<<numBones<<" bones for this mesh"<<std::endl;
    for(int i=0; i<numBones; i++)
    {
      BoneRef data = BoneRef (new Bone());
      data->read(stream);
      mBones.push_back(data);
    }
    
    int nBoneIndices;
    stream>>nBoneIndices;
    mBoneIndices.resize(nBoneIndices);  
    for(int i=0;i<nBoneIndices;i++)
    {
      Util::readVec4f(stream, mBoneIndices[i]);
    }
    int nBoneWeights;
    stream>>nBoneWeights;
    mBoneWeights.resize(nBoneWeights);
    
    for(int i=0;i<nBoneWeights;i++)
    {
      Util::readVec4f(stream, mBoneWeights[i]);
    }
  }
  
}//namespace MeshFu