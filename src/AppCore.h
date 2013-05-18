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

#ifndef _APPCORE_H_
#define _APPCORE_H_

#include "MetaApp.h"
#include "MeshFuRiggedGeometry.h"
#include "cinder/MayaCamUI.h"

#include <vector>

using namespace std;

class Model
{
public:
  Model(std::string modelPath);
  ~Model();
  
  void draw();
  void update();
  
protected:
  MeshFu::RiggedGeometry mLoader;
  vector< string > mNodeNames;
  vector< string > mMeshNames;
  bool mValidAsset;
};

class AppCore : public MetaApp
{
public:
  AppCore(AppProxyAbstract* appProxy);
  
  virtual void setup();
  virtual void update();
  virtual void draw();
  
  void resize( ResizeEvent event );
  void mouseDown( MouseEvent event );
  void mouseDrag( MouseEvent event );
  void touchesBegan( TouchEvent event );
  void touchesMoved( TouchEvent event );
  
private:
  //float mFps;
  MayaCamUI mMayaCam;
  
  bool mDoneLoading;
  
  void loadModel(const std::string path);
  
  // this is our list of finished textures
  vector<Model *> mModels;
  
  // this is our queue of things we want to load:
  vector<std::string> mModelsToLoad;
  
  CameraPersp mCam;
};


#endif