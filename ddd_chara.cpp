#include "ddd.h"

int DDDChara::loadFiles() throw(...){

	//karireimu.setImage(DIR_UP,    DDDLoader::getCharactorHandle(2100));
	//karireimu.setImage(DIR_RIGHT, DDDLoader::getCharactorHandle(2100));
	//karireimu.setImage(DIR_DOWN,  DDDLoader::getCharactorHandle(2100));
	//karireimu.setImage(DIR_LEFT,  DDDLoader::getCharactorHandle(2100));


	return 0;
}


void DDDChara::init(){
	chara_list.clear();

	CharaData new_cd;
	new_cd.life = 3000;
	new_cd.point_x = 1;
	new_cd.point_y = 0;
	new_cd.direction = RIGHT;
	new_cd.anim = DDDLoader::getCharactorAnim( 2000 );
	new_cd.anim->getSize(0, &new_cd.image_w, &new_cd.image_h);
	new_cd.image_x = ( new_cd.point_x * 48 - new_cd.point_y * 48 ) + 48 - ( new_cd.image_w / 2 );
	new_cd.image_y = ( new_cd.point_x * 24 + new_cd.point_y * 24 ) -  8 - ( new_cd.image_h / 2 );

	chara_list.push_back( new_cd );
	new_cd.point_y = 1;
	new_cd.image_x = ( new_cd.point_x * 48 - new_cd.point_y * 48 ) + 48 - ( new_cd.image_w / 2 );
	new_cd.image_y = ( new_cd.point_x * 24 + new_cd.point_y * 24 ) -  8 - ( new_cd.image_h / 2 );
	chara_list.push_back( new_cd );
	

}

void DDDChara::draw(int camera_x, int camera_y){
	std::list<CharaData>::iterator it;
	for(it=chara_list.begin(); it!=chara_list.end(); it++ ){
		(*it).anim->draw((*it).direction, (*it).image_x - camera_x, (*it).image_y - camera_y);
	}
}
