#include "Trusteeship.h"
#include "GameData.h"
#include "NetworkManage.h"

bool Trusteeship::init(){
	if (!Layer::init()){
		return false;
	}
	drawDialog();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(Trusteeship::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,
		this);
	return true;
}

void Trusteeship::drawDialog(){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto bg = LayerColor::create(Color4B(0, 0, 0, 150), visibleSize.width, visibleSize.height);
	this->addChild(bg, 20);
	auto text = Sprite::create("trusteeship/trusteeship_cancel.png");
	text->setPosition(640,200);
	this->addChild(text);
	auto hand = Sprite::create("trusteeship/trusteeship_hand.png");
	hand->setPosition(640, 280);
	this->addChild(hand);

}

void Trusteeship::closeView(){
	this->removeFromParent();
}

bool Trusteeship::onTouchBegan(Touch *touch, Event  *event){
	if (this->isVisible()){
		NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->getTrusteeshipCancelCommand());
	/*	GAMEDATA::getInstance()->setIsTrusteeship(false);
		this->setVisible(false);*/
		return true;
	}
	return false;
}
