//
//  RikaLive2DSprite.cpp
//  RikaCocos
//
//  Created by Akira Suzuki on 2016/03/05.
//
//

#include "RikaLive2DSprite.h"
#include "base/CCDirector.h"
#include "util/UtSystem.h"
#include "Common.h"
#include "graphics/DrawProfileCocos2D.h"
#include "platform/CCFileUtils.h"

using namespace live2d;
USING_NS_CC;
const char* TEXTURES[] ={
    "rika.2048/texture_00.png" ,
    NULL
} ;
RikaLive2DSprite::RikaLive2DSprite()
: live2DModel(nullptr)
{
}

RikaLive2DSprite::~RikaLive2DSprite()
{
    delete live2DModel;
    // テクスチャを解放
    Director::getInstance()->getTextureCache()->removeTextureForKey(std::string(TEXTURES[0]));
    //	Director::getInstance()->getTextureCache()->removeTextureForKey(std::string(TEXTURES[1]));
    //	Director::getInstance()->getTextureCache()->removeTextureForKey(std::string(TEXTURES[2]));
    
}

bool RikaLive2DSprite::init()
{
    if (DrawNode::init())
    {
        if (live2DModel != nullptr)
        {
            delete live2DModel;
            live2DModel = nullptr;
        }
        
        const char* MODEL = "rika.moc" ;
        
        unsigned char* buf;
        ssize_t bufSize;
        buf = FileUtils::getInstance()->getFileData(MODEL,"rb", &bufSize);
        
        live2DModel = Live2DModelOpenGL::loadModel( buf,bufSize ) ;
        free(buf);
        
        auto chacheMgr = Director::getInstance()->getTextureCache();
        for( int i = 0 ; TEXTURES[i] != NULL ; i++ )
        {
            Texture2D* texture = chacheMgr->addImage(TEXTURES[i]);
            
            Texture2D::TexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
            texture->setTexParameters(texParams);
            texture->generateMipmap();
            
            int glTexNo = texture->getName() ;
            
            live2DModel->setTexture( i , glTexNo ) ;// テクスチャとモデルを結びつける
        }
        
        // Live2Dモデルはローカル座標で左上を原点にして(CanvasWidth、CanvasWidth)
        // のサイズになるため、以下のようにして調整してください。
        displaySizeX = Director::getInstance()->getWinSize().width;
        displaySizeY = Director::getInstance()->getWinSize().height;
        float scx = 2.0 / live2DModel->getCanvasWidth() ;
        float scy = -2.0 / live2DModel->getCanvasWidth() * (displaySizeX/displaySizeY);
        float x = -1 ;
        float y = 1 ;
        float matrix []= {
            scx , 0 , 0 , 0 ,
            0 , scy ,0 , 0 ,
            0 , 0 , 1 , 0 ,
            x , y , 0 , 1
        };
        
        isTouch = false;
        touchPosX = 0.0;
        touchPosY = 0.0;
        
        live2DModel->setMatrix(matrix) ;// 位置を設定
        
        live2DModel->setPremultipliedAlpha( true );
    }
    
    return true;
}

