#include "ddd_dicecard.h"

int DDDDice::loadFiles()
{
	int last = 0;		//残り処理数
	int chk;
	int gh[15];
	if ( dice_anim.empty() == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\dice.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\dice.bmp", 15, 5, 3, 64, 64, gh );
		if ( chk == -1 ) throw( -4 );					//非同期読み込みがエラーならthrow
		else if ( chk >= 0 ) {							//非同期読み込みが終わっていればアニメーションに追加
			dice_anim.setImage( SUMMON, gh[0] );		//入れる順に注意
			dice_anim.setImage( ATTACK, gh[1] );
			dice_anim.setImage( SPECIAL, gh[2] );
			dice_anim.setImage( MOVE, gh[3] );
			dice_anim.setImage( MONEY, gh[4] );
			dice_anim.setImage( PIETY, gh[5] );
			dice_anim.setImage( MAGIC_STONE, gh[6] );
		} else last++;									//非同期読み込み中なら残り処理＋１
	}
	return last;
}

void DDDDice::init()
{
	wait_time = true;
	dice_visible = false;
	dice_shuffle = false;
	stop_frame = 0;
	for ( auto &d_dice : dicision_dice ) {
		d_dice = NO_SYMBOL;
	}
}

void DDDDice::setDice( int pn, DeckData *deck )
{
	for ( int i = 0; i < 3; i++ ) {
		for ( int j = 0; j < 6; j++ ) {
			dice_list[pn][i][j] = deck->my_dice[i][j];
		}
	}
}

void DDDDice::shuffleDice()
{
	dice_visible = true;
	dice_shuffle = true;
	wait_time = true;
}


void DDDDice::mouseAction( MOUSE_ACTION_DATA* mouse_data )
{
	if ( mouse_data->command == DICE_STOP ) {
		//ダイスロール
		if ( mouse_data->phase == DICEROLL_PHASE && mouse_data->action == LEFT_CLICK ) {
			int player_pc = PlayerDataList::getPlayerPC();
			if ( dice_shuffle == false ) return;
			dicision_dice[0] = dice_list[player_pc][0][GetRand( 5 )];
			dicision_dice[1] = dice_list[player_pc][1][GetRand( 5 )];
			dicision_dice[2] = dice_list[player_pc][2][GetRand( 5 )];
			dice_shuffle = false;
			stop_frame = 0;
		}
	}
}

void DDDDice::draw()
{
	if ( dice_visible == true ) {
		//ダイス表示
		if ( dice_shuffle == true ) {
			for ( int i = 0; i < 3; i++ ) {
				dice_anim.draw( dice_list[PlayerDataList::getPlayerPC()][i][GetRand( 5 )], 380 + i * 128, 300 - 64 );
			}
		} else {
			for ( int i = 0; i < 3; i++ ) {
				dice_anim.draw( dicision_dice[i], 380 + i * 128, 300 - 64 );
			}
			if ( stop_frame > 60 ) {
				dice_visible = false;	//ダイスが止まってから60Fで不可視にする	
				wait_time = false;
			}
			stop_frame++;
		}
	}

	//SetFontSize( 15 ) ;
	//SetFontThickness( 2 ) ;
	//ChangeFontType( DX_FONTTYPE_NORMAL );
	//DrawString( 250 , 240, "X123456789このカードはカードの効果によって破壊されない" ,GetColor(255,255,255));

}

//============================================================================================================

int DDDCard::loadFiles()
{

	int last = 0;		//残り処理数
	int chk;
	int gh[20];
	if ( mini_symbol.empty() == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\symbol.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\symbol.bmp", 15, 5, 3, 32, 32, gh );
		if ( chk == -1 ) throw( -5 );						//非同期読み込みがエラーならthrow
		else if ( chk >= 0 ) {							//非同期読み込みが終わっていればアニメーションに追加
			for ( int i = 0; i < 20; i++ ) {
				mini_symbol.setImage( i, gh[i] );
			}
		} else last++;									//非同期読み込み中なら残り処理＋１
	}
	if ( chara_map.empty() == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\map_mini2.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\map_mini2.bmp", 10, 10, 1, 9, 5, gh );
		if ( chk == -1 ) throw( -2 );
		else if ( chk >= 0 ) {
			for ( int i = 0; i < 10; i++ ) {
				chara_map.setImage( i, gh[i] );
			}
		} else last++;
	}
	return last;

}

