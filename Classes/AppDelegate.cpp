/**
 *
 *  You can modify and use this source freely
 *  only for the development of application related Live2D.
 *
 *  (c) Live2D Inc. All rights reserved.
 */
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MainScene.h"
#include "Live2D.h"

#ifdef L2D_TARGET_ANDROID_ES2
    //android��̃V�F�[�_�ݒ�̂��߂ɓǂݍ���
    #include "graphics/DrawParam_OpenGLES2.h"
    #include "base/CCEventListenerCustom.h"
#endif

using namespace live2d;
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
#ifdef L2D_TARGET_ANDROID_ES2
    Director::getInstance()->getEventDispatcher()->removeEventListener(_recreatedEventlistener);
#endif
	Live2D::dispose();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Live2D Simple");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("images");
    
	//Live2D
	Live2D::init();

	#ifdef L2D_TARGET_ANDROID_ES2
        char *exts = (char*)glGetString(GL_EXTENSIONS);
        if(strstr(exts, "GL_NV_shader_framebuffer_fetch ")){
            live2d::DrawParam_OpenGLES2::setExtShaderMode( true , true );
        }

        _recreatedEventlistener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, 
	        [this](EventCustom*) 
	        {
	            live2d::DrawParam_OpenGLES2::reloadShader();
	        });
        director->getEventDispatcher()->addEventListenerWithFixedPriority(_recreatedEventlistener, -1);
	#endif
    
    // create a scene. it's an autorelease object
//    auto scene = HelloWorld::createScene();
    auto scene = MainScene::createScene();
    

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
