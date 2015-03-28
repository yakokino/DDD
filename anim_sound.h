#ifndef __ANIMDRAW_H__
#define __ANIMDRAW_H__

#include <vector>
#include "DxLib.h"
#include "game_frame.h"

class AnimDraw{
	struct animpat{
		std::vector<int> ghlist;
		int anigap;
		animpat(){ anigap = 1; }
	};
	std::vector<animpat> aplist;
public:
	AnimDraw(){
		aplist.push_back( animpat() );
	}

	int setImage(int apid, int ghandle);
	int setGap(int apid, int gap);
	void getSize(int apid, int *wide, int *height);
	int empty(int apid = 0);
	
	void draw(int apid, int x, int y );
	void draw(int apid, int x, int y, double rate);
};

class SoundBox{
	static std::vector<int> sounds;		//サウンドハンドルを記録する
	static int bgm;						//現在再生中のBGMのサウンドハンドル
public:
	static int set(int shandle);		//登録
	static void play(int snum);			//再生
	static void stop(int snum);			//停止
	static void playbgm(int snum);		//BGMの再生
};

#endif