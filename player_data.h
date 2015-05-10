#pragma once

#include <string>
#include "chara_data.h"


const int PLAYER_DATA_X = 350;	//�v���C���[�̏��ʒu
const int PLAYER_DATA_Y = 440;
const int OTHER_PLAYER_DATA_X = 823;
const int OTHER_PLAYER_DATA_Y = 20;
const int PLAYER_DATA_SPACE = 96+16;	//�v���C���[���́i�㉺�j�Ԋu


class PlayerDataList{

	static PlayerData pd[4];		//4P���̃v���C���[�f�[�^
	static int player_pc;		//����PC�𑀍삷��v���C���[�i���o�[
	int name_fh;			//���O�\���p�t�H���g�n���h��
	int other_fh;
	AnimDraw symbol_anim;
	
public:
	PlayerDataList(){
		player_pc = 0;
		pd[0].p_name = "���R�L�m";
		pd[1].p_name = "�����~�N";
		pd[2].p_name = "��������";
		pd[3].p_name = "�������J";
		pd[0].p_no = 0;
		pd[1].p_no = 1;
		pd[2].p_no = 2;
		pd[3].p_no = 3;
	}
	int loadFiles() throw(...);
	void init();
	void setPlayerPC( int pn ){ player_pc = pn; }		//����PC�v���C���[�i���o�[���Z�b�g����
	static int getPlayerPC(){ return player_pc; }
	static void damage( int pn, int damage){ pd[pn].p_life -= damage; }
	static void heal( int pn, int heal){ pd[pn].p_life += heal; }

	static void giveStock( int p_no, DICE_SYMBOL symbol ){ pd[p_no].p_stock.symbol[symbol]++; }		//�N���X�g�v�[���ɖ�͒ǉ�
	static void expendStock( int p_no, DICE_SYMBOL symbol ){ pd[p_no].p_stock.symbol[symbol]--; }		//�N���X�g�v�[�������͏���
	static bool isEnoughSymbol( const int p_no, SYMBOL_DATA* cost );			//�R�X�g�`�F�b�N�֐��B�R�X�g������Ă����1��Ԃ�

	static DICE_SYMBOL getMyDice(int pn, int dice, int men){ return pd[pn].my_dice[dice][men]; }

	void draw();

};
