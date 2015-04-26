#include "ddd.h"


void DDDWorld::mouseCheck()
{
	GetMousePoint( &nx, &ny );
	int no;
	COMMAND ncom = ddd_ui.checkCommand( nx, ny, &no );

	int input = GetMouseInput();
	if ( input & MOUSE_INPUT_LEFT ) {
		if ( ++pressed_frame[LEFT_CLICK] == 1 ) {
			click_x = nx;
			click_y = ny;
			lclick_x = click_x;
			lclick_y = click_y;

			push_com = ncom;											//�}�E�X�N���b�N���ꂽ�ꏊ�̃R�}���h���L������
			push_no = no;

			ddd_dice.actionCommand( push_com, 1000 + phase * 100 + no );		//�}�E�X�N���b�N���ꂽ��i���o�[��1000�����ăR�}���h���s
			ddd_card.actionCommand( push_com, 1000 + phase * 100 + no );
			actionCommand( push_com, 1000 + no );

		} else if ( pressed_frame[LEFT_CLICK] > 7 ) {	//TODO:�ϐ��� 8�t���[���ȏ㉟�������Ńh���b�O����
			drag = true;
			int drag_x, drag_y;
			GetMousePoint( &drag_x, &drag_y );
		}
	} else {
		if ( pressed_frame[LEFT_CLICK] > 0 ) {
			if ( push_com != NO_COMMAND ) {
				if ( push_com == ncom && push_no == no ) {		//�N���b�N�������ꂽ���A�N���b�N���ꂽ���̃R�}���h�ƈ�v���Ă���Ȃ�
					ddd_dice.actionCommand( push_com, 2000 + phase * 100 + no );			//�i���o�[��2000�����ăR�}���h���s
					ddd_card.actionCommand( push_com, 2000 + phase * 100 + no );
					actionCommand( push_com, 2000 + no );
				}
			}
		}
		ddd_map.actionCommand( ncom );
		ddd_dice.actionCommand( ncom, phase * 100 + no );						//�}�E�X�I�[�o�[���ꂽ���Ƀi���o�[���̂܂܂ŃR�}���h���s
		ddd_card.actionCommand( ncom, phase * 100 + no );
		actionCommand( push_com, no );
		pressed_frame[LEFT_CLICK] = 0;
		drag = false;

	}
	if ( input & MOUSE_INPUT_RIGHT ) {
		if ( ++pressed_frame[RIGHT_CLICK] == 1 ) {
			GetMousePoint( &click_x, &click_y );
		}
	} else {
		pressed_frame[RIGHT_CLICK] = 0;
	}
	if ( input & MOUSE_INPUT_MIDDLE ) {
		if ( ++pressed_frame[MIDDLE_CLICK] == 1 ) {
			GetMousePoint( &click_x, &click_y );
		}
	} else {
		pressed_frame[MIDDLE_CLICK] = 0;
	}

	mouse_wheel = GetMouseWheelRotVol();		//�}�E�X�z�C�[����]�ʎ擾

	if ( push_com == MAP_VIEW ) {
		//�h���b�O�ɂ��}�b�v�X�N���[���i���j
		if ( pressed_frame[LEFT_CLICK] > 0 && scroll == true ) {
			int dis_x = lclick_x - nx;
			int dis_y = lclick_y - ny;
			lclick_x = nx;
			lclick_y = ny;
			camera_x += dis_x;
			camera_y += dis_y;
		}
	}

}


void DDDWorld::actionCommand( COMMAND com, int no )
{
	if ( no != 2000 ) return;
	if ( com == ATTACK_PHASE_BUTTON ) {
		next_phase = ATTACK_PHASE;
	} else if ( com == MAIN_PHASE2_BUTTON ) {
		next_phase = MAIN_PHASE2;
	} else if ( com == END_PHASE_BUTTON ) {
		next_phase = END_PHASE;
	}
}

