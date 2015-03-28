#ifndef __GAME_FRAME_H__
#define __GAME_FRAME_H__

#include "DxLib.h"
#include <string>

enum RESOLUTION{
	SCREENWIDH		= 1024,	//�摜�𑜓x
	SCREENHEIGHT	= 640,
};

enum GAMESCENE{
	MIJISSOU = -1,
	//���[�f�B���O���
	LOADING = 0,
	//�^�C�g�����
	GAME_TITLE,
	//���C���Q�[��
	GAME_MAIN,
	//�l�b�g���[�N���[�h
	GMAE_NET,
	//�I�v�V����
	OPTION,

	GAME_CLEAR,

	GAME_OVER,

	QUIT,
};

//enum CONFDATA{
//	//�E�B���h�E���[�h(0:�t���X�N���[�� 1:�E�B���h�E 2:���񕷂�)
//	//�t���[���X�L�b�v(0: �ʏ� 1:1/2�`�� 2:1/3�`��)
//	//vsync�����҂������邩�ǂ����̃t���O�Btrue�œ���
//	WIN_MODE, VSYNC, FRAME_SKIP, BGM_VOL, SE_VOL,
//	//PAD_1P, PAD_2P -1:�L�[�{�[�h 0:���� 1�`16:�p�b�h(�p�b�h�ԍ��ɑΉ�)
//	PAD_1P, UP_1P, DOWN_1P, LEFT_1P, RIGHT_1P, A_1P, B_1P, C_1P, D_1P, START_1P,
//	PAD_2P, UP_2P, DOWN_2P, LEFT_2P, RIGHT_2P, A_2P, B_2P, C_2P, D_2P, START_2P
//};

//�Q�[���̍��g��GameFrame�N���X===============================================================================================
class GameFrame{
	virtual int loadFiles(GAMESCENE scene) throw(...){return -1;}//�t�@�C���ǂݍ��݊֐��`��֐�
	//virtual void getConf(){}			//�R���t�B�O�f�[�^�擾�֐�
	virtual void initScene(GAMESCENE scene){}	//�X�e�[�W�������֐�
	virtual void drawFrameRate();
	int old_time, frame_count, frame_rate;//�t���[�����[�g�v���p�ϐ�
	int large_font, small_font;
	GAMESCENE game_scene;				//���݃V�[����\���ϐ�
	static unsigned int scene_frame;	//���݂̃V�[�����n�܂��Ă���̑��t���[����
	bool loading;						//true�Ń��[�f�B���O��

	bool quit;							//true�ŏI��
protected:
	virtual void drawGameTitle();		//�^�C�g����ʕ`��֐�
	virtual void drawGameMain();		//���C���Q�[���`��֐�
	virtual void drawGameOver();		//�Q�[���I�[�o�[��ʕ`��֐�
	virtual void drawGameClear();		//�Q�[���N���A�[��ʕ`��֐�
	virtual void drawLoading(GAMESCENE scene);	//���[�f�B���O��ʕ`��֐�
	virtual GAMESCENE setGameScene(GAMESCENE scene);//scene�ύX�֐�
public:
	virtual ~GameFrame(){}				//�f�X�g���N�^
	virtual void doMainLoop();			//���C�����[�v���s�֐�
	static unsigned int getSceneFrame();
};

//============================================================================================================================

#endif