#ifndef __LOBBY_SCENE_H__
#define __LOBBY_SCENE_H__
#include "cocos2d.h"
#include "userdata/UserData.h"
#include "server/NetworkManage.h"
#include "server/CommandManage.h"
#include "game/utils/StringUtil.h"
#include "game/loading/Loading.h"
#include "game/mahjong/state/GameData.h"
#include "game/mahjong/core/MjGameScene.h"
#include "game/mahjong/dialog/setting/UserSetting.h"
#include "game/mahjong/dialog/friend/FriendListView.h"
#include "game/mahjong/dialog/friend/FriendRoom.h"
#include "game/mahjong/dialog/bill/BillInfo.h"

USING_NS_CC;

#define ROOM_1 1001
#define ROOM_2 1002
#define ROOM_3 1003

class LobbyScene : public cocos2d::Scene
{
public:

    virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	void startGame(Ref* psend);
    void updateHeroInfo();
    CREATE_FUNC(LobbyScene);

private:
    EventListenerCustom* enterRoomListener;
    EventListenerCustom* enterFriendRoomListener;
    EventListenerCustom* openFriendRoomListener;
    EventListenerCustom* friendInviteListener;
    EventListenerCustom* dailySignListener;
    
	Label* nickName;
	LabelAtlas* goldNum;
	LabelAtlas* diamondNum;
	LabelAtlas* lequanNum;
	LabelAtlas* lockDiamondNum;
    
    CC_SYNTHESIZE(int, currentSelectRoomId, CurrentSelectRoomId);

	void initView();
	void drawSceneMid();
	void drawSceneTop();
	void drawSceneBot();
    void chargeGold();
    void chargeDiamond();
    void exchangeLequan();
	void showFirstCharge();
	void showRedWallet();
	void showDayTask();
	void showAddFriend();
	void showOpenRoom();
	void showPlayerBill();
	void showGameSetting();
	void showHotActivity();
	void showHeroInfo();
    void showLoading();
    void removeLoading();
    void showLobbyAnim();
	void addEventListener();
	void addCustomEventListener();
    void scrollLightSpot(float dt);
    void signUpdate(float dt);
};
#endif