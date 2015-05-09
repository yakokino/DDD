#pragma once

#include "card_info.h"
#include "anim_sound.h"
#include "ddd_dicecard.h"

struct CharaData{

public:

	int card_no;			//���̃J�[�h�̃J�[�h�i���o�[
	
	int max_life, life;		//�ő僉�C�t�ƌ��݃��C�t
	std::vector<ActionData> action;	//�s�����X�g
	int defense;			//���ݖh���
	int speed;				//���݈ړ���
	//�푰�H
	//�^�C�v�H
	//�F�H

	
	int point_x, point_y;	//�L�����̍��W�iDDD�}�b�v���W�j
	int direction;	//�L�����̌���
	CSTATE state;			//���݂̏�Ԉُ�

	int image_x, image_y;	//�L�����̉摜�ʒu���W
	int image_w, image_h;	//�摜�T�C�Y
	AnimDraw *anim;			//�L�����A�j���[�V����
	
	int yobi[10];			//�p�b�V�u���ʂȂǂɎg����̗\���ϐ�

};
