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

#include "AppCore.h"
#include "cinder/gl/Light.h"

#include "cinder/thread.h"
#include "cinder/timer.h"

#include "gzstream.h"
#include "MeshFuGeometry.h"
#include "MeshFuUtil.h"

AppCore::AppCore(AppProxyAbstract* appProxy) :
MetaApp(appProxy)
{
  
}

void AppCore::setup()
{
  gl::enableAlphaBlending();
  
  float aspectRatio = mAppProxy->getWindowAspectRatio();
  mCam.setPerspective( 60, aspectRatio , 0.1f, 1000.0f );
  
  std::string asset("seymour/astroboy_walk.dae.cmf");
  mCam.setEyePoint( Vec3f( 0, 7, 20 ) );
  mCam.setCenterOfInterestPoint( Vec3f( 0, 7, 0 ) );
  std::string assetPathString = getAssetPath( asset );
  mModelsToLoad.push_back(getAssetPath( asset ));

  mDoneLoading = false;
  
  mMayaCam.setCurrentCam( mCam );
}

void AppCore::update()
{
  if (!mModelsToLoad.empty())
  {
    if (mAppProxy->getElapsedSeconds() > 1.0)
    {
      std::string modelToLoad = mModelsToLoad.back();
      mModelsToLoad.pop_back();
      loadModel(modelToLoad);
    }
  }
  else
  {
    mDoneLoading=true;
    
  }
  
  if (mDoneLoading)
  {
    Matrix44f projMatrix =  mMayaCam.getCamera().getProjectionMatrix() * mMayaCam.getCamera().getModelViewMatrix();
    
    MeshFu::Util::AppState::getAppState()->mCameraMvpMat = projMatrix;
    for (int i=0; i<mModels.size();i++)
      mModels[i]->update();
  }
}

void
AppCore::loadModel(const std::string path)
{
  Model* model = new Model(path);
  mModels.push_back(model);
}

void AppCore::draw()
{
  gl::clear( Color::black() );

  if (mDoneLoading)
  {
    for (int i=0; i<mModels.size();i++)
      mModels[i]->draw();
  }
}

void AppCore::mouseDown( MouseEvent event )
{
  mMayaCam.mouseDown( event.getPos() );
}

void AppCore::mouseDrag( MouseEvent event )
{
  mMayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
}

void AppCore::touchesBegan( TouchEvent event )
{
  Vec2f eventPos;
  for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
    eventPos = touchIt->getPos();
    break;
  }
  mMayaCam.mouseDown(eventPos);
}

void AppCore::touchesMoved( TouchEvent event )
{
  Vec2f eventPos;
  for( vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt ) {
    eventPos = touchIt->getPos();
    break;
  }
  
  bool leftDown = true;
  bool middleDown = false;
  bool rightDown = false;
  if (event.getTouches().size()==2)
  {
    leftDown = false;
    rightDown = true;
  }
  else if (event.getTouches().size()==3)
  {
    leftDown = false;
    middleDown = true;
  }
  mMayaCam.mouseDrag(eventPos, leftDown,rightDown,middleDown);
}

void AppCore::resize( ResizeEvent event )
{
  CameraPersp cam = mMayaCam.getCamera();
  cam.setAspectRatio( getWindowAspectRatio() );
  mMayaCam.setCurrentCam( cam );
}

//----------

Model::Model(std::string modelPath) :
mValidAsset(true)
{
  if (modelPath == "")
    return;
  
  //TODO: break this into UI friendly pieces
  mLoader = MeshFu::ModelWrangler( modelPath );
  igzstream testIn;
  testIn.open(modelPath.c_str());
  mLoader.read(testIn);
  testIn.close();
  mLoader.buildAll();
  
  mNodeNames = mLoader.getNodeNames();
  vector<string> tmpMeshNames = mLoader.getMeshNames();
  vector<string>::iterator it = tmpMeshNames.begin();
  while (it != tmpMeshNames.end())
  {
    string meshName = *it;
    MeshFu::MeshContainerRef meshfu = mLoader.getMeshByName(meshName);
    if (meshfu->mMeshTri.mMorphs.size() != 0)
      mMeshNames.push_back(meshName);
    it++;
  }
}

Model::~Model()
{
  
}

void
Model::draw()
{
  if (mValidAsset)
  {
    mLoader.draw();
  }
}

void
Model::update()
{
  if (mValidAsset)
  {
    static float morphAnim = 0.0;
    static bool morphUp = true;
    if (morphUp)
      morphAnim += 0.1;
    else
      morphAnim -= 0.1;
    
    if (morphAnim>1.0)
      morphUp = false;
    else if (morphAnim<0.0)
      morphUp = true;
    
    for (int i = 0; i < mMeshNames.size(); i++)
    {
      MeshFu::MeshContainerRef meshfu = mLoader.getMeshByName(mMeshNames[i]);
      meshfu->mMorphWeights[0] = morphAnim;
    }
    
    
    mLoader.update();
  }
}

