//
//  RikaLive2DSprite.h
//  RikaCocos
//
//  Created by Akira Suzuki on 2016/03/05.
//
//

#ifndef __RikaCocos__RikaLive2DSprite__
#define __RikaCocos__RikaLive2DSprite__

#include "cocos2d.h"
#include "2d/CCSprite.h"
#include <vector>
#include "Live2DModelOpenGL.h"
#include "Common.h"


class RikaLive2DSprite :public cocos2d::DrawNode {
    
    live2d::Live2DModelOpenGL* live2DModel ;
    
public:
    RikaLive2DSprite();
    virtual ~RikaLive2DSprite();
    
    virtual bool init();
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);
    void moveEye(enum OpeType, float touchX, float touchY);
    static RikaLive2DSprite* createDrawNode();

protected:
    cocos2d::CustomCommand _customCommand;
    
private:
    bool isTouch;
    float displaySizeX;
    float displaySizeY;
    float touchPosX;
    float touchPosY;
};


#endif /* defined(__RikaCocos__RikaLive2DSprite__) */