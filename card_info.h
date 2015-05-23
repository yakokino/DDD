#pragma once

#include <string>



const int SYMBOL_TYPE_NUM = 6;	//ダイスシンボル数(-1)

//ダイスシンボルの種類(種類の増減時は上記のシンボル数も変更すること)
enum DICE_SYMBOL
{
	NO_SYMBOL = -1,	//ダイス管理用
	SUMMON = 0,		//召喚
	ATTACK,			//物理攻撃
	SPECIAL,		//特殊攻撃
	MOVE,			//移動
	MONEY,			//お金

	PIETY,			//信仰心（東方）
	MAGIC_STONE,	//魔法石（パズドラ）

};

//シンボルデータ
struct SYMBOL_DATA
{
	int symbol[ SYMBOL_TYPE_NUM ];	//[SUMMON] == 1で召喚紋章1必要
};

//マップデータ
enum MAP_DATA{
	MAP_TILE,
	MAP_WAY_RED,
	MAP_WAY_GREEN,
	MAP_WAY_BLUE

};

//状態異常
enum CSTATE{
	CSTATE_NORMAL,	//状態異常なし
	CSTATE_POIZON,	//毒
};

//デッキデータ
struct DeckData{
	DICE_SYMBOL my_dice[3][6];	//マイダイス
	int card_no[60];	//カードナンバー
};


//右上の方角を上方向とする
const int UP	= 0;	//(右)上方向
const int RIGHT = 1;	//右(下)方向
const int DOWN	= 2;	//(左)下方向
const int LEFT	= 3;	//左(上)方向

//キャラクターマップ用
const int P0_UP		= 4;
const int P0_RIGHT	= 5;
const int P0_DOWN	= 6;
const int P0_LEFT	= 7;
const int P1_UP		= 8;
const int P1_RIGHT	= 9;
const int P1_DOWN	= 10;
const int P1_LEFT	= 11;
const int P2_UP		= 12;
const int P2_RIGHT	= 13;
const int P2_DOWN	= 14;
const int P2_LEFT	= 15;
const int P3_UP		= 16;
const int P3_RIGHT	= 17;
const int P3_DOWN	= 18;
const int P3_LEFT	= 19;
const int P4_UP		= 20;
const int P4_RIGHT	= 21;
const int P4_DOWN	= 22;
const int P4_LEFT	= 23;


//カードタイプ
enum C_TYPE{
	CHARACTOR,
	MAGIC,
	TRAP

};

//移動タイプ
enum M_TYPE{
	WALK,
	FLY
};

//種族
enum RACE{
	HUMAN,		//人間
	WITCH,		//魔女
	YOUKAI,		//妖怪
	GOD,		//神
	DEVIL,		//悪魔
	HEALER,		//回復タイプ
	
	//MIKO,		//巫女
	//BALANCE,	//バランスタイプ
};

//ジャンル
enum GENRE{
	ORIGINAL,
	TOHO,
	PAZZLE

};

//スキル効果分類
enum S_TYPE{
	ACTIVE,		//アクティブスキル（アタックフェイズに一度使え、アクティブスキルを複数持っているキャラは一ターンにそのうちひとつしか使えない）赤色表記
	IGNITION,	//起動効果（メインフェイズで使用できる。特に指定がなければ何度でも使用可能）青色表記
	PASSIVE,	//永続効果（常に効果が発動している）緑色表記
	TRIGGER		//誘発効果（条件を満たせば発動する）紫色表記

};


//ダメージタイプ
enum D_TYPE{
	DAMAGE,			//通常ダメージ
	THROUGH,		//防御無視ダメージ
	HEAL,			//回復
	BACK,			//通常ダメージ＋ノックバック
	ATTACK_UP,		//攻撃力アップ
	ATTACK_DOWN,	//		ダウン
	DEFENSE_UP,		//防御力アップ
	DEFENSE_DOWN,	//		ダウン
	SPEED_UP,		//移動量アップ
	SPEED_DOWN		//		ダウン

};

