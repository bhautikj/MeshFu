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

#include "MeshFuGeometry.h"
#include "MeshFuUtil.h"

#include "cinder/ImageIo.h"
#include "cinder/gl/gl.h"

namespace
{
  const bool debugOut = false;
}

namespace MeshFu
{
  
  void PointCloud::write(ogzstream& stream)
  {
    Util::writeString(stream, mName);
    int nVerts = mVertices.size();
    stream<<nVerts<<std::endl;
    
    for (int i=0; i<nVerts; i++)
    {
      Util::writeVec3f(stream, mVertices[i]);
    }
    
    int nNormals = mNormals.size();
    stream<<nNormals<<std::endl;
    for (int i=0; i<nNormals; i++)
    {
      Util::writeVec3f(stream, mNormals[i]);
    }
  }
  
  void PointCloud::read(igzstream& stream)
  {
    Util::readString(stream, mName);
    int nVerts;
    stream>>nVerts;
    if (debugOut)
      std::cout<<"reading nverts="<<nVerts<<std::endl;
    
    for(int i=0; i<nVerts;i++)
    {
      cinder::Vec3f data;
      Util::readVec3f(stream, data);
      mVertices.push_back(data);
    }
    
    int nNormals;
    stream>>nNormals;
    if (debugOut)
      std::cout<<"reading nNormals="<<nNormals<<std::endl;
    for(int i=0; i<nNormals;i++)
    {
      cinder::Vec3f data;
      Util::readVec3f(stream, data);
      mNormals.push_back(data);
    }
  }
  
  void MeshTri::write(ogzstream& stream)
  {
    PointCloud::write(stream);
    stream<<mHasTexture<<std::endl;
    int numTextureCoords = mTextureCoords.size();
    stream<<numTextureCoords<<std::endl;
    for(int i=0;i<numTextureCoords;i++)
    {
      Util::writeVec2f(stream, mTextureCoords[i]);
    }
    
    int numColors = mColors.size();
    stream<<numColors<<std::endl;
    for(int i=0;i<numColors;i++)
    {
      Util::writeColorAf(stream, mColors[i]);
    }
    
    int numFaces = mFaces.size();
    stream<<numFaces<<std::endl;
    for(int i=0;i<numFaces;i++)
    {
      stream<<mFaces[i][0]<<" "<<mFaces[i][1]<<" "<<mFaces[i][2]<<std::endl;
    }
    
    int numMorphs = mMorphs.size();
    stream<<numMorphs<<std::endl;
    for(int i=0; i<numMorphs;i++)
    {
      Util::writeString(stream, "startMorph");
      mMorphs[i]->write(stream);
      Util::writeString(stream, "endMorph");
    }
    Util::writeString(stream, "endMesh");
  }
  
  void MeshTri::read(igzstream& stream)
  {
    PointCloud::read(stream);
    stream>>mHasTexture;
    int numTextureCoords;
    stream>>numTextureCoords;
    if (debugOut)
      std::cout<<"reading "<<numTextureCoords<<" texture coords"<<std::endl;
    for(int i=0;i<numTextureCoords;i++)
    {
      cinder::Vec2f data;
      Util::readVec2f(stream, data);
      mTextureCoords.push_back(data);
    }
    
    int numColors;
    stream>>numColors;
    for(int i=0;i<numColors;i++)
    {
      cinder::ColorAf data;
      Util::readColorAf(stream, data);
      mColors.push_back(data);
    }
    
    int numFaces;
    stream>>numFaces;
    if (debugOut)
      std::cout<<"reading "<<numFaces<<" faces"<<std::endl;
    
    for(int i=0;i<numFaces;i++)
    {
      int A,B,C;
      stream>>A;
      stream>>B;
      stream>>C;
      mFaces.push_back(MeshTri::Face(A,B,C));
    }
    
    int numMorphs;
    stream>>numMorphs;
    //std::cout<<numMorphs<<std::endl;
    if (debugOut)
      std::cout<<"reading "<<numMorphs<<" morphs"<<std::endl;
    for(int i=0; i<numMorphs;i++)
    {
      std::string dummy;
      Util::readString(stream, dummy);
      if (dummy != "startMorph")
      {
        std::cout<<"expecting startMorph but found "<<dummy<<std::endl;
        assert(0);
      }
      
      PointCloudRef morph = PointCloudRef(new PointCloud());
      morph->read(stream);
      mMorphs.push_back(morph);
      Util::readString(stream, dummy);
      if (dummy != "endMorph")
      {
        std::cout<<"expecting endMorph but found:"<<dummy<<":now what?"<<std::endl;
        for(int i=0;i<100;i++)
        {
          stream>>dummy;
          std::cout<<dummy<<std::endl;
        }
        assert(0);
      }
    }
    std::string dummy2;
    Util::readString(stream, dummy2);
    if (dummy2 != "endMesh")
    {
      std::cout<<"expecting endMesh but found:"<<dummy2<<":now what?"<<std::endl;
      assert(0);
    }
  }
  
  //----------------------------
  
  void Bone::write(ogzstream& stream)
  {
    Util::writeString(stream, mName);
    Util::writeMatrix44f(stream, mOffsetMatrix);
  }
  
  void Bone::read(igzstream& stream)
  {
    Util::readString(stream, mName);
    Util::readMatrix44f(stream, mOffsetMatrix);
  }
  
  //-----------------------------
  
}//namespace MeshFu