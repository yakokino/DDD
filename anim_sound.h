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
		animpat()	//animpat�\���̂̃R���X�g���N�^
		{
			anigap = 1;
		}
	};
	std::vector<animpat> aplist;
	bool is_loaded;
public:
	AnimDraw()		//AnimDraw�R���X�g���N�^
	{
		//aplist.push_back( animpat() );	//TODO:�����K�v
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
	static std::vector<int> sounds;		//�T�E���h�n���h�����L�^����
	static int bgm;						//���ݍĐ�����BGM�̃T�E���h�n���h��
public:
	static int set( int shandle );		//�o�^
	static void play( int snum );			//�Đ�
	static void stop( int snum );			//��~
	static void playbgm( int snum );		//BGM�̍Đ�
};
