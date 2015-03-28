#include "anim_sound.h"

//�O���t�B�b�N�n���h���̃Z�b�g
int AnimDraw::setImage(int apid, int ghandle){
	if( apid >= (int)aplist.size() ){
		aplist.push_back( animpat() );
		apid = aplist.size() - 1;
	}
	aplist.at(apid).ghlist.push_back(ghandle);
	return(apid);
}
//�A�j���[�V�����M���b�v�̃Z�b�g
int AnimDraw::setGap(int apid, int gap){
	if( apid >= (int)aplist.size() ){
		aplist.push_back( animpat() );
		apid = aplist.size() - 1;
	}
	aplist.at(apid).anigap = gap;
	return(apid);
}
//�O���t�B�b�N�f�[�^�̃T�C�Y�𓾂�
void AnimDraw::getSize(int apid, int *wide, int *height){
	GetGraphSize( aplist.at(apid).ghlist.at(0), wide, height );
}

//�󂩂ǂ����𒲂ׂ�
int AnimDraw::empty( int apid ){
	if( apid >= aplist.size() ) return 1;
	if( aplist.at(apid).ghlist.size() == 0 ) return 1;
	return 0;
}
//�A�j���[�V�����`��
void AnimDraw::draw(int apid, int x, int y ){
	if( aplist.at(apid).ghlist.size() > 1 ){
		int curpat = GameFrame::getSceneFrame() /aplist.at(apid).anigap % aplist.at(apid).ghlist.size();
		DrawGraph( x, y, aplist.at(apid).ghlist.at(curpat), TRUE );
	} else {
		DrawGraph( x, y, aplist.at(apid).ghlist.at(0), TRUE );
	}
}
//�A�j���[�V�����g��k���`��
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


//SoundBox�N���X�����o�ϐ��̎���
std::vector<int> SoundBox::sounds;
int SoundBox::bgm = -1;

//�T�E���h�n���h���̓o�^�i�o�^�C���f�b�N�X��Ԃ��j
int SoundBox::set(int shandle){
	sounds.push_back(shandle);
	return( sounds.size()-1 );
}
//�T�E���h�̍Đ�
void SoundBox::play(int snum){
	PlaySoundMem( sounds.at(snum), DX_PLAYTYPE_BACK );
}
//�T�E���h�̒�~
void SoundBox::stop(int snum){
	StopSoundMem( sounds.at(snum) );
}
//BGM�̍Đ��i���݂�BGM�͒�~�j
void SoundBox::playbgm(int snum){
	int newbgm = sounds.at(snum);
	if( newbgm == bgm ) return;
	StopSoundMem(bgm);
	bgm = newbgm;
	PlaySoundMem( bgm, DX_PLAYTYPE_LOOP);
}