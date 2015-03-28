#ifndef __CARD_INFO_H__
#define __CARD_INFO_H__

#include <string>

//�}�E�X����ɂ��R�}���h
enum COMMAND{
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

//�R�}���h�f�[�^
struct COMMAND_DATA{
	int x1, y1;		//�R�}���h���W����
	int x2, y2;		//�R�}���h���W�E��
	int no;			//�R�}���h�i���o�[�i��D�̔��ʂ�N���b�N��񂪓���j
	COMMAND command;//�R�}���h
};

//�}�b�v�f�[�^
enum MAP_DATA{
	MAP_TILE,
	MAP_WAY1

};

//��Ԉُ�
enum CSTATE{
	CSTATE_NORMAL,	//��Ԉُ�Ȃ�
	CSTATE_POIZON,	//��
};

//�_�C�X�V���{��
enum DICE_SYMBOL{
	NO_SYMBOL = -1,	//�_�C�X�Ǘ��p
	SUMMON = 0,		//����
	ATTACK,			//�����U��
	SPECIAL,		//����U��
	MOVE,			//�ړ�
	MONEY,			//����

	PIETY,			//�M�S�i�����j
	MAGIC_STONE,	//���@�΁i�p�Y�h���j

};

//�f�b�L�f�[�^
struct DeckData{
	DICE_SYMBOL my_dice[3][6];	//�}�C�_�C�X
	int card_no[60];	//�J�[�h�i���o�[
};


//�E��̕��p��������Ƃ���
const int UP	= 0;	//(�E)�����
const int RIGHT = 1;	//�E(��)����
const int DOWN	= 2;	//(��)������
const int LEFT	= 3;	//��(��)����

//�L�����N�^�[�}�b�v�p
const int P0_UP		= 4;
const int P0_RIGHT	= 5;
const int P0_DOWN	= 6;
const int P0_LEFT	= 7;
const int P1_UP		= 8;
const int P1_RIGHT	= 9;
const int P1_DOWN	= 10;
const int P1_LEFT	= 11;
const int P2_UP		= 12;
const int P2_RIGHT	= 13;
const int P2_DOWN	= 14;
const int P2_LEFT	= 15;
const int P3_UP		= 16;
const int P3_RIGHT	= 17;
const int P3_DOWN	= 18;
const int P3_LEFT	= 19;
const int P4_UP		= 20;
const int P4_RIGHT	= 21;
const int P4_DOWN	= 22;
const int P4_LEFT	= 23;


//�J�[�h�^�C�v
enum C_TYPE{
	CHARACTOR,
	MAGIC,
	TRAP

};

//�ړ��^�C�v
enum M_TYPE{
	WALK,
	FLY
};

//�푰
enum RACE{
	HUMAN,		//�l��
	WITCH,		//����
	YOUKAI,		//�d��
	GOD,		//�_
	DEVIL,		//����
	HEALER,		//�񕜃^�C�v
	
	//MIKO,		//�ޏ�
	//BALANCE,	//�o�����X�^�C�v
};

//�W������
enum GENRE{
	ORIGINAL,
	TOHO,
	PAZZLE

};

//�X�L�����ʕ���
enum S_TYPE{
	ACTIVE,		//�A�N�e�B�u�X�L���i�A�^�b�N�t�F�C�Y�Ɉ�x�g���A�A�N�e�B�u�X�L���𕡐������Ă���L�����͈�^�[���ɂ��̂����ЂƂ����g���Ȃ��j�ԐF�\�L
	IGNITION,	//�N�����ʁi���C���t�F�C�Y�Ŏg�p�ł���B���Ɏw�肪�Ȃ���Ή��x�ł��g�p�\�j�F�\�L
	PASSIVE,	//�i�����ʁi��Ɍ��ʂ��������Ă���j�ΐF�\�L
	TRIGGER		//�U�����ʁi�����𖞂����Δ�������j���F�\�L

};


//�_���[�W�^�C�v
enum D_TYPE{
	DAMAGE,			//�ʏ�_���[�W
	THROUGH,		//�h�䖳���_���[�W
	HEAL,			//��
	BACK,			//�ʏ�_���[�W�{�m�b�N�o�b�N
	ATTACK_UP,		//�U���̓A�b�v
	ATTACK_DOWN,	//		�_�E��
	DEFENSE_UP,		//�h��̓A�b�v
	DEFENSE_DOWN,	//		�_�E��
	SPEED_UP,		//�ړ��ʃA�b�v
	SPEED_DOWN		//		�_�E��

};

//�Ώۃ^�C�v
enum T_TYPE{
	//�ʏ�˒��i�~�`�j
	ENEMY_SIMPLE,	//�˒��͈͓��̓G����Ώۂ���I��
	ENEMY_DOUBLE,	//							��I��
	FRIEND_SIMPLE,	//�˒��͈͓��̖�������Ώۂ���I��
	FRIEND_DOUBLE,	//							��I��
	ENEMY_FRIEND_SIMPLE,//�˒��͈͓��̓G�A��������Ώۂ���I��
	ENEMY_FRIEND_DOUBLE,//								��I��

