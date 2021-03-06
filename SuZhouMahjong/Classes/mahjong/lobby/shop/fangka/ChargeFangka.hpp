//
//  ChargeDiamond.hpp
//  Mahjong
//
//  Created by qiuzhong on 16/4/18.
//
//

#ifndef ChargeFangka_hpp
#define ChargeFangka_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class ChargeFangka:public Layer{

public :
    virtual bool init();
    void onEnter() override;
    void onExit() override;
    CREATE_FUNC(ChargeFangka);
private:
    void showChargeDialog();
    void closeView();
    void copyText();
    void toast();

};
#endif /* ChargeDiamond_hpp */
