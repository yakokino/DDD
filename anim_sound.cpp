#include "anim_sound.h"

//グラフィックハンドルのセット
int AnimDraw::setImage(int apid, int ghandle){
	if( apid >= (int)aplist.size() ){
		aplist.push_back( animpat() );
		apid = aplist.size() - 1;
	}
	aplist.at(apid).ghlist.push_back(ghandle);
	return(apid);
}
//アニメーションギャップのセット
int AnimDraw::setGap(int apid, int gap){
	if( apid >= (int)aplist.size() ){
		aplist.push_back( animpat() );
		apid = aplist.size() - 1;
	}
	aplist.at(apid).anigap = gap;
	return(apid);
}
//グラフィックデータのサイズを得る
void AnimDraw::getSize(int apid, int *wide, int *height){
	GetGraphSize( aplist.at(apid).ghlist.at(0), wide, height );
}

//空かどうかを調べる
int AnimDraw::empty( int apid ){
	if( apid >= aplist.size() ) return 1;
	if( aplist.at(apid).ghlist.size() == 0 ) return 1;
	return 0;
}
//アニメーション描画
void AnimDraw::draw(int apid, int x, int y ){
	if( aplist.at(apid).ghlist.size() > 1 ){
		int curpat = GameFrame::getSceneFrame() /aplist.at(apid).anigap % aplist.at(apid).ghlist.size();
		DrawGraph( x, y, aplist.at(apid).ghlist.at(curpat), TRUE );
	} else {
		DrawGraph( x, y, aplist.at(apid).ghlist.at(0), TRUE );
	}
}
//アニメーション拡大縮小描画
void AnimDraw::draw(int apid, int x, int y, double rate){
	int x2, y2;
	GetGraphSize( aplist.at(apid).ghlist.at(0), &x2, &y2 );
	x2 = x + x2 * rate;
	y2 = y + y2 * rate;
	if( aplist.at(apid).ghlist.size() > 1 ){
		int curpat = GameFrame::getSceneFrame() / aplist.at(apid).anigap % aplist.at(apid).ghlist.size();
		DrawExtendGraph( x, y, x2, y2, aplist.at(apid).ghlist.at(curpat), TRUE );
		//DrawRotaGraph( x, y, rate, 0.0, aplist.at(apid).ghlist.at(curpat), true, false );
	} else {
		DrawExtendGraph( x, y, x2, y2, aplist.at(apid).ghlist.at(0), TRUE );
		//DrawRotaGraph( x, y, rate, 0.0, aplist.at(apid).ghlist.at(0), true, false );
	}
}


//SoundBoxクラスメンバ変数の実体
std::vector<int> SoundBox::sounds;
int SoundBox::bgm = -1;

//サウンドハンドルの登録（登録インデックスを返す）
int SoundBox::set(int shandle){
	sounds.push_back(shandle);
	return( sounds.size()-1 );
}
//サウンドの再生
void SoundBox::play(int snum){
	PlaySoundMem( sounds.at(snum), DX_PLAYTYPE_BACK );
}
//サウンドの停止
void SoundBox::stop(int snum){
	StopSoundMem( sounds.at(snum) );
}
//BGMの再生（現在のBGMは停止）
void SoundBox::playbgm(int snum){
	int newbgm = sounds.at(snum);
	if( newbgm == bgm ) return;
	StopSoundMem(bgm);
	bgm = newbgm;
	PlaySoundMem( bgm, DX_PLAYTYPE_LOOP);
}