#pragma once

#include <list>
#include "ddd_loader.h"
#include "ddd_mouse.h"
//#include "card_info.h"
#include "anim_sound.h"

//�}�b�v�T�C�Y
const int DDDMAP_W = 20;	//�}�b�v�̉��}�X���i���ォ��E���ցj
const int DDDMAP_H = 20;	//�}�b�v�̏c�}�X��
//�}�b�v�摜�T�C�Y
const int MAPTILE_W = 96;
const int MAPTILE_H = 48;
const int MAPTILE_UPSPACE = 7;	//�}�b�v�^�C���摜�㕔�̉摜���Ȃ�������pixel��
const double MAPTILE_S = 0.5;
const int MAPTILE_HW = MAPTILE_W / 2;
const int MAPTILE_HH = MAPTILE_H / 2;
const int MAP_WAY_UP_PIXEL = 4;	//�}�b�v�^�C���łȂ��ꍇ�̐���オ���Ă��镪��px��

//UI�֘A
const int DDDUI_ALPHA = 230;	//UI�s�����x(0�`255)
const int WINDOW_GRAPH_W = 16;	//window.bmp�̕����T�C�Y
const int WINDOW_GRAPH_H = 16;
const int PHASE_ALPHA = 100;	//�t�F�C�Y�{�^���s�����x
const int PHASE_POSX = 300;		//�t�F�C�Y�{�^���ʒu
const int PHASE_POSY = 400;
const int PHASE_SPACE = 64;		//�t�F�C�Y�{�^���́i���E�j�Ԋu


struct CAMERA_DATA
{
	int nx, ny;
	bool enable_scroll;
};

class DDDMap
{
	MAP_DATA map_data[DDDMAP_H][DDDMAP_W];		//�}�b�v�f�[�^�i�������d�l��[H],[W]�̏��Ŋm�ہj
	int point_x, point_y;		//�}�E�X�|�C���^�̃}�b�v���W
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




//�E�B���h�E�^�C�v
enum WINDOW_TYPE
{
	NORMAL_WINDOW,
	POPUP_WINDOW
};

//�E�B���h�E���ʃ^�O
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
	static std::vector<WINDOW_DATA> window_list;		//�E�B���h�E���X�g
	static AnimDraw ui_anim;
	static AnimDraw window_anim;

public:
	static int loadFiles() throw( ... );
	static void createWindow( int x, int y, int w, int h, WINDOW_TYPE type, WINDOW_TAG tag );	//�E�B���h�E�𐶐�����B16px�P�ʂł����T�C�Y���ω����Ȃ��B�؂�̂�
	static void deleteWindow( WINDOW_TAG tag );										//�E�B���h�E����������
	static void draw( PHASE phase );
};
