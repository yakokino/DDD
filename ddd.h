#ifndef __DDD_H__
#define __DDD_H__

#include "ddd_loader.h"
#include "ddd_map_ui.h"
#include "card_info.h"
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

//マウス関連
enum MOUSE_CLICK{
	LEFT_CLICK,
	RIGHT_CLICK,
	MIDDLE_CLICK

};

class DDDWorld{
	PlayerDataList player;			//プレイヤーデータリスト
	DDDDice ddd_dice;
	DDDCard ddd_card;
	DDDUI ddd_ui;
	//int player_pc;
	int turn_player;				//ターンプレイヤーナンバー（0～3(1～4Pを表す)）
	PHASE phase;					//現在のフェイズ
	PHASE next_phase;				//次のフェイズ（通常NO_PHASEを入れ、フェイズを変更する時に変更先を代入）
	DICE_SYMBOL turn_dice[3][6];	//このターンのダイス情報
	DDDMap ddd_map;					//マップデータ
	DDDChara ddd_chara;				//キャラデータ

	unsigned int phase_frame;
	//カメラ操作用
	bool scroll;		//trueでスクロール許可
	int camera_x, camera_y;
	int lclick_x, lclick_y;		//前フレームのマウス座標

	//マウスチェック用
	int nx, ny;						//現在のポインタ位置
	int click_x,click_y;			//クリック開始時の位置
	bool drag;						//trueでドラッグ中を示す
	unsigned int pressed_frame[3];	//押されているフレーム数
	int mouse_wheel;				//マウスホイール回転量（前フレームとの差）
	COMMAND push_com;				//クリックされた時のコマンド
	int push_no;					//クリックされた時のコマンドナンバー

	void mouseCheck();				//毎フレーム呼んでマウス情報を入力する関数
	void actionCommand(COMMAND com, int no);

public:
	DDDWorld(){
		phase = INIT_PHASE;
		turn_player = 0;
		for(int i=0 ; i<3 ; i++ ){
			for(int j=0 ; j<6 ; j++ ){
				turn_dice[i][j] = SUMMON;
			}
		}
		scroll = true;
		camera_x = -300;
		camera_y = 0;
		drag = false;
		pressed_frame[0] = 0;
		pressed_frame[1] = 0;
		pressed_frame[2] = 0;


	}

	int loadFiles() throw(...);
	void init();
	void phaseControl();
	void draw();
};

#endif