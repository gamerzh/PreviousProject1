//
//  WanJiaQunLayer.hpp
//  Mahjong
//
//  Created by qiuzhong on 2016/11/9.
//
//

#ifndef ShareToFriendLayer_hpp
#define ShareToFriendLayer_hpp
#include "cocos2d.h"

USING_NS_CC;

class ShareToFriendLayer : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(ShareToFriendLayer);
private:
    MenuItemToggle* qunToggle;
    MenuItemToggle* fangkaToggle;
    Sprite* wanjia;
    Sprite* daili;
    Sprite* image;
    Sprite* content;
    void closeView();
    void showWanJiaQun(Ref* ref);
    void showFangkaDai(Ref* ref);
};
#endif /* WanJiaQunLayer_hpp */