void RikaLive2DSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    DrawNode::draw(renderer, transform, flags);
    
    
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(RikaLive2DSprite::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void RikaLive2DSprite::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
    kmGLPushMatrix();
    kmGLLoadMatrix(&transform);
    
    
    // モデルのパラメータを変更。動作確認用です。
    // 首振り
//    double t = (UtSystem::getUserTimeMSec()/1000.0) * 2 * M_PI  ; // 1秒ごとに2π(1周期)増える
//    double cycle=3.0;// パラメータが一周する時間(秒)
//    double value=sin( t/cycle );// -1から1の間を周期ごとに変化する
//    live2DModel->setParamFloat( "PARAM_ANGLE_X" , (float) (30 * value) ) ; // PARAM_ANGLE_Xのパラメータが[cycle]秒ごとに-30から30まで変化する
  
    // 呼吸
    double breath_t = (UtSystem::getUserTimeMSec()/1000.0) * M_PI  ;// 1秒ごとにπ(半周期)増える
    double breath_cycle = 2.2;// パラメータが一周する時間(秒)
    double breath_value = sin( breath_t/breath_cycle );// 0から1の間を周期ごとに変化する
    live2DModel->setParamFloat( "PARAM_BREATH" , (float) breath_value ) ;// PARAM_ANGLE_Xのパラメータが[cycle]秒ごとに0から1まで変化する
    
    // 目・顔・身体の動き
    float eyeX = live2DModel->getParamFloat("PARAM_EYE_BALL_X");
    float eyeY = live2DModel->getParamFloat("PARAM_EYE_BALL_Y");
    float faceX = live2DModel->getParamFloat("PARAM_ANGLE_X");
    float faceY = live2DModel->getParamFloat("PARAM_ANGLE_Y");
    float bodyX = live2DModel->getParamFloat("PARAM_BODY_ANGLE_X");
    if(isTouch) {
        float eyeSpeed = 0.3f; // 目の速度
        float faceSpeed = 5.0f; // 顔の速度
        float bodySpeed = 1.0f; // 身体の速度
        
        // 目をタッチした方向に向かせる（X軸）
        float valueX =  (touchPosX-displaySizeX/2)/(displaySizeX/2);
        if(eyeX < valueX) { // 目が左を見る時
            eyeX += eyeSpeed;
            if(eyeX > valueX) eyeX = valueX;
        } else if (eyeX > valueX) { // 目が右をる時
            eyeX -= eyeSpeed;
            if(eyeX < valueX) eyeX = valueX;
        }

        // 顔をタッチした方向に向かせる（X軸）
        float f_valueX = valueX * 30;
        if(faceX < f_valueX) { // 顔が左を見る時
            faceX += faceSpeed;
            if(faceX > f_valueX) faceX = f_valueX;
        } else if (faceX > f_valueX) { // 顔が右をる時
            faceX -= faceSpeed;
            if(faceX < f_valueX) faceX = f_valueX;
        }

        // 身体をタッチした方向に向かせる（X軸）
        float b_valueX = valueX * 10;
        if(bodyX < b_valueX) { // 顔が左を見る時
            bodyX += bodySpeed;
            if(bodyX > b_valueX) bodyX = b_valueX;
        } else if (bodyX > b_valueX) { // 顔が右をる時
            bodyX -= bodySpeed;
            if(bodyX < b_valueX) bodyX = b_valueX;
        }

        
        // 目をタッチした方向に向かせる（Y軸）
        float valueY =  (touchPosY-displaySizeY/2)/(displaySizeY/2);
        if(eyeY < valueY) { // 目が左を見る時
            eyeY += eyeSpeed;
            if(eyeY > valueY) eyeY = valueY;
        } else if (eyeY > valueY) { // 目が右をる時
            eyeY -= eyeSpeed;
            if(eyeY < valueY) eyeY = valueY;
        }
        
        // 顔をタッチした方向に向かせる（Y軸）
        float f_valueY = valueY * 30;
        if(faceY < f_valueY) { // 顔が左を見る時
            faceY += faceSpeed;
            if(faceY > f_valueY) faceY = f_valueY;
        } else if (faceY > f_valueY) { // 顔が右をる時
            faceY -= faceSpeed;
            if(faceY < f_valueY) faceY = f_valueY;
        }
        
    } else {
        // タッチがされてない時、目がニュートラル位置ではない場合はニュートラル位置に戻す
        float eyeSpeed = 0.3f; // 目の速度
        float faceSpeed = 3.0f; // 顔の速度
        float bodySpeed = 3.0f; // 身体の速度
        
        // 目の向きをニュートラル位置に設定（X軸）
        if(eyeX < 0) { // 目が左を見ている時
            eyeX += eyeSpeed;
            if(eyeX > 0) eyeX = 0.0f;
        } else if (eyeX > 0) { // 目が右を見ている時
            eyeX -= eyeSpeed;
            if(eyeX < 0) eyeX = 0.0f;
        }
        
        // 顔の向きをニュートラル位置に設定（X軸）
        if(faceX < 0) { // 顔が左を見る時
            faceX += faceSpeed;
            if(faceX > 0) faceX = 0.0f;
        } else if (faceX > 0) { // 顔が右をる時
            faceX -= faceSpeed;
            if(faceX < 0) faceX = 0.0f;
        }
        
        // 身体の向きをニュートラル位置に設定（X軸）
        if(bodyX < 0) { // 顔が左を見る時
            bodyX += bodySpeed;
            if(bodyX > 0) bodyX = 0.0f;
        } else if (bodyX > 0) { // 顔が右をる時
            bodyX -= bodySpeed;
            if(bodyX < 0) bodyX = 0.0f;
        }
        
        // 目の向きをニュートラル位置に設定（Y軸）
        if(eyeY < 0) { // 目が下を見ている時
            eyeY += eyeSpeed;
            if(eyeY > 0) eyeY = 0.0f;
        } else if (eyeY > 0) { // 目が上を見ている時
            eyeY -= eyeSpeed;
            if(eyeY < 0) eyeY = 0.0f;
        }
        
        // 顔の向きをニュートラル位置に設定（Y軸）
        if(faceY < 0) { // 顔が左を見る時
            faceY += faceSpeed;
            if(faceY > 0) faceY = 0.0f;
        } else if (faceY > 0) { // 顔が右をる時
            faceY -= faceSpeed;
            if(faceY < 0) faceY = 0.0f;
        }
   }
    live2DModel->setParamFloat( "PARAM_EYE_BALL_X" , eyeX) ;
    live2DModel->setParamFloat( "PARAM_EYE_BALL_Y" , eyeY) ;
    live2DModel->setParamFloat( "PARAM_ANGLE_X" , faceX) ;
    live2DModel->setParamFloat( "PARAM_ANGLE_Y" , faceY) ;
    live2DModel->setParamFloat( "PARAM_BODY_ANGLE_X" , bodyX) ;
    

    
    // Live2Dの描画前と描画後に以下の関数を呼んでください
    // live2d::DrawProfileCocos2D::preDraw() ;
    // live2d::DrawProfileCocos2D::postDraw() ;
    // これはOpenGLの状態をもとに戻すための処理です。
    // これを行わない場合、Cocos2DかLive2Dどちらかで状態の不整合が起こります。
    live2d::DrawProfileCocos2D::preDraw();
    
    live2DModel->update() ;
    live2DModel->draw() ;
    
    live2d::DrawProfileCocos2D::postDraw() ;
    
    kmGLPopMatrix();
}

void RikaLive2DSprite::moveEye(enum OpeType opeType, float tX, float tY) {
    
    //live2DModel->setParamFloat( "PARAM_EYE_BALL_X" , (float) 1.0) ;
    touchPosX = tX;
    touchPosY = tY;
    
    switch (opeType) {
        case TOUCH_START:
        
        case TOUCH_HOLD:
            isTouch = true;
            break;
        
        case TOUCH_END:
            isTouch = false;
            break;
        
        default:
            isTouch = false;
            break;
    }
}

RikaLive2DSprite* RikaLive2DSprite::createDrawNode()
{
    RikaLive2DSprite *ret = new RikaLive2DSprite();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    
}

