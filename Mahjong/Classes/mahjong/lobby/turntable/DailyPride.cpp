#include "mahjong/lobby/turntable/DailyPride.h"
#include "mahjong/lobby/turntable/PrideCell.h"
#include "mahjong/common/dialog/prompt/HintDialog.hpp"
#include "mahjong/common/state/GameData.h"
#include "mahjong/common/utils/StringUtil.h"
#include "mahjong/common/widget/ParticleUtil.hpp"
#include "mahjong/common/utils/Chinese.h"
#include "server/NetworkManage.h"
#include "userdata/UserData.h"
#include "wechat/android/CallAndroidMethod.h"
#include "wechat/ios/CallIOSMethod.h"
#include "math.h"


bool DailyPride::init(){
    if (!Layer::init()){
        return false;
    }
    showDailyPrideLayer();
    NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->sendTurntableCommmand());
    return true;
}

void DailyPride::onEnter(){
    Layer::onEnter();
    
    prideCallBackListener1 = EventListenerCustom::create(MSG_PLAYER_TURNTABLE_PRIDE, [=](EventCustom* event){
        updateData();
    });
    _eventDispatcher->addEventListenerWithFixedPriority(prideCallBackListener1, 1);
    
    prideCallBackListener2 = EventListenerCustom::create(MSG_PLAYER_TURNTABLE_PRIDE_RESULT, [=](EventCustom* event){
        if(GAMEDATA::getInstance()->getTodayPrideData().result == "2"){
            HintDialog* hit = HintDialog::create(ChineseWord("dialog_text_11"), nullptr, nullptr);
            addChild(hit,10);
            m_turnBg->stopAllActions();
        }else{
            schedule([=](float dt){
                //刷新用户信息
                NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->getPlayerInfoCommand());
                m_turnBg->stopAllActions();
                TurnTableData data = GAMEDATA::getInstance()->getTurnTableData();
                for (int i = 0; i < data.prides.size(); i++){
                    if( ((PrideCell*)m_turnBg->getChildByTag(100+i))->getPropId() == GAMEDATA::getInstance()->getTurnTablePrideData().pride.type && ((PrideCell*)m_turnBg->getChildByTag(100+i))->getPropNum() ==GAMEDATA::getInstance()->getTurnTablePrideData().pride.number){
                        m_turnBg->setRotation(-90+36*i-18);
                    }
                }
            },0.0f,0,2.0f,"m_turnBg");
        }
    });
    _eventDispatcher->addEventListenerWithFixedPriority(prideCallBackListener2, 1);
}

void DailyPride::onExit(){
    Layer::onExit();
    _eventDispatcher->removeEventListener(prideCallBackListener1);
    _eventDispatcher->removeEventListener(prideCallBackListener2);
    
}

void DailyPride::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
}


void DailyPride::showDailyPrideLayer(){
    
    MenuItem* item1 = MenuItem::create();
    item1->setContentSize(Size(1280, 720));
    Menu* menu1 = Menu::create(item1, NULL);
    this->addChild(menu1);
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 80), 1280, 720);
    addChild(bg);
    
    auto rightGirl = Sprite::create("daily/right_girl.png");
    rightGirl->setPosition(780,380);
    addChild(rightGirl);
    
    
    
    auto close = MenuItemImage::create("common/close_btn_1.png", "common/close_btn_1.png",
                                       CC_CALLBACK_0(DailyPride::closeView, this));
    auto closeMenu = Menu::create(close, NULL);
    closeMenu->setPosition(1050, 357);
    this->addChild(closeMenu);
    
    auto luck_bg = Sprite::create("daily/luck_bg.png");
    luck_bg->setPosition(463, 325);
    addChild(luck_bg);
    
    m_turnBg = Sprite::create("daily/circle_bg.png");
    m_turnBg->setPosition(463, 332);
    addChild(m_turnBg);
    
    TurnTableData data = GAMEDATA::getInstance()->getTurnTableData();
    for (int i = 0; i < data.prides.size(); i++){
        PrideCell* cell = PrideCell::create(data.prides.at(i).type, data.prides.at(i).number);
        cell->setRotation(90-i*36+18);
        cell->setTag(100+i);
        cell->setPosition(getPosByRotation(Point(219,219), 100, i * 36-18));
        m_turnBg->addChild(cell);
    }
    auto itemImage = MenuItemImage::create("daily/go_btn_1.png", "daily/go_btn_2.png",
                                           CC_CALLBACK_1(DailyPride::beginPride, this));
    startMenu = Menu::create(itemImage, NULL);
    startMenu->setPosition(463, 340);
    addChild(startMenu);
    
    auto shareText = Sprite::create("daily/text_info.png");
    shareText->setTag(1000);
    shareText->setPosition(857,270);
    addChild(shareText);
    
    auto shareBtn = MenuItemImage::create("daily/share_pride_bnt_1.png", "daily/share_pride_bnt_2.png",CC_CALLBACK_0(DailyPride::shareTurntable, this));
    auto menu = Menu::create(shareBtn,NULL);
    menu->setTag(1001);
    menu->setPosition(850,180);
    addChild(menu);
    
    auto number = LabelAtlas::create("1", "daily/pride_num_red.png", 32, 46, '0');
    number->setPosition(807,240);
    number->setTag(1002);
    addChild(number);
    auto shareText2 = Sprite::create("daily/chance_text.png");
    shareText2->setPosition(857,240);
    shareText2->setTag(1003);
    addChild(shareText2);

    
    auto shareText3 = Sprite::create("daily/no_chance_text.png");
    shareText3->setPosition(857,240);
    shareText3->setTag(1004);
    addChild(shareText3);

    
    if(GAMEDATA::getInstance()->getTurntableNumber() == "0"){
        shareText->setVisible(true);
        menu->setVisible(true);
        number->setVisible(false);
        shareText2->setVisible(false);
        shareText3->setVisible(false);
    }else if(GAMEDATA::getInstance()->getTurntableNumber() == "1"){
        shareText->setVisible(false);
        menu->setVisible(false);
        number->setVisible(true);
        shareText2->setVisible(true);
        shareText3->setVisible(false);
    }else{
        shareText->setVisible(false);
        menu->setVisible(false);
        number->setVisible(false);
        shareText2->setVisible(false);
        shareText3->setVisible(true);
    }
    scheduleUpdate();
}


