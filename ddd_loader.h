#ifndef __DDD_LOADERD_H__
#define __DDD_LOADERD_H__

#include "DxLib.h"
#include "anim_sound.h"
#include <map>
#include <vector>
#include <string>

struct CardGraph{
	int card;		//�J�[�h�摜�̃O���t�n���h��
	int charactor[16];	//�L�����摜�̃O���t�n���h��
	AnimDraw anim;		//4��4�R�}�̃A�j��
};

class DDDLoader{
	static std::vector<int> shlist;
	static std::map<std::string, int> name_handle;	//�t�@�C���ꏊ�{�t�@�C�����ƃC���[�W�n���h���igh�ɕϊ������0�ɂ���j
	static std::map< int, CardGraph > card_handle;		//�J�[�h�i���o�[�ƃO���t�B�b�N�n���h���A�A�j���N���X���܂܂��
public:
	static int load_rate;		//���[�h�̐i�s��

	static int setLoadFile( char *str );			//�摜�≹�y�t�@�C���Ȃǂ̃t�@�C������n����name_handle�ɒǉ�����
	static int getHandle( char *str );				//name_handle�ɕۑ�����Ă���n���h�����擾����iBMP�Ȃǂ̓������͎w�肵�Ȃ��j

	static int setLoadCard( int card_no );			//�w�肵���J�[�h�i���o�[�̃J�[�h�摜�A�L�����摜��card_handle�ɒǉ�����
	static int getCardHandle( int card_no );		//�w�肵���J�[�h�i���o�[�̃J�[�h�摜�̃n���h�����擾����i�L�����N�^�[�J�[�h�������͓��{�̃n���h�����擾����
	//static int getCharactorHandle( int card_no );	//�w�肵���J�[�h�i���o�[�̃L�����N�^�[�摜�i�}�b�v�ɕ\�������摜�j
	static int setCharactorAnim();
	static AnimDraw *getCharactorAnim( int card_no );

	static int setCardGraphHandle();
	static int deleteImageHandle();		//�쐬�ς݂̃C���[�W�n���h�����������

	static int getGraphHandle( char *filename, int not_trans = 0 );
	static int getGraphHandle(char *filename, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *gh);
};

#endif