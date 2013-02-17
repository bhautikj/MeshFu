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

#include "glkitFuSketch.h"

#include "cinder/ImageIO.h"
#include "cinder/app/App.h"


#include "MetaApp.h"
#include "AppCore.h"


struct AppProxyGlkit : public AppProxyAbstract
{
  AppProxyGlkit (CinderGLSketch* sketch) : mSketch(sketch), AppProxyAbstract()
  {
    
  }
  
  virtual float getWindowAspectRatio()
  {
    return mSketch->getAspectRatio();
  }
  
  virtual float getElapsedSeconds()
  {
    return mSketch->getElapsedSeconds();
  }
  
  CinderGLSketch* mSketch;
};


void glkitFuSketch::setup()
{
  mMetaApp = new AppCore(new AppProxyGlkit(this));
  mMetaApp->setup();
}

void glkitFuSketch::update()
{
  mMetaApp->update();
}

void glkitFuSketch::draw(const Area &area)
{
  gl::clear(ColorA(0, 0, 0, 1));
  
  mMetaApp->draw();
}

void glkitFuSketch::touchesBegan(app::TouchEvent event)
{
  mMetaApp->touchesBegan(event);
}

void glkitFuSketch::glkitFuSketch::touchesMoved(app::TouchEvent event)
{
  mMetaApp->touchesMoved(event);
}
