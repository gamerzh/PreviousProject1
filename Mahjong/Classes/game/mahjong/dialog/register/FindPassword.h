#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class FindPassword :public Layer, public EditBoxDelegate{
public:
	virtual bool init();
	CREATE_FUNC(FindPassword);

    virtual void editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox);

    virtual void editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox);

    virtual void editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text);

    virtual void editBoxReturn(cocos2d::extension::EditBox* editBox);
private:
    EditBox* _phone;
    EditBox* _account;
	void showDialog();
	void closeView();
    void findPassword();
};