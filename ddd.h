#pragma once

#include "ddd_loader.h"
#include "card_info.h"
#include "ddd_mouse.h"
#include "ddd_map_ui.h"
#include "player_data.h"
#include "ddd_dicecard.h"


class DDDChara{
	std::list<CharaData> chara_list;
	AnimDraw karireimu;
public:
	int loadFiles() throw(...);
	void init();
	void draw(int camera_x, int camera_y);
	
};

class DDDWorld{
	PlayerDataList player;			//�v���C���[�f�[�^���X�g
	DDDMouse ddd_mouse;				//�}�E�X�N���X
	DDDDice ddd_dice;				//�_�C�X�N���X
	DDDCard ddd_card;				//�J�[�h�N���X
	DDDUI ddd_ui;					//UI�N���X
	DDDMap ddd_map;					//�}�b�v�N���X
	DDDChara ddd_chara;				//�L�����N���X
	//int player_pc;	//TODO:����
	int turn_player;				//�^�[���v���C���[�i���o�[�i0�`3(1�`4P��\��)�j
	PHASE phase;					//���݂̃t�F�C�Y
	PHASE next_phase;				//���̃t�F�C�Y�i�ʏ�NO_PHASE�����A�t�F�C�Y��ύX���鎞�ɕύX������j
	DICE_SYMBOL turn_dice[3][6];	//���̃^�[���̃_�C�X���

	unsigned int phase_frame;
	//TODO: �J�����n��UI�ɓ��ꂽ�ق������������@�J��������p
	bool scroll;		//true�ŃX�N���[������
	int camera_x, camera_y;

	//void mouseCheck();				//���t���[���Ă�Ń}�E�X������͂���֐�
	void mouseAction( MOUSE_ACTION_DATA* mouse_data );

public:
	DDDWorld(){
		phase = INIT_PHASE;
		turn_player = 0;
		for(int i=0 ; i<3 ; i++ ){
			for(int j=0 ; j<6 ; j++ ){
				turn_dice[i][j] = SUMMON;
			}
		}
		scroll = true;
		camera_x = -300;
		camera_y = 0;
		//drag = false;
		//pressed_frame[0] = 0;
		//pressed_frame[1] = 0;
		//pressed_frame[2] = 0;


	}

	int loadFiles() throw(...);
	void init();
	void phaseControl();
	void draw();
};