int DDDWorld::loadFiles() throw( ... )
{
	if ( DDDLoader::load_rate == 0 ) {

		int sum = 0;	//�������̐����v

		sum += player.loadFiles();
		sum += ddd_map.loadFiles();
		sum += ddd_ui.loadFiles();
		sum += ddd_dice.loadFiles();
		sum += ddd_card.loadFiles();

		if ( sum == 0 ) DDDLoader::load_rate = 1;

	} else if ( DDDLoader::load_rate == 1 ) {
		//�f�[�^�ǂݍ���
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
	for each ( DICE_SYMBOL dice in turn_dice )
	{
		dice = MOVE;
	}


	scroll = true;
	camera_x = -300;
	camera_y = 0;
	drag = false;
	pressed_frame[0] = 0;
	pressed_frame[1] = 0;
	pressed_frame[2] = 0;
	phase_frame = 0;
	mouse_wheel = 0;
	GetMouseWheelRotVol();	//�z�C�[����]�ʃ��Z�b�g
}

void DDDWorld::phaseControl()
{
	switch ( phase ) {
	case INIT_PHASE:
		//�������t�F�C�Y

		//player_pc = 0;
		turn_player = 0;
		player.setPlayerPC( 0 );
		//ddd_dice.setPlayerPC( 0 );
		//ddd_card.setPlayerPC( 0 );

		for ( int i = 0; i < 4; i++ ) {
			ddd_card.deckShuffle( i );			//�S���̃f�b�L���V���b�t������
		}

		//�R�}���h�z�u
		ddd_ui.setCommand( 230, 0, 800, 640, MAP_VIEW );		//�}�b�v�r���[�R�}���h�z�u
		for ( int i = 0; i < 5; i++ ) {
			ddd_card.drawCard( 0 );
			//��D�R�}���h�z�u
			ddd_ui.setCommand( ddd_card.getCardPosX( i ), ddd_card.getCardPosY( i ), HAND_CARD_W, HAND_CARD_H, HAND_CARD, i );
		}
		//ddd_card.drawCard( 0 );
		//ddd_card.drawCard( 0 );
		//ddd_card.drawCard( 0 );
		//ddd_card.drawCard( 0 );



		//for(int i=0 ; i<6 ; i++ ){
		//	ddd_ui.setCommand(266+ i*86, 530, 80, 110, CARD_OVER, i);						//�J�[�h�I��p�R�}���h�z�u
		//}
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
			next_phase = DICEROLL_PHASE;	//�Ƃ肠�����f�o�b�O�Ƀt�F�C�Y�ڍs
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
			ddd_ui.setCommand( 300, 150, 480, 224, DICE_STOP, 1 );		//�_�C�X���[���R�}���h�z�u
			ddd_dice.shuffleDice();	//�_�C�X�V���b�t���X�^�[�g
		}
		if ( ddd_dice.getWaitTime() == false ) {	//�_�C�X���[���̏������I����
			ddd_ui.deleteWindow( DICE_ROLL_WINDOW );		//�_�C�X���[���p�E�B���h�E����
			ddd_ui.deleteCommand( DICE_STOP );				//�_�C�X���[���R�}���h����
			for ( int i = 0; i < 3; i++ ) {
				player.giveStock( turn_player, ddd_dice.getSymbol( i ) );	//�v���C���[�f�[�^�ɖ�͂��X�g�b�N����
			}
			ddd_card.drawCard( turn_player );				//�^�[���v���C���[�̓J�[�h���h���[
			if ( turn_player == PlayerDataList::getPlayerPC() ) {
				int hand = ddd_card.getHand( PlayerDataList::getPlayerPC() ) - 1;
				ddd_ui.setCommand( ddd_card.getCardPosX( hand ), ddd_card.getCardPosY( hand ), HAND_CARD_W, HAND_CARD_H, HAND_CARD, hand );
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
			ddd_ui.setCommand( PHASE_POSX + ATTACK_PHASE * PHASE_SPACE, PHASE_POSY, 43, 28, ATTACK_PHASE_BUTTON );
			ddd_ui.setCommand( PHASE_POSX + END_PHASE * PHASE_SPACE, PHASE_POSY, 43, 28, END_PHASE_BUTTON );
			//�J�[�h����\�t���O�����Ă�
			ddd_card.setOperate( true );
		}
		if ( next_phase == ATTACK_PHASE ) {
			ddd_ui.deleteCommand( ATTACK_PHASE_BUTTON );
			phase = ATTACK_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		} else if ( next_phase == END_PHASE ) {
			ddd_ui.deleteCommand( ATTACK_PHASE_BUTTON );
			ddd_ui.deleteCommand( END_PHASE_BUTTON );
			phase = END_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}

		break;
	case ATTACK_PHASE:
		//�A�^�b�N�t�F�C�Y
		if ( phase_frame <= 1 ) {
			ddd_ui.setCommand( PHASE_POSX + MAIN_PHASE2 * PHASE_SPACE, PHASE_POSY, 43, 28, MAIN_PHASE2_BUTTON );
		}
		if ( next_phase == MAIN_PHASE2 ) {
			ddd_ui.deleteCommand( MAIN_PHASE2_BUTTON );
			phase = MAIN_PHASE2;
			next_phase = NO_PHASE;
			phase_frame = 0;
		} else if ( next_phase == END_PHASE ) {
			ddd_ui.deleteCommand( MAIN_PHASE2_BUTTON );
			ddd_ui.deleteCommand( END_PHASE_BUTTON );
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
			ddd_ui.deleteCommand( END_PHASE_BUTTON );
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
		ddd_ui.setCommandPos( HAND_CARD, i, ddd_card.getCardPosX( i ), ddd_card.getCardPosY( i ) );
	}
	mouseCheck();										//�ŏ��Ƀ}�E�X���X�V
	ddd_map.setPointXY( camera_x, camera_y, nx, ny );	//�}�E�X�|�C���^�̈ʒu���}�b�v���W�ɕϊ�

	ddd_map.draw( camera_x, camera_y );		//�}�b�v�\��
	ddd_chara.draw( camera_x, camera_y );		//�L�����\��
	ddd_ui.draw( phase );					//UI�\��
	player.draw();							//�v���C���[���\��
	ddd_card.draw();						//�J�[�h�\��
	ddd_dice.draw();			//�_�C�X�\��

	phase_frame++;
}

