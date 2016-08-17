//
//  ChatDialog.hpp
//  Mahjong
//
//  Created by qiuzhong on 16/8/11.
//
//

#ifndef ChatDialog_hpp
#define ChatDialog_hpp
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/UIListView.h"
#include "ui/UIImageView.h"
#include "ui/UIRichText.h"
#include "game/mahjong/state/GameData.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class ChatDialog : public Layer{
public:
    virtual bool init(std::string poxiaoId);
    static ChatDialog* create(std::string poxiaoId);
    void onEnter() override;
    void onExit() override;
    
private:
    ListView* listView;
    EventListenerCustom* roomChatListener;
    void closeView();
    void showChatList(std::string poxiaoId);
    void showChatInfo(ChatData data);
    void showQuickChatList();
    void showFaceList();
    void sendMessage();
    void testData();
};
#endif /* ChatDialog_hpp */
