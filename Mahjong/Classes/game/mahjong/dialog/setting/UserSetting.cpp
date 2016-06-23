#include "game/mahjong/dialog/setting/UserSetting.h"
#include "game/utils/Audio.h"

bool UserSetting::init(){
	if (!BaseDialog::init()){
		return false;
	}
	return true;
}

void UserSetting::drawDialog(){
    MenuItem* item1 = MenuItem::create();
    item1->setContentSize(Size(1280, 720));
    Menu* menu1 = Menu::create(item1, NULL);
    this->addChild(menu1);
    
    auto all_bg = Sprite::create("common/all_bg.jpg");
    all_bg->setPosition(640, 360);
    addChild(all_bg);
    
    
    auto  dialogBg = Sprite::create("common/dialog_bg.png");
    dialogBg->setPosition(640, 360);
    this->addChild(dialogBg);
    
    auto closeImage = MenuItemImage::create("common/close_btn_1.png", "common/close_btn_1.png", CC_CALLBACK_0(UserSetting::closeView, this));
    auto closeMenu = Menu::create(closeImage, NULL);
    closeMenu->setPosition(980, 580);
    addChild(closeMenu);
    
    auto paodai = Sprite::create("common/piaodai_zi.png");
    paodai->setPosition(640, 590);
    this->addChild(paodai);
    auto titleIcon = Sprite::create("setting/settting_title.png");
    titleIcon->setPosition(640, 615);
    addChild(titleIcon);

    auto musicBg = Sprite::create("setting/setting_bg.png");
    musicBg->setPosition(640,380);
    addChild(musicBg);
    
    auto soudnText = Sprite::create("setting/sound.png");
    soudnText->setPosition(385,440);
    addChild(soudnText);
    
    ControlSlider* slide_control = ControlSlider::create("setting/progress_1.png", "setting/progress_2.png",
                                                         "setting/slide_btn_1.png");
    slide_control->setMinimumValue(0.0f);//设置最小值
    slide_control->setMaximumValue(100.0f);//设置最大值
    slide_control->setValue(0.0f);//设置初始值
    slide_control->setTag(20);
    slide_control->addTargetWithActionForControlEvents(this,cccontrol_selector(UserSetting::slideCallback),Control::EventType::VALUE_CHANGED);//设置拖动回调
    slide_control->setPosition(720,440);
    addChild(slide_control);

    
    
    auto musicText = Sprite::create("setting/music.png");
    musicText->setPosition(385,342);
    addChild(musicText);
    
    
    ControlSlider* slide_control2 = ControlSlider::create("setting/progress_1.png", "setting/progress_2.png",
                                                    "setting/slide_btn_1.png");
    slide_control2->setMinimumValue(0.0f);//设置最小值
    slide_control2->setMaximumValue(100.0f);//设置最大值
    slide_control2->setValue(0.0f);//设置初始值
    slide_control2->setTag(30);
    slide_control2->addTargetWithActionForControlEvents(this,cccontrol_selector(UserSetting::slideCallback),Control::EventType::VALUE_CHANGED);//设置拖动回调
    slide_control2->setPosition(720,345);
    addChild(slide_control2);
    
    auto helpImage = MenuItemImage::create("setting/game_help_btn_1.png","setting/game_help_btn_2.png",
                                           CC_CALLBACK_0(UserSetting::showGameHelp, this));
    auto feedImage = MenuItemImage::create("setting/feedback_btn_1.png","setting/feedback_btn_2.png",
                                           CC_CALLBACK_0(UserSetting::feedBack, this));
    
    Menu* myMneu = Menu::create(helpImage,feedImage,NULL);
    myMneu->setPosition(650,185);
    myMneu->alignItemsHorizontallyWithPadding(20);
    addChild(myMneu);

}



void UserSetting::slideCallback(Object *sender, Control::EventType controlEvent)
{
    auto slide_control = (ControlSlider*)sender;//通过回调参数sender 获得ControlSlider
    int current_value = slide_control->getValue();//获取slide当前的值
    if(slide_control->getTag() == 20){
        Audio::getInstance()->setBGMValue(current_value/100.0f);
    }
    
    if(slide_control->getTag() == 30){
        Audio::getInstance()->setEffectValue(current_value/100.0f);
    }
    
}


void UserSetting::showGameHelp(){


}


void UserSetting::feedBack(){



}