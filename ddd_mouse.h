#pragma once

#include <vector>
#include "DxLib.h"
#include "card_info.h"

//�}�E�X�{�^��
enum MOUSE_BUTTON
{
	MOUSE_LEFT		= 0,
	MOUSE_RIGHT		= 1,
	MOUSE_MIDDLE	= 2
};

//�}�E�X����ɂ��R�}���h
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
	OVER,				//�}�E�X�I�[�o�[
	LEFT_PUSH_DOWN,		//���}�E�X�{�^���������ꂽ�u��
	LEFT_PUSH_UP,		//���}�E�X�{�^���������ꂽ�u��
	LEFT_CLICK,			//���}�E�X�{�^����������Ă��������ꂽ
	LEFT_DRAG,			//���}�E�X�{�^�����������ςȂ�
	RIGHT_PUSH_DOWN,	//�E�}�E�X�{�^���������ꂽ�u��
	RIGHT_PUSH_UP,		//�E�}�E�X�{�^���������ꂽ�u��
	RIGHT_CLICK,		//�E�}�E�X�{�^����������Ă��������ꂽ
	RIGHT_DRAG,			//�E�}�E�X�{�^�����������ςȂ�
	MIDDLE_PUSH_DOWN,	//�}�E�X�z�C�[���{�^���������ꂽ�u��
	MIDDLE_PUSH_UP,		//�}�E�X�z�C�[���{�^���������ꂽ�u��
	MIDDLE_CLICK,		//�}�E�X�z�C�[���{�^����������Ă��������ꂽ
	MIDDLE_DRAG			//�}�E�X�z�C�[���{�^�����������ςȂ�
};

//�}�E�X�A�N�V���������s����ׂɓn�����
struct MOUSE_ACTION_DATA
{
	MOUSE_ACTION action;
	COMMAND command;
	int command_no;
	PHASE phase;		//�}�E�X�A�N�V���������̓s���ケ���ɂ��t�F�C�Y��������
};



//�R�}���h�f�[�^
struct COMMAND_DATA
{
	int x1, y1;		//�R�}���h���W����
	int x2, y2;		//�R�}���h���W�E��
	int no;			//�R�}���h�i���o�[�i��D�̔��ʂ�N���b�N��񂪓���j
	COMMAND command;//�R�}���h
};


class DDDMouse
{
	int nx, ny;						//���݂̃|�C���^�ʒu
	int click_x, click_y;			//�N���b�N�J�n���̈ʒu
	int lclick_x, lclick_y;			//�O�t���[���̃}�E�X���W
	int dis_x, dis_y;				//�O�t���[���Ƃ̍�
	int pressed_frames[3];			//���ꂼ��̃}�E�X�{�^����������Ă���t���[����
	int mouse_wheel;				//�}�E�X�z�C�[����]�ʁi�O�t���[���Ƃ̍��j
	COMMAND push_com;				//�v�b�V���_�E�����ꂽ���̃R�}���h
	int push_no;					//�v�b�V���_�E�����ꂽ���̃R�}���h�i���o�[
	//COMMAND ncom;					//���݂̃R�}���h
	//int nno;						//���݂̃R�}���h�i���o�[
	//MOUSE_ACTION mouse_act;			//�}�E�X�A�N�V�����i�N���b�N�A�h���b�O�Ȃǁj
	MOUSE_ACTION_DATA mouse_act_data;

	std::vector<COMMAND_DATA> command_list;	//�}�E�X�Ŕ�������ʒu�Ƃ��̃R�}���h���X�g

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
	void getInput();				//���t���[���Ă�Ń}�E�X���͏����擾����֐�
	void setCommand( int x, int y, int w, int h, COMMAND com, int no = 0 );		//�}�E�X����ł���ʒu��ǉ�����
	void getCommandPos( COMMAND com, int no, int *x, int *y );							//�R�}���h�̈ʒu���擾����
	void setCommandPos( COMMAND com, int no, int x, int y );							//�R�}���h�̈ʒu���Z�b�g�i�ύX�j����
	void deleteCommand( COMMAND com );											//�R�}���h���폜����
	COMMAND checkCommand( int x, int y, int *no );	//�}�E�X�̈ʒu���}�E�X����\�Ȉʒu���ǂ����𔻒肵�A�R�}���h��Ԃ�
	int getNowX() { return nx; }		//���݂̃|�C���^�ʒu�𓾂�
	int getNowY() { return ny; }
	int getDisX() { return dis_x; }		//�|�C���^�ʒu�̑O�t���[���Ƃ̍��𓾂�
	int getDisY() { return dis_y; }
	MOUSE_ACTION_DATA* getMouseAct() { return &mouse_act_data; }
};
