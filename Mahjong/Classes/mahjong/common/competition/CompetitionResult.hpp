//
//  CompetitionResult.hpp
//  Mahjong
//
//  Created by qiuzhong on 2017/5/22.
//
//

#ifndef CompetitionResult_hpp
#define CompetitionResult_hpp

#include "cocos2d.h"
#include "mahjong/common/state/GameData.h"
USING_NS_CC;


class CompetitionResult : public Layer{
public:
    virtual bool init();
    void showCompetiotionResult(std::string type,std::string rank,std::string pride,std::string score);
    CREATE_FUNC(CompetitionResult);
private:
    void share();
    void quit();
    void continueCompetition();
    void afterCaptured(bool succeed, const std::string &outputFile);

};

#endif /* CompetitionResult_hpp */
