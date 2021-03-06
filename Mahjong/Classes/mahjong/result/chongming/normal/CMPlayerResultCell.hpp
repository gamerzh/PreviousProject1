//
//  PlayerResultCell.hpp
//  Mahjong
//
//  Created by qiuzhong on 2016/11/21.
//
//

#ifndef CMPlayerResultCell_hpp
#define CMPlayerResultCell_hpp
#include "cocos2d.h"
#include "mahjong/common/state/GameData.h"
USING_NS_CC;

class CMPlayerResultCell : public Sprite{
public:
    static CMPlayerResultCell* create(GameResultData data);
    virtual bool init(GameResultData data);
};

#endif /* PlayerResultCell_hpp */
