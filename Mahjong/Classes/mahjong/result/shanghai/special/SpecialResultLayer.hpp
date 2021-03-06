//
//  PublicResultLayer.hpp
//  Mahjong
//
//  Created by qiuzhong on 2016/11/21.
//
//

#ifndef SpecialResultLayer_hpp
#define SpecialResultLayer_hpp
#include "cocos2d.h"
USING_NS_CC;

class SpecialResultLayer :public  Layer{
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(SpecialResultLayer);
private:
    void showResultTitle();
    void showGameReslut();
    void showLayerBtn();
    void shareResult();
    void gotoLobby();
    void afterCaptured(bool succeed, const std::string &outputFile);
    void update(float dt);
    EventListenerCustom* myCoreLoginRespListener;
    EventListenerCustom* networkBreakListener;

};
#endif /* PublicResultLayer_hpp */
