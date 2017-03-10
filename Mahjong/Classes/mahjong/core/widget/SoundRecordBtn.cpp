//
//  SoundRecordBtn.cpp
//  Mahjong
//
//  Created by qiuzhong on 2017/3/9.
//
//

#include "mahjong/core/widget/SoundRecordBtn.hpp"
#include "mahjong/chat/GameRecordAudioManage.h"

bool SoundRecordBtn::init(){
    if(!Layer::init()){
        return false;
    }
    
    playBtn = Sprite::create("gameview/chat_sound_1.png");
    playBtn->setTag(1888);
    playBtn->setPosition(Vec2(80, 50));
    addChild(playBtn);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(SoundRecordBtn::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(SoundRecordBtn::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(SoundRecordBtn::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);
    
    scheduleUpdate();
    return true;
}

bool SoundRecordBtn::onTouchBegan(Touch *touch, Event  *event){
    if(statProtected)
        return true;
    if(playBtn->getBoundingBox().containsPoint(touch->getLocation())){
        playBtn->setTexture("gameview/chat_sound_2.png");
        auto soudn = Sprite::create("gameview/record_sound_ing.png");
        soudn->setPosition(640,320);
        soudn->setTag(1789);
        addChild(soudn,5);
        GameAudioManage::getInstance()->beginRecordAudio();
        startRecord = true;
    }
    return true;
}

void SoundRecordBtn::onTouchMoved(Touch *touch, Event  *event){
    
    if(statProtected)
        return;
}

void SoundRecordBtn::onTouchEnded(Touch *touch, Event  *event){
    if(statProtected)
        return;
    playBtn->setTexture("gameview/chat_sound_1.png");
    if(startRecord){
        GameAudioManage::getInstance()->endRecordAudio();
        if(NULL != getChildByTag(1789)){
            getChildByTag(1789)->removeFromParent();
        }
        statProtected = true;
        protectedTime =0;
        startRecord = false;
    }
}

void SoundRecordBtn::update(float dt){
    if(statProtected){
        protectedTime+=dt;
        if(protectedTime>=1.8){
            statProtected = false;
        }
    }

}