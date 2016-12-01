#ifndef __SPLSH_SCENE_H__
#define __SPLSH_SCENE_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "server/NetworkManage.h"
#include "server/CommandManage.h"
USING_NS_CC;
USING_NS_CC_EXT;

class SplashScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
    void onEnterTransitionDidFinish() override;
	CREATE_FUNC(SplashScene);

private:
    
	EventListenerCustom* loginRespListener;
	EventListenerCustom* roomRespListener;
	EventListenerCustom* reConnectAgain;
    EventListenerCustom* reEnterFriendRoomListener;
    EventListenerCustom* reOpenFriendRoomListener;
    void loginByWechat();
	void addTocuhListener();
	void drawLonginScene();
    void showSplashAnim();
    void scrollLightSpot(float dt);
	void showLoading();
	void removeLoading();
};
#endif