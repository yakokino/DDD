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
	static std::vector<int> sounds;		//�T�E���h�n���h�����L�^����
	static int bgm;						//���ݍĐ�����BGM�̃T�E���h�n���h��
public:
	static int set(int shandle);		//�o�^
	static void play(int snum);			//�Đ�
	static void stop(int snum);			//��~
	static void playbgm(int snum);		//BGM�̍Đ�
};

#endif