void DDDCard::init()
{
	over_card = -1;
	card_info_no = -1;
	select_card = -1;
	mode = CARD_OPERATE_MODE_NO_OPERATE;
	info_name_fh = CreateFontToHandle( "ＭＳ Ｐゴシック", 17, 10, DX_FONTTYPE_ANTIALIASING );
	info_middle_fh = CreateFontToHandle( "ＭＳ Ｐゴシック", 16, 6, DX_FONTTYPE_ANTIALIASING );
	info_small_fh = CreateFontToHandle( "ＭＳ Ｐゴシック", 12, 1, DX_FONTTYPE_ANTIALIASING );
	//CreateFontToHandle( "ＭＳ Ｐゴシック", 10, 5, DX_FONTTYPE_NORMAL );
	info_mini_fh = CreateFontToHandle( "ＭＳ Ｐゴシック", 12, 5, DX_FONTTYPE_NORMAL, -1, -1, TRUE );
}

//デッキデータをセットする（マイダイスデータも含む）
void DDDCard::setDeck( int pn, DeckData *deck )
{
	for ( auto card_no : deck->card_no ) {
		if ( card_no == -1 ) return;
		deck_list[pn].push_back( card_no );
	}
}

void DDDCard::deckShuffle( int p_no )
{
	std::vector<int>::iterator it = deck_list[p_no].begin();
	while ( it != deck_list[p_no].end() ) {
		int rand = GetRand( deck_list[p_no].size() - 1 );
		int buff = ( *it );
		( *it ) = deck_list[p_no].at( rand );
		deck_list[p_no].at( rand ) = buff;
		it++;
	}
}

void DDDCard::drawCard( int p_no )
{
	hand_list[p_no].push_back( deck_list[p_no].back() );
	image_x.push_back( 1000 );
	deck_list[p_no].pop_back();
}

void DDDCard::setCardInfo( int card_no )
{
	if ( card_info_no != card_no ) {
		card_info_no = card_no;
		getCardData( card_no, &card_info_data );
	}
}

void DDDCard::setCharactorMap( int x, int y, int spin, int num, ... )
{
	va_list list;
	va_start( list, num );
	int cur_x, cur_y;
	switch ( spin % 4 ) {
	case 0:
		cur_x = x;
		cur_y = y;
		break;
	case 1:
		cur_x = 4 - y;
		cur_y = x;
		break;
	case 2:
		cur_x = 4 - x;
		cur_y = 4 - y;
		break;
	case 3:
		cur_x = y;
		cur_y = 4 - x;
		break;
	}
	//キャラが最初に立っているマスを設置
	card_info_data.map[cur_y][cur_x] = MAP_WAY_GREEN + 1000;
	int p_x[5] = { -1, -1, -1, -1, -1 };		//分岐点登録用配列
	int p_y[5] = { -1, -1, -1, -1, -1 };
	for ( int i = 0; i < num; i++ ) {
		int buff = va_arg( list, int );
		int dir = buff % 4 + spin;
		dir %= 4;
		int point = buff / 4 - 1;
		if ( point >= 0 ) {
			if ( p_x[point] == -1 ) {
				//分岐点が設定されていなければ
				p_x[point] = cur_x;
				p_y[point] = cur_y;
			} else {
				//分岐点が設定されていればカレント位置をずらす
				cur_x = p_x[point];
				cur_y = p_y[point];
			}
		}
		switch ( dir ) {
		case UP:
			cur_y--;
			break;
		case RIGHT:
			cur_x++;
			break;
		case DOWN:
			cur_y++;
			break;
		case LEFT:
			cur_x--;
			break;
		}
		//カーソルの位置に道追加
		if ( cur_x >= 0 && cur_x < 5 && cur_y >= 0 && cur_y < 5 ) {
			card_info_data.map[cur_y][cur_x] = MAP_WAY_GREEN;
		} else {
			break;
		}
	}
	va_end( list );
}

