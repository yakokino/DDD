#include "game_frame.h"
#include "ddd.h"

class DDD :public GameFrame{
	DDDWorld ddd_world;
	AnimDraw loading;

	int loadFiles(GAMESCENE scene) throw(...);
	void initScene(GAMESCENE scene);
	void drawGameTitle();
	void drawGameMain();
	void drawLoading(GAMESCENE scene);
};

int DDD::loadFiles(GAMESCENE scene) throw(...){
	int no_load_sum = 0;
	switch( scene ){
		case GAME_TITLE:

			break;
		case GAME_MAIN:
			no_load_sum += ddd_world.loadFiles();
			break;
	}
	return no_load_sum;
}

void DDD::initScene(GAMESCENE scene){
	switch( scene ){
	case GAME_TITLE:

		break;
	case GAME_MAIN:
		ddd_world.loadFiles();	//必要なファイルを読み込む
		ddd_world.init();

		break;
	case GAME_OVER:

		break;
	case GAME_CLEAR:

		break;
	}
}

void DDD::drawGameTitle(){

	if( ( GetMouseInput() & MOUSE_INPUT_RIGHT ) != 0 ){
		DeckData deck_data;
		for(int i=0 ; i<3 ; i++ ){
			for(int j=0 ; j<6 ; j++ ){
				deck_data.my_dice[i][j] = SUMMON;
			}
		}
		deck_data.my_dice[0][0] = ATTACK;
		deck_data.my_dice[0][3] = SPECIAL;
		deck_data.my_dice[0][5] = MOVE;
		deck_data.my_dice[1][0] = MONEY;
		deck_data.my_dice[1][1] = MONEY;
		deck_data.my_dice[1][2] = MAGIC_STONE;
		deck_data.my_dice[1][3] = MAGIC_STONE;
		deck_data.my_dice[1][4] = PIETY;
		deck_data.my_dice[1][5] = PIETY;
		for(int i=0 ; i<20 ; i++ ){
			deck_data.card_no[i] = 2100;
		}
		for(int i=20 ; i<30 ; i++ ){
			deck_data.card_no[i] = 2001;
		}
		for(int i=30 ; i<40 ; i++ ){
			deck_data.card_no[i] = 2000;
		}
		for(int i=40 ; i<60 ; i++ ){
			deck_data.card_no[i] = -1;
		}
		FILE *fp;
		if( fopen_s( &fp, "Deck\\deck_1p.deck", "wb" ) != 0 ){	//バイナリファイルを開く
			int test = 5;
		}
		fwrite( &deck_data, sizeof(deck_data), 1, fp ); // DeckData構造体の中身を出力
		fclose( fp );//ファイルを閉じる
		
		
		//ネットワーク接続待機させたい
	}
	GameFrame::drawGameTitle();

}

void DDD::drawGameMain(){
	ddd_world.phaseControl();
	ddd_world.draw();

}

void DDD::drawLoading(GAMESCENE scene){
	GameFrame::drawLoading( scene );
}


//WinMain関数==============================================================================================================
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){
	DDD *ddd = new DDD();		//GameFrame派生クラスポインタを生成
	ddd->doMainLoop();						//GameFrameクラスのメンバ関数MainLoopを実行
	delete ddd;							//GameFrameクラスポインタを開放
}

//=========================================================================================================================