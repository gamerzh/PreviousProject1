//
//  SZPlayerResultCell.cpp
//  Mahjong
//
//  Created by qiuzhong on 2016/11/21.
//
//

#include "mahjong/result/suzhou/normal/SZPlayerResultCell.hpp"
#include "mahjong/common/widget/HeadImage.hpp"
#include "mahjong/common/utils/Chinese.h"
#include "mahjong/common/utils/StringUtil.h"
#include "mahjong/common/utils/SeatIdUtil.h"
#include "userdata/UserData.h"

SZPlayerResultCell* SZPlayerResultCell::create(GameResultData data){
    SZPlayerResultCell* cell = new SZPlayerResultCell();
    if(cell && cell->init(data)){
        cell -> autorelease();
        return cell;
    }
    CC_SAFE_DELETE(cell);
    return NULL;
}

bool SZPlayerResultCell::init(GameResultData data){
    if(!Sprite::init()){
        return false;
    }
    //result0为平,1为自摸,2为放炮 3胡牌  4输
    auto spritebg = Sprite::create();
    addChild(spritebg);
    
    auto headImage = HeadImage::createByImage(data.pic,Size(90,90));
    headImage->setPosition(-470, 0);
    addChild(headImage, 10);
    
    auto nickName = Label::createWithSystemFont(data.nickName,"Arial",20);
    nickName->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    nickName->setPosition(-405,20);
    addChild(nickName);
    
    auto idNumber = Label::createWithSystemFont(StringUtils::format("ID:%s",data.umark.c_str()),"Arial",20);
    idNumber->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    idNumber->setPosition(-230,20);
    addChild(idNumber);
    
   
    
    if(GAMEDATA::getInstance()->getCurrentBank() ==  data.seatId){
        auto bank = Sprite::create("result/zhaung.png");
        bank->setPosition(-580,0);
        addChild(bank);
    }
    
    auto resultNum = LabelAtlas::create(StringUtils::format("%s","0"), "result/game_result_win_num.png", 37, 53, '0');
    addChild(resultNum);
    
    std::vector<std::string> showPokers = StringUtil::split(data.showPoker, ",");
    bool win = false;
    if(data.result == 1||data.result == 3){
        win = true;
        spritebg->setTexture("result/jie_suan_lan_suc.png");
        nickName->setColor(Color3B(230,215,30));
        idNumber->setColor(Color3B(230,215,30));
        Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("result/game_result_win_num.png");
        resultNum->setTexture(texture);
        if(GAMEDATA::getInstance()->getMahjongRoomType() == MahjongRoom::privateRoom){
            resultNum->setString(StringUtils::format(":%d",data.jifendelta));
        }else{
            resultNum->setString(StringUtils::format(":%d",data.golddelta));
        }
        
//        auto huaIcon = Sprite::create("gameview/flower.png");
//        huaIcon->setScale(0.7f);
//        huaIcon->setPosition(-48, 28);
//        addChild(huaIcon);
//        auto hua = Label::createWithSystemFont(StringUtils::format("X%d",data.hua),"Arial",20);
//        hua->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
//        hua->setColor(Color3B(230,215,30));
//        hua->setPosition(-30,15);
//        addChild(hua);
        
        if(data.tip != ""){
            auto huTip = Label::createWithSystemFont(data.tip, "Arial",20);
            huTip->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            huTip->setColor(Color3B(230,215,30));
            huTip->setPosition(30,15);
            addChild(huTip);
        }
        //胡的玩家需要单独排序
        vector<std::string>::iterator itor;
        for(itor=showPokers.begin();itor!=showPokers.end();)
        {
            if(GAMEDATA::getInstance()->getDiaopao()==*itor)
            {
                itor=showPokers.erase(itor);
                break;
            }
            else
            {
                itor++;
            }
        }
        if(GAMEDATA::getInstance()->getDiaopao()!= ""){
            showPokers.push_back(GAMEDATA::getInstance()->getDiaopao());
        }
        auto chuchong = Sprite::create("gameview/font_hu.png");
        chuchong->setScale(0.4f);
        chuchong->setPosition(500,-10);
        addChild(chuchong);
        
    }else{
        win = false;
        spritebg->setTexture("result/jie_suan_lan_fai.png");
        Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("result/game_result_lose_num.png");
        resultNum->setTexture(texture);
        if(GAMEDATA::getInstance()->getMahjongRoomType() == MahjongRoom::privateRoom){
            resultNum->setString(StringUtils::format(":%d",abs(data.jifendelta)));
        }else{
            resultNum->setString(StringUtils::format(":%d",abs(data.golddelta)));
        }
        if(data.result == 2){
            auto chuchong = Sprite::create("result/chuchong.png");
            chuchong->setPosition(485,-10);
            addChild(chuchong);
        }
    }
    //在展示的牌前加上吃碰杠的显示
    std::vector<std::vector<std::string>> finalShowPoker;
    PlayerCpgRecShow recShow = GAMEDATA::getInstance()->getPlayerCpgRecShow();
    int clientId =  SeatIdUtil::getClientSeatId(GAMEDATA::getInstance()->getHeroSeatId(), data.seatId);
    for(auto var:recShow.playercpg){
        if(var.clientseatid == clientId){
            for(auto pokers: var.cpg){
                std::vector<std::string> playerShowPoker;
                for(int i=0; i<pokers.size();i++){
                    //强行限制杠牌会画出5张牌的情况
                    if(i<4){
                        playerShowPoker.push_back(StringUtils::format("%d",pokers.at(i)));
                    }
                }
                sort(playerShowPoker.begin(), playerShowPoker.end());
                finalShowPoker.push_back(playerShowPoker);
            }
        }
    }
    finalShowPoker.push_back(showPokers);
    int pokerNum =0;
    for(int i=0;i<finalShowPoker.size();i++){
        for (int j = 0; j < finalShowPoker.at(i).size(); j++){
            Jong* jong = Jong::create();
            jong->showJong(heroplayed, atoi(finalShowPoker.at(i).at(j).c_str()));
            if(i == finalShowPoker.size()-1){
                if(j == finalShowPoker.at(i).size()-1&&win){
                    jong->setPosition(-400+pokerNum * 33+22+4*i, -20);
                }else{
                    jong->setPosition(-400+pokerNum * 33+4*i+14, -20);
                }
            }else{
                jong->setPosition(-400+pokerNum * 33+4*i, -20);
            }
            addChild(jong, 35 - i);
            pokerNum++;
        }
    }
    if(GAMEDATA::getInstance()->getMahjongRoomType() != MahjongRoom::privateRoom){
        
        auto goldIcon =  Sprite::create("mjlobby/gold_icon.png");
        goldIcon->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        goldIcon->setPosition(210,-10);
        addChild(goldIcon);
        resultNum->setPosition(270,-10);
        resultNum->setScale(0.7f);
        resultNum->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        if(data.lequandelta>0){
            goldIcon->setPositionY(20);
            resultNum->setPositionY(20);
            auto lequanIcon =  Sprite::create("mjlobby/lequan_icon.png");
            lequanIcon->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            lequanIcon->setPosition(210,-25);
            addChild(lequanIcon);
            auto lequanNum = LabelAtlas::create(StringUtils::format(":%d",data.lequandelta), "result/game_result_win_num.png", 37, 53, '0');
            lequanNum->setScale(0.7f);
            lequanNum->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
            lequanNum->setPosition(270,-25);
            addChild(lequanNum);
        }
    }else{
        
        auto jifenIcon =  Sprite::create("common/jifen_icon.png");
        jifenIcon->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        jifenIcon->setPosition(220,-10);
        addChild(jifenIcon);
        
        resultNum->setScale(1.0f);
        resultNum->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        resultNum->setPosition(290,-10);
    }
    
    return true;
}
