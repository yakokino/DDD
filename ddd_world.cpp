#include "ddd.h"




void DDDWorld::mouseAction( MOUSE_ACTION_DATA* mouse_data )
{
	if ( mouse_data->command == ATTACK_PHASE_BUTTON ) {
		if ( mouse_data->action == LEFT_CLICK ) {
			next_phase = ATTACK_PHASE;
		}
	} else if ( mouse_data->command == MAIN_PHASE2_BUTTON ) {
		if ( mouse_data->action == LEFT_CLICK ) {
			next_phase = MAIN_PHASE2;
		}
	} else if ( mouse_data->command == END_PHASE_BUTTON ) {
		if ( mouse_data->action == LEFT_CLICK ) {
			next_phase = END_PHASE;
		}
	}
}

int DDDWorld::loadFiles() throw( ... )
{
	if ( DDDLoader::load_rate == 0 ) {		//TODO: load_rate�̂����݂������Ȃ̂Ő����K�v

		int sum = 0;	//�������̐����v

		sum += player.loadFiles();
		sum += ddd_map.loadFiles();
		sum += ddd_ui.loadFiles();
		sum += ddd_dice.loadFiles();
		sum += ddd_card.loadFiles();

		if ( sum == 0 ) DDDLoader::load_rate = 1;

	} else if ( DDDLoader::load_rate == 1 ) {
		//�f�b�L�f�[�^�ǂݍ���
		DeckData load_data;
		FILE *fp;
		if ( fopen_s( &fp, "Deck\\deck_1p.deck", "rb" ) != 0 ) {
			throw ( -1000 );
		}
		fread( &load_data, sizeof( load_data ), 1, fp );
		fclose( fp );		//�t�@�C�������
		ddd_card.setDeck( 0, &load_data );		//�f�b�L�����v���C���[�f�[�^�ɃR�s�[
		ddd_dice.setDice( 0, &load_data );

		ddd_card.deckShuffle( 0 );

		//SetTransColor(0,0,0);
		DDDLoader::setLoadCard( 0 );		//�L�����J�[�h�p�f�[�^���Z�b�g����
		for ( auto card : load_data.card_no ) {
			if ( card == -1 ) break;
			DDDLoader::setLoadCard( card );		//�K�v�ȃJ�[�h��S�Ă����œǂݍ���
		}


		DDDLoader::load_rate = 2;

	} else if ( DDDLoader::load_rate == 2 ) {
		if ( GetASyncLoadNum() == 0 ) {
			DDDLoader::setCharactorAnim();		//�L�����N�^�[�A�j���̑S�Z�b�g

			DDDLoader::load_rate = 3;
		}
	} else if ( DDDLoader::load_rate == 3 ) {
		if ( GetASyncLoadNum() == 0 ) {



			//ddd_chara.loadFiles();
			DDDLoader::load_rate = 0;
			return 0;							//���[�h�I���
		}
	}

	return 10;
}

void DDDWorld::init()
{
	player.init();
	ddd_dice.init();
	ddd_card.init();
	ddd_map.init();
	ddd_chara.init();

	phase = INIT_PHASE;
	turn_player = 0;
	for each ( DICE_SYMBOL dice in turn_dice ) {
		dice = MOVE;
	}
	mouse_data = ddd_mouse.getMouseAct();
	camera_data = ddd_map.getCameraData();

	phase_frame = 0;
	GetMouseWheelRotVol();	//�z�C�[����]�ʃ��Z�b�g
}

