#include "player_data.h"

PlayerData PlayerDataList::pd[4];
int PlayerDataList::player_pc;

int PlayerDataList::loadFiles()
{
	int last = 0;		//�c�菈����
	int chk;
	int graphic_handles[20];
	if ( symbol_anim.empty() == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\symbol.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\symbol.bmp", 15, 5, 3, 32, 32, graphic_handles );
		if ( symbol_anim.getLoadFlag() == false ) {
			if ( chk == -1 ) {
				throw( -5 );						//�񓯊��ǂݍ��݂��G���[�Ȃ�throw
			} else if ( chk >= 0 ) {					//�񓯊��ǂݍ��݂��I����Ă���΃A�j���[�V�����ɒǉ�
				for ( int gh : graphic_handles ) {
					symbol_anim.setImage( 1000, gh );	//apid����Ƀv�b�V�����Ēǉ�����ׂ� aoid �� 1000 �w��
				}
				symbol_anim.setLoadFlag( true );

			} else {
				last++;									//�񓯊��ǂݍ��ݒ��Ȃ�c�菈���{�P
			}
		}
	}
	return last;
}

void PlayerDataList::init()
{
	name_fh = CreateFontToHandle( 0, 14, 5, DX_FONTTYPE_EDGE );
	
}

int PlayerDataList::checkStock( const int p_no, const int *symbol )
{
	for ( int i = 0; i<20; i++ ) {
		if ( symbol[i] > pd[p_no].p_stock[i] ) return 0;
	}
	return 1;
}

void PlayerDataList::draw()
{
	//�Ƃ肠�����e�v���C���[�̖��O��\��
	int dy = OTHER_PLAYER_DATA_Y;
	for ( int i = 0; i < 4; i++ ) {
		if ( pd[i].p_no == player_pc ) {
			//PC�v���C���[�̂ݕʏꏊ�ɕ\��
			DrawFormatStringToHandle( PLAYER_DATA_X, PLAYER_DATA_Y, GetColor( 255, 255, 255 ), name_fh, "%s", pd[i].p_name.c_str() );
			//�N���X�g�v�[���\��
			int px = PLAYER_DATA_X + 2;
			for ( int j = 0; j < 20; j++ ) {
				for ( int k = 0; k < pd[i].p_stock[j]; k++ ) {
					symbol_anim.draw( j, px, PLAYER_DATA_Y + 40 );
					px += 32;
				}
			}
		} else {
			DrawFormatStringToHandle( OTHER_PLAYER_DATA_X, dy, GetColor( 255, 255, 255 ), name_fh, "%s", pd[i].p_name.c_str() );
			dy += PLAYER_DATA_SPACE;
		}
	}

}