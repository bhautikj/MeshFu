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

#include "gzstream.h"

#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/Quaternion.h"
#include "cinder/Matrix44.h"
#include "cinder/gl/Texture.h"

namespace MeshFu
{
  
  namespace Util
  {
    
    static int MAXBONES = 64;
    
    void writeString(ogzstream& stream, const std::string& str);
    void readString(igzstream& stream, std::string& str);
    
    void writeVec4f(ogzstream& stream, const cinder::Vec4f &vec);
    void readVec4f(igzstream& stream, cinder::Vec4f &vec);
    
    void writeVec3f(ogzstream& stream, const cinder::Vec3f &vec);
    void readVec3f(igzstream& stream, cinder::Vec3f &vec);
    
    void writeVec2f(ogzstream& stream, const cinder::Vec2f &vec);
    void readVec2f(igzstream& stream, cinder::Vec2f &vec);
    
    void writeColorAf(ogzstream& stream, const cinder::ColorAf& col);
    void readColorAf(igzstream& stream, cinder::ColorAf& col);
    
    void writeQuatf(ogzstream& stream, const cinder::Quatf& q);
    void readQuatf(igzstream& stream, cinder::Quatf& q);
    
    void writeMatrix44f(ogzstream& stream, const cinder::Matrix44f& mat);
    void readMatrix44f(igzstream& stream, cinder::Matrix44f& mat);
    
    void writeGLenum(ogzstream& stream, const GLenum& en);
    void readGLenum(igzstream& stream, GLenum& en);
    
    
    class MeshFuExc : public std::exception
    {
    public:
      MeshFuExc( const std::string &log ) throw()
      {
        strncpy( mMessage, log.c_str(), 512 );
      }
      
      virtual const char* what() const throw()
      {
        return mMessage;
      }
      
    private:
      char mMessage[ 513 ];
    };
    
    struct AppState
    {
      cinder::Matrix44f mCameraMvpMat;
      
      static AppState* getAppState()
      {
        if (theAppState == 0)
        {
          theAppState = new AppState;
        }
        return theAppState;
      }
    private:
      static AppState* theAppState;
    };
    
  }//namespace Util
  
}//namespace MeshFu

