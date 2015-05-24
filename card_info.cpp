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
			(*cd).card_name = "バイリンガル・巡音ルカ";	(*cd).card_spell = "めぐりねるか";	(*cd).c_type = CHARACTOR;
			(*cd).cost_data.symbol[SUMMON] = 3;		(*cd).life = 5000;		(*cd).defense = 1000;	(*cd).speed = 3;		(*cd).m_type = WALK;
			(*cd).genre = TOHO;			(*cd).race = HUMAN;
			setCharactorMap(2, 3, 0, 8, P0_UP, UP, P1_RIGHT, P2_RIGHT, P2_UP, P1_LEFT, P3_LEFT, P3_UP );
			
			ad.skill_name = "マグロたたき";
			ad.skill_info = "まぐろで叩きます。テスト的に色々書いてます。色々種類はあるけれど、やっぱりおすすめは大間産です。まぐろたべたい。焼いても生でもとてもおいしいのです。4行目はどうかしら";
			ad.cost.symbol[ATTACK] = 1;	ad.cost.symbol[MOVE] = 1;		ad.d_type = DAMAGE;
			ad.power = 1500;		ad.combo = 1;	ad.cstate = CSTATE_NORMAL;
			ad.range = 1;	ad.no_range = 0;	ad.t_type = ENEMY_SIMPLE;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );
			ad.init();
			ad.skill_name = "クール・ボイス";
			ad.skill_info = "自分含め、周囲3マス内の味方キャラの攻撃力を500上げる";
			ad.cost.symbol[SPECIAL] = 2;	ad.d_type = ATTACK_UP;
			ad.power = 500;		ad.combo = 1;	ad.cstate = CSTATE_NORMAL;
			ad.range = 3;	ad.no_range = -1;	ad.t_type = FRIEND_ALL;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );
			ad.init();
			ad.skill_name = "ハスキー・ボイス";
			ad.skill_info = "周囲3マス内の敵キャラの攻撃力を1000下げる";
			ad.cost.symbol[SPECIAL] = 2;	ad.d_type = DEFENSE_DOWN;
			ad.power = 500;		ad.combo = 1;	ad.cstate = CSTATE_NORMAL;
			ad.range = 3;	ad.no_range = -1;	ad.t_type = FRIEND_ALL;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );

			(*cd).flavor_text = "「食べ物を粗末にしてはいけません！」";
			break;

		case 2000:
			(*cd).card_name = "2000";	(*cd).card_spell = "にせん";	(*cd).c_type = CHARACTOR;
			( *cd ).cost_data.symbol[SUMMON] = 2;		( *cd ).life = 2000;		( *cd ).defense = 200;	( *cd ).speed = 2;
			setCharactorMap(2, 3, 0, 3, UP, UP, UP);
			ad.skill_name = "2000デバッグアタック";
			ad.skill_info = "通常ダメージ";
			ad.cost.symbol[ATTACK] = 1;
			ad.power = 2000;		ad.combo = 1;	ad.cstate = CSTATE_NORMAL;
			ad.range = 2;	ad.no_range = 0;	ad.t_type = ENEMY_SIMPLE;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );

			(*cd).flavor_text = "「2000番カードですよっと」";

			break;

		case 2001:
			(*cd).card_name = "2001";	(*cd).card_spell = "にせんいち";	(*cd).c_type = CHARACTOR;
			( *cd ).cost_data.symbol[SUMMON] = 1;		( *cd ).cost_data.symbol[SPECIAL] = 1;	( *cd ).life = 2000;		( *cd ).defense = 200;	( *cd ).speed = 1;
			setCharactorMap(2, 3, 0, 7, UP, UP, UP, P0_RIGHT, RIGHT, P0_LEFT, LEFT);
			ad.skill_name = "2001デバッグアタック";
			ad.skill_info = "通常ダメージ";
			ad.cost.symbol[ATTACK] = 3;
			ad.power = 2001;		ad.combo = 2;	ad.cstate = CSTATE_NORMAL;
			ad.range = 1;	ad.no_range = 0;	ad.t_type = ENEMY_SIMPLE;
			ad.delay = 0;
			(*cd).skill_list.push_back( ad );

			(*cd).flavor_text = "「2001番カードですよっと」";

			break;
	}
}