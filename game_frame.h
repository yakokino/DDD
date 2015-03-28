#ifndef __GAME_FRAME_H__
#define __GAME_FRAME_H__

#include "DxLib.h"
#include <string>

enum RESOLUTION{
	SCREENWIDH		= 1024,	//画像解像度
	SCREENHEIGHT	= 640,
};

enum GAMESCENE{
	MIJISSOU = -1,
	//ローディング画面
	LOADING = 0,
	//タイトル画面
	GAME_TITLE,
	//メインゲーム
	GAME_MAIN,
	//ネットワークモード
	GMAE_NET,
	//オプション
	OPTION,

	GAME_CLEAR,

	GAME_OVER,

	QUIT,
};

//enum CONFDATA{
//	//ウィンドウモード(0:フルスクリーン 1:ウィンドウ 2:毎回聞く)
//	//フレームスキップ(0: 通常 1:1/2描画 2:1/3描画)
//	//vsync同期待ちをするかどうかのフラグ。trueで同期
//	WIN_MODE, VSYNC, FRAME_SKIP, BGM_VOL, SE_VOL,
//	//PAD_1P, PAD_2P -1:キーボード 0:無効 1～16:パッド(パッド番号に対応)
//	PAD_1P, UP_1P, DOWN_1P, LEFT_1P, RIGHT_1P, A_1P, B_1P, C_1P, D_1P, START_1P,
//	PAD_2P, UP_2P, DOWN_2P, LEFT_2P, RIGHT_2P, A_2P, B_2P, C_2P, D_2P, START_2P
//};

//ゲームの骨組みGameFrameクラス===============================================================================================
class GameFrame{
	virtual int loadFiles(GAMESCENE scene) throw(...){return -1;}//ファイル読み込み関数描画関数
	//virtual void getConf(){}			//コンフィグデータ取得関数
	virtual void initScene(GAMESCENE scene){}	//ステージ初期化関数
	virtual void drawFrameRate();
	int old_time, frame_count, frame_rate;//フレームレート計測用変数
	int large_font, small_font;
	GAMESCENE game_scene;				//現在シーンを表す変数
	static unsigned int scene_frame;	//現在のシーンが始まってからの総フレーム数
	bool loading;						//trueでローディング中

	bool quit;							//trueで終了
protected:
	virtual void drawGameTitle();		//タイトル画面描画関数
	virtual void drawGameMain();		//メインゲーム描画関数
	virtual void drawGameOver();		//ゲームオーバー画面描画関数
	virtual void drawGameClear();		//ゲームクリアー画面描画関数
	virtual void drawLoading(GAMESCENE scene);	//ローディング画面描画関数
	virtual GAMESCENE setGameScene(GAMESCENE scene);//scene変更関数
public:
	virtual ~GameFrame(){}				//デストラクタ
	virtual void doMainLoop();			//メインループ実行関数
	static unsigned int getSceneFrame();
};

//============================================================================================================================

#endif