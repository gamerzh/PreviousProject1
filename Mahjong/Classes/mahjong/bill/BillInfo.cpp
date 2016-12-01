#include "mahjong/bill/BillInfo.h"
#include "mahjong/bill/BillDetailInfo.h"
#include "mahjong/loading/Loading.h"
#include "mahjong/utils/StringUtil.h"
#include "mahjong/utils/Chinese.h"
#include "mahjong/dialog/prompt/HintDialog.hpp"
#include "server/NetworkManage.h"
#include "userdata/UserData.h"
#include "payment/android/CallAndroidMethod.h"
#import "payment/ios/IOSBridge.h"

bool BillInfo::init()
{
    if (!Layer::init())
    {
        return false;
    }
    MenuItem* item = MenuItem::create();
    item->setContentSize(Size(1280, 720));
    Menu* bg = Menu::create(item, NULL);
    this->addChild(bg,-1);
    
    auto  dialogBg = Sprite::create("friend/dialog_bg.png");
    dialogBg->setTag(101);
    dialogBg->setPosition(640, 360);
    this->addChild(dialogBg);
    
    auto closeImage = MenuItemImage::create("common/close_btn_1.png", "common/close_btn_1.png", CC_CALLBACK_0(BillInfo::closeView, this));
    auto closeMenu = Menu::create(closeImage, NULL);
    closeMenu->setPosition(980, 655);
    closeMenu->setTag(102);
    addChild(closeMenu);
    
    auto icon = Sprite::create("bill/bill_title.png");
    icon->setPosition(640, 668);
    icon->setTag(104);
    addChild(icon);
    
    tableView = TableView::create(this, Size(722, 550));
    tableView->setAnchorPoint(Point::ANCHOR_MIDDLE);
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    tableView->setPosition(275, 60);
    tableView->setTag(105);
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    this->addChild(tableView);
    tableView->reloadData();
    
    auto xuanyao = MenuItemImage::create("bill/share_bill_1.png","bill/share_bill_2.png",CC_CALLBACK_0(BillInfo::screenShot, this));
    auto shareBtn = Menu::create(xuanyao,NULL);
    shareBtn->setTag(2016);
    shareBtn->setPosition(360,625);
    addChild(shareBtn);
    
    if(GAMEDATA::getInstance()->getBillInfoAll().needInit){
        Loading* load =Loading::create(true);
        load->setTag(1000);
        addChild(load);
    }else{
        showKongBill();
    }
    return true;
}

void BillInfo::onEnter(){
    Layer::onEnter();
    //刷新账单信息
    playerBillListener = EventListenerCustom::create(MSG_PLAYER_BILL, [=](EventCustom* event){
        updateBillInfo();
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(playerBillListener, 1);
}

void BillInfo::onExit(){
    Layer::onExit();
    _eventDispatcher->removeEventListener(playerBillListener);
}



void BillInfo::tableCellTouched(TableView* table, TableViewCell* cell)
{
    log("cell touched at index: %ld", cell->getIdx());
    BillInfoAll info = GAMEDATA::getInstance()->getBillInfoAll();
    string bId =cell->getName();
    NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->getBillDetailCommand(bId));
    for(int i=0;i<info.bills.size();i++){
        if(info.bills.at(i).billId == bId){
            GAMEDATA::getInstance()->setBillInfoData(info.bills.at(i));
        }
    }
    BillDetailInfo* detail =BillDetailInfo::create();
    if(getIsPrivateBill()){
        detail->setPrivatePostion();
    }
    addChild(detail);
}

Size BillInfo::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(722, 152);
}

