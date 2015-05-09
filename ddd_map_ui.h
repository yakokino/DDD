#pragma once

#include <list>
#include "ddd_mouse.h"
#include "card_info.h"
#include "anim_sound.h"

//マップサイズ
const int DDDMAP_W = 20;	//マップの横マス数（左上から右下へ）
const int DDDMAP_H = 20;	//マップの縦マス数
const int MAPTILE_W = 96;
const int MAPTILE_H = 48;
const int MAPTILE_UPSPACE = 7;
const double MAPTILE_S = 0.5;
const int MAPTILE_HW = MAPTILE_W / 2;
const int MAPTILE_HH = MAPTILE_H / 2;

//UI関連
const int DDDUI_ALPHA = 230;	//UI不透明度(0～255)
const int WINDOW_GRAPH_W = 16;	//window.bmpの分割サイズ
const int WINDOW_GRAPH_H = 16;
const int PHASE_ALPHA = 100;	//フェイズボタン不透明度
const int PHASE_POSX = 300;		//フェイズボタン位置
const int PHASE_POSY = 400;
const int PHASE_SPACE = 64;		//フェイズボタンの（左右）間隔



class DDDMap{
	MAP_DATA map_data[DDDMAP_H][DDDMAP_W];		//マップデータ（メモリ仕様上[H],[W]の順で確保）
	int point_x, point_y;		//マウスポインタのマップ座標
	bool view;					//コマンドがMAP_VIEWかどうか
	int select_col;
	AnimDraw back_anim;
	AnimDraw map_anim;
	AnimDraw mini_map;
public:
	int loadFiles() throw(...);
	void init();
	void setPointXY(int camera_x, int camera_y, int mouse_x, int mouse_y);
	int getPointX(){ return point_x; }
	int getPointY(){ return point_y; }
	void mouseAction( MOUSE_ACTION_DATA* mouse_data );
	void draw(int camera_x, int camera_y);
};




//ウィンドウタイプ
enum WINDOW_TYPE{
	NORMAL_WINDOW,
	POPUP_WINDOW
};

//ウィンドウ識別タグ
enum WINDOW_TAG{
	DICE_ROLL_WINDOW,
	YES_NO_WINDOW
};


class DDDUI{
	struct WINDOW_DATA{
		int x, y, h_num, w_num;
		WINDOW_TYPE type;
		WINDOW_TAG tag;
	};
	std::vector<WINDOW_DATA> window_list;		//ウィンドウリスト
	//std::vector<COMMAND_DATA> command_list;	//マウスで反応する位置とそのコマンドリスト
	AnimDraw ui_anim;
	AnimDraw window_anim;
public:
	int loadFiles() throw(...);
	void createWindow( int x, int y, int w, int h ,WINDOW_TYPE type, WINDOW_TAG tag );		//ウィンドウを生成する
	void deleteWindow( WINDOW_TAG tag );										//ウィンドウを消去する
	//void setCommand( int x, int y, int w, int h, COMMAND com, int no = 0 );		//マウス操作できる位置を追加する
	//void getCommandPos( COMMAND com, int no, int *x, int *y );							//コマンドの位置を取得する
	//void setCommandPos( COMMAND com, int no, int x, int y );							//コマンドの位置をセットする
	//void deleteCommand( COMMAND com );											//コマンドを削除する
	//COMMAND checkCommand( int x, int y, int *no );	//マウスの位置がマウス操作可能な位置かどうかを判断し、コマンドを返す
	void draw( PHASE phase );
};
