#include "game_frame.h"


//シーン変更関数=============================================================================================================
GAMESCENE GameFrame::setGameScene(GAMESCENE scene){
	if(game_scene != scene){
		game_scene = scene;
		scene_frame = 0;
		if(game_scene == GAME_TITLE){
			initScene(scene);
		} else if(game_scene == GAME_MAIN){
			loading = true;
			//初期化はロードが終わってから実行するのでここではしない
		}
	}
	return game_scene;
}
unsigned int GameFrame::scene_frame = 0;

//シーンフレーム取得関数
unsigned int GameFrame::getSceneFrame(){
	return scene_frame;
}

//フレームレート計測、表示関数===============================================================================================
void GameFrame::drawFrameRate(){
	frame_count++;
	int time = GetNowCount() - old_time;
	if(time >= 995){
		frame_rate = frame_count;
		old_time = GetNowCount();
		frame_count = 0;
	}
	DrawFormatString( SCREENWIDH - 18, SCREENHEIGHT - 15, 0, "%02d", frame_rate );
}

//タイトル画面の表示関数======================================================================================================
void GameFrame::drawGameTitle(){
	int col = GetColor(255,0,255);
	int w = GetDrawStringWidthToHandle("PUSH LEFT CLICK", 15, small_font);
	DrawStringToHandle( (SCREENWIDH - w) / 2, SCREENHEIGHT - 100, "PUSH LEFT CLICK", col, small_font);
	if( ( GetMouseInput() & MOUSE_INPUT_LEFT ) != 0 ){
		setGameScene(GAME_MAIN);
	}
}

//メイン画面の表示関数
void GameFrame::drawGameMain(){
	int col = GetColor(255,255,255);
	int w = GetDrawStringWidthToHandle("GAME MAIN", 9, large_font);
	DrawStringToHandle( (SCREENWIDH - w) / 2, 100, "GAME MAIN", col, large_font);
	if( ( GetMouseInput() & MOUSE_INPUT_MIDDLE ) != 0 ){
		setGameScene(GAME_CLEAR);
	} else if( ( GetMouseInput() & MOUSE_INPUT_RIGHT ) != 0 ){
		setGameScene(GAME_OVER);
	}
}

//ゲームクリア画面の表示関数
void GameFrame::drawGameClear(){
	int col = GetColor(0,0,255);
	int w = GetDrawStringWidthToHandle("GAME CLEAR", 10, large_font);
	DrawStringToHandle( (SCREENWIDH - w) / 2, 100, "GAME CLEAR", col, large_font);
	if( scene_frame >= 60 ){
		setGameScene(GAME_TITLE);
	}
}

//ゲームオーバー画面の表示関数
void GameFrame::drawGameOver(){
	int col = GetColor(255,0,0);
	int w = GetDrawStringWidthToHandle("GAME OVER", 9, large_font);
	DrawStringToHandle( (SCREENWIDH - w) / 2, 100, "GAME OVER", col, large_font);
	 if( scene_frame >= 60 ){
		 setGameScene(GAME_TITLE);
	 }
}

//ローディング画面描画関数
void GameFrame::drawLoading(GAMESCENE scene){
	if( game_scene == GAME_MAIN ){
		//ロード画面表示
		DrawString(900,600,"Loading...",0); 
	}
}

//メインループ関数 MainLoop関数===============================================================================================
void GameFrame::doMainLoop(){
	//初期設定-----------------------------------------------------------------------------------------
	ChangeWindowMode( TRUE );
	SetGraphMode( SCREENWIDH, SCREENHEIGHT, 16 );			//解像度指定
	SetBackgroundColor( 255, 255, 255 ) ;
	SetWindowStyleMode( 9 );
	SetMainWindowText("DDD ver0.01");	//ウィンドウテクストの変更
	SetDoubleStartValidFlag( TRUE );
	//SetWindowSizeChangeEnableFlag( TRUE );	//ウィンドウサイズ可変
	SetAlwaysRunFlag( TRUE );				//非アクティブでも実行するように変更
	SetOutApplicationLogValidFlag( FALSE );	//ログ出力をしないように変更
	if(DxLib_Init() == -1){					//DXライブラリの初期化
		MessageBox(NULL, "ライブラリ初期化エラーです", "エラー", MB_OK );
		return;
	}
	quit = false;
	loading = true;
	old_time =  GetNowCount();
	frame_count = 0;
	frame_rate = 0;
	scene_frame = 0;
	large_font = CreateFontToHandle(0,100,9,DX_FONTTYPE_EDGE);
	small_font = CreateFontToHandle(0,32,9,DX_FONTTYPE_NORMAL);

	SetDrawScreen(DX_SCREEN_BACK);	//描画対象の指定

	try{
		loadFiles(GAME_TITLE);
	}
	catch(int error_code){
		MessageBox(NULL, "ファイル読み込みエラー", "エラー", MB_OK );
		return;
	}
	setGameScene( GAME_TITLE );		//タイトル画面に設定
	loading = false;
	SetUseASyncLoadFlag(TRUE);

	//メインループ---------------------------------------------------------------------------------------
	while(ProcessMessage() == 0 && quit == false){
		ClsDrawScreen();			//画面消去
		if( loading == true ){
			//ロード中なら
			drawLoading(game_scene);

			if( game_scene == GAME_MAIN ){
				
				try{
					if( loadFiles(GAME_MAIN) == 0 ){
						//ロードが全て完了したら
						loading = false;
						initScene(GAME_MAIN);
					}
				}
				catch(int error_code){
					std::string str = "ファイル読み込みエラー";
					char c[10];
					sprintf(c, "%d", error_code);
					str += c;
					MessageBox(NULL, str.c_str(), "エラー", MB_OK );
					return;
				}
			}
		} else {
			switch(game_scene){
				case GAME_TITLE:
					drawGameTitle();
					break;
				case GAME_MAIN:
					drawGameMain();
					break;
				case GAME_OVER:
					drawGameOver();
					break;
				case GAME_CLEAR:
					drawGameClear();
					break;
			}
		}
		drawFrameRate();
		scene_frame++;
		ScreenFlip();
		if(CheckHitKey( KEY_INPUT_LSHIFT ))Sleep(300);		//デバッグ用スローモード
		if( CheckHitKey( KEY_INPUT_F8 ) ){					//デバッグ用ポーズ。解除はF9
			while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_F9) == 0){
				Sleep(16);
				drawFrameRate();
			}
		}
		if( CheckHitKey( KEY_INPUT_ESCAPE ) )quit = true;	//デバッグ用ESCキーで終了

		keybd_event(0x07, 0, KEYEVENTF_KEYUP, 0);			//システムキーの無効化
	}
	DxLib_End();					//DXライブラリ終了処理
}

//============================================================================================================================