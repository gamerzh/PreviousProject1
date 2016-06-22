//
//  LingHongbao.cpp
//  Mahjong
//
//  Created by qiuzhong on 16/4/21.
//
//

#include "LingHongbao.hpp"
#include "NetworkManage.h"
#include "GameData.h"
#include "Loading.h"

bool LingHongbao::init(){
    if(!Layer::init()){
        
        return false;
    }
    
    showLingHongbao();
    return true;
    
}


void LingHongbao::onEnter(){
    Layer::onEnter();
    redWalletReciveListener = EventListenerCustom::create(MSG_RECIVE_RED_WALLET_INFO, [=](EventCustom* event){
        if(NULL != getChildByTag(1000)){
            getChildByTag(1000)->removeFromParent();
        }
        //TODO 动画
    });
    _eventDispatcher->addEventListenerWithFixedPriority(redWalletReciveListener, 1);
}

void LingHongbao::onExit(){
    Layer::onExit();
    _eventDispatcher->removeEventListener(redWalletReciveListener);
}

void LingHongbao::showLingHongbao(){
    MenuItem* item1 = MenuItem::create();
    item1->setContentSize(Size(1280, 720));
    Menu* menu0 = Menu::create(item1, NULL);
    this->addChild(menu0);
    
    auto all_bg = Sprite::create("common/all_bg.jpg");
    all_bg->setPosition(640, 360);
    addChild(all_bg);
    
    auto  dialogBg = Sprite::create("shop/shop_bg_2.png");
    dialogBg->setPosition(640, 360);
    this->addChild(dialogBg);
    
    auto closeImage = MenuItemImage::create("common/close_btn_1.png", "common/close_btn_1.png", CC_CALLBACK_0(LingHongbao::closeView, this));
    auto closeMenu = Menu::create(closeImage, NULL);
    closeMenu->setPosition(980, 580);
    addChild(closeMenu);
    
    
    auto title = Sprite::create("shop/linhongbao.png");
    title->setPosition(640,610);
    addChild(title);
    
    auto text = Sprite::create("shop/shurukoulin.png");
    text->setPosition(640,480);
    addChild(text);
    
    auto inputBg = Scale9Sprite::create("common/input_box_bg.png");
    inputBg->setContentSize(Size(450, 81));
    inputBg->setPosition(640, 360);
    addChild(inputBg);
    _editPwd  = EditBox::create(Size(445, 81), Scale9Sprite::create());
    _editPwd->setPosition(Point(720, 360));
    _editPwd->setFont("American Typewriter", 24);
    _editPwd->setInputMode(EditBox::InputMode::PHONE_NUMBER);
    _editPwd->setReturnType(EditBox::KeyboardReturnType::DONE);
    _editPwd->setTag(1);
    addChild(_editPwd);
    
    
    auto btnImage = MenuItemImage::create("shop/linqujiangquan_1.png","shop/linqujiangquan_2.png",CC_CALLBACK_0(LingHongbao::confirmHongbao, this));
    auto menu1= Menu::create(btnImage,NULL);
    menu1->setPosition(640,220);
    addChild(menu1);
    
    
}



void  LingHongbao::closeView(){
    removeFromParent();
}

void LingHongbao::confirmHongbao(){
    std::string text = _editPwd->getText();
    if(text != ""){
        NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->getReciveRedWalletCommand(text));
        Loading* lo = Loading::create();
        lo->setTag(1000);
        addChild(lo);
    }
}
