//
//  OtherPlayerInfo.cpp
//  Mahjong
//
//  Created by qiuzhong on 16/5/18.
//
//

#include "game/mahjong/heroinfo/OtherPlayerInfo.hpp"
#include "game/mahjong/dialog/prompt/HintDialog.hpp"
#include "game/mahjong/widget/HeadImage.hpp"
#include "game/mahjong/state/GameData.h"
#include "game/utils/Chinese.h"
#include "game/utils/SeatIdUtil.h"
#include "userdata/UserData.h"
#include "server/NetworkManage.h"

OtherPlayerInfo* OtherPlayerInfo::create(Player* player){
    OtherPlayerInfo* bRet = new OtherPlayerInfo();
    if (bRet&&bRet->init(player))
    {
        bRet->autorelease();
        return bRet;
    }
    else
    {
        delete bRet;
        bRet = NULL;
        return NULL;
    }
}

void OtherPlayerInfo::onEnter(){
    Layer::onEnter();
    addFriendRespListener2 = EventListenerCustom::create(MSG_ADD_FRIEND_RESP, [=](EventCustom* event){
        char* buf = static_cast<char*>(event->getUserData());
        std::string result = buf;
        if (result == "1"){
            NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->getFriendListCommand());
            HintDialog* hint = HintDialog::create("添加好友成功",NULL);
            addChild(hint);
        }else{
            HintDialog* hint = HintDialog::create("添加好友失败",NULL);
            addChild(hint);
        }
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(addFriendRespListener2, 1);
}

void OtherPlayerInfo::onExit(){
    Layer::onExit();
    Director::getInstance()->getEventDispatcher()->removeEventListener(addFriendRespListener2);
}



bool OtherPlayerInfo::init(Player* player){
    if(!Layer::init()){
        return false;
    }
    setPlayer(player);
    MenuItem* item1 = MenuItem::create();
    item1->setContentSize(Size(1280, 720));
    item1->setCallback(CC_CALLBACK_0(OtherPlayerInfo::closeView, this));
    Menu* menu1 = Menu::create(item1, NULL);
    addChild(menu1);
    auto  dialogBg = Sprite::create("gameview/other_player_bg.png");
    int cSeatId = SeatIdUtil::getClientSeatId(GAMEDATA::getInstance()->getHeroSeatId(), player->getSeatId());
    if(cSeatId == ClientSeatId::left){
        dialogBg->setPosition(265, 455);
    }else if(cSeatId == ClientSeatId::opposite){
        dialogBg->setPosition(745, 605);
    }else if(cSeatId == ClientSeatId::right){
        dialogBg->setPosition(1015, 455);
    }else{
        removeFromParent();
        return true;
    }
    addChild(dialogBg);
    
    
    auto headBg = HeadImage::createByImage(player->getPicture(), Size(90,90));
    headBg->setPosition(70,160);
    dialogBg->addChild(headBg);
    

    nickNameLabel = Label::create(player->getNickname(), "arial", 20);
    nickNameLabel->setColor(Color3B(87, 195, 186));
    nickNameLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    nickNameLabel->setPosition(130, 190);
    dialogBg->addChild(nickNameLabel);
    
    
    std::string gen = player->getGender() == 0 ? "playerinfo/female.png" : "playerinfo/male.png";
    playerGender = Sprite::create(gen);
    playerGender->setPosition(140, 140);
    playerGender->setScale(0.6f);
    dialogBg->addChild(playerGender);

    
    //diamond
    auto diamond_bg = Sprite::create("gameview/other_player_num_bg.png");
    diamond_bg->setPosition(90, 80);
    dialogBg->addChild(diamond_bg);
    auto diamond_icon = Sprite::create("gameview/other_player_diamond.png");
    diamond_icon->setPosition(50, 75);
    dialogBg->addChild(diamond_icon);
    auto diamondNum = Label::createWithSystemFont(StringUtils::format("%d", player->getDiamond()),"arial",20);
    diamondNum->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    diamondNum->setColor(Color3B(87, 195, 186));
    diamondNum->setPosition(75, 80);
    dialogBg->addChild(diamondNum);
    
//    //lock diamond
    auto lock_diamond_bg = Sprite::create("gameview/other_player_num_bg.png");
    lock_diamond_bg->setPosition(90, 40);
    dialogBg->addChild(lock_diamond_bg);
    auto lock_diamond_icon = Sprite::create("gameview/other_player_lockdiamond.png");
    lock_diamond_icon->setPosition(50, 35);
    dialogBg->addChild(lock_diamond_icon);
    auto lockDiamondNum = Label::createWithSystemFont(StringUtils::format("%d", player->getLockDiamond()),
                                             "arial",20);
    lockDiamondNum->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    lockDiamondNum->setColor(Color3B(87, 195, 186));
    lockDiamondNum->setPosition(75, 40);
    dialogBg->addChild(lockDiamondNum);
    //gold
    auto gold_bg = Sprite::create("gameview/other_player_num_bg.png");
    gold_bg->setPosition(220, 80);
    dialogBg->addChild(gold_bg);
    auto gold_icon = Sprite::create("gameview/other_player_gold.png");
    gold_icon->setPosition(180, 75);
    dialogBg->addChild(gold_icon);
    auto goldNum = Label::createWithSystemFont(StringUtils::format("%d", player->getGold()),"arial",20);
    goldNum->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    goldNum->setColor(Color3B(87, 195, 186));
    goldNum->setPosition(205, 80);
    dialogBg->addChild(goldNum);
    //lequan
    auto lequan_bg = Sprite::create("gameview/other_player_num_bg.png");
    lequan_bg->setPosition(220, 40);
    dialogBg->addChild(lequan_bg);
    auto lequan_icon = Sprite::create("gameview/other_player_lequan.png");
    lequan_icon->setPosition(180, 35);
    dialogBg->addChild(lequan_icon);
    auto lequanNum = Label::create(cocos2d::String::createWithFormat("%d", player->getTicket())->_string,"arial",20);
    lequanNum->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    lequanNum->setColor(Color3B(87, 195, 186));
    lequanNum->setPosition(205, 40);
    dialogBg->addChild(lequanNum);

    return true;
}

void OtherPlayerInfo::closeView(){
    
    removeFromParent();
}


void OtherPlayerInfo::addFriend(Ref* ref){
    MenuItemImage* temp = (MenuItemImage*)ref;
    temp->setVisible(false);
    NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->getAddFriendCommand(getPlayer()->getPoxiaoId(),getPlayer()->getNickname()));
}