#include "game_frame.h"


//�V�[���ύX�֐�=============================================================================================================
GAMESCENE GameFrame::setGameScene(GAMESCENE scene){
	if(game_scene != scene){
		game_scene = scene;
		scene_frame = 0;
		if(game_scene == GAME_TITLE){
			initScene(scene);
		} else if(game_scene == GAME_MAIN){
			loading = true;
			//�������̓��[�h���I����Ă�����s����̂ł����ł͂��Ȃ�
		}
	}
	return game_scene;
}
unsigned int GameFrame::scene_frame = 0;

//�V�[���t���[���擾�֐�
unsigned int GameFrame::getSceneFrame(){
	return scene_frame;
}

//�t���[�����[�g�v���A�\���֐�===============================================================================================
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

//�^�C�g����ʂ̕\���֐�======================================================================================================
void GameFrame::drawGameTitle(){
	int col = GetColor(255,0,255);
	int w = GetDrawStringWidthToHandle("PUSH LEFT CLICK", 15, small_font);
	DrawStringToHandle( (SCREENWIDH - w) / 2, SCREENHEIGHT - 100, "PUSH LEFT CLICK", col, small_font);
	if( ( GetMouseInput() & MOUSE_INPUT_LEFT ) != 0 ){
		setGameScene(GAME_MAIN);
	}
}

//���C����ʂ̕\���֐�
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

//�Q�[���N���A��ʂ̕\���֐�
void GameFrame::drawGameClear(){
	int col = GetColor(0,0,255);
	int w = GetDrawStringWidthToHandle("GAME CLEAR", 10, large_font);
	DrawStringToHandle( (SCREENWIDH - w) / 2, 100, "GAME CLEAR", col, large_font);
	if( scene_frame >= 60 ){
		setGameScene(GAME_TITLE);
	}
}

//�Q�[���I�[�o�[��ʂ̕\���֐�
void GameFrame::drawGameOver(){
	int col = GetColor(255,0,0);
	int w = GetDrawStringWidthToHandle("GAME OVER", 9, large_font);
	DrawStringToHandle( (SCREENWIDH - w) / 2, 100, "GAME OVER", col, large_font);
	 if( scene_frame >= 60 ){
		 setGameScene(GAME_TITLE);
	 }
}

//���[�f�B���O��ʕ`��֐�
void GameFrame::drawLoading(GAMESCENE scene){
	if( game_scene == GAME_MAIN ){
		//���[�h��ʕ\��
		DrawString(900,600,"Loading...",0); 
	}
}

//���C�����[�v�֐� MainLoop�֐�===============================================================================================
void GameFrame::doMainLoop(){
	//�����ݒ�-----------------------------------------------------------------------------------------
	ChangeWindowMode( TRUE );
	SetGraphMode( SCREENWIDH, SCREENHEIGHT, 16 );			//�𑜓x�w��
	SetBackgroundColor( 255, 255, 255 ) ;
	SetWindowStyleMode( 9 );
	SetMainWindowText("DDD ver0.01");	//�E�B���h�E�e�N�X�g�̕ύX
	SetDoubleStartValidFlag( TRUE );
	//SetWindowSizeChangeEnableFlag( TRUE );	//�E�B���h�E�T�C�Y��
	SetAlwaysRunFlag( TRUE );				//��A�N�e�B�u�ł����s����悤�ɕύX
	SetOutApplicationLogValidFlag( FALSE );	//���O�o�͂����Ȃ��悤�ɕύX
	if(DxLib_Init() == -1){					//DX���C�u�����̏�����
		MessageBox(NULL, "���C�u�����������G���[�ł�", "�G���[", MB_OK );
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

	SetDrawScreen(DX_SCREEN_BACK);	//�`��Ώۂ̎w��

	try{
		loadFiles(GAME_TITLE);
	}
	catch(int error_code){
		MessageBox(NULL, "�t�@�C���ǂݍ��݃G���[", "�G���[", MB_OK );
		return;
	}
	setGameScene( GAME_TITLE );		//�^�C�g����ʂɐݒ�
	loading = false;
	SetUseASyncLoadFlag(TRUE);

	//���C�����[�v---------------------------------------------------------------------------------------
	while(ProcessMessage() == 0 && quit == false){
		ClsDrawScreen();			//��ʏ���
		if( loading == true ){
			//���[�h���Ȃ�
			drawLoading(game_scene);

			if( game_scene == GAME_MAIN ){
				
				try{
					if( loadFiles(GAME_MAIN) == 0 ){
						//���[�h���S�Ċ���������
						loading = false;
						initScene(GAME_MAIN);
					}
				}
				catch(int error_code){
					std::string str = "�t�@�C���ǂݍ��݃G���[";
					char c[10];
					sprintf(c, "%d", error_code);
					str += c;
					MessageBox(NULL, str.c_str(), "�G���[", MB_OK );
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
		if(CheckHitKey( KEY_INPUT_LSHIFT ))Sleep(300);		//�f�o�b�O�p�X���[���[�h
		if( CheckHitKey( KEY_INPUT_F8 ) ){					//�f�o�b�O�p�|�[�Y�B������F9
			while(ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_F9) == 0){
				Sleep(16);
				drawFrameRate();
			}
		}
		if( CheckHitKey( KEY_INPUT_ESCAPE ) )quit = true;	//�f�o�b�O�pESC�L�[�ŏI��

		keybd_event(0x07, 0, KEYEVENTF_KEYUP, 0);			//�V�X�e���L�[�̖�����
	}
	DxLib_End();					//DX���C�u�����I������
}

//============================================================================================================================