void DailyPride::shareTurntable(){
    GAMEDATA::getInstance()->setIsTurnTableShare(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CallAndroidMethod::getInstance()->shareToWeChat(GAMEDATA::getInstance()->getMahjongShareData2().url,GAMEDATA::getInstance()->getMahjongShareData2().head,GAMEDATA::getInstance()->getMahjongShareData2().content,true);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CallIOSMethod::getInstance()->doWechatShareWeb(GAMEDATA::getInstance()->getMahjongShareData2().url,GAMEDATA::getInstance()->getMahjongShareData2().head,GAMEDATA::getInstance()->getMahjongShareData2().content,1);
#endif
}

void DailyPride::updateData(){
    TurnTableData data = GAMEDATA::getInstance()->getTurnTableData();
    for (int i = 0; i < data.prides.size(); i++){
        PrideCell* cell = PrideCell::create(data.prides.at(i).type, data.prides.at(i).number);
        cell->setRotation(90-i*36+18);
        cell->setTag(100+i);
        cell->setPosition(getPosByRotation(Point(219,219), 100, i * 36-18));
        m_turnBg->addChild(cell);
    }
}


void DailyPride::beginPride(Ref* ref){
    
    NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->sendTurntableStartCommand());
    startMenu->setEnabled(false);
    srand(unsigned(time(NULL)));
    float angleZ = rand() % 720 + 720;
    auto pAction = EaseExponentialIn::create(RotateBy::create(2, Vec3(0, 0, angleZ)));
    auto roate = RotateBy::create(2, Vec3(0, 0, angleZ));
    auto repeat = Repeat::create(roate, CC_REPEAT_FOREVER);
    auto sequence = Sequence::create(pAction, repeat,NULL);
    m_turnBg->runAction(sequence);
}


Point DailyPride::getPosByRotation(Point pos, float r, float a){
    return Point(pos.x + cos(CC_DEGREES_TO_RADIANS(a))*r, pos.y + sin(CC_DEGREES_TO_RADIANS(a))*r);
}


void DailyPride::closeView(){
    removeFromParent();
}

void DailyPride::update(float dt){
    if(GAMEDATA::getInstance()->getTurntableNumber() == "0"){
        getChildByTag(1000)->setVisible(true);
        getChildByTag(1001)->setVisible(true);
        getChildByTag(1002)->setVisible(false);
        getChildByTag(1003)->setVisible(false);
        getChildByTag(1004)->setVisible(false);
    }else if(GAMEDATA::getInstance()->getTurntableNumber() == "1"){
        getChildByTag(1000)->setVisible(false);
        getChildByTag(1001)->setVisible(false);
        getChildByTag(1002)->setVisible(true);
        getChildByTag(1003)->setVisible(true);
        getChildByTag(1004)->setVisible(false);
    }else{
        getChildByTag(1000)->setVisible(false);
        getChildByTag(1001)->setVisible(false);
        getChildByTag(1002)->setVisible(false);
        getChildByTag(1003)->setVisible(false);
        getChildByTag(1004)->setVisible(true);
    }
}
