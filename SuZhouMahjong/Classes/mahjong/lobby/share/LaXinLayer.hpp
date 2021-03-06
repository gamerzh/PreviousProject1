//
//  LaXinLayer.hpp
//  Mahjong
//
//  Created by qiuzhong on 2017/8/15.
//
//

#ifndef LaXinLayer_hpp
#define LaXinLayer_hpp
#include "cocos2d.h"
#include "cocos-ext.h"
#include "mahjong/common/state/GameData.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LaXinLayer : public Layer,public TableViewDelegate, public TableViewDataSource{
    
public:
    bool init() override;
    CREATE_FUNC(LaXinLayer);
    
    void onEnter() override;
    void onExit() override;
    
    TableView* tableView;
    
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    
    
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    
    
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}

private:
    void closeView();
    void shareToFriend();
    void shareToQuan();
    CC_SYNTHESIZE(YongHuTuiGuang, yongHuTuiGuang, YongHuTuiGuang);
};

#endif /* LaXinLayer_hpp */
