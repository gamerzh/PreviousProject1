#ifndef _MENU_SCENE_PAY_HANDLER_H_
#define _MENU_SCENE_PAY_HANDLER_H_
#include "cocos2d.h"

class MahjongPayHandler
{       
public:
	MahjongPayHandler() {};
	~MahjongPayHandler() {};
	void dealEventCallBack(int eventId, int result,int propIds[], int propNums[], int count);
	static MahjongPayHandler* getInstance();
private:
	void doPayEvent1(int result);
	void doPayEvent2(int result);
	void doPayEvent3(int result);
};
#endif
