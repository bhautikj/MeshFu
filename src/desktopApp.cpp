#include "cinder/app/App.h"
#include "cinder/app/AppBasic.h"

#include "MetaApp.h"
#include "AppCore.h"

using namespace ci;
using namespace ci::app;

struct AppProxyCinder : public AppProxyAbstract
{
    virtual float getWindowAspectRatio()
    {
        return cinder::app::getWindowAspectRatio();
    }
    
    virtual float getElapsedSeconds()
    {
        return cinder::app::getElapsedSeconds();
    }
};

//----------

class AssimpApp : public AppBasic
{
    public:
    void setup();
    void prepareSettings( Settings *settings );
    void resize( ResizeEvent event );
    void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    
    void update();
    void draw();
    
    private:
    
    AppCore* mMetaApp;
};

void AssimpApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 300, 300 );
    settings->setFrameRate( 60.0f );
}

void AssimpApp::setup()
{
    mMetaApp = new AppCore(new AppProxyCinder());
    mMetaApp->setup();
}

void AssimpApp::update()
{
    mMetaApp->update();
}

void AssimpApp::draw()
{
    mMetaApp->draw();
}

void AssimpApp::mouseDown( MouseEvent event )
{
    mMetaApp->mouseDown(event);
}

void AssimpApp::mouseDrag( MouseEvent event )
{
    mMetaApp->mouseDrag(event);
}

void AssimpApp::resize( ResizeEvent event )
{
    mMetaApp->resize(event);
}


CINDER_APP_BASIC( AssimpApp, RendererGl(0) )
