#include "ddd_dicecard.h"
#include "card_info.h"

void DDDCard::getCardData( int card_no, CardData *cd){
	ActionData ad;
	//(*cd).init();
	ad.init();
	(*cd).skill_list.clear();
	(*cd).card_no = card_no;
	switch( card_no ){
		case 2100:
			(*cd).card_name = "�o�C�����K���E�������J";	(*cd).card_spell = "�߂���˂邩";	(*cd).c_type = CHARACTOR;
			(*cd).cost_data.symbol[SUMMON] = 3;		(*cd).life = 5000;		(*cd).defense = 1000;	(*cd).speed = 3;		(*cd).m_type = WALK;
			(*cd).genre = TOHO;			(*cd).race = HUMAN;
			setCharactorMap(2, 3, 0, 8, P0_UP, UP, P1_RIGHT, P2_RIGHT, P2_UP, P1_LEFT, P3_LEFT, P3_UP );
			
			ad.skill_name = "�}�O��������";
			ad.skill_info = "�܂���Œ@���܂��B�e�X�g�I�ɐF�X�����Ă܂��B�F�X��ނ͂��邯��ǁA����ς肨�����߂͑�ԎY�ł��B�܂��낽�ׂ����B�Ă��Ă����ł��ƂĂ����������̂ł��B4�s�ڂ͂ǂ�������";
			ad.cost.symbol[ATTACK] = 1;	ad.cost.symbol[MOVE] = 1;		ad.d_type = DAMAGE;
			ad.power = 1500;		ad.combo = 1;	ad.cstate = CSTATE_NORMAL;
			ad.range = 1;	ad.no_range = 0;	ad.t_type = ENEMY_SIMPLE;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );
			ad.init();
			ad.skill_name = "�N�[���E�{�C�X";
			ad.skill_info = "�����܂߁A����3�}�X���̖����L�����̍U���͂�500�グ��";
			ad.cost.symbol[SPECIAL] = 2;	ad.d_type = ATTACK_UP;
			ad.power = 500;		ad.combo = 1;	ad.cstate = CSTATE_NORMAL;
			ad.range = 3;	ad.no_range = -1;	ad.t_type = FRIEND_ALL;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );
			ad.init();
			ad.skill_name = "�n�X�L�[�E�{�C�X";
			ad.skill_info = "����3�}�X���̓G�L�����̍U���͂�1000������";
			ad.cost.symbol[SPECIAL] = 2;	ad.d_type = DEFENSE_DOWN;
			ad.power = 500;		ad.combo = 1;	ad.cstate = CSTATE_NORMAL;
			ad.range = 3;	ad.no_range = -1;	ad.t_type = FRIEND_ALL;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );

			(*cd).flavor_text = "�u�H�ו���e���ɂ��Ă͂����܂���I�v";
			break;

		case 2000:
			(*cd).card_name = "2000";	(*cd).card_spell = "�ɂ���";	(*cd).c_type = CHARACTOR;
			( *cd ).cost_data.symbol[SUMMON] = 2;		( *cd ).life = 2000;		( *cd ).defense = 200;	( *cd ).speed = 2;
			setCharactorMap(2, 3, 0, 3, UP, UP, UP);
			ad.skill_name = "2000�f�o�b�O�A�^�b�N";
			ad.skill_info = "�ʏ�_���[�W";
			ad.cost.symbol[ATTACK] = 1;
			ad.power = 2000;		ad.combo = 1;	ad.cstate = CSTATE_NORMAL;
			ad.range = 2;	ad.no_range = 0;	ad.t_type = ENEMY_SIMPLE;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );

			(*cd).flavor_text = "�u2000�ԃJ�[�h�ł�����Ɓv";

			break;

		case 2001:
			(*cd).card_name = "2001";	(*cd).card_spell = "�ɂ��񂢂�";	(*cd).c_type = CHARACTOR;
			( *cd ).cost_data.symbol[SUMMON] = 1;		( *cd ).cost_data.symbol[SPECIAL] = 1;	( *cd ).life = 2000;		( *cd ).defense = 200;	( *cd ).speed = 1;
			setCharactorMap(2, 3, 0, 7, UP, UP, UP, P0_RIGHT, RIGHT, P0_LEFT, LEFT);
			ad.skill_name = "2001�f�o�b�O�A�^�b�N";
			ad.skill_info = "�ʏ�_���[�W";
			ad.cost.symbol[ATTACK] = 3;
			ad.power = 2001;		ad.combo = 2;	ad.cstate = CSTATE_NORMAL;
			ad.range = 1;	ad.no_range = 0;	ad.t_type = ENEMY_SIMPLE;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );

			(*cd).flavor_text = "�u2001�ԃJ�[�h�ł�����Ɓv";

			break;
	}
}