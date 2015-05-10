#pragma once

#include "ddd_mouse.h"
#include "card_info.h"
#include "anim_sound.h"
#include "ddd_loader.h"
#include "player_data.h"


class DDDDice
{
	//int player_pc;				//���̃v���C���[�̃v���C���[�i���o�[
	bool wait_time;					//true�ŏ�������\���A����ł��Ȃ���Ԃɂ���
	DICE_SYMBOL dice_list[4][3][6];	//�S�_�C�X���[p_no][�H��][��]
	DICE_SYMBOL dicision_dice[3];	//���肵���_�C�X�V���{��
	bool dice_visible;				//�_�C�X�����ture�ŕ\��
	bool dice_shuffle;				//true�Ń_�C�X�V���b�t����ԁAfalse�Ń_�C�X�X�g�b�v���
	unsigned int stop_frame;		//�_�C�X���~�߂��Ă���̃t���[�������L������ϐ�
	AnimDraw dice_anim;				//�_�C�X�摜
public:
	int loadFiles() throw( ... );
	void init();
	//void setPlayerPC(int p_no){ player_pc = p_no; }
	void setDice( int p_no, DeckData *deck );
	void shuffleDice();				//�_�C�X���V���b�t����Ԃɂ���
	//void setTurnDice( int dice, int men, DICE_SYMBOL sym){ turn_dice[dice][men] = sym; }
	bool getWaitTime() { return wait_time; }
	DICE_SYMBOL getSymbol( int dice ) { return dicision_dice[dice]; }
	void mouseAction( MOUSE_ACTION_DATA* mouse_data );	//�R�}���h���s

	void draw();
};

//=================================================================================================================

const int CARD_W = 200;			//�J�[�h�̓��{�摜�̑傫��
const int CARD_H = 280;

const int HAND_X = 265;			//��D�̈�̍�����W
const int HAND_Y = 525;
const int HAND_AREA_W = 600;	//��D�̈�̉���
const double FRICTION = 0.2;	//��D�̌�����
const double REDUCE = 0.4;		//��D�̃J�[�h�̉摜�k����
const double INFO_REDUCE = 0.5;	//�L�����J�[�h���������̃C���t�H�J�[�h�k����

const int HAND_CARD_W = (int)(CARD_W * REDUCE);
const int HAND_CARD_H = (int)(CARD_H * REDUCE);

const int INFO_NAME_CX = 125;		//�J�[�h�C���t�H�̖��O�\���ʒu
const int INFO_NAME_Y = 294;

const int INFO_LINE2 = 323;
const int INFO_DEFENSE_X = 35;
const int INFO_SPEED_X = 115;
const int INFO_GENRE_X = 140;

const int INFO_LINE3 = 346;
const int INFO_HP_X = 43;
const int INFO_MAX_HP_X = 80;

const int INFO_SKILL_X = 60-52;
const int INFO_SKILL_Y = 385;
const int INFO_SKILL_SPACE = 67;

const int INFO_FLAVOR_X = 12;
const int INFO_FLAVOR_Y = 580;

enum CARD_OPERATE_MODE
{
	CARD_OPERATE_MODE_NO_OPERATE,
	CARD_OPERATE_MODE_MAIN_PHASE,
	CARD_OPERATE_MODE_ATTACK_PHASE,
	CARD_OPERATE_MODE_MAIN_PHASE_2,
	CARD_OPERATE_MODE_END_PHASE
};


struct CardData
{
	int card_no;				//�J�[�h�i���o�[
	std::string card_name;		//�J�[�h�̖��O
	std::string card_spell;		//�J�[�h���̂ӂ肪��
	SYMBOL_DATA cost_data;		//�����A�����R�X�gcost[SUMMON]==1�ŏ������1�K�v
	int map[5][5];				//���}�b�v�iX���W�͉E���Ȃ̂Œ��Ӂ@�L�����J�[�h����H�j
	C_TYPE c_type;				//�J�[�h�^�C�v
	RACE race;					//�푰�i�L�����J�[�h����j
	GENRE genre;				//�W�������i�L�����J�[�h����j
	int life;					//���C�t
	int defense;				//�h���
	M_TYPE m_type;				//�ړ��^�C�v
	int speed;					//�ړ���
	std::vector<ActionData> action_list;	//�s���f�[�^���X�g
	std::string flavor_text;	//�t���[�o�[�e�N�X�g

	CardData()
	{
		for ( int &c: cost_data.symbol) {
			c = 0;
		}
		for each ( int &m in map) {
			m = MAP_TILE;
		}
		card_name = "NO_NAME";
		card_spell = "�́[�ˁ[��";
		c_type = CHARACTOR;
		genre = ORIGINAL;
		race = HUMAN;
		life = 1000;
		defense = 0;
		m_type = WALK;
		speed = 1;
		flavor_text = "�J�[�h�f�[�^�����͂ł��ĂȂ���H";
	}
};

class DDDCard
{
	std::vector<int> deck_list[4];		//�S�v���C���[�f�b�L���X�g
	std::vector<int> hand_list[4];		//�S�v���C���[��D���X�g
	std::vector<int> dust_list[4];		//�S�v���C���[�̂ĎD���X�g
	std::vector<int> image_x;			//�\����D�̉摜�ʒuX
	int over_card;					//�}�E�X�I�[�o�[����Ă���J�[�h(��D������0�A1�A2�E�E�E)
	int card_info_no;					//���݂̃J�[�h�C���t�H�ɕ\�����Ă���J�[�h�i���o�[
	CardData card_info_data;			//�J�[�h�C���t�H�̃J�[�h�f�[�^

	CARD_OPERATE_MODE mode;						//����\�t���O�itrue�ő���\�j
	int select_card;
	int cost_enough;					//�\���p�i�R�X�g������Ă���ꍇ��1������j

	AnimDraw mini_symbol;
	AnimDraw chara_map;
	int info_name_fh;
	int info_middle_fh;
	int info_small_fh;
	int info_mini_fh;
public:
	int loadFiles() throw( ... );
	void init();
	void setDeck( int pn, DeckData *deck );
	void deckShuffle( int p_no );				//�w�肵���v���C���[�̃f�b�L���V���b�t������
	void drawCard( int p_no );					//�w�肵���v���C���[���f�b�L����J�[�h��1���h���[����
	void setCardInfo( int card_no );
	void setCharactorMap( int x, int y, int spin, int num, ... );
	void setOperate( CARD_OPERATE_MODE new_mode ) { mode = new_mode; }

	int getHand( int p_no ) { return hand_list[p_no].size(); }	//�w�肵���v���C���[�̃n���h�i��D�j���𓾂�
	int getCardPosX( int hand_no ) { return image_x.at( hand_no ); }
	int getCardPosY( int hand_no ) { return HAND_Y; }

	void getCardData( int card_no, CardData *card_data );		//card_info.cpp�ɋL�ځB�J�[�h���𓾂�

	void mouseAction( MOUSE_ACTION_DATA* mouse_data );
	void draw();

};
