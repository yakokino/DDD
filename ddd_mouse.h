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
	CARD_SUMMON,	// �J�[�h�̏���
	CARD_ACTIVE,	// �J�[�h�̔���
	CARD_DETAIL,	// �J�[�h�ڍ�
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
	int nx, ny;			//���݂̃|�C���^�ʒu
	int dis_x, dis_y;
	MOUSE_ACTION action;	//�N���b�N���ꂽ�A�h���b�O���Ȃǂ̃A�N�V����
	COMMAND command;
	int command_no;
	COMMAND push_command;			//�v�b�V���_�E�����ꂽ���̃R�}���h
	int push_no;					//�v�b�V���_�E�����ꂽ���̃R�}���h�i���o�[
	PHASE phase;		//�}�E�X�A�N�V���������̐��̓s���ケ���ɂ��t�F�C�Y��������
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
	static int click_x, click_y;			//�N���b�N�J�n���̈ʒu
	static int lclick_x, lclick_y;			//�O�t���[���̃}�E�X���W
	static int pressed_frames[3];			//���ꂼ��̃}�E�X�{�^����������Ă���t���[����
	static int mouse_wheel;				//�}�E�X�z�C�[����]�ʁi�O�t���[���Ƃ̍��j
	static MOUSE_ACTION_DATA mouse_data;

	static std::vector<COMMAND_DATA> command_list;	//�}�E�X�Ŕ�������ʒu�Ƃ��̃R�}���h���X�g

	static void checkCommand();	//�}�E�X�ʒu���}�E�X����\�Ȉʒu���ǂ����𔻒肵�Amouse_data��command�Ano����
	static void checkMouseState();	//mouse_data��action�Ȃǂ���

public:
	DDDMouse()
	{
		GetMousePoint( &mouse_data.nx, &mouse_data.ny );
		click_x = mouse_data.nx; click_y = mouse_data.ny;
		for ( auto &init_frame : pressed_frames ) {
			init_frame = 0;
		}
		mouse_wheel = 0;
		mouse_data.push_command = NO_COMMAND;
		mouse_data.push_no = 0;
	}
	static void getInput();				//���t���[���Ă�Ń}�E�X���͏����擾����֐�
	static void setCommand( int x, int y, int w, int h, COMMAND com, int no = 0 );		//�}�E�X����ł���ʒu��ǉ�����
	static void getCommandPos( COMMAND com, int no, int *x, int *y );							//�R�}���h�̈ʒu���擾����
	static void setCommandPos( COMMAND com, int no, int x, int y );							//�R�}���h�̈ʒu���Z�b�g�i�ύX�j����
	static void deleteCommand( COMMAND com );											//�R�}���h���폜����
	static MOUSE_ACTION_DATA* getMouseAct() { return &mouse_data; }
};
