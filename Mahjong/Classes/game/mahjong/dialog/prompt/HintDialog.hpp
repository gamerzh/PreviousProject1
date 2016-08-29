//
//  HintDialog.hpp
//  Mahjong
//
//  Created by qiuzhong on 16/6/7.
//
//

#ifndef HintDialog_hpp
#define HintDialog_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

#define CLOSE_HINT_DIALOG "close_hint_dialog"

class HintDialog : public Layer{
public:
    static HintDialog* create(std::string msg,const ccMenuCallback& callback);
    bool init(std::string msg,const ccMenuCallback& callback);
};
#endif /* HintDialog_hpp */
