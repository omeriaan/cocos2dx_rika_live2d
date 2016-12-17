//
//  MainScene.h
//  RikaCocos
//
//  Created by Akira Suzuki on 2016/03/04.
//
//

#

#ifndef __RikaCocos__MainScene__
#define __RikaCocos__MainScene__

#include "cocos2d.h"
#include "RikaLive2DSprite.h"

class MainScene :public cocos2d::Layer
{
protected:
    MainScene();
    virtual ~MainScene();
    bool init() override;

private:
    RikaLive2DSprite* pLive2DSprite;
    
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(MainScene);
    CC_SYNTHESIZE_RETAIN(cocos2d::Sprite *, _beatle, Beatle);
};

#endif /* defined(__RikaCocos__MainScene__) */
