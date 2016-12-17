//
//  MainScene.cpp
//  RikaCocos
//
//  Created by Akira Suzuki on 2016/03/04.
//
//

#include "MainScene.h"
#include "RikaLive2DSprite.h"
#include "Common.h"

USING_NS_CC;

MainScene::MainScene()
: _beatle(nullptr)
, pLive2DSprite(nullptr)
{
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(_beatle); // _beatleをreleaseしてメモリリークを防ぐ
    CC_SAFE_RELEASE_NULL(pLive2DSprite);
}

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainScene::init()
{
    if(!Layer::init()) {
        return false;
    }
    
    // Directorを取り出す
    auto diretor = Director::getInstance();
    // 画面サイズを取り出す
    auto size = diretor->getWinSize();
    // 背景スプライトを生成する
    auto background = Sprite::create("background_2048_1536.png");
    // スプライトの表示位置を設定する
    background->setPosition(Vec2(size.width / 2.0, size.height /2.0));
    // 親ノードにスプライトを追加する
    this->addChild(background);
    
    pLive2DSprite = RikaLive2DSprite::createDrawNode();
    this->addChild(pLive2DSprite);
    
    this->setBeatle(Sprite::create("beatle.png")); // Spriteを生成して_beatleに格納
    
    auto listner = EventListenerTouchOneByOne::create();
    listner->onTouchBegan = [this](Touch* touch, Event* event) {
        // タッチされた時の処理
        
        log("TouchBegan at (%f, %f)", touch->getLocation().x, touch->getLocation().y);
        
        // _beatleをタッチした位置に表示
        _beatle->setPosition(Vec2(touch->getLocation().x, touch->getLocation().y));
        this->addChild(_beatle); // シーンに_beatleを配置
        
        pLive2DSprite->moveEye(TOUCH_START, touch->getLocation().x, touch->getLocation().y);
        
        return true;
    };
    listner->onTouchMoved = [this](Touch* touch, Event* event) {
        // タッチ位置が動いた時の処理
        
        // 前回とのタッチ位置との差をベクトルで取得する
        Vec2 delta = touch->getDelta();
        
        // 現在のカブトムシの座標を取得する
        Vec2 position = _beatle->getPosition();
        
        // 現在座標 + 移動量を新たな座標にする
        Vec2 newPosition = position + delta;
        
        // カブト虫が画面から飛び出ないようにする
        auto winSize = Director::getInstance()->getWinSize(); // 画面サイズを取り出す
        newPosition = newPosition.getClampPoint(Vec2(0, 0), Vec2(winSize.width, winSize.height));
        
        pLive2DSprite->moveEye(TOUCH_HOLD, touch->getLocation().x, touch->getLocation().y);
        
        _beatle->setPosition(newPosition);
        
    };
    listner->onTouchEnded = [this](Touch* touch, Event* event) {
        // タッチが離れた時の処理
        log("TouchEnd at (%f, %f)", touch->getLocation().x, touch->getLocation().y);
        this->removeChild(_beatle, true); // _beatleをシーンから外す
        
        pLive2DSprite->moveEye(TOUCH_END, touch->getLocation().x, touch->getLocation().y);
        
        return true;
    };
    diretor->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listner, this);
    
    return true;
}