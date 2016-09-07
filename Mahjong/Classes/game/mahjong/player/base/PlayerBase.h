#ifndef __PLAYER_BASE__
#define __PLAYER_BASE__
#include "cocos2d.h"
#include "game/mahjong/player/playerinfo/Player.h"
#include "game/mahjong/jong/Jong.h"
#include "game/mahjong/state/GameData.h"
#include "game/utils/StringUtil.h"
#include "game/utils/SeatIdUtil.h"
#include "game/utils/Audio.h"

USING_NS_CC;
using namespace cocos2d::ui;

class PlayerBase :public Layer{
public:
    static	const int jongNum = 13;//基准手牌数量
    static Sprite* biaoji;//每次打出的黄色小标记
    static Sprite* currentBigJongBg;//打出的牌展示背景
    static Jong* currentBigJong;//打出的牌放大展示牌
    static set<int> playedPokers;//全部玩家打出过的牌
    
    int huaIndex;//换花计数
    
    Vector<Jong*> playerHandJongs;//玩家手牌
    Vector<Jong*> playerPlayedJongs;//玩家出的牌
    vector<PlayerCpgRecord>  playerCpgRecords;//玩家吃碰杠记录
    
    virtual bool init();
    virtual void removeLastJong(){};
    virtual void doEventTimeOverUi(){};
    virtual void doEventTimeOver(int type){};
    virtual void showCurrentPlayedJongIcon(bool isShow){};
    virtual void JongShowdown(vector<int> jongs){};
    virtual Point getCpgShowPostion(int index){ return Point(0, 0); };
    
    void onEnter() override;
    void onExit() override;
    void initPlayer(Player* playerInfo);//初始化玩家UI
    void replaceHandHua(JongViewType tpye);//玩家手牌换花
    void replaceTurnHua(PlayerTurnData data);//摸牌换花
    void showPlayedJong(int ctype);//玩家打牌
    void showPlayerChi(string chiPoker, PlayerBase* playerBase);//玩家吃牌
    void showPlayerPeng(PlayerCpgtData data,PlayerBase* playerBase);//玩家碰牌
    void showPlayerGang(PlayerCpgtData data, PlayerBase* playerBase);//玩家杠牌
    void showPlayerHua(int num);//显示玩家花数
    void setPlayerTingState(bool b);//设置玩家的听牌状态
    void showCurrentBigJong(int cType);
    void hideCurrentBigJong();
    void setIsReady(bool b);//准备状态
    void setIsOffLine(bool b);//是否断线
    void setIsTrustee(bool b);//是否托管
    void showPlayerInfo();//显示玩家信息
    void startTimeClockAnim();
    void startTimeClockAnim(int time, int type);
    void stopTimeClockAnim();
    void playerCpgAnim(CpgType cpgType, ClientSeatId type);//玩家吃碰杠动画
    void updatePlayerInfo(int num);//刷新头像下的金币或者积分信息
    void recoverHua(std::string hua);//断线后花恢复
    void recoverPlayed(std::string played);//断线后打出的牌恢复
    void recoverCpg(vector<PlayerChiData> chi,vector<PlayerPengData> peng,vector<PlayerGangData> gang,std::string angang);//断线后吃碰杠牌恢复
    void recoverHand(std::string hand);//断线后手牌恢复
    void hideHandJongs();//隐藏玩家的手牌
    void playSoundHuPai(int type);
    Jong* getCurrentJong();
    
    CC_SYNTHESIZE(Player*, playerInfo, PlayerInfo);
    CC_SYNTHESIZE(ReplaceJong, replacePoker, ReplacePoker);
    CC_SYNTHESIZE(int, clientSeat, ClientSeat);
    CC_SYNTHESIZE(int,huaNum,HuaNum);
    CC_SYNTHESIZE(bool, stateCpg, StateCpg);//吃碰杠状态
    CC_SYNTHESIZE(int, chiNumber, ChiNumber);//吃的数量
    CC_SYNTHESIZE(int, pokerNumber, PokerNumber);//打牌数量
    CC_SYNTHESIZE(int, lastPoker, LastPoker);//最近一次出牌
    CC_SYNTHESIZE(bool, isPlayHuaChi, IsPlayHuaChi);//花痴音效
    CREATE_FUNC(PlayerBase);
    
private:
    
    bool isReady;
    int mCDTime;
    std::vector<Jong*> needReplace;//需要替换的花牌
    EventListenerCustom* roomChatListener;
    Sprite* tingTitle;
    Sprite* playerHua;
    Menu* headimage;
    Label* nickName;
    LabelAtlas* diamondNum;
    LabelAtlas* timeClock;
    LabelAtlas* playerHuaCount;
    ProgressTimer* mProgressTimer;
    Layer* chatShowLayer;//聊天显示图层
    void initData();
    void updateTime(float dt);
    Point getPostionBySeat(int seatId);
    Point getBigJongPos(int type);
    Point getHuaNumPos(int type);
    Point getVec2BySeatId(int seatId);
    std::string getHeadImageName(std::string id);
};

#endif