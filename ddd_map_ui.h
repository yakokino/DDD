#pragma once

#include <list>
#include "ddd_mouse.h"
#include "card_info.h"
#include "anim_sound.h"

//�}�b�v�T�C�Y
const int DDDMAP_W = 20;	//�}�b�v�̉��}�X���i���ォ��E���ցj
const int DDDMAP_H = 20;	//�}�b�v�̏c�}�X��
const int MAPTILE_W = 96;
const int MAPTILE_H = 48;
const int MAPTILE_UPSPACE = 7;
const double MAPTILE_S = 0.5;
const int MAPTILE_HW = MAPTILE_W / 2;
const int MAPTILE_HH = MAPTILE_H / 2;

//UI�֘A
const int DDDUI_ALPHA = 230;	//UI�s�����x(0�`255)
const int WINDOW_GRAPH_W = 16;	//window.bmp�̕����T�C�Y
const int WINDOW_GRAPH_H = 16;
const int PHASE_ALPHA = 100;	//�t�F�C�Y�{�^���s�����x
const int PHASE_POSX = 300;		//�t�F�C�Y�{�^���ʒu
const int PHASE_POSY = 400;
const int PHASE_SPACE = 64;		//�t�F�C�Y�{�^���́i���E�j�Ԋu



class DDDMap{
	MAP_DATA map_data[DDDMAP_H][DDDMAP_W];		//�}�b�v�f�[�^�i�������d�l��[H],[W]�̏��Ŋm�ہj
	int point_x, point_y;		//�}�E�X�|�C���^�̃}�b�v���W
	bool view;					//�R�}���h��MAP_VIEW���ǂ���
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




//�E�B���h�E�^�C�v
enum WINDOW_TYPE{
	NORMAL_WINDOW,
	POPUP_WINDOW
};

//�E�B���h�E���ʃ^�O
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
	std::vector<WINDOW_DATA> window_list;		//�E�B���h�E���X�g
	//std::vector<COMMAND_DATA> command_list;	//�}�E�X�Ŕ�������ʒu�Ƃ��̃R�}���h���X�g
	AnimDraw ui_anim;
	AnimDraw window_anim;
public:
	int loadFiles() throw(...);
	void createWindow( int x, int y, int w, int h ,WINDOW_TYPE type, WINDOW_TAG tag );		//�E�B���h�E�𐶐�����
	void deleteWindow( WINDOW_TAG tag );										//�E�B���h�E����������
	//void setCommand( int x, int y, int w, int h, COMMAND com, int no = 0 );		//�}�E�X����ł���ʒu��ǉ�����
	//void getCommandPos( COMMAND com, int no, int *x, int *y );							//�R�}���h�̈ʒu���擾����
	//void setCommandPos( COMMAND com, int no, int x, int y );							//�R�}���h�̈ʒu���Z�b�g����
	//void deleteCommand( COMMAND com );											//�R�}���h���폜����
	//COMMAND checkCommand( int x, int y, int *no );	//�}�E�X�̈ʒu���}�E�X����\�Ȉʒu���ǂ����𔻒f���A�R�}���h��Ԃ�
	void draw( PHASE phase );
};