void DDDCard::setMode( CARD_OPERATE_MODE new_mode )
{
	if ( new_mode != mode ) {
		initMode( new_mode );
		mode = new_mode;
	}
}

void DDDCard::initMode( CARD_OPERATE_MODE new_mode )
{
	auto mouse_data = DDDMouse::getMouseAct();

	// モード終了時の処理
	switch ( mode ) {
	case CARD_OPERATE_MODE_NO_OPERATE:

		break;
	case CARD_OPERATE_MODE_MAIN_PHASE_SELECT_CARD:
		DDDUI::deleteWindow( CARD_SUMMON_WINDOW );
		select_card = -1;
		break;
	case CARD_OPERATE_MODE_ATTACK_PHASE:

		break;
	case CARD_OPERATE_MODE_END_PHASE:

		break;
	}
	// モード開始時の処理
	switch ( new_mode ) {
	case CARD_OPERATE_MODE_NO_OPERATE:

		break;
	case CARD_OPERATE_MODE_MAIN_PHASE_SELECT_CARD:
		DDDUI::createWindow( mouse_data->nx - 80, mouse_data->ny - 16*8, 160, 16*10, NORMAL_WINDOW, CARD_SUMMON_WINDOW );
		select_card = over_card;		// 選択カードを代入
		//DDDMouse::setCommand
		break;
	case CARD_OPERATE_MODE_ATTACK_PHASE:

		break;
	case CARD_OPERATE_MODE_END_PHASE:

		break;
	}
}

void DDDCard::mouseAction( MOUSE_ACTION_DATA* mouse_data )
{
	if ( mouse_data->command == HAND_CARD ) {
		// 手札関連
		over_card = mouse_data->command_no;
		if ( mouse_data->action == OVER ) {
			// 手札にマウスオーバー
			setCardInfo( hand_list[PlayerDataList::getPlayerPC()].at( over_card ) );	//カードインフォをセット
		} else if ( mouse_data->action == LEFT_CLICK ) {
			// 手札をクリック
			if ( mode == CARD_OPERATE_MODE_NO_OPERATE ) {
				if ( mouse_data->phase == MAIN_PHASE || mouse_data->phase == MAIN_PHASE2 ) {
					setMode( CARD_OPERATE_MODE_MAIN_PHASE_SELECT_CARD );	// 操作モード変更
				}
			}
		}

	} else if ( mouse_data->command == CARD_SUMMON ) {
		// 召喚コマンド関連
		if ( mouse_data->action == OVER ) {
			// 召喚コマンドにマウスオーバー

		} else if ( mouse_data->action == LEFT_CLICK ) {
			// 召喚コマンドクリック

		}

	} else {
		// カード関連以外
		over_card = -1;
		if ( mouse_data->action == LEFT_CLICK ) {
			// カード関連以外のコマンドをクリック
			if ( mode == CARD_OPERATE_MODE_MAIN_PHASE_SELECT_CARD ) {
				setMode( CARD_OPERATE_MODE_NO_OPERATE );
			}
		}
	}

	if ( mode == CARD_OPERATE_MODE_MAIN_PHASE_SELECT_CARD ) {
		setCardInfo( hand_list[PlayerDataList::getPlayerPC()].at( select_card ) );
	}

}

