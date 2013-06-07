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


#include "meshFuGlkitAppSketch.h"

#include "MetaApp.h"
#include "AppCore.h"

class DummyApp : public cinder::app::App
{
  public:

  
  virtual void	quit() {}
  virtual const Settings&	getSettings() const { App::Settings* x = new App::Settings; return *x; };
  virtual WindowRef	getWindow() const {return (WindowRef)0; };
	virtual size_t		getNumWindows() const { return 0; }
	virtual WindowRef	getWindowIndex( size_t index ) const { return 0;}
	virtual float		getFrameRate() const { return 0.0; }
	virtual void		setFrameRate( float frameRate ) {}
  virtual fs::path			getAppPath() const { return fs::path(""); }
  virtual void	launch( const char *title, int argc, char * const argv[] ) {}

  static void initDummy()
  {
    DummyApp* dummy = new DummyApp();
    App::sInstance = dummy;
  }
  
  protected:
    DummyApp() : cinder::app::App() {}
    ~DummyApp() {}
};

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


void meshFuGlkitAppSketch::setup()
{
  DummyApp::initDummy();
  mMetaApp = new AppCore(new AppProxyGlkit(this));
  mMetaApp->setup();
}

void meshFuGlkitAppSketch::update()
{
  mMetaApp->update();
}

void meshFuGlkitAppSketch::draw(const Area &area)
{
  gl::clear(ColorA(0, 0, 0, 1));
  
  mMetaApp->draw();
}

void meshFuGlkitAppSketch::touchesBegan(app::TouchEvent event)
{
  mMetaApp->touchesBegan(event);
}

void meshFuGlkitAppSketch::touchesMoved(app::TouchEvent event)
{
  mMetaApp->touchesMoved(event);
}