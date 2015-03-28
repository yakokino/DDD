#include "ddd_loader.h"

int DDDLoader::load_rate = 0;
std::vector<int> DDDLoader::shlist;
std::map<std::string, int> DDDLoader::name_handle;		//�n���h�����X�g�̎���
std::map< int, CardGraph > DDDLoader::card_handle;

int DDDLoader::setLoadFile( char *str ){
	if( name_handle.find( str ) != name_handle.end() ) return -1;	//�����L�[����ꍇ��-1��Ԃ�
	//�C���[�W�n���h�����쐬���Aname_handle�ɉ�����
	int sh = LoadSoftImage( str );
	shlist.push_back( sh );
	name_handle.insert( std::pair<std::string, int>( str, sh ) );
	return 0;	//����I���iname_handle�ɉ��������j��0��Ԃ�
}

int DDDLoader::getHandle( char *str ){
	std::map<std::string, int>::iterator it = name_handle.find( str );
	if( it != name_handle.end() ){
		if( (*it).second != 0 ){
			int chk = CheckHandleASyncLoad( (*it).second );
			switch( chk ){
			case FALSE:
				//�񓯊��ǂݍ��݂��I����Ă���΃n���h����Ԃ�
				//DeleteSoftImage( (*it).second );
				//(*it).second = 0;
				return (*it).second;
			case TRUE:
				//�񓯊��ǂݍ��݂��I����Ă��Ȃ����-2��Ԃ�
				return -2;
			}
		} else {
			//���Ƀn���h�����擾����Ă���ꍇ��-3��Ԃ�
			return -3;
		}
	}
	//�L�[�����݂��Ȃ����-1��Ԃ�
	return -1;
}

int DDDLoader::setLoadCard( int card_no ){
	if( card_handle.find( card_no ) != card_handle.end() ) return -1;	//�����L�[������ꍇ��-1��Ԃ�
	//�C���[�W�n���h�����쐬���Acard_handle�ɉ�����
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
	return 0;	//����I���icard_handle�ɉ��������j��0��Ԃ�
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



//�S�ăC���[�W�n���h����ǂݍ��񂾌�i�񓯊��ǂݍ��݂��S�ďI�������j�Ɏ��s����֐��@�C���[�W�n���h�����O���t�B�b�N�n���h���ɕϊ�����
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

//�p���b�g0�Ԗڂ̐F���擾���ăO���t�B�b�N�n���h���ɕύX����֐��i�񓯊��ǂݍ��݂��I����Ă�����gh��Ԃ��A�񓯊��ǂݍ��ݒ��Ȃ�-2��Ԃ��Bnot_trans==1�Ȃ�΃p���b�g0�Ԃ̐F�͎擾���Ȃ��j
int DDDLoader::getGraphHandle( char *filename, int not_trans ){
	std::map<std::string, int>::iterator it = name_handle.find( filename );
	if( it != name_handle.end() ){
		if( (*it).second != 0 ){
			int chk = CheckHandleASyncLoad( (*it).second );
			switch( chk ){
			case FALSE:
				{
					//�񓯊��ǂݍ��݂��I����Ă���΃O���t�B�b�N�n���h����Ԃ�
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
				//�񓯊��ǂݍ��݂��I����Ă��Ȃ����-2��Ԃ�
				return -2;
			}
		} else {
			//���Ƀn���h�����擾����Ă���ꍇ��-3��Ԃ�
			return -3;
		}
	}
	//�L�[�����݂��Ȃ����-1��Ԃ�
	return -1;
}

//�p���b�g0�Ԗڂ̐F���擾���ĕ����O���t�B�b�N�n���h���ɕύX����֐�2�i�񓯊��ǂݍ��݂��I����Ă�����0��Ԃ��j
int DDDLoader::getGraphHandle(char *filename, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *gh){
	std::map<std::string, int>::iterator it = name_handle.find( filename );
	if( it != name_handle.end() ){
		if( (*it).second != 0 ){
			int chk = CheckHandleASyncLoad( (*it).second );
			switch( chk ){
			case FALSE:
				//�񓯊��ǂݍ��݂��I����Ă���Ε����O���t�B�b�N�n���h����Ԃ�
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
				//�񓯊��ǂݍ��݂��I����Ă��Ȃ����-2��Ԃ�
				return -2;
			}
		} else {
			//���Ƀn���h�����擾����Ă���ꍇ��-3��Ԃ�
			return -3;
		}
	}
	//�L�[�����݂��Ȃ����-1��Ԃ�
	return -1;
}