void DDDCard::draw()
{
	int x2, y2;
	int wide, height;

	//デッキ表示

	//魔法、罠カード表示


	//カードインフォ表示
	if ( card_info_no != -1 ) {
		GetGraphSize( DDDLoader::getCardHandle( card_info_no ), &wide, &height );
		x2 = wide;
		y2 = height;
		//画像表示
		if ( card_info_data.c_type == CHARACTOR ) {		//キャラカードだったら縮小表示
			//SetDrawMode( DX_DRAWMODE_BILINEAR );
			x2 = (int)( x2 * INFO_REDUCE );
			y2 = (int)( y2 * INFO_REDUCE );
			DrawGraph( 0, 0, DDDLoader::getCardHandle( 0 ), TRUE );
			DrawExtendGraph( 0, 0, x2, y2, DDDLoader::getCardHandle( card_info_no ), TRUE );
		} else {
			DrawGraph( 0, 0, DDDLoader::getCardHandle( card_info_no ), TRUE );
		}
		//召喚、発動コスト表示
		int c = 0;
		for ( int i = 0; i < 20; i++ ) {
			if ( card_info_data.cost_data.symbol[i] != 0 ) {
				for ( int j = 0; j < card_info_data.cost_data.symbol[i]; j++ ) {
					mini_symbol.draw( i, 5 + c * 16, 5, 0.5 );
					c++;
				}
			}
		}
		//名前表示
		int str_width = GetDrawStringWidthToHandle( card_info_data.card_name.c_str(), strlen( card_info_data.card_name.c_str() ), info_name_fh );
		DrawFormatStringToHandle( INFO_NAME_CX - str_width / 2, INFO_NAME_Y, 0, info_name_fh, card_info_data.card_name.c_str() );
		//防御力表示、移動量表示
		DrawFormatStringToHandle( INFO_DEFENSE_X, INFO_LINE2, 0, info_middle_fh, "%d", card_info_data.defense );
		DrawFormatStringToHandle( INFO_SPEED_X, INFO_LINE2, 0, info_middle_fh, "%d", card_info_data.speed );
		//ジャンル、種族表示
		std::string str;
		switch ( card_info_data.genre ) {
		case ORIGINAL:
			str = "DDD"; break;
		case TOHO:
			str = "東方"; break;
		case PAZZLE:
			str = "パズドラ"; break;
		}
		switch ( card_info_data.race ) {
		case HUMAN:
			str += "/人間"; break;
		case WITCH:
			str += "/魔女"; break;
		case YOUKAI:
			str += "/妖怪"; break;
		case GOD:
			str += "/ 神 "; break;
		case DEVIL:
			str += "/悪魔"; break;
		case HEALER:
			str += "/回復"; break;
		}
		DrawFormatStringToHandle( INFO_GENRE_X, INFO_LINE2, 0, info_middle_fh, "%s", str.c_str() );
		//HP表示
		DrawFormatStringToHandle( INFO_HP_X, INFO_LINE3, 0, info_middle_fh, "%d", card_info_data.life );
		DrawStringToHandle( INFO_MAX_HP_X, INFO_LINE3, "/", 0, info_middle_fh );
		DrawFormatStringToHandle( INFO_MAX_HP_X + 10, INFO_LINE3, 0, info_middle_fh, "%d", card_info_data.life );
		//技コスト、技名、技説明表示
		std::vector<ActionData>::iterator it = card_info_data.skill_list.begin();
		int count = 0;
		while ( it != card_info_data.skill_list.end() ) {
			//技コスト表示
			c = 0;
			for ( int one_cost : ( *it ).cost.symbol ) {
				if ( one_cost > 0 ) {
					for ( int j = 0; j < one_cost; j++ ) {
						mini_symbol.draw( one_cost, INFO_SKILL_X + c * 16, INFO_SKILL_Y + count * 67, 0.5 );
						c++;
					}
				}
			}
			//技名表示
			DrawFormatStringToHandle( INFO_SKILL_X + 52, INFO_SKILL_Y + count * 67, 0, info_middle_fh, "%s", ( *it ).skill_name.c_str() );
			//技説明表示
			int size = ( *it ).skill_info.length();
			int line = 0;
			int s_pos = 0;
			int i = 0;
			while ( i < size ) {
				int buff = 0;
				unsigned char c = ( *it ).skill_info.at( i );
				if ( ( ( c >= 0x81 ) && ( c <= 0x9f ) ) || ( ( c >= 0xe0 ) && ( c <= 0xfc ) ) ) {		//マルチバイト文字かどうか
					i += 2;
					buff = 2;
				} else {
					do {			//半角文字が終わるまでループ（半角文字中に改行されないように）
						i++;
						buff++;
						if ( i >= size ) break;
						c = ( *it ).skill_info.at( i );
					} while ( !( ( c >= 0x81 ) && ( c <= 0x9f ) ) || ( ( c >= 0xe0 ) && ( c <= 0xfc ) ) );
				}
				std::string str = ( *it ).skill_info.substr( s_pos, i - s_pos );
				int w = GetDrawStringWidthToHandle( ( *it ).skill_info.substr( s_pos, i - s_pos ).c_str(), i - s_pos + buff, info_small_fh );
				if ( GetDrawStringWidthToHandle( ( *it ).skill_info.substr( s_pos, i - s_pos ).c_str(), i - s_pos + buff, info_small_fh ) > 240 ) {
					//技説明文改行処理
					DrawStringToHandle( INFO_SKILL_X, INFO_SKILL_Y + 20 + count * 67 + line * 11, ( *it ).skill_info.substr( s_pos, i - s_pos - buff ).c_str(), 0, info_small_fh );
					line++;
					s_pos = i - buff;
				}
			}
			DrawStringToHandle( INFO_SKILL_X, INFO_SKILL_Y + 20 + count * 67 + line * 11, ( *it ).skill_info.substr( s_pos, i - s_pos ).c_str(), 0, info_small_fh );
			//DrawBox(0,0,241+INFO_SKILL_X-52,600,0,FALSE);

			count++;
			it++;
		}
		//フレーバーテクスト表示
		DrawFormatStringToHandle( INFO_FLAVOR_X, INFO_FLAVOR_Y, 0, info_mini_fh, "%s", card_info_data.flavor_text.c_str() );
	}

	//手札表示
	//SetDrawMode( DX_DRAWMODE_BILINEAR );
	std::vector<int>::iterator  hand_it = hand_list[PlayerDataList::getPlayerPC()].begin();
	std::vector<int>::iterator  pos_it = image_x.begin();
	int count = 0;
	while ( hand_it != hand_list[PlayerDataList::getPlayerPC()].end() ) {
		//手札の移動処理
		int dis = (int)( HAND_X + 90 * count - ( *pos_it ) );		//目的地から現在位置を引いた値
		( *pos_it ) += (int)( dis * FRICTION );
		GetGraphSize( DDDLoader::getCardHandle( ( *hand_it ) ), &wide, &height );
		int image_y = HAND_Y;
		x2 = (int)( ( *pos_it ) + ( CARD_W  * REDUCE ) );
		y2 = (int)( HAND_Y + ( CARD_H * REDUCE ) );
		if ( over_card == count ) {	//マウスオーバーされてる手札は位置をずらす
			image_y -= 20;
			y2 -= 20;
		}
		//手札画像表示
		if ( height > 280 ) {		//キャラ画像なら縮小表示
			DrawExtendGraph( ( *pos_it ), image_y, x2, y2, DDDLoader::getCardHandle( 0 ), TRUE );
			DrawExtendGraph( ( *pos_it ), image_y, x2, y2, DDDLoader::getCardHandle( ( *hand_it ) ), TRUE );
		} else {
			DrawExtendGraph( ( *pos_it ), image_y, x2, y2, DDDLoader::getCardHandle( ( *hand_it ) ), TRUE );
		}
		//キャラマップ表示
		if ( over_card == count ) {
			for ( int i = 0; i < 5; i++ ) {
				for ( int j = 0; j < 5; j++ ) {
					if ( card_info_data.map[i][j] > 1000 ) {
						chara_map.draw( 4, ( *pos_it ) + 35 + j * 5 - i * 5, image_y + j * 3 + i * 3 );
					} else {
						chara_map.draw( card_info_data.map[i][j], ( *pos_it ) + 35 + j * 5 - i * 5, image_y + j * 3 + i * 3 );
					}
				}
			}
		}
		count++;
		hand_it++;
		pos_it++;
	}


}