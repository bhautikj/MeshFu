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

#include "MeshFuUtil.h"
#include <map>
#include <limits>

namespace
{
  
  float sanitizeFloat(const float& val)
  {
    if (val < 1e-30 && val > -1e-30)
      return 0.0;
    
    return val;
  }
  
}

namespace MeshFu
{
  
  namespace Util
  {
    
    //---------------------------------
    
    AppState* AppState::theAppState = 0;
    
    //---------------------------------
    
    void writeVec4f(ogzstream& stream, const cinder::Vec4f &vec)
    {
      stream<<sanitizeFloat(vec.x)<<" "<<sanitizeFloat(vec.y)<<" "<<sanitizeFloat(vec.z)<<" "<<sanitizeFloat(vec.w)<<std::endl;
    }
    
    void readVec4f(igzstream& stream, cinder::Vec4f &vec)
    {
      stream>>vec.x;
      stream>>vec.y;
      stream>>vec.z;
      stream>>vec.w;
    }
    
    void writeVec3f(ogzstream& stream, const cinder::Vec3f &vec)
    {
      stream<<sanitizeFloat(vec.x)<<" "<<sanitizeFloat(vec.y)<<" "<<sanitizeFloat(vec.z)<<std::endl;
    }
    
    void readVec3f(igzstream& stream, cinder::Vec3f &vec)
    {
      stream>>vec.x;
      stream>>vec.y;
      stream>>vec.z;
    }
    
    void writeVec2f(ogzstream& stream, const cinder::Vec2f &vec)
    {
      stream<<vec.x<<" "<<vec.y<<std::endl;
    }
    
    void readVec2f(igzstream& stream, cinder::Vec2f &vec)
    {
      stream>>vec.x;
      stream>>vec.y;
    }
    
    void writeColorAf(ogzstream& stream, const cinder::ColorAf& col)
    {
      stream<<col.r<<" "<<col.g<<" "<<col.b<<" "<<col.a<<std::endl;
    }
    
    void readColorAf(igzstream& stream, cinder::ColorAf& col)
    {
      stream>>col.r;
      stream>>col.g;
      stream>>col.b;
      stream>>col.a;
    }
    
    void writeString(ogzstream& stream, const std::string& str)
    {
      if (str == "")
        stream<<"_empty_"<<std::endl;
      else
        stream<<str<<std::endl;
    }
    
    void readString(igzstream& stream, std::string& str)
    {
      std::string tmp;
      stream>>tmp;
      if (tmp == "_empty_")
        str="";
      else
        str=tmp;
    }
    
    void writeQuatf(ogzstream& stream, const cinder::Quatf& q)
    {
      float w = q.w;
      cinder::Vec3f v = q.v;
      stream<<w<<" "<<v.x<<" "<<v.y<<" "<<v.z<<std::endl;
    }
    
    void readQuatf(igzstream& stream, cinder::Quatf& q)
    {
      float w,x,y,z;
      stream>>w;
      stream>>x;
      stream>>y;
      stream>>z;
      q.set(w,x,y,z);
    }
    
    void writeMatrix44f(ogzstream& stream, const cinder::Matrix44f& mat)
    {
      for(int i=0;i<15;i++)
        stream<<mat[i]<<" ";
      stream<<mat[15];
      stream<<std::endl;
    }
    
    void readMatrix44f(igzstream& stream, cinder::Matrix44f& mat)
    {
      float data[16];
      for(int i=0;i<16;i++)
        stream>>data[i];
      mat.set(data);
    }
    
    void writeGLenum(ogzstream& stream, const GLenum& en)
    {
      std::string data;
      if (en==GL_FRONT_AND_BACK)
        data="GL_FRONT_AND_BACK";
      else if (en==GL_FRONT)
        data="GL_FRONT";
      else if (en==GL_REPEAT)
        data="GL_REPEAT";
      else if(en==GL_CLAMP_TO_EDGE)
        data="GL_CLAMP_TO_EDGE";
      else
        assert(0);

      stream<<data<<std::endl;
    }
    
    void readGLenum(igzstream& stream, GLenum& en)
    {
      std::string data;
      stream>>data;
      if (data == "GL_FRONT_AND_BACK")
        en = GL_FRONT_AND_BACK;
      else if (data == "GL_FRONT")
        en = GL_FRONT;
      else if (data == "GL_REPEAT")
        en = GL_REPEAT;
      else if (data == "GL_CLAMP_TO_EDGE")
        en = GL_CLAMP_TO_EDGE;
      else
        assert(0);
    }
    
  }//namespace Util
  
}//namespace MeshFu