	CONDITION_ENEMY_SIMPLE,	//�i�˒��͈͂Ɓj����̏����𖞂������G����ЂƂI��
	CONDITION_ENEMY_ALL,	//									�G�S��
	CONDITION_FRIEND_SIMPLE,
	CONDITION_FRIEND_ALL,
	CONDITION_ENEMY_FRIEND_ALL,

	ENEMY_ALL,		//�˒��͈͓��̓G�S��
	FRIEND_ALL,		//			�����S��
	ENEMY_FRIEND_ALL,//			�G�A�����S��

	//�����˒�
	STRAIGHT_ENEMY_SIMPLE,	//�����Ă�������ň�ԋ߂��G��i�����L�����������ꍇ�͂��̕����Ɏg���Ȃ��j
	STRAIGHT_ENEMY_ALL,		//				�̓G�S��
	STRAIGHT_FRIEND_SIMPLE,
	STRAIGHT_FRIEND_ALL,
	STRAIGHT_ENEMY_FRIEND_ALL
};

////�˒��^�C�v
//enum R_TYPE{
//	RANGE,		//�ʏ�˒��i�~�`�j
//	STRAIGHT,	//�����˒�
//};

struct ActionData{
	std::string skill_name;		//�Z��
	std::string skill_info;		//�Z�̐�����
	int cost[20];	//�R�X�g[SUMMON]==2 �ŏ������2�R�X�g
	S_TYPE s_type;	//���ʕ���
	D_TYPE d_type;	//�_���[�W�^�C�v
	int power;		//�U����
	int combo;		//�U����
	//R_TYPE r_type;	//�˒��^�C�v
	int range;		//�˒��͈́i�˒�0�ł��̃L�����̂݁j
	int no_range;	//�˒��O�͈́irange��菬�����l������B0�ŃL�����̈ʒu�ȊO���˒��ƂȂ�A-1�ŃL�����̈ʒu���˒����ƂȂ�j
	CSTATE cstate;	//��Ԉُ�
	int delay;		//�x���^�[���i�ʏ��0�ő��������j
	T_TYPE t_type;	//�Ώۃ^�C�v

	
	void init(){
		for(int i=0 ; i<20 ; i++ ){
			cost[i] = 0;
		}
		s_type = ACTIVE;
		d_type = DAMAGE;
		power = 1000;
		combo = 1;
		range = 1;
		no_range = 0;
		cstate = CSTATE_NORMAL;
		delay = 0;
		t_type = ENEMY_SIMPLE;
	}
};

//�t�F�C�Y�Ǘ��p
enum PHASE{
	NO_PHASE = -10,	//���̃t�F�C�Y�ֈڍs����ׂ̃t���O�Ɏg������
	INIT_PHASE = -1,	//�B���t�F�C�Y�B�X�^���o�C�t�F�C�Y�O�ɕK�v�ȏ�����
	STANDBY_PHASE = 0,
	DICEROLL_PHASE,
	MAIN_PHASE,
	ATTACK_PHASE,
	MAIN_PHASE2,
	END_PHASE

};

//��l�̃v���C���[�����f�[�^
struct PlayerData{
	std::string p_name;	//�v���C���[�l�[��
	int p_no;			//���̃f�[�^�̃v���C���[�i���o�[
	int p_life;			//���݃��C�t
	int indicate_life;	//�\���p���C�t
	int last_time;		//�c�莞��(frame)
	int hand;			//��D�̖���
	int dust;			//�̂ĎD�̖���
	//int deck_num;		//�f�b�L����
	//int deck_all[60];	//�f�b�L�̃J�[�h�i���o�[
	DICE_SYMBOL my_dice[3][6];	//�}�C�_�C�X���
	int p_stock[20];	//�X�g�b�N�ip_stock[SUMMON]=5�ŏ������5�X�g�b�N�j
public:
	PlayerData(){	//�v���C���[�f�[�^������
		p_name = "NO NAME";
		p_no = -1;
		p_life = 10000;
		last_time = 60*60;
		hand = 5;
		dust = 0;
		//deck_num = 0;
		//for(int i=0 ; i<60 ; i++ ){
		//	deck_all[i] = -1;
		//}
		for(int i=0 ; i<20 ; i++ ){
			p_stock[i] = 0;
		}
	}
};


#endif