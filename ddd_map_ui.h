#pragma once

#include <list>
#include "ddd_loader.h"
#include "ddd_mouse.h"
//#include "card_info.h"
#include "anim_sound.h"

//マップサイズ
const int DDDMAP_W = 20;	//マップの横マス数（左上から右下へ）
const int DDDMAP_H = 20;	//マップの縦マス数
//マップ画像サイズ
const int MAPTILE_W = 96;
const int MAPTILE_H = 48;
const int MAPTILE_UPSPACE = 7;	//マップタイル画像上部の画像がない部分のpixel数
const double MAPTILE_S = 0.5;
const int MAPTILE_HW = MAPTILE_W / 2;
const int MAPTILE_HH = MAPTILE_H / 2;
const int MAP_WAY_UP_PIXEL = 4;	//マップタイルでない場合の盛り上がっている分のpx数

//UI関連
const int DDDUI_ALPHA = 230;	//UI不透明度(0～255)
const int WINDOW_GRAPH_W = 16;	//window.bmpの分割サイズ
const int WINDOW_GRAPH_H = 16;
const int PHASE_ALPHA = 100;	//フェイズボタン不透明度
const int PHASE_POSX = 300;		//フェイズボタン位置
const int PHASE_POSY = 400;
const int PHASE_SPACE = 64;		//フェイズボタンの（左右）間隔


struct CAMERA_DATA
{
	int nx, ny;
	bool enable_scroll;
};

class DDDMap
{
	MAP_DATA map_data[DDDMAP_H][DDDMAP_W];		//マップデータ（メモリ仕様上[H],[W]の順で確保）
	int point_x, point_y;		//マウスポインタのマップ座標
	int select_col;
	AnimDraw back_anim;
	AnimDraw map_anim;
	AnimDraw mini_map;

	CAMERA_DATA camera_data;

public:
	int loadFiles() throw( ... );
	void init();
	void setPointXY( int mouse_x, int mouse_y );
	void mouseAction( MOUSE_ACTION_DATA* mouse_data );

	void setEnableScroll( bool enable_flag );
	CAMERA_DATA* getCameraData();

	void draw();
};




//ウィンドウタイプ
enum WINDOW_TYPE
{
	NORMAL_WINDOW,
	POPUP_WINDOW
};

//ウィンドウ識別タグ
enum WINDOW_TAG
{
	DICE_ROLL_WINDOW,
	YES_NO_WINDOW,
	CARD_SUMMON_WINDOW

};

struct WINDOW_DATA
{
	int x, y, h_num, w_num;
	WINDOW_TYPE type;
	WINDOW_TAG tag;
};

class DDDUI
{
	static std::vector<WINDOW_DATA> window_list;		//ウィンドウリスト
	static AnimDraw ui_anim;
	static AnimDraw window_anim;

public:
	static int loadFiles() throw( ... );
	static void createWindow( int x, int y, int w, int h, WINDOW_TYPE type, WINDOW_TAG tag );	//ウィンドウを生成する。16px単位でしかサイズが変化しない。切り捨て
	static void deleteWindow( WINDOW_TAG tag );										//ウィンドウを消去する
	static void draw( PHASE phase );
};
