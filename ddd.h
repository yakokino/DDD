#ifndef __DDD_H__
#define __DDD_H__

#include "ddd_loader.h"
#include "ddd_map_ui.h"
#include "card_info.h"
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

//�}�E�X�֘A
enum MOUSE_CLICK{
	LEFT_CLICK,
	RIGHT_CLICK,
	MIDDLE_CLICK

};

class DDDWorld{
	PlayerDataList player;			//�v���C���[�f�[�^���X�g
	DDDDice ddd_dice;
	DDDCard ddd_card;
	DDDUI ddd_ui;
	//int player_pc;
	int turn_player;				//�^�[���v���C���[�i���o�[�i0�`3(1�`4P��\��)�j
	PHASE phase;					//���݂̃t�F�C�Y
	PHASE next_phase;				//���̃t�F�C�Y�i�ʏ�NO_PHASE�����A�t�F�C�Y��ύX���鎞�ɕύX������j
	DICE_SYMBOL turn_dice[3][6];	//���̃^�[���̃_�C�X���
	DDDMap ddd_map;					//�}�b�v�f�[�^
	DDDChara ddd_chara;				//�L�����f�[�^

	unsigned int phase_frame;
	//�J��������p
	bool scroll;		//true�ŃX�N���[������
	int camera_x, camera_y;
	int lclick_x, lclick_y;		//�O�t���[���̃}�E�X���W

	//�}�E�X�`�F�b�N�p
	int nx, ny;						//���݂̃|�C���^�ʒu
	int click_x,click_y;			//�N���b�N�J�n���̈ʒu
	bool drag;						//true�Ńh���b�O��������
	unsigned int pressed_frame[3];	//������Ă���t���[����
	int mouse_wheel;				//�}�E�X�z�C�[����]�ʁi�O�t���[���Ƃ̍��j
	COMMAND push_com;				//�N���b�N���ꂽ���̃R�}���h
	int push_no;					//�N���b�N���ꂽ���̃R�}���h�i���o�[

	void mouseCheck();				//���t���[���Ă�Ń}�E�X������͂���֐�
	void actionCommand(COMMAND com, int no);

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
		drag = false;
		pressed_frame[0] = 0;
		pressed_frame[1] = 0;
		pressed_frame[2] = 0;


	}

	int loadFiles() throw(...);
	void init();
	void phaseControl();
	void draw();
};

#endif