#pragma once

#include <string>
#include "chara_data.h"


const int PLAYER_DATA_X = 350;	//プレイヤーの情報位置
const int PLAYER_DATA_Y = 440;
const int OTHER_PLAYER_DATA_X = 823;
const int OTHER_PLAYER_DATA_Y = 20;
const int PLAYER_DATA_SPACE = 96+16;	//プレイヤー情報の（上下）間隔


class PlayerDataList{

	static PlayerData pd[4];		//4P分のプレイヤーデータ
	static int player_pc;		//このPCを操作するプレイヤーナンバー
	int name_fh;			//名前表示用フォントハンドル
	int other_fh;
	AnimDraw symbol_anim;
	
public:
	PlayerDataList(){
		player_pc = 0;
		pd[0].p_name = "ヤコキノ";
		pd[1].p_name = "初音ミク";
		pd[2].p_name = "鏡音リン";
		pd[3].p_name = "巡音ルカ";
		pd[0].p_no = 0;
		pd[1].p_no = 1;
		pd[2].p_no = 2;
		pd[3].p_no = 3;
	}
	int loadFiles() throw(...);
	void init();
	void setPlayerPC( int pn ){ player_pc = pn; }		//このPCプレイヤーナンバーをセットする
	static int getPlayerPC(){ return player_pc; }
	static void damage( int pn, int damage){ pd[pn].p_life -= damage; }
	static void heal( int pn, int heal){ pd[pn].p_life += heal; }

	static void giveStock( int p_no, DICE_SYMBOL symbol ){ pd[p_no].p_stock.symbol[symbol]++; }		//クレストプールに紋章追加
	static void expendStock( int p_no, DICE_SYMBOL symbol ){ pd[p_no].p_stock.symbol[symbol]--; }		//クレストプールから紋章消費
	static bool isEnoughSymbol( const int p_no, SYMBOL_DATA* cost );			//コストチェック関数。コストが足りていれば1を返す

	static DICE_SYMBOL getMyDice(int pn, int dice, int men){ return pd[pn].my_dice[dice][men]; }

	void draw();

};
