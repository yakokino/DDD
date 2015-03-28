#include "ddd_loader.h"

int DDDLoader::load_rate = 0;
std::vector<int> DDDLoader::shlist;
std::map<std::string, int> DDDLoader::name_handle;		//ハンドルリストの実体
std::map< int, CardGraph > DDDLoader::card_handle;

int DDDLoader::setLoadFile( char *str ){
	if( name_handle.find( str ) != name_handle.end() ) return -1;	//同じキーある場合は-1を返す
	//イメージハンドルを作成し、name_handleに加える
	int sh = LoadSoftImage( str );
	shlist.push_back( sh );
	name_handle.insert( std::pair<std::string, int>( str, sh ) );
	return 0;	//正常終了（name_handleに加えた時）は0を返す
}

int DDDLoader::getHandle( char *str ){
	std::map<std::string, int>::iterator it = name_handle.find( str );
	if( it != name_handle.end() ){
		if( (*it).second != 0 ){
			int chk = CheckHandleASyncLoad( (*it).second );
			switch( chk ){
			case FALSE:
				//非同期読み込みが終わっていればハンドルを返す
				//DeleteSoftImage( (*it).second );
				//(*it).second = 0;
				return (*it).second;
			case TRUE:
				//非同期読み込みが終わっていなければ-2を返す
				return -2;
			}
		} else {
			//既にハンドルを取得されている場合は-3を返す
			return -3;
		}
	}
	//キーが存在しなければ-1を返す
	return -1;
}

int DDDLoader::setLoadCard( int card_no ){
	if( card_handle.find( card_no ) != card_handle.end() ) return -1;	//同じキーがある場合は-1を返す
	//イメージハンドルを作成し、card_handleに加える
	char buff[8];
	std::string str1 = "Media\\Graphic\\Card\\";
	sprintf(buff, "%d", card_no);
	str1 += buff;
	str1 += ".png";
	std::string str2 = "Media\\Graphic\\Charactor\\";
	str2 += buff;
	str2 += ".png";
	CardGraph card_graph;
	card_graph.card = LoadGraph( str1.c_str() );
	LoadDivGraph( str2.c_str(), 16, 4, 4, 256/4, 384/4, card_graph.charactor );
	card_handle.insert( std::pair<int, CardGraph>( card_no, card_graph ) );
	return 0;	//正常終了（card_handleに加えた時）は0を返す
}

int DDDLoader::deleteImageHandle(){
	while( shlist.size() > 0 ){
		DeleteSoftImage( shlist.back() );
		shlist.pop_back();
	}
	return 0;
}


int DDDLoader::getCardHandle( int card_no ){
	std::map<int, CardGraph>::iterator it = card_handle.find( card_no );
	return (*it).second.card;
}

//int DDDLoader::getCharactorHandle( int card_no ){
//	std::map<int, CardGraph>::iterator it = card_handle.find( card_no );
//	return (*it).second.charactor[0];
//}



int DDDLoader::setCharactorAnim(){
	std::map<int, CardGraph>::iterator it = card_handle.begin();
	while( it != card_handle.end() ){
		for(int i=0 ; i<4 ; i++ ){
			for(int j=0 ; j<4 ; j++ ){
				(*it).second.anim.setImage( i, (*it).second.charactor[ i * 4 + j ] );
			}
			(*it).second.anim.setGap( i, 15 );
		}
		it++;
	}
	return 0;
}

AnimDraw *DDDLoader::getCharactorAnim( int card_no ){
	std::map<int, CardGraph>::iterator it = card_handle.find( card_no );
	return &(*it).second.anim;
}



//全てイメージハンドルを読み込んだ後（非同期読み込みが全て終わった後）に実行する関数　イメージハンドルをグラフィックハンドルに変換する
int DDDLoader::setCardGraphHandle(){
	std::map<int, CardGraph>::iterator it = card_handle.begin();
	while( it != card_handle.end() ){
		for(int i=0 ; i<2 ; i++ ){
			int sh;
			if( i < 1 ) sh = (*it).second.card;
			else sh = (*it).second.charactor[0];
			shlist.push_back( sh );
			int r, g, b;
			GetPaletteSoftImage(sh, 0, &r, &g, &b, 0);
			SetTransColor(r, g, b);
			if( i < 1 ){
				int gh = CreateGraphFromSoftImage( sh );
				(*it).second.card = gh;
			} else {
				int w,h;
				GetSoftImageSize( sh, &w, &h );
				CreateDivGraphFromSoftImage(sh, 16, 4, 4, w/4, h/4, (*it).second.charactor );
			}
			
		}
		it++;
	}
	return 0;
}

//パレット0番目の色を取得してグラフィックハンドルに変更する関数（非同期読み込みが終わっていたらghを返し、非同期読み込み中なら-2を返す。not_trans==1ならばパレット0番の色は取得しない）
int DDDLoader::getGraphHandle( char *filename, int not_trans ){
	std::map<std::string, int>::iterator it = name_handle.find( filename );
	if( it != name_handle.end() ){
		if( (*it).second != 0 ){
			int chk = CheckHandleASyncLoad( (*it).second );
			switch( chk ){
			case FALSE:
				{
					//非同期読み込みが終わっていればグラフィックハンドルを返す
					int sh = (*it).second;
					if( not_trans == 0 ){
						int r, g, b;
						GetPaletteSoftImage(sh, 0, &r, &g, &b, 0);
						SetTransColor(r, g, b);
					} else {
						SetTransColor(255,255,255);
					}
					int gh = CreateGraphFromSoftImage( sh );
					shlist.push_back( sh );
					return gh;
				}
			case TRUE:
				//非同期読み込みが終わっていなければ-2を返す
				return -2;
			}
		} else {
			//既にハンドルを取得されている場合は-3を返す
			return -3;
		}
	}
	//キーが存在しなければ-1を返す
	return -1;
}

//パレット0番目の色を取得して分割グラフィックハンドルに変更する関数2（非同期読み込みが終わっていたら0を返す）
int DDDLoader::getGraphHandle(char *filename, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *gh){
	std::map<std::string, int>::iterator it = name_handle.find( filename );
	if( it != name_handle.end() ){
		if( (*it).second != 0 ){
			int chk = CheckHandleASyncLoad( (*it).second );
			switch( chk ){
			case FALSE:
				//非同期読み込みが終わっていれば分割グラフィックハンドルを返す
				{
					int sh = (*it).second;
					int r,g,b;
					GetPaletteSoftImage(sh, 0, &r, &g, &b, 0);
					SetTransColor(r, g, b);
					CreateDivGraphFromSoftImage(sh, AllNum, XNum, YNum, SizeX, SizeY, gh);
					shlist.push_back( sh );
					return 0;
				}
			case TRUE:
				//非同期読み込みが終わっていなければ-2を返す
				return -2;
			}
		} else {
			//既にハンドルを取得されている場合は-3を返す
			return -3;
		}
	}
	//キーが存在しなければ-1を返す
	return -1;
}
