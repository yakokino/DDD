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

			push_com = ncom;											//マウスクリックされた場所のコマンドを記憶する
			push_no = no;

			ddd_dice.actionCommand( push_com, 1000 + phase * 100 + no );		//マウスクリックされたらナンバーに1000足してコマンド実行
			ddd_card.actionCommand( push_com, 1000 + phase * 100 + no );
			actionCommand( push_com, 1000 + no );

		} else if ( pressed_frame[LEFT_CLICK] > 7 ) {	//TODO:変数化 8フレーム以上押し続けでドラッグ判定
			drag = true;
			int drag_x, drag_y;
			GetMousePoint( &drag_x, &drag_y );
		}
	} else {
		if ( pressed_frame[LEFT_CLICK] > 0 ) {
			if ( push_com != NO_COMMAND ) {
				if ( push_com == ncom && push_no == no ) {		//クリックが離された時、クリックされた時のコマンドと一致しているなら
					ddd_dice.actionCommand( push_com, 2000 + phase * 100 + no );			//ナンバーに2000足してコマンド実行
					ddd_card.actionCommand( push_com, 2000 + phase * 100 + no );
					actionCommand( push_com, 2000 + no );
				}
			}
		}
		ddd_map.actionCommand( ncom );
		ddd_dice.actionCommand( ncom, phase * 100 + no );						//マウスオーバーされた時にナンバーそのままでコマンド実行
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

	mouse_wheel = GetMouseWheelRotVol();		//マウスホイール回転量取得

	if ( push_com == MAP_VIEW ) {
		//ドラッグによるマップスクロール（仮）
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

		int sum = 0;	//未処理の数合計

		sum += player.loadFiles();
		sum += ddd_map.loadFiles();
		sum += ddd_ui.loadFiles();
		sum += ddd_dice.loadFiles();
		sum += ddd_card.loadFiles();

		if ( sum == 0 ) DDDLoader::load_rate = 1;

	} else if ( DDDLoader::load_rate == 1 ) {
		//データ読み込み
		DeckData load_data;
		FILE *fp;
		if ( fopen_s( &fp, "Deck\\deck_1p.deck", "rb" ) != 0 ) {
			throw ( -1000 );
		}
		fread( &load_data, sizeof( load_data ), 1, fp );
		fclose( fp );		//ファイルを閉じる
		ddd_card.setDeck( 0, &load_data );		//デッキ情報をプレイヤーデータにコピー
		ddd_dice.setDice( 0, &load_data );

		ddd_card.deckShuffle( 0 );

		//SetTransColor(0,0,0);
		DDDLoader::setLoadCard( 0 );		//キャラカード用データをセットする
		for ( auto card : load_data.card_no ) {
			if ( card == -1 ) break;
			DDDLoader::setLoadCard( card );		//必要なカードを全てここで読み込む
		}


		DDDLoader::load_rate = 2;

	} else if ( DDDLoader::load_rate == 2 ) {
		if ( GetASyncLoadNum() == 0 ) {
			DDDLoader::setCharactorAnim();		//キャラクターアニメの全セット

			DDDLoader::load_rate = 3;
		}
	} else if ( DDDLoader::load_rate == 3 ) {
		if ( GetASyncLoadNum() == 0 ) {



			//ddd_chara.loadFiles();
			DDDLoader::load_rate = 0;
			return 0;							//ロード終わり
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
	GetMouseWheelRotVol();	//ホイール回転量リセット
}

void DDDWorld::phaseControl()
{
	switch ( phase ) {
	case INIT_PHASE:
		//初期化フェイズ

		//player_pc = 0;
		turn_player = 0;
		player.setPlayerPC( 0 );
		//ddd_dice.setPlayerPC( 0 );
		//ddd_card.setPlayerPC( 0 );

		for ( int i = 0; i < 4; i++ ) {
			ddd_card.deckShuffle( i );			//全員のデッキをシャッフルする
		}

		//コマンド配置
		ddd_ui.setCommand( 230, 0, 800, 640, MAP_VIEW );		//マップビューコマンド配置
		for ( int i = 0; i < 5; i++ ) {
			ddd_card.drawCard( 0 );
			//手札コマンド配置
			ddd_ui.setCommand( ddd_card.getCardPosX( i ), ddd_card.getCardPosY( i ), HAND_CARD_W, HAND_CARD_H, HAND_CARD, i );
		}
		//ddd_card.drawCard( 0 );
		//ddd_card.drawCard( 0 );
		//ddd_card.drawCard( 0 );
		//ddd_card.drawCard( 0 );



		//for(int i=0 ; i<6 ; i++ ){
		//	ddd_ui.setCommand(266+ i*86, 530, 80, 110, CARD_OVER, i);						//カード選択用コマンド配置
		//}
		next_phase = STANDBY_PHASE;

		if ( next_phase == STANDBY_PHASE ) {
			phase = STANDBY_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}
		break;
	case STANDBY_PHASE:
		//スタンバイフェイズ

		if ( phase_frame > 30 ) {
			next_phase = DICEROLL_PHASE;	//とりあえずデバッグにフェイズ移行
		}

		if ( next_phase == DICEROLL_PHASE ) {
			phase = DICEROLL_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}
		break;
	case DICEROLL_PHASE:
		//ダイスロールフェイズ
		if ( phase_frame <= 1 ) {
			ddd_ui.createWindow( 300, 150, 480, 224, NORMAL_WINDOW, DICE_ROLL_WINDOW );	//ダイスロール用ウィンドウ生成
			ddd_ui.setCommand( 300, 150, 480, 224, DICE_STOP, 1 );		//ダイスロールコマンド配置
			ddd_dice.shuffleDice();	//ダイスシャッフルスタート
		}
		if ( ddd_dice.getWaitTime() == false ) {	//ダイスロールの処理が終われば
			ddd_ui.deleteWindow( DICE_ROLL_WINDOW );		//ダイスロール用ウィンドウ消去
			ddd_ui.deleteCommand( DICE_STOP );				//ダイスロールコマンド消去
			for ( int i = 0; i < 3; i++ ) {
				player.giveStock( turn_player, ddd_dice.getSymbol( i ) );	//プレイヤーデータに紋章をストックする
			}
			ddd_card.drawCard( turn_player );				//ターンプレイヤーはカードをドロー
			if ( turn_player == PlayerDataList::getPlayerPC() ) {
				int hand = ddd_card.getHand( PlayerDataList::getPlayerPC() ) - 1;
				ddd_ui.setCommand( ddd_card.getCardPosX( hand ), ddd_card.getCardPosY( hand ), HAND_CARD_W, HAND_CARD_H, HAND_CARD, hand );
			}
			next_phase = MAIN_PHASE;	//メインフェイズへ
		}
		if ( next_phase == MAIN_PHASE ) {
			phase = MAIN_PHASE;
			next_phase = NO_PHASE;
			phase_frame = 0;
		}

		break;
	case MAIN_PHASE:
		//メインフェイズ
		if ( phase_frame <= 1 ) {
			//フェイズボタンのコマンド配置
			ddd_ui.setCommand( PHASE_POSX + ATTACK_PHASE * PHASE_SPACE, PHASE_POSY, 43, 28, ATTACK_PHASE_BUTTON );
			ddd_ui.setCommand( PHASE_POSX + END_PHASE * PHASE_SPACE, PHASE_POSY, 43, 28, END_PHASE_BUTTON );
			//カード操作可能フラグをたてる
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
		//アタックフェイズ
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
		//メインフェイズ2
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
		//手札のカードコマンド位置のセット
		ddd_ui.setCommandPos( HAND_CARD, i, ddd_card.getCardPosX( i ), ddd_card.getCardPosY( i ) );
	}
	mouseCheck();										//最初にマウス情報更新
	ddd_map.setPointXY( camera_x, camera_y, nx, ny );	//マウスポインタの位置をマップ座標に変換

	ddd_map.draw( camera_x, camera_y );		//マップ表示
	ddd_chara.draw( camera_x, camera_y );		//キャラ表示
	ddd_ui.draw( phase );					//UI表示
	player.draw();							//プレイヤー情報表示
	ddd_card.draw();						//カード表示
	ddd_dice.draw();			//ダイス表示

	phase_frame++;
}

