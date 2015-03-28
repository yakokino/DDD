#ifndef __DDD_LOADERD_H__
#define __DDD_LOADERD_H__

#include "DxLib.h"
#include "anim_sound.h"
#include <map>
#include <vector>
#include <string>

struct CardGraph{
	int card;		//カード画像のグラフハンドル
	int charactor[16];	//キャラ画像のグラフハンドル
	AnimDraw anim;		//4＊4コマのアニメ
};

class DDDLoader{
	static std::vector<int> shlist;
	static std::map<std::string, int> name_handle;	//ファイル場所＋ファイル名とイメージハンドル（ghに変換すると0にする）
	static std::map< int, CardGraph > card_handle;		//カードナンバーとグラフィックハンドル、アニメクラスも含まれる
public:
	static int load_rate;		//ロードの進行率

	static int setLoadFile( char *str );			//画像や音楽ファイルなどのファイル名を渡してname_handleに追加する
	static int getHandle( char *str );				//name_handleに保存されているハンドルを取得する（BMPなどの透明情報は指定しない）

	static int setLoadCard( int card_no );			//指定したカードナンバーのカード画像、キャラ画像をcard_handleに追加する
	static int getCardHandle( int card_no );		//指定したカードナンバーのカード画像のハンドルを取得する（キャラクターカードだったは等倍のハンドルを取得する
	//static int getCharactorHandle( int card_no );	//指定したカードナンバーのキャラクター画像（マップに表示される画像）
	static int setCharactorAnim();
	static AnimDraw *getCharactorAnim( int card_no );

	static int setCardGraphHandle();
	static int deleteImageHandle();		//作成済みのイメージハンドルを解放する

	static int getGraphHandle( char *filename, int not_trans = 0 );
	static int getGraphHandle(char *filename, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *gh);
};

#endif