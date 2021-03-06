#pragma once

#include "ddd_loader.h"
#include "card_info.h"
#include "ddd_mouse.h"
#include "ddd_map_ui.h"
#include "player_data.h"
#include "ddd_dicecard.h"


class DDDChara{
	std::list<CharaData> chara_list;
	AnimDraw karireimu;
public:
	int loadFiles() throw(...);
	void init();
	void draw(int camera_x, int camera_y);
	
};

class DDDWorld{
	PlayerDataList player;			//プレイヤーデータリスト
	DDDMouse ddd_mouse;				//マウスクラス
	DDDDice ddd_dice;				//ダイスクラス
	DDDCard ddd_card;				//カードクラス
	DDDUI ddd_ui;					//UIクラス
	DDDMap ddd_map;					//マップクラス
	DDDChara ddd_chara;				//キャラクラス
	//int player_pc;	//TODO:整理
	int turn_player;				//ターンプレイヤーナンバー（0〜3(1〜4Pを表す)）
	PHASE phase;					//現在のフェイズ
	PHASE next_phase;				//次のフェイズ（通常NO_PHASEを入れ、フェイズを変更する時に変更先を代入）
	DICE_SYMBOL turn_dice[3][6];	//このターンのダイス情報

	MOUSE_ACTION_DATA* mouse_data;
	CAMERA_DATA* camera_data;

	unsigned int phase_frame;

	void mouseAction( MOUSE_ACTION_DATA* mouse_data );

public:
	DDDWorld(){
		phase = INIT_PHASE;
		turn_player = 0;
		for(int i=0 ; i<3 ; i++ ){
			for(int j=0 ; j<6 ; j++ ){
				turn_dice[i][j] = SUMMON;
			}
		}
		//scroll = true;
		//camera_x = -300;
		//camera_y = 0;


	}

	int loadFiles() throw(...);
	void init();
	void phaseControl();
	void draw();
};
