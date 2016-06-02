#include "game/mahjong/dialog/playerinfo/BoundPhone.h"
#include "game/mahjong/dialog/playerinfo/UserInfo.h"
#include "game/mahjong/state/GameData.h"
#include "server/NetworkManage.h"
#include "userdata/UserData.h"
#include "game/loading/Loading.h"
#include <Regex>

bool BoundPhone::init(){
	if (!Layer::init()){
		
		return false;
	}
	showDialog();
	return true;
}

void BoundPhone::onEnter(){
	Layer::onEnter();
	boundPhoneListener = EventListenerCustom::create(MSG_BOUND_PHONE_RESP, [=](EventCustom* event){

		BoundPhoneData data = GAMEDATA::getInstance()->getBoundPhoneData();
		if (data.result == 1){
			UserData::getInstance()->setBoundPhone(data.phoneNum);
			removeAllChildren();
			showDialog();
		}
	});
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(boundPhoneListener, 1);

}

void BoundPhone::onExit(){
	Layer::onExit();
	_eventDispatcher->removeEventListener(boundPhoneListener);
}

void BoundPhone::showDialog(){
	if (UserData::getInstance()->getBoundPhone() == "unknow"){
		auto bound_phone = Sprite::create("playerinfo/bound_phone.png");
		bound_phone->setPosition(406, 402);
		addChild(bound_phone);

		auto input_bg_1 = Scale9Sprite::create("playerinfo/input_text_2.png");
		input_bg_1->setContentSize(Size(450, 81));
		input_bg_1->setPosition(720, 410);
		addChild(input_bg_1);
		_phoneNum = EditBox::create(Size(220, 81), Scale9Sprite::create());
		_phoneNum->setPosition(Point(635, 410));
		_phoneNum->setTag(0);
		_phoneNum->setFont("arial", 24);
		_phoneNum->setDelegate(this);
		addChild(_phoneNum);

		auto itemImage = MenuItemImage::create("playerinfo/verification_btn_1.png", "playerinfo/verification_btn_2.png",
			CC_CALLBACK_0(BoundPhone::getVerification, this));
		Menu* myMneu = Menu::create(itemImage, NULL);
		myMneu->setPosition(835, 410);
		addChild(myMneu);

		hintSprite = Sprite::create("playerinfo/phone_error_hint.png");
		hintSprite->setPosition(640, 360);
		hintSprite->setVisible(false);
		addChild(hintSprite);

		auto text = Sprite::create("playerinfo/verification_text.png");
		text->setPosition(825, 410);
		addChild(text);

		clock = Label::create(cocos2d::String::createWithFormat("%ds", 60)->_string, "arial", 20);
		clock->setColor(Color3B(243, 188, 59));
		clock->setPosition(908, 410);
		addChild(clock);

		auto verification = Sprite::create("playerinfo/verification_code.png");
		verification->setPosition(410, 305);
		addChild(verification);
		auto input_bg_2 = Scale9Sprite::create("common/input_box_bg.png");
		input_bg_2->setContentSize(Size(450, 81));
		input_bg_2->setPosition(720, 305);
		addChild(input_bg_2);
		_verification = EditBox::create(Size(230, 81), Scale9Sprite::create());
		_verification->setPosition(Point(635, 305));
		_verification->setTag(0);
		_verification->setFont("arial", 24);
		_verification->setDelegate(this);
		addChild(_verification);

		auto confirm = MenuItemImage::create("common/confirm_btn_1.png", "common/confirm_btn_2.png",
			CC_CALLBACK_0(BoundPhone::confirmBound, this));
		auto confirmMenu = Menu::create(confirm, NULL);
		confirmMenu->setPosition(640, 170);
		this->addChild(confirmMenu);
	}
	else{

		auto bound_phone = Sprite::create("playerinfo/bound_phone.png");
		bound_phone->setPosition(406, 360);
		addChild(bound_phone);
		auto input_bg_1 = Scale9Sprite::create("common/input_box_bg.png");
		input_bg_1->setContentSize(Size(450, 81));
		input_bg_1->setPosition(720, 360);
		addChild(input_bg_1);
		Label* label = Label::create(UserData::getInstance()->getBoundPhone(),"arail",30);
		label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
		label->setPosition(530, 360);
		addChild(label);
	}
}


void BoundPhone::getVerification(){
	totalTime = 60;
	if (checkPhone(_phoneNum->getText())){
		NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->getVerifyCommand(_phoneNum->getText()));
		schedule(schedule_selector(BoundPhone::updateSecond), 1, CC_REPEAT_FOREVER, 0);
	}
	else{
		hintSprite->setVisible(true);
	}
}

bool BoundPhone::checkPhone(std::string phone){
	if (phone.length() != 11){
		return false;
	}
	std::regex rx("[0-9]+");
	bool result = std::regex_match(phone.begin(), phone.end(), rx);
	return result;
}

void BoundPhone::confirmBound(){
	std::string phone = _phoneNum->getText();
	std::string ver = _verification->getText();
	if (!hintSprite->isVisible() && phone != ""&&ver != ""){
		Loading* load = Loading::create();
		addChild(load);
		NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->getBoundPhoneCommand(phone, ver));
	}
}

void BoundPhone::updateSecond(float dt){
	totalTime--;
	if (totalTime > 0){
		clock->setString(cocos2d::String::createWithFormat("%ds", totalTime)->_string);
	}
}


//开始进入编辑
void BoundPhone::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox){


}
//结束编辑
void BoundPhone::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox){
	if (checkPhone(_phoneNum->getText())){
		hintSprite->setVisible(false);
	}
	else{
		hintSprite->setVisible(true);
	}
}
//编辑框文本改变
void BoundPhone::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string& text){


}
//当触发return后的回调函数
void BoundPhone::editBoxReturn(cocos2d::extension::EditBox* editBox){


}