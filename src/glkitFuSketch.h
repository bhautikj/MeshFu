#pragma once

#include "CinderGLSketch.h"

#include "cinder/gl/GlslProg.h"
#include "Vbo.h"

#include "cinder/Vector.h"
#include "cinder/Camera.h"
#include "cinder/Perlin.h"

using namespace ci;

class AppCore;

class glkitFuSketch : public CinderGLSketch {
public:
    
    void setup();
    void update();
    void draw(const Area &area);

    virtual void touchesBegan(app::TouchEvent event);
    virtual void touchesMoved(app::TouchEvent event);
    /*
    gl::GlslProg m_color_shader;
    
    CameraPersp m_camera;
    Matrix44f m_mvp_matrix;
    Matrix44f m_model_matrix;
    gl::VboRef m_vbo;
    */
    
    AppCore* mMetaApp;
};