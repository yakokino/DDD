#pragma once

#include "card_info.h"
#include "anim_sound.h"
#include "ddd_dicecard.h"

struct CharaData{

public:

	int card_no;			//このカードのカードナンバー
	
	int max_life, life;		//最大ライフと現在ライフ
	std::vector<ActionData> action;	//行動リスト
	int defense;			//現在防御力
	int speed;				//現在移動量
	//種族？
	//タイプ？
	//色？

	
	int point_x, point_y;	//キャラの座標（DDDマップ座標）
	int direction;	//キャラの向き
	CSTATE state;			//現在の状態異常

	int image_x, image_y;	//キャラの画像位置座標
	int image_w, image_h;	//画像サイズ
	AnimDraw *anim;			//キャラアニメーション
	
	int yobi[10];			//パッシブ効果などに使われるの予備変数

};
