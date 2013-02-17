#pragma once

#include "CinderGLSketch.h"

#include "cinder/gl/GlslProg.h"
#include "Vbo.h"

#include "cinder/Vector.h"
#include "cinder/Camera.h"

#include "AppCore.h"

using namespace ci;

class meshFuGlkitAppSketch : public CinderGLSketch {
public:
    
    void setup();
    void update();
    void draw(const Area &area);

    virtual void touchesBegan(app::TouchEvent event);
    virtual void touchesMoved(app::TouchEvent event);
    
    AppCore* mMetaApp;
    
};