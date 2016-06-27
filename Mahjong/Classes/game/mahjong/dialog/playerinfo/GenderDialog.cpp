#include "game/mahjong/dialog/playerinfo/GenderDialog.h"
#include "game/mahjong/dialog/playerinfo/UserInfo.h"
#include "server/NetworkManage.h"
#include "userdata/UserData.h"
#include "game/loading/Loading.h"

bool GenderDialog::init(){
	if (!Layer::init()){
		
		return false;
	}
	showDialog();
	return true;
}

void GenderDialog::onEnter(){
	Layer::onEnter();
}

void GenderDialog::onExit(){
	Layer::onExit();
}

void GenderDialog::showDialog(){
	MenuItem* item1 = MenuItem::create();
	item1->setContentSize(Size(1280, 720));
	Menu* menu1 = Menu::create(item1, NULL);
	this->addChild(menu1);

	auto bg = Sprite::create("playerinfo/player_info_bg.png");
	bg->setPosition(640,360);
	addChild(bg);
	auto closeImage = MenuItemImage::create("common/close_btn_1.png", "common/close_btn_1.png", CC_CALLBACK_0(GenderDialog::closeView, this));
	auto closeMenu = Menu::create(closeImage, NULL);
	closeMenu->setPosition(870, 495);
	addChild(closeMenu);

	auto input_bg_1 = Sprite::create("playerinfo/gender_bg.png");
	input_bg_1->setPosition(640, 375);
	addChild(input_bg_1);


	auto box1 = MenuItemImage::create("common/select_box.png", "common/select_box.png",
		CC_CALLBACK_1(GenderDialog::choseGender,this));
	box1->setTag(0);
	auto box2 = MenuItemImage::create("common/select_box.png", "common/select_box.png",
		CC_CALLBACK_1(GenderDialog::choseGender, this));
	box2->setTag(1);
	Menu* menu = Menu::create(box1, box2,NULL);
	menu->setPosition(620, 375);
	menu->alignItemsHorizontallyWithPadding(140);
	addChild(menu);

	nike = Sprite::create("common/select_nike.png");
	if (UserData::getInstance()->getGender()==1){
		nike->setTag(0);
		nike->setPosition(520, 375);
	}
	else{
		nike->setTag(1);
		nike->setPosition(720, 375);
	}
	addChild(nike);
	
	auto nan = Sprite::create("playerinfo/font_nan.png");
	nan->setPosition(575, 375);
	addChild(nan);

	auto nv = Sprite::create("playerinfo/font_nv.png");
	nv->setPosition(775, 375);
	addChild(nv);

	auto confirm = MenuItemImage::create("common/confirm_btn_1.png", "common/confirm_btn_2.png",
		CC_CALLBACK_0(GenderDialog::changeGender, this));
	auto confirmMenu = Menu::create(confirm, NULL);
	confirmMenu->setPosition(640, 245);
	this->addChild(confirmMenu);
}


void GenderDialog::choseGender(Ref* ref){
	MenuItemImage* temp = (MenuItemImage*)ref;
	if (temp->getTag() == 0){
		nike->setTag(0);
		nike->setPosition(520, 375);
	}
	else if (temp->getTag() == 1){
		nike->setTag(1);
		nike->setPosition(720, 375);
	}
}


void GenderDialog::closeView(){
	removeFromParent();
}


void GenderDialog::changeGender(){
	std::string gender = nike->getTag() == 0 ? "1" :"0";
	Loading* load = Loading::create();
	addChild(load);
	NetworkManage::getInstance()->sendMsg(CommandManage::getInstance()->getChangeGenderCommand(gender));
}

