#include "player_data.h"

PlayerData PlayerDataList::pd[4];
int PlayerDataList::player_pc;

int PlayerDataList::loadFiles(){

	
	
	int last = 0;		//残り処理数
	int chk;
	int gh[20];
	if( symbol_anim.empty() == 1 ){
		DDDLoader::setLoadFile("Media\\Graphic\\symbol.bmp");
		chk = DDDLoader::getGraphHandle("Media\\Graphic\\symbol.bmp", 15, 5, 3, 32, 32, gh);
		if( chk == -1 ) throw(-5);						//非同期読み込みがエラーならthrow
		else if( chk >= 0 ){							//非同期読み込みが終わっていればアニメーションに追加
			for(int i=0 ; i<20 ; i++ ){
				symbol_anim.setImage(i, gh[i]);
			}
		} else last++;									//非同期読み込み中なら残り処理＋１
	}
	return last;
}

void PlayerDataList::init(){
	name_fh = CreateFontToHandle( 0, 14, 5, DX_FONTTYPE_EDGE);
}

////デッキデータをセットする（マイダイスデータも含む）
//void PlayerDataList::setDeck( int pn, DeckData *deck ){
//	for(int i=0 ; i<3 ; i++ ){
//		for(int j=0 ; j<6 ; j++ ){
//			pd[pn].my_dice[i][j] = deck->my_dice[i][j];
//		}
//	}
//	for(int i=0 ; i<60 ; i++ ){
//		int card_no = deck->card_no[i];
//		if( card_no == -1 ) return;
//		pd[pn].deck_num++;
//		pd[pn].deck_all[i] = card_no;
//	}
//}

int PlayerDataList::checkStock( const int p_no, const int *symbol ){
	for(int i=0 ; i<20 ; i++ ){
		if( symbol[i] > pd[p_no].p_stock[i] ) return 0;
	}
	return 1;
}


void PlayerDataList::draw(){		//とりあえず各プレイヤーの名前を表示
	int dy = OTHER_PLAYER_DATA_Y;
	for(int i=0 ; i<4 ; i++ ){
		if( pd[i].p_no == player_pc ) {
			//PCプレイヤーのみ別場所に表示
			DrawFormatStringToHandle( PLAYER_DATA_X, PLAYER_DATA_Y, GetColor(255,255,255), name_fh, "%s", pd[i].p_name.c_str() );
			//クレストプール表示
			int px = PLAYER_DATA_X + 2;
			for(int j=0 ; j<20 ; j++ ){
				for(int k=0 ; k<pd[i].p_stock[j] ; k++ ){
					symbol_anim.draw(j, px, PLAYER_DATA_Y+40);
					px += 32;
				}
			}
		} else {
			DrawFormatStringToHandle( OTHER_PLAYER_DATA_X, dy, GetColor(255,255,255), name_fh, "%s", pd[i].p_name.c_str() );
			dy += PLAYER_DATA_SPACE;
		}
	}

}