void DDDWorld::phaseControl()
{
	switch ( phase ) {
	case INIT_PHASE:
		//�������t�F�C�Y

		turn_player = 0;
		player.setPlayerPC( 0 );

		for ( int i = 0; i < 4; i++ ) {
			ddd_card.deckShuffle( i );			//�S���̃f�b�L���V���b�t������
		}

		//�R�}���h�z�u
		ddd_mouse.setCommand( 230, 0, 800, 640, MAP_VIEW );		//�}�b�v�r���[�R�}���h�z�u
		for ( int i = 0; i < 5; i++ ) {
			ddd_card.drawCard( 0 );
			//��D�R�}���h�z�u
			ddd_mouse.setCommand( ddd_card.getCardPosX( i ), ddd_card.getCardPosY( i ), HAND_CARD_W, HAND_CARD_H, HAND_CARD, i );
		}

		next_phase = STANDBY_PHASE;

		if ( next_phase == STANDBY_PHASE ) {
			phase = STANDBY_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}
		break;
	case STANDBY_PHASE:
		//�X�^���o�C�t�F�C�Y

		if ( phase_frame > 30 ) {
			next_phase = DICEROLL_PHASE;	//TODO:�X�^���o�C�t�F�C�Y�ɂ��鏈���̒ǋL �Ƃ肠�����f�o�b�O�Ƀt�F�C�Y�ڍs
		}

		if ( next_phase == DICEROLL_PHASE ) {
			phase = DICEROLL_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}
		break;
	case DICEROLL_PHASE:
		//�_�C�X���[���t�F�C�Y
		if ( phase_frame <= 1 ) {
			ddd_ui.createWindow( 300, 150, 480, 224, NORMAL_WINDOW, DICE_ROLL_WINDOW );	//�_�C�X���[���p�E�B���h�E����
			ddd_mouse.setCommand( 300, 150, 480, 224, DICE_STOP, 1 );		//�_�C�X���[���R�}���h�z�u
			ddd_dice.shuffleDice();	//�_�C�X�V���b�t���X�^�[�g
		}
		if ( ddd_dice.getWaitTime() == false ) {	//�_�C�X���[���̏������I����
			ddd_ui.deleteWindow( DICE_ROLL_WINDOW );		//�_�C�X���[���p�E�B���h�E����
			ddd_mouse.deleteCommand( DICE_STOP );				//�_�C�X���[���R�}���h����
			for ( int i = 0; i < 3; i++ ) {
				player.giveStock( turn_player, ddd_dice.getSymbol( i ) );	//�v���C���[�f�[�^�ɖ�͂��X�g�b�N����
			}
			ddd_card.drawCard( turn_player );				//�^�[���v���C���[�̓J�[�h���h���[
			if ( turn_player == PlayerDataList::getPlayerPC() ) {	//�h���[�����̂��v���C���[PC�̏ꍇ
				int hand_num = ddd_card.getHand( PlayerDataList::getPlayerPC() ) - 1;	//�n���h������h���[�����n���h�i���o�[�擾
				ddd_mouse.setCommand( ddd_card.getCardPosX( hand_num ), ddd_card.getCardPosY( hand_num ), HAND_CARD_W, HAND_CARD_H, HAND_CARD, hand_num );
			}
			next_phase = MAIN_PHASE;	//���C���t�F�C�Y��
		}
		if ( next_phase == MAIN_PHASE ) {
			phase = MAIN_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}

		break;
	case MAIN_PHASE:
		//���C���t�F�C�Y
		if ( phase_frame <= 1 ) {
			//�t�F�C�Y�{�^���̃R�}���h�z�u
			ddd_mouse.setCommand( PHASE_POSX + ATTACK_PHASE * PHASE_SPACE, PHASE_POSY, 43, 28, ATTACK_PHASE_BUTTON );
			ddd_mouse.setCommand( PHASE_POSX + END_PHASE * PHASE_SPACE, PHASE_POSY, 43, 28, END_PHASE_BUTTON );
			//�J�[�h����\�t���O�����Ă�
			ddd_card.setOperate( true );
		}
		if ( next_phase == ATTACK_PHASE ) {
			ddd_mouse.deleteCommand( ATTACK_PHASE_BUTTON );
			phase = ATTACK_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		} else if ( next_phase == END_PHASE ) {
			ddd_mouse.deleteCommand( ATTACK_PHASE_BUTTON );
			ddd_mouse.deleteCommand( END_PHASE_BUTTON );
			phase = END_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}

		break;
	case ATTACK_PHASE:
		//�A�^�b�N�t�F�C�Y
		if ( phase_frame <= 1 ) {
			ddd_mouse.setCommand( PHASE_POSX + MAIN_PHASE2 * PHASE_SPACE, PHASE_POSY, 43, 28, MAIN_PHASE2_BUTTON );
		}
		if ( next_phase == MAIN_PHASE2 ) {
			ddd_mouse.deleteCommand( MAIN_PHASE2_BUTTON );
			phase = MAIN_PHASE2;
			next_phase = NO_PHASE;
			phase_frame = 0;
		} else if ( next_phase == END_PHASE ) {
			ddd_mouse.deleteCommand( MAIN_PHASE2_BUTTON );
			ddd_mouse.deleteCommand( END_PHASE_BUTTON );
			phase = END_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}
		break;
	case MAIN_PHASE2:
		//���C���t�F�C�Y2
		if ( phase_frame <= 1 ) {

		}
		if ( next_phase == END_PHASE ) {
			ddd_mouse.deleteCommand( END_PHASE_BUTTON );
			phase = END_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}
		break;
	case END_PHASE:
		if ( phase_frame > 60 ) {
			next_phase = STANDBY_PHASE;
		}
		if ( next_phase == STANDBY_PHASE ) {
			phase = STANDBY_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}

	}


}


void DDDWorld::draw()
{
	for ( int i = 0; i < ddd_card.getHand( PlayerDataList::getPlayerPC() ); i++ ) {
		//��D�̃J�[�h�R�}���h�ʒu�̃Z�b�g
		ddd_mouse.setCommandPos( HAND_CARD, i, ddd_card.getCardPosX( i ), ddd_card.getCardPosY( i ) );
	}
	ddd_mouse.getInput();		//�ŏ��Ƀ}�E�X���X�V

	//�\���̃|�C���^���g���ă}�E�X�A�N�V�����֐��ɓn���������y��
	mouse_data->phase = phase;
	mouseAction( mouse_data );
	ddd_card.mouseAction( mouse_data );
	ddd_dice.mouseAction( mouse_data );
	//ddd_chara.mouseAction( mouse_data );
	ddd_map.mouseAction( mouse_data );

	camera_data = ddd_map.getCameraData();
	

	ddd_map.draw();		//�}�b�v�\��
	ddd_chara.draw( camera_data->nx, camera_data->ny );	//�L�����\��
	ddd_ui.draw( phase );					//UI�\��
	player.draw();							//�v���C���[���\��
	ddd_card.draw();						//�J�[�h�\��
	ddd_dice.draw();						//�_�C�X�\��

	phase_frame++;
}

