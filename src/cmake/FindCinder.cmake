include(LibFindMacros)


find_path(cinder_DIR "lib/libcinder.a" DOC "Root directory of cinder")

if(EXISTS "${cinder_DIR}")
  find_path(cinder_INCLUDE_DIR
    NAMES cinder/Cinder.h 
    PATHS ${cinder_DIR}/include 
  )
  
  REQUIRE_HEADER(cinder_INCLUDE_DIR cinder/app/AppBasic.h)

  find_library(cinder_LIBRARY
    NAMES cinder 
    PATHS ${cinder_DIR}/lib 
  )
   
  find_library(cinder_boost_system_LIBRARIES
    NAMES boost_system
    PATHS ${cinder_DIR}/lib/macosx
  )
  
  find_library(cinder_boost_filesystem_LIBRARIES
    NAMES boost_filesystem
    PATHS ${cinder_DIR}/lib/macosx
  )
  
  find_library(cinder_boost_date_time_LIBRARIES
    NAMES boost_date_time
    PATHS ${cinder_DIR}/lib/macosx
  )
  
    
  
  set(cinder_PROCESS_INCLUDES cinder_INCLUDE_DIR cinder_INCLUDE_DIRS)
  set(cinder_PROCESS_LIBS cinder_LIBRARY)
  libfind_process(cinder)

  #assimp cinder block
#   if(EXISTS "${cinder_DIR}/blocks/assimp")
#     find_path(cinderassimp_PATH
#       NAMES AssimpLoader.h
#       PATHS ${cinder_DIR}/blocks/assimp/src
#     )
# 
#     REQUIRE_HEADER(cinderassimp_PATH AssimpLoader.h)
# 
#     SET(CINDERASSIMP_SOURCES 
#     	${cinderassimp_PATH}/AssimpLoader.h
#     	${cinderassimp_PATH}/AssimpLoader.cpp
#     	${cinderassimp_PATH}/Node.cpp
#     	${cinderassimp_PATH}/Node.h
#     	${cinderassimp_PATH}/AssimpMesh.h
#     	)
# 
#     ADD_LIBRARY("cinderassimp" STATIC ${CINDERASSIMP_SOURCES})
# 
#     FIND_LIBRARY(assimp_LIBRARY assimp PATHS ${cinder_DIR}/blocks/assimp/lib/macosx)
#     TARGET_LINK_LIBRARIES("cinderassimp" ${assimp_LIBRARY})
#   endif(EXISTS "${cinder_DIR}/blocks/assimp")

#   if(EXISTS "${cinder_DIR}/blocks/cinder-glkit")
#       find_path(cinderglkit_PATH
#       NAMES Vbo.h
#       PATHS ${cinder_DIR}/blocks/cinder-glkit/include
#     )
#     
#     REQUIRE_HEADER(cinderglkit_PATH include/Vbo.h)
#     
#     SET(cinderglkit_SOURCES 
#       ${cinderglkit_PATH}/include/Vbo.h
#       ${cinderglkit_PATH}/src/Vbo.cpp  
#     )
# 
#     ADD_LIBRARY("cinder_glkit" STATIC ${cinderglkit_SOURCES})
#   
#   endif(EXISTS "${cinder_DIR}/blocks/cinder-glkit") 
  
  #ciui cinder block
  if(EXISTS "${cinder_DIR}/blocks/ciUI")
    find_path(ciUI_PATH
      NAMES ciUI.h
      PATHS ${cinder_DIR}/blocks/ciUI/src
    )

    REQUIRE_HEADER(ciUI_PATH ciUI.h)

    SET(CIUI_SOURCES 
      ${ciUI_PATH}/ciUI.h
      ${ciUI_PATH}/ciUI2DPad.h
      ${ciUI_PATH}/ciUIBiLabelSlider.h
      ${ciUI_PATH}/ciUIButton.h
      ${ciUI_PATH}/ciUICanvas.h
      ${ciUI_PATH}/ciUICircleSlider.h
      ${ciUI_PATH}/ciUIDropDownList.h
      ${ciUI_PATH}/ciUIEvent.h
      ${ciUI_PATH}/ciUIFPS.h
      ${ciUI_PATH}/ciUIFPSSlider.h
      ${ciUI_PATH}/ciUIImage.h
      ${ciUI_PATH}/ciUIImageButton.h
      ${ciUI_PATH}/ciUIImageSampler.h
      ${ciUI_PATH}/ciUIImageSlider.h
      ${ciUI_PATH}/ciUIImageToggle.h
      ${ciUI_PATH}/ciUILabel.h
      ${ciUI_PATH}/ciUILabelButton.h
      ${ciUI_PATH}/ciUILabelToggle.h
      ${ciUI_PATH}/ciUIMinimalSlider.h
      ${ciUI_PATH}/ciUIMovingGraph.h
      ${ciUI_PATH}/ciUIMultiImageButton.h
      ${ciUI_PATH}/ciUIMultiImageToggle.h
      ${ciUI_PATH}/ciUINumberDialer.h
      ${ciUI_PATH}/ciUIRadio.h
      ${ciUI_PATH}/ciUIRangeSlider.h
      ${ciUI_PATH}/ciUIRectangle.h
      ${ciUI_PATH}/ciUIRotarySlider.h
      ${ciUI_PATH}/ciUIScrollableCanvas.h
      ${ciUI_PATH}/ciUISlider.h
      ${ciUI_PATH}/ciUISpacer.h
      ${ciUI_PATH}/ciUISpectrum.h
      ${ciUI_PATH}/ciUITextInput.h
      ${ciUI_PATH}/ciUIToggle.h
      ${ciUI_PATH}/ciUIToggleMatrix.h
      ${ciUI_PATH}/ciUIWaveform.h
      ${ciUI_PATH}/ciUIWidget.h
      ${ciUI_PATH}/ciUIWidgetWithLabel.h
      )

    #ADD_CUSTOM_TARGET(ciUI CIUI_SOURCES ${CIUI_SOURCES})
  endif(EXISTS "${cinder_DIR}/blocks/ciUI")
   
  #opencv cinder block
  if(EXISTS "${cinder_DIR}/blocks/opencv")
    find_path(cinderopencv_PATH
      NAMES CinderOpenCV.h
      PATHS ${cinder_DIR}/blocks/opencv/include
    )
    REQUIRE_HEADER(cinderopencv_PATH CinderOpenCV.h) 

    find_library(cinderopencv_core_LIBRARY
      NAMES opencv_core
      PATHS ${cinder_DIR}/blocks/opencv/lib/macosx
    )
    
    #libopencv_calib3d.a		libopencv_flann.a		libopencv_legacy.a
    #libopencv_contrib.a		libopencv_gpu.a			libopencv_ml.a
    #libopencv_core.a		libopencv_haartraining_engine.a	libopencv_objdetect.a
    #libopencv_features2d.a		libopencv_imgproc.a		libopencv_video.a

  endif(EXISTS "${cinder_DIR}/blocks/opencv")
  
else(EXISTS "${cinder_DIR}")
        set(ERR_MSG "Please specify cinder directory using cinder_DIR env. variable")
endif(EXISTS "${cinder_DIR}")