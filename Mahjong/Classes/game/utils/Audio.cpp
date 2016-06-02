#include "game/utils/Audio.h"
#include "SimpleAudioEngine.h"
#include "userdata/UserData.h"


Audio* Audio::m_instance = nullptr;
Audio* Audio::getInstance(){
	if(m_instance == nullptr){
		m_instance = new Audio();
	}
	return m_instance;
}

void Audio::playBGM(char* soundName){
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(soundName,true);

}
void Audio::pauseBGM()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void Audio::playSound(char* soundName){

}
void Audio::setBGMValue(float value)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value);
}
void Audio::setEffectValue(float value)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(value);
}
void Audio::prepare(){
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGORUNDMUSIC);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(COUNTDOWN);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(CLICK);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(FINISHTASK);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(QUICKFIND);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(REVIVE);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SELECTCORRECT);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SELECTERROR);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SELECTTIME);
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SHOWPAYPOINT);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.4f);
}