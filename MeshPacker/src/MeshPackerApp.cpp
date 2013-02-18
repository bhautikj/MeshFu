#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MeshPackerApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void MeshPackerApp::setup()
{
}

void MeshPackerApp::mouseDown( MouseEvent event )
{
}

void MeshPackerApp::update()
{
}

void MeshPackerApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}


CINDER_APP_BASIC( MeshPackerApp, RendererGl )
