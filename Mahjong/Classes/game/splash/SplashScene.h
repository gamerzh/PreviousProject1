#ifndef __SPLSH_SCENE_H__
#define __SPLSH_SCENE_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "server/NetworkManage.h"
#include "server/CommandManage.h"
USING_NS_CC;
USING_NS_CC_EXT;

class SplashScene : public cocos2d::Layer,public EditBoxDelegate
{
public:
	EditBox* _editName;
	EditBox* _editPwd;
    static cocos2d::Scene* createScene();
    virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;

	void loadResource();
	void loginByPass();
	void loginByVisitor();
	void setChangeNickName(std::string name);

	//开始进入编辑
	virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);
	//结束编辑
	virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);
	//编辑框文本改变
	virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text);
	//当触发return后的回调函数
	virtual void editBoxReturn(cocos2d::extension::EditBox* editBox);

	CREATE_FUNC(SplashScene);

private:
		
	EventListenerCustom* loginRespListener;
	EventListenerCustom* roomRespListener;
	EventListenerCustom* reConnectAgain;
	Sprite* username_text;
	Sprite* password_text;
	void addTocuhListener();
	void drawLonginScene();
	void addCustomEventListener();
	void showUserRegister();
	void findbackPwd();
	void showLoading();
	void removeLoading();
};
#endif