//対象タイプ
enum T_TYPE{
	//通常射程（円形）
	ENEMY_SIMPLE,	//射程範囲内の敵から対象を一つ選ぶ
	ENEMY_DOUBLE,	//							二つ選ぶ
	FRIEND_SIMPLE,	//射程範囲内の味方から対象を一つ選ぶ
	FRIEND_DOUBLE,	//							二つ選ぶ
	ENEMY_FRIEND_SIMPLE,//射程範囲内の敵、味方から対象を一つ選ぶ
	ENEMY_FRIEND_DOUBLE,//								二つ選ぶ

	CONDITION_ENEMY_SIMPLE,	//（射程範囲と）特定の条件を満たした敵からひとつ選ぶ
	CONDITION_ENEMY_ALL,	//									敵全て
	CONDITION_FRIEND_SIMPLE,
	CONDITION_FRIEND_ALL,
	CONDITION_ENEMY_FRIEND_ALL,

	ENEMY_ALL,		//射程範囲内の敵全て
	FRIEND_ALL,		//			味方全て
	ENEMY_FRIEND_ALL,//			敵、味方全て

	//直線射程
	STRAIGHT_ENEMY_SIMPLE,	//向いている方向で一番近い敵一つ（味方キャラだった場合はその方向に使えない）
	STRAIGHT_ENEMY_ALL,		//				の敵全て
	STRAIGHT_FRIEND_SIMPLE,
	STRAIGHT_FRIEND_ALL,
	STRAIGHT_ENEMY_FRIEND_ALL
};

////射程タイプ
//enum R_TYPE{
//	RANGE,		//通常射程（円形）
//	STRAIGHT,	//直線射程
//};

struct ActionData{
	std::string skill_name;		//技名
	std::string skill_info;		//技の説明文
	SYMBOL_DATA cost;	//コスト[SUMMON]==2 で召喚紋章2コスト
	S_TYPE s_type;	//効果分類
	D_TYPE d_type;	//ダメージタイプ
	int power;		//攻撃力
	int combo;		//攻撃回数
	//R_TYPE r_type;	//射程タイプ
	int range;		//射程範囲（射程0でそのキャラのみ）
	int no_range;	//射程外範囲（rangeより小さい値が入る。0でキャラの位置以外が射程となり、-1でキャラの位置も射程内となる）
	CSTATE cstate;	//状態異常
	int delay;		//遅延ターン（通常は0で即時発動）
	T_TYPE t_type;	//対象タイプ

	
	void init(){
		for ( int &c : cost.symbol ) {
			c = 0;
		}
		s_type = ACTIVE;
		d_type = DAMAGE;
		power = 1000;
		combo = 1;
		range = 1;
		no_range = 0;
		cstate = CSTATE_NORMAL;
		delay = 0;
		t_type = ENEMY_SIMPLE;
	}
};

//フェイズ管理用
enum PHASE{
	NO_PHASE = -10,	//次のフェイズへ移行する為のフラグに使ったり
	INIT_PHASE = -1,	//隠しフェイズ。スタンバイフェイズ前に必要な初期化
	STANDBY_PHASE = 0,
	DICEROLL_PHASE,
	MAIN_PHASE,
	ATTACK_PHASE,
	MAIN_PHASE2,
	END_PHASE

};

//一人のプレイヤーが持つデータ
struct PlayerData{
	std::string p_name;	//プレイヤーネーム
	int p_no;			//このデータのプレイヤーナンバー
	int p_life;			//現在ライフ
	int indicate_life;	//表示用ライフ
	int last_time;		//残り時間(frame)
	int hand;			//手札の枚数
	int dust;			//捨て札の枚数
	DICE_SYMBOL my_dice[3][6];	//マイダイス情報
	SYMBOL_DATA p_stock;	//ストック（p_stock[SUMMON]=5で召喚紋章5ストック）
public:
	PlayerData(){	//プレイヤーデータ初期化
		p_name = "NO NAME";
		p_no = -1;
		p_life = 10000;
		last_time = 60*60;
		hand = 5;
		dust = 0;
		for(int &cost : p_stock.symbol ){
			cost = 0;
		}
	}
};
