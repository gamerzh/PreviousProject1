#ifndef __PLAYER_OPPOSSITE__
#define __PLAYER_OPPOSSITE__
#include "cocos2d.h"
#include "game/mahjong/player/base/PlayerBase.h"
USING_NS_CC;

class PlayerOpposite : public PlayerBase{

public:
	virtual bool init();
	void drawHandJong();
	void drawPlayedJong(int ctype);
	void drawHuaJong();
	void removeLastJong();
	void showCurrentPlayedJongIcon(bool isShow) override;
	Point getCpgShowPostion(int index) override;
	void drawOppositePlayerPlay(int type);
	void drawOppositePlayerTurn();
	void playerTurnReplace(PlayerTurnData data);
	void drawPlayerChi(PlayerCpgtData data, PlayerBase* playerBase);
	void drawPlayerPeng(PlayerCpgtData data, PlayerBase* playerBase);
	void drawPlayerGang(PlayerCpgtData data, PlayerBase* playerBase);
	Point getPlayedJongPos(int count);
	void recoverHua(int hua);
	void recoverPlayed(std::string played);
	void recoverCpg(vector<PlayerChiData> chi,vector<PlayerPengData> peng,vector<PlayerGangData> gang,std::string angang);
	void recoverHand(std::string hand);
    void updateHandJongs(std::string jongs);
	CREATE_FUNC(PlayerOpposite);

private:
	static const int OPPOSITE_POS_X = 295;
	static const int OPPOSITE_POS_Y = 675;
	Point getHuaJongPos(int number);
};
#endif 