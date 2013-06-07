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

#ifndef _METAAPP_H_
#define _METAAPP_H_

#include <string>
#include "cinder/app/App.h"

#if defined( CINDER_COCOA )
#include "cinder/app/AppCocoaTouch.h"
#endif

using namespace ci;
using namespace ci::app;

struct AppProxyAbstract
{
  virtual float getWindowAspectRatio() { assert(0); }
  virtual float getElapsedSeconds() { assert(0); }
};

class MetaApp
{
public:
  MetaApp(AppProxyAbstract* appProxy) : mAppProxy(appProxy) {}
  
  virtual void setup() = 0;
  virtual void update() = 0;
  virtual void draw() = 0 ;
  
  std::string getAssetPath(std::string asset)
  {
    //Boost on iOS barfs on fs::path::exists() so workaround.
    //Should be fixed when cinder moves to boost 1.50
#if defined( CINDER_COCOA )
    std::string resourcePathString = cinder::app::App::getResourcePath().generic_string();
    std::string assetPathString = resourcePathString + "/assets/" + asset;
#else
    std::string assetPathString = getAssetPath( asset ).generic_string();
#endif
    return assetPathString;
  }
  
  //virtual void resize( ResizeEvent event ) = 0;
  virtual void mouseDown( MouseEvent event ) = 0;
  virtual void mouseDrag( MouseEvent event ) = 0;
#if defined( CINDER_COCOA )
  virtual void touchesBegan( TouchEvent event ) = 0;
  virtual void touchesMoved( TouchEvent event ) = 0;
#endif
  
  AppProxyAbstract* mAppProxy;
  
};



#endif