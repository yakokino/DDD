#pragma once

#include <vector>
#include "DxLib.h"
#include "card_info.h"

//マウスボタン
enum MOUSE_BUTTON
{
	MOUSE_LEFT		= 0,
	MOUSE_RIGHT		= 1,
	MOUSE_MIDDLE	= 2
};

//マウス操作によるコマンド
enum COMMAND
{
	NO_COMMAND = -1,
	MAP_VIEW,
	DICE_STOP,
	HAND_CARD,
	CARD_SELECT,
	CARD_SUMMON,
	ATTACK_PHASE_BUTTON,
	MAIN_PHASE2_BUTTON,
	END_PHASE_BUTTON

};

enum MOUSE_ACTION
{
	OVER,				//マウスオーバー
	LEFT_PUSH_DOWN,		//左マウスボタンが押された瞬間
	LEFT_PUSH_UP,		//左マウスボタンが離された瞬間
	LEFT_CLICK,			//左マウスボタンが押されてすぐ離された
	LEFT_DRAG,			//左マウスボタンを押しっぱなし
	RIGHT_PUSH_DOWN,	//右マウスボタンが押された瞬間
	RIGHT_PUSH_UP,		//右マウスボタンが離された瞬間
	RIGHT_CLICK,		//右マウスボタンが押されてすぐ離された
	RIGHT_DRAG,			//右マウスボタンを押しっぱなし
	MIDDLE_PUSH_DOWN,	//マウスホイールボタンが押された瞬間
	MIDDLE_PUSH_UP,		//マウスホイールボタンが離された瞬間
	MIDDLE_CLICK,		//マウスホイールボタンが押されてすぐ離された
	MIDDLE_DRAG			//マウスホイールボタンを押しっぱなし
};

//マウスアクションを実行する為に渡す情報
struct MOUSE_ACTION_DATA
{
	MOUSE_ACTION action;
	COMMAND command;
	int command_no;
	PHASE phase;		//マウスアクション引数の都合上ここにもフェイズ情報を入れる
};



//コマンドデータ
struct COMMAND_DATA
{
	int x1, y1;		//コマンド座標左上
	int x2, y2;		//コマンド座標右下
	int no;			//コマンドナンバー（手札の判別やクリック情報が入る）
	COMMAND command;//コマンド
};


class DDDMouse
{
	int nx, ny;						//現在のポインタ位置
	int click_x, click_y;			//クリック開始時の位置
	int lclick_x, lclick_y;			//前フレームのマウス座標
	int dis_x, dis_y;				//前フレームとの差
	int pressed_frames[3];			//それぞれのマウスボタンが押されているフレーム数
	int mouse_wheel;				//マウスホイール回転量（前フレームとの差）
	COMMAND push_com;				//プッシュダウンされた時のコマンド
	int push_no;					//プッシュダウンされた時のコマンドナンバー
	//COMMAND ncom;					//現在のコマンド
	//int nno;						//現在のコマンドナンバー
	//MOUSE_ACTION mouse_act;			//マウスアクション（クリック、ドラッグなど）
	MOUSE_ACTION_DATA mouse_act_data;

	std::vector<COMMAND_DATA> command_list;	//マウスで反応する位置とそのコマンドリスト

	void checkMouseState();

public:
	DDDMouse()
	{
		GetMousePoint( &nx, &ny );
		click_x = nx; click_y = ny;
		for ( auto &init_frame : pressed_frames ) {
			init_frame = 0;
		}
		mouse_wheel = 0;
		push_com = NO_COMMAND;
		push_no = 0;
	}
	void getInput();				//毎フレーム呼んでマウス入力情報を取得する関数
	void setCommand( int x, int y, int w, int h, COMMAND com, int no = 0 );		//マウス操作できる位置を追加する
	void getCommandPos( COMMAND com, int no, int *x, int *y );							//コマンドの位置を取得する
	void setCommandPos( COMMAND com, int no, int x, int y );							//コマンドの位置をセット（変更）する
	void deleteCommand( COMMAND com );											//コマンドを削除する
	COMMAND checkCommand( int x, int y, int *no );	//マウスの位置がマウス操作可能な位置かどうかを判定し、コマンドを返す
	int getNowX() { return nx; }		//現在のポインタ位置を得る
	int getNowY() { return ny; }
	int getDisX() { return dis_x; }		//ポインタ位置の前フレームとの差を得る
	int getDisY() { return dis_y; }
	MOUSE_ACTION_DATA* getMouseAct() { return &mouse_act_data; }
};
