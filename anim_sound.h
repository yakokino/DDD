#pragma once

#include <vector>
#include "DxLib.h"
#include "game_frame.h"

class AnimDraw
{
	struct animpat
	{
		std::vector<int> ghlist;
		int anigap;
		animpat()	//animpat構造体のコンストラクタ
		{
			anigap = 1;
		}
	};
	std::vector<animpat> aplist;
	bool is_loaded;
public:
	AnimDraw()		//AnimDrawコンストラクタ
	{
		//aplist.push_back( animpat() );	//TODO:整理必要
		is_loaded = false;
	}

	int setImage( int apid, int ghandle );
	int setGap( int apid, int gap );
	void getSize( int apid, int *wide, int *height );
	int empty( int apid = 0 );

	void setLoadFlag( bool load_complete = true ) { is_loaded = load_complete; }
	bool getLoadFlag() { return is_loaded; }

	void draw( int apid, int x, int y );
	void draw( int apid, int x, int y, double rate );
};

class SoundBox
{
	static std::vector<int> sounds;		//サウンドハンドルを記録する
	static int bgm;						//現在再生中のBGMのサウンドハンドル
public:
	static int set( int shandle );		//登録
	static void play( int snum );			//再生
	static void stop( int snum );			//停止
	static void playbgm( int snum );		//BGMの再生
};
