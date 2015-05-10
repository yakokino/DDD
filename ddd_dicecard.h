#pragma once

#include "ddd_mouse.h"
#include "card_info.h"
#include "anim_sound.h"
#include "ddd_loader.h"
#include "player_data.h"


class DDDDice
{
	//int player_pc;				//このプレイヤーのプレイヤーナンバー
	bool wait_time;					//trueで処理中を表し、操作できない状態にする
	DICE_SYMBOL dice_list[4][3][6];	//全ダイス情報[p_no][？個目][面]
	DICE_SYMBOL dicision_dice[3];	//決定したダイスシンボル
	bool dice_visible;				//ダイス可視状態tureで表示
	bool dice_shuffle;				//trueでダイスシャッフル状態、falseでダイスストップ状態
	unsigned int stop_frame;		//ダイスが止められてからのフレーム数を記憶する変数
	AnimDraw dice_anim;				//ダイス画像
public:
	int loadFiles() throw( ... );
	void init();
	//void setPlayerPC(int p_no){ player_pc = p_no; }
	void setDice( int p_no, DeckData *deck );
	void shuffleDice();				//ダイスをシャッフル状態にする
	//void setTurnDice( int dice, int men, DICE_SYMBOL sym){ turn_dice[dice][men] = sym; }
	bool getWaitTime() { return wait_time; }
	DICE_SYMBOL getSymbol( int dice ) { return dicision_dice[dice]; }
	void mouseAction( MOUSE_ACTION_DATA* mouse_data );	//コマンド実行

	void draw();
};

//=================================================================================================================

const int CARD_W = 200;			//カードの等倍画像の大きさ
const int CARD_H = 280;

const int HAND_X = 265;			//手札領域の左上座標
const int HAND_Y = 525;
const int HAND_AREA_W = 600;	//手札領域の横幅
const double FRICTION = 0.2;	//手札の減速率
const double REDUCE = 0.4;		//手札のカードの画像縮小率
const double INFO_REDUCE = 0.5;	//キャラカードだった時のインフォカード縮小率

const int HAND_CARD_W = (int)(CARD_W * REDUCE);
const int HAND_CARD_H = (int)(CARD_H * REDUCE);

const int INFO_NAME_CX = 125;		//カードインフォの名前表示位置
const int INFO_NAME_Y = 294;

const int INFO_LINE2 = 323;
const int INFO_DEFENSE_X = 35;
const int INFO_SPEED_X = 115;
const int INFO_GENRE_X = 140;

const int INFO_LINE3 = 346;
const int INFO_HP_X = 43;
const int INFO_MAX_HP_X = 80;

const int INFO_SKILL_X = 60-52;
const int INFO_SKILL_Y = 385;
const int INFO_SKILL_SPACE = 67;

const int INFO_FLAVOR_X = 12;
const int INFO_FLAVOR_Y = 580;

enum CARD_OPERATE_MODE
{
	CARD_OPERATE_MODE_NO_OPERATE,
	CARD_OPERATE_MODE_MAIN_PHASE,
	CARD_OPERATE_MODE_ATTACK_PHASE,
	CARD_OPERATE_MODE_MAIN_PHASE_2,
	CARD_OPERATE_MODE_END_PHASE
};


struct CardData
{
	int card_no;				//カードナンバー
	std::string card_name;		//カードの名前
	std::string card_spell;		//カード名のふりがな
	SYMBOL_DATA cost_data;		//召喚、発動コストcost[SUMMON]==1で召喚紋章1必要
	int map[5][5];				//作るマップ（X座標は右側なので注意　キャラカード限定？）
	C_TYPE c_type;				//カードタイプ
	RACE race;					//種族（キャラカード限定）
	GENRE genre;				//ジャンル（キャラカード限定）
	int life;					//ライフ
	int defense;				//防御力
	M_TYPE m_type;				//移動タイプ
	int speed;					//移動量
	std::vector<ActionData> action_list;	//行動データリスト
	std::string flavor_text;	//フレーバーテクスト

	CardData()
	{
		for ( int &c: cost_data.symbol) {
			c = 0;
		}
		for each ( int &m in map) {
			m = MAP_TILE;
		}
		card_name = "NO_NAME";
		card_spell = "のーねーむ";
		c_type = CHARACTOR;
		genre = ORIGINAL;
		race = HUMAN;
		life = 1000;
		defense = 0;
		m_type = WALK;
		speed = 1;
		flavor_text = "カードデータが入力できてないよ？";
	}
};

class DDDCard
{
	std::vector<int> deck_list[4];		//全プレイヤーデッキリスト
	std::vector<int> hand_list[4];		//全プレイヤー手札リスト
	std::vector<int> dust_list[4];		//全プレイヤー捨て札リスト
	std::vector<int> image_x;			//表示手札の画像位置X
	int over_card;					//マウスオーバーされているカード(手札左から0、1、2・・・)
	int card_info_no;					//現在のカードインフォに表示しているカードナンバー
	CardData card_info_data;			//カードインフォのカードデータ

	CARD_OPERATE_MODE mode;						//操作可能フラグ（trueで操作可能）
	int select_card;
	int cost_enough;					//表示用（コストが足りている場合に1が入る）

	AnimDraw mini_symbol;
	AnimDraw chara_map;
	int info_name_fh;
	int info_middle_fh;
	int info_small_fh;
	int info_mini_fh;
public:
	int loadFiles() throw( ... );
	void init();
	void setDeck( int pn, DeckData *deck );
	void deckShuffle( int p_no );				//指定したプレイヤーのデッキをシャッフルする
	void drawCard( int p_no );					//指定したプレイヤーがデッキからカードを1枚ドローする
	void setCardInfo( int card_no );
	void setCharactorMap( int x, int y, int spin, int num, ... );
	void setOperate( CARD_OPERATE_MODE new_mode ) { mode = new_mode; }

	int getHand( int p_no ) { return hand_list[p_no].size(); }	//指定したプレイヤーのハンド（手札）数を得る
	int getCardPosX( int hand_no ) { return image_x.at( hand_no ); }
	int getCardPosY( int hand_no ) { return HAND_Y; }

	void getCardData( int card_no, CardData *card_data );		//card_info.cppに記載。カード情報を得る

	void mouseAction( MOUSE_ACTION_DATA* mouse_data );
	void draw();

};
