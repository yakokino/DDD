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
	if ( DDDLoader::load_rate == 0 ) {		//TODO: load_rateのしくみが微妙なので整理必要

		int sum = 0;	//未処理の数合計

		sum += player.loadFiles();
		sum += ddd_map.loadFiles();
		sum += ddd_ui.loadFiles();
		sum += ddd_dice.loadFiles();
		sum += ddd_card.loadFiles();

		if ( sum == 0 ) DDDLoader::load_rate = 1;

	} else if ( DDDLoader::load_rate == 1 ) {
		//デッキデータ読み込み
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
	for each ( DICE_SYMBOL dice in turn_dice ) {
		dice = MOVE;
	}
	mouse_data = ddd_mouse.getMouseAct();
	camera_data = ddd_map.getCameraData();

	phase_frame = 0;
	GetMouseWheelRotVol();	//ホイール回転量リセット
}

void DDDWorld::phaseControl()
{
	switch ( phase ) {
	case INIT_PHASE:
		//初期化フェイズ

		turn_player = 0;
		player.setPlayerPC( 0 );

		for ( int i = 0; i < 4; i++ ) {
			ddd_card.deckShuffle( i );			//全員のデッキをシャッフルする
		}

		//コマンド配置
		ddd_mouse.setCommand( 230, 0, 800, 640, MAP_VIEW );		//マップビューコマンド配置
		for ( int i = 0; i < 5; i++ ) {
			ddd_card.drawCard( 0 );
			//手札コマンド配置
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
		//スタンバイフェイズ

		if ( phase_frame > 30 ) {
			next_phase = DICEROLL_PHASE;	//TODO:スタンバイフェイズにする処理の追記 とりあえずデバッグにフェイズ移行
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
			ddd_mouse.setCommand( 300, 150, 480, 224, DICE_STOP, 1 );		//ダイスロールコマンド配置
			ddd_dice.shuffleDice();	//ダイスシャッフルスタート
		}
		if ( ddd_dice.getWaitTime() == false ) {	//ダイスロールの処理が終われば
			ddd_ui.deleteWindow( DICE_ROLL_WINDOW );		//ダイスロール用ウィンドウ消去
			ddd_mouse.deleteCommand( DICE_STOP );				//ダイスロールコマンド消去
			for ( int i = 0; i < 3; i++ ) {
				player.giveStock( turn_player, ddd_dice.getSymbol( i ) );	//プレイヤーデータに紋章をストックする
			}
			ddd_card.drawCard( turn_player );				//ターンプレイヤーはカードをドロー
			if ( turn_player == PlayerDataList::getPlayerPC() ) {	//ドローしたのがプレイヤーPCの場合
				int hand_num = ddd_card.getHand( PlayerDataList::getPlayerPC() ) - 1;	//ハンド数からドローしたハンドナンバー取得
				ddd_mouse.setCommand( ddd_card.getCardPosX( hand_num ), ddd_card.getCardPosY( hand_num ), HAND_CARD_W, HAND_CARD_H, HAND_CARD, hand_num );
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
			ddd_mouse.setCommand( PHASE_POSX + ATTACK_PHASE * PHASE_SPACE, PHASE_POSY, 43, 28, ATTACK_PHASE_BUTTON );
			ddd_mouse.setCommand( PHASE_POSX + END_PHASE * PHASE_SPACE, PHASE_POSY, 43, 28, END_PHASE_BUTTON );
			//カード操作可能フラグをたてる
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
		//アタックフェイズ
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
		//メインフェイズ2
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
		//手札のカードコマンド位置のセット
		ddd_mouse.setCommandPos( HAND_CARD, i, ddd_card.getCardPosX( i ), ddd_card.getCardPosY( i ) );
	}
	ddd_mouse.getInput();		//最初にマウス情報更新

	//構造体ポインタを使ってマウスアクション関数に渡す引数を軽く
	mouse_data->phase = phase;
	mouseAction( mouse_data );
	ddd_card.mouseAction( mouse_data );
	ddd_dice.mouseAction( mouse_data );
	//ddd_chara.mouseAction( mouse_data );
	ddd_map.mouseAction( mouse_data );

	camera_data = ddd_map.getCameraData();
	

	ddd_map.draw();		//マップ表示
	ddd_chara.draw( camera_data->nx, camera_data->ny );	//キャラ表示
	ddd_ui.draw( phase );					//UI表示
	player.draw();							//プレイヤー情報表示
	ddd_card.draw();						//カード表示
	ddd_dice.draw();						//ダイス表示

	phase_frame++;
}

