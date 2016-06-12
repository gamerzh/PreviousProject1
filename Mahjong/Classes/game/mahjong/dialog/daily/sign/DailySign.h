#ifndef __DAILY_SIGN_H__
#define __DAILY_SIGN_H__
#include "cocos2d.h"
USING_NS_CC;

class DailySign :public Layer{
public:
	virtual bool init();
	void onEnter() override;
	void onExit() override;
    void updateData();
	CREATE_FUNC(DailySign);
private:
    EventListenerCustom* signListener;
	EventListenerCustom* todaySignListener;
	void drawLayer();
	void closeView();
	void confirmSign(Ref* ref);
	void drawDayBgAndTitle();
	std::string getImageNameById(int id,bool normal);
    void showLightAnim(Sprite* sprite);
    void showDialogAnim();
};
#endif