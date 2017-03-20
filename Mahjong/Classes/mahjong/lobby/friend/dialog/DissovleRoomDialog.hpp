//
//  HintDialog.hpp
//  Mahjong
//
//  Created by qiuzhong on 16/6/7.
//
//

#ifndef DissovleRoomDialog_hpp
#define DissovleRoomDialog_hpp

#include "cocos2d.h"
USING_NS_CC;

#define CLOSE_HINT_DIALOG "close_hint_dialog"

class DissovleRoomDialog : public Layer{
public:
    bool init();
    CREATE_FUNC(DissovleRoomDialog);
    void setNickName(std::string msg);
private:
    Label* nickNameText;
    void agree();
    void disAgree();
    void closeView();
};
#endif /* HintDialog_hpp */