TableViewCell* BillInfo::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto string = StringUtils::format("%ld", idx);
    BillInfoAll info = GAMEDATA::getInstance()->getBillInfoAll();
    
    BillInfoData data = info.bills.at(idx);
    
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new (std::nothrow) TableViewCell();
        cell->autorelease();
        auto sprite1 = Sprite::create("bill/bill_bg.png");
        sprite1->setAnchorPoint(Vec2::ZERO);
        sprite1->setPosition(Vec2(0, 0));
        cell->addChild(sprite1);
        
        Label* date = Label::createWithSystemFont(data.date,"Arial",22);
        date->setTag(100);
        date->setColor(Color3B(21,50,91));
        date->setAnchorPoint(Vec2::ZERO);
        date->setPosition(Vec2(18, 115));
        cell->addChild(date);
        
        Label* fanghao = Label::createWithSystemFont("房号:","Arial",22);
        fanghao->setColor(Color3B(21,50,91));
        fanghao->setAnchorPoint(Vec2::ZERO);
        fanghao->setPosition(Vec2(240, 115));
        cell->addChild(fanghao);
        
        
        Label* prID = Label::createWithSystemFont(data.prid,"Arial",22);
        prID->setTag(400);
        prID->setColor(Color3B(21,50,91));
        prID->setAnchorPoint(Vec2::ZERO);
        prID->setPosition(Vec2(290, 115));
        cell->addChild(prID);
        
        Label* ju = Label::createWithSystemFont("局数:","Arial",22);
        ju->setColor(Color3B(21,50,91));
        ju->setAnchorPoint(Vec2::ZERO);
        ju->setPosition(Vec2(400, 115));
        cell->addChild(ju);
        
        Label* jushu = Label::createWithSystemFont(data.atype == "0" ? "8": "16","Arial",22);
        jushu->setTag(401);
        jushu->setColor(Color3B(21,50,91));
        jushu->setAnchorPoint(Vec2::ZERO);
        jushu->setPosition(Vec2(450, 115));
        cell->addChild(jushu);
        
        std::vector<BillContent> conBill = sortBillInfo(data.content);
        for (int i = 0; i < conBill.size(); i++){
            Label* name = Label::createWithSystemFont(conBill.at(i).nickName, "Arial", 24);
            name->setTag(200+i);
            name->setAnchorPoint(Point::ANCHOR_MIDDLE);
            name->setPosition(Vec2(85+185*i, 80));
            if(conBill.at(i).nickName == UserData::getInstance()->getNickName()){
                name->setColor(Color3B(91,220,168));
            }else{
                name->setColor(Color3B(38,158,228));
            }
            cell->addChild(name);
            
            std::string imageName ="bill/yellow_num.png";
            int score =atoi(conBill.at(i).score.c_str());
            if(score<0){
                imageName="bill/purper_num.png";
            }
            std::string myScore =  ":"+StringUtil::itos(abs(score));
            
            LabelAtlas* playNum = LabelAtlas::create(myScore,imageName,20,30,'0');
            playNum->setAnchorPoint(Point::ANCHOR_MIDDLE);
            playNum->setPosition(Vec2(85 + 185 * i, 30));
            playNum->setTag(300+i);
            cell->addChild(playNum);
        }
        
        MenuItemImage* detailImage = MenuItemImage::create("bill/arrows.png","bill/arrows.png",                                                           CC_CALLBACK_1(BillInfo::showDetailInfo, this));
        detailImage->setTag(idx);
        Menu* myMenu = Menu::create(detailImage,NULL);
        myMenu->setPosition(690,125);
        cell->addChild(myMenu);
        cell->setName(data.billId);
    }else{
        ((Label*)cell->getChildByTag(100))->setString(data.date);
        ((Label*)cell->getChildByTag(400))->setString(data.prid);
        ((Label*)cell->getChildByTag(401))->setString(data.atype == "0" ? "8": "16");
        std::vector<BillContent> conBill = sortBillInfo(data.content);
        for (int i = 0; i < conBill.size(); i++){
            ((Label*)cell->getChildByTag(200+i))->setString(conBill.at(i).nickName);
            int score =atoi(conBill.at(i).score.c_str());
            std::string myScore =  ":"+StringUtil::itos(abs(score));
            if(score<0){
                Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("bill/purper_num.png");
                ((LabelAtlas*)cell->getChildByTag(300+i))->setTexture(texture);
            }else{
                Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("bill/yellow_num.png");
                ((LabelAtlas*)cell->getChildByTag(300+i))->setTexture(texture);
            }
            ((LabelAtlas*)cell->getChildByTag(300+i))->setString(myScore);
        }
        cell->setName(data.billId);
    }
    return cell;
}

ssize_t BillInfo::numberOfCellsInTableView(TableView *table)
{
    BillInfoAll info = GAMEDATA::getInstance()->getBillInfoAll();
    return info.bills.size();
}


void BillInfo::showDetailInfo(Ref* ref){
    //    MenuItemImage* image = (MenuItemImage*)ref;
    //    BillInfoAll info = GAMEDATA::getInstance()->getBillInfoAll();
}

void BillInfo::closeView(){
    this->removeFromParent();
}


void BillInfo::updateBillInfo(){
    if(NULL != getChildByTag(1000)){
        getChildByTag(1000)->removeFromParent();
    }
    showKongBill();
    tableView->reloadData();
}


void BillInfo::showKongBill(){
    if(GAMEDATA::getInstance()->getBillInfoAll().bills.size()==0){
        if(getChildByTag(1001)==NULL){
            Label* lalala = Label::createWithSystemFont(ChineseWord("kongzhangdan"),"arial",20);
            lalala -> setTag(1001);
            if(getIsPrivateBill()){
                lalala->setPosition(900,360);
            }else{
                lalala->setPosition(640,360);
            }
            addChild(lalala);
        }
    }else{
        if(NULL != getChildByTag(1001)){
            getChildByTag(1001)->removeFromParent();
        }
        if(NULL != getChildByTag(2016)){
            getChildByTag(2016)->setVisible(true);
        }
    }
}

std::vector<BillContent> BillInfo::sortBillInfo(std::vector<BillContent> content){
    for (int i=1; i<content.size(); i++) {
        if(content.at(i).nickName == UserData::getInstance()->getNickName()){
            swap(content.at(i), content.at(0));
        }
    }
    return content;
}

void BillInfo::setShowPosition(){
    setIsPrivateBill(true);
    getChildByTag(101)->setPositionX(890);
    getChildByTag(102)->setPositionX(1230);
    getChildByTag(104)->setPositionX(890);
    getChildByTag(105)->setPositionX(525);
    getChildByTag(2016)->setPositionX(605);
    if(NULL != getChildByTag(1001))
        getChildByTag(1001)->setPositionX(900);
}


void BillInfo::screenShot(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string path =StringUtils::format("%s/mahjong_screen_shot.png",CallAndroidMethod::getInstance()->getSdCardDir().c_str());
    log("screenShot path = %s",path.c_str());
    utils::captureScreen(CC_CALLBACK_2(BillInfo::afterCaptured, this) ,path);
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::string path =StringUtils::format("%smahjong_screen_shot.png",FileUtils::sharedFileUtils()->getWritablePath().c_str());
    log("screenShot path = %s",path.c_str());
    utils::captureScreen(CC_CALLBACK_2(BillInfo::afterCaptured, this) ,path);
#endif
}

void BillInfo::afterCaptured(bool succeed, const std::string &outputFile)
{
    if (succeed) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        CallAndroidMethod::getInstance()->shareImageToWeChat("mahjong_screen_shot.png", false);
#endif
        
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        IOSBridge::getInstance()->doWechatShareImg(outputFile, 0);
#endif
    }
}