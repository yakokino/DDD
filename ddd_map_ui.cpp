#include "ddd_loader.h"
#include "ddd_map_ui.h"

int DDDMap::loadFiles() throw( ... )
{
	int last = 0;		//�c�菈����
	int chk;
	int gh[18];
	if ( back_anim.empty() == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\back001.jpg" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\back001.jpg", 1 );
		if ( chk == -1 ) throw( 2 );						//�񓯊��ǂݍ��݂��G���[�Ȃ�throw
		else if ( chk >= 0 ) back_anim.setImage( 0, chk );	//�񓯊��ǂݍ��݂��I����Ă���΃A�j���[�V�����ɒǉ�
		else last++;									//�񓯊��ǂݍ��ݒ��Ȃ�c�菈���{�P
	}
	if ( map_anim.empty( MAP_TILE ) == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\map_tile.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\map_tile.bmp" );
		if ( chk == -1 ) throw( -2 );
		else if ( chk >= 0 ) map_anim.setImage( MAP_TILE, chk );
		else last++;
	}
	if ( map_anim.empty( MAP_WAY_GREEN ) == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\map_way1.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\map_way1.bmp" );
		if ( chk == -1 ) throw( -2 );
		else if ( chk >= 0 ) map_anim.setImage( MAP_WAY_GREEN, chk );
		else last++;
	}
	if ( mini_map.empty() == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\map_mini.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\map_mini.bmp", 10, 10, 1, 9, 5, gh );
		if ( chk == -1 ) throw( -2 );
		else if ( chk >= 0 ) {
			for ( int i = 0; i < 10; i++ ) {
				mini_map.setImage( i, gh[i] );
			}
		} else last++;
	}
	return last;
}

void DDDMap::init()
{
	for ( int i = 0; i < DDDMAP_H; i++ ) {
		for ( int j = 0; j < DDDMAP_W; j++ ) {
			map_data[i][j] = MAP_TILE;
		}
	}

	camera_data.nx = -300;	//TODO:�����G���A�ʒu�ɂ���ď������ʒu��ύX����K�v����
	camera_data.ny = 0;
	camera_data.enable_scroll = false;

	map_data[0][1] = MAP_WAY_GREEN;
	map_data[1][1] = MAP_WAY_GREEN;													//TODO: �폜
	map_data[9][10] = MAP_WAY_GREEN;													//�e�X�g���(X���W�͉E���Ȃ̂Œ���)
	map_data[10][10] = MAP_WAY_GREEN;
	map_data[11][10] = MAP_WAY_GREEN;
	map_data[12][10] = MAP_WAY_GREEN;
	map_data[10][11] = MAP_WAY_GREEN;
	map_data[10][9] = MAP_WAY_GREEN;
	map_data[0][9] = MAP_WAY_GREEN;

	map_data[13][5] = MAP_WAY_GREEN;

}

void DDDMap::setPointXY( int mouse_x, int mouse_y )
{
	// �|�C���^�ʒu���X�V
	int cx = ( DDDMAP_W - 1 )*MAPTILE_HW - ( DDDMAP_H - 1 )*MAPTILE_HW - camera_data.nx;
	int cy = ( DDDMAP_W - 1 )*MAPTILE_HH + ( DDDMAP_H - 1 )*MAPTILE_HH - camera_data.ny + MAPTILE_HH + MAPTILE_UPSPACE;
	int tmp = (int)( MAPTILE_S * ( mouse_x - cx ) );
	int tmp1 = -tmp + cy;
	int tmp2 = tmp + cy - MAPTILE_H;
	bool may_chx = false;	// ���W�ς�邩���t���O
	bool may_chy = false;

	point_x = -1;
	point_y = -1;

	if ( mouse_y >= tmp1 + MAPTILE_H ) {	// �����͈֔͊O���ǂ����̃`�F�b�N
		point_x = -1;
	} else if ( mouse_y >= tmp2 + MAPTILE_H ) {
		point_y = -1;
	} else {
		// X���W�̌���i������j
		for ( int j = DDDMAP_W - 1; j >= 0; j-- ) {
			if ( mouse_y >= tmp1 - ( DDDMAP_W - 1 - j ) * MAPTILE_H - MAP_WAY_UP_PIXEL ) {
				if ( mouse_y < tmp1 - ( DDDMAP_W - 1 - j ) * MAPTILE_H ) {
					may_chx = true;	// ���W�ʒu���^�C���̏ꍇ���W���ς��
				}
				point_x = j;
				break;
			}
		}
		// Y���W�̌���i������j
		for ( int k = DDDMAP_H - 1; k >= 0; k-- ) {
			if ( mouse_y >= tmp2 - ( DDDMAP_H - 1 - k ) * MAPTILE_H - MAP_WAY_UP_PIXEL ) {
				if ( mouse_y < tmp2 - ( DDDMAP_H - 1 - k ) * MAPTILE_H ) {
					may_chy = true;	// ���W�ʒu���^�C���̏ꍇ���W���ς��
				}
				point_y = k;
				break;
			}
		}

		// �����W�ʒu���^�C���̏ꍇ�A�t���O�ɂ���ă}�b�v���W�ʒu�����炷
		if ( map_data[point_y][point_x] == MAP_TILE ) {
			if ( may_chx ) {
				point_x--;
			}
			if ( may_chy ) {
				point_y--;
			}
		}
	}
	// �}�b�v�O�Ȃ�}�b�v�O�������
	if ( point_x < 0 || point_y < 0 ) {
		point_x = -1;
		point_y = -1;
	}
}

void DDDMap::mouseAction( MOUSE_ACTION_DATA* mouse_data )
{
	if ( mouse_data->push_command == MAP_VIEW && mouse_data->action == LEFT_DRAG ) {
		camera_data.nx += mouse_data->dis_x;	//�J�����X�N���[��
		camera_data.ny += mouse_data->dis_y;
	}
	if ( mouse_data->command == MAP_VIEW ) {
		setPointXY( mouse_data->nx, mouse_data->ny );	//�|�C���^���W���}�b�v���W�֕ϊ�
	} else {
		point_x = -1;
		point_y = -1;
	}
}

void DDDMap::draw()
{
	//�w�i�\��
	back_anim.draw( 0, 0, 0, 1.1 );
	//back_anim.draw( 0, -350, -100 );

	DrawFormatString( 230, 0, 0, "(%2d,%2d)", point_x, point_y );		// TODO:���� �Ƃ肠�������ɍ��W�\��


	//�}�b�v�\��
	for ( int i = 0; i < DDDMAP_H; i++ ) {
		for ( int j = 0; j < DDDMAP_W; j++ ) {
			int kx = j*MAPTILE_HW - i*MAPTILE_HW - camera_data.nx;
			int ky = j*MAPTILE_HH + i*MAPTILE_HH - camera_data.ny;
			if ( map_data[i][j] == MAP_TILE ) {
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, 80 );		//TODO:�����x�萔��
			}
			if ( point_x == j && point_y == i ) {
				SetDrawBright( 50, 150, 255 );		//TODO:�F�����x�萔��
			}
			map_anim.draw( map_data[i][j], kx, ky );
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
			SetDrawBright( 255, 255, 255 );
		}
	}

	//�~�j�}�b�v�\��
	for ( int i = 0; i < DDDMAP_H; i++ ) {
		for ( int j = 0; j < DDDMAP_W; j++ ) {
			mini_map.draw( map_data[i][j], 920 + j * 5 - i * 5, 520 + j * 3 + i * 3 );	//TODO:�~�j�}�b�v�\���ʒu�萔��
		}
	}

	//�~�j�}�b�v�G���A�\��
	int x = 920 + (int)( ( (double)camera_data.nx ) / 9.6 );		//TODO:�k�����萔��
	int y = 520 - 1 + (int)( ( (double)camera_data.ny ) / 8.2 );
	DrawBox( x, y, x + 106, y + 80, GetColor( 255, 0, 0 ), FALSE );

}

//================================================================================================================

int DDDUI::loadFiles()
{
	int last = 0;		//�c�菈����
	int chk;
	int gh[18];
	if ( ui_anim.empty() == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\UserInterface.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\UserInterface.bmp" );
		if ( chk == -1 ) throw( -3 );						//�񓯊��ǂݍ��݂��G���[�Ȃ�throw
		else if ( chk >= 0 ) ui_anim.setImage( 0, chk );	//�񓯊��ǂݍ��݂��I����Ă���΃A�j���[�V�����ɒǉ�
		else last++;
	}
	if ( ui_anim.empty( 2 ) == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\phase_button.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\phase_button.bmp", 6, 6, 1, 48, 28, gh );
		if ( chk == -1 ) throw( -3 );
		else if ( chk >= 0 ) {
			for ( int i = 0; i < 6; i++ ) {
				ui_anim.setImage( i + 1, gh[i] );
			}
		} else last++;
	}
	if ( window_anim.empty() == 1 ) {
		DDDLoader::setLoadFile( "Media\\Graphic\\window.bmp" );
		chk = DDDLoader::getGraphHandle( "Media\\Graphic\\window.bmp", 18, 6, 3, WINDOW_GRAPH_W, WINDOW_GRAPH_H, gh );
		if ( chk == -1 ) throw( -3 );
		else if ( chk >= 0 ) {
			for ( int i = 0; i < 18; i++ ) {
				window_anim.setImage( i, gh[i] );
			}
		} else last++;
	}
	return last;

}

void DDDUI::createWindow( int x, int y, int w, int h, WINDOW_TYPE type, WINDOW_TAG tag )
{
	int w_num = w / WINDOW_GRAPH_W;
	int h_num = h / WINDOW_GRAPH_H;
	WINDOW_DATA wd;
	wd.x = x;
	wd.y = y;
	wd.w_num = w_num;
	wd.h_num = h_num;
	wd.type = type;
	wd.tag = tag;
	window_list.push_back( wd );
}

void DDDUI::deleteWindow( WINDOW_TAG tag )
{
	std::vector<WINDOW_DATA>::iterator it = window_list.begin();
	while ( it != window_list.end() ) {
		if ( ( *it ).tag == tag ) {
			it = window_list.erase( it );
		} else {
			it++;
		}
	}
}

void DDDUI::draw( PHASE phase )
{
	//�S��UI�\��
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, DDDUI_ALPHA );
	ui_anim.draw( 0, 0, 0 );
	//�t�F�C�Y�\��
	for ( int i = 0; i < 6; i++ ) {
		if ( phase == MAIN_PHASE && ( i == ATTACK_PHASE || i == END_PHASE ) ||
			phase == ATTACK_PHASE && ( i == MAIN_PHASE2 || i == END_PHASE ) ||
			phase == MAIN_PHASE2 && i == END_PHASE || phase == i ) {
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		} else {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, PHASE_ALPHA );
		}
		ui_anim.draw( i + 1, PHASE_POSX + i * PHASE_SPACE, PHASE_POSY );
	}

	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 150 );
	//�E�B���h�E���X�g�\��
	std::vector<WINDOW_DATA>::iterator it = window_list.begin();
	for ( it = window_list.begin(); it != window_list.end(); it++ ) {
		for ( int i = 0; i < ( *it ).h_num; i++ ) {
			for ( int j = 0; j < ( *it ).w_num; j++ ) {
				if ( i == 0 ) {
					if ( j == 0 ) {
						window_anim.draw( 3, ( *it ).x, ( *it ).y );						//����\��
						window_anim.draw( 0, ( *it ).x, ( *it ).y );
					} else if ( j == ( *it ).w_num - 1 ) {
						window_anim.draw( 5, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y );	//�E��\��
						window_anim.draw( 2, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y );
					} else {
						window_anim.draw( 4, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y );	//��\��
						window_anim.draw( 1, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y );
					}

				} else if ( i == ( *it ).h_num - 1 ) {
					if ( j == 0 ) {
						window_anim.draw( 15, ( *it ).x, ( *it ).y + i * WINDOW_GRAPH_H );	//�����\��
						window_anim.draw( 12, ( *it ).x, ( *it ).y + i * WINDOW_GRAPH_H );
					} else if ( j == ( *it ).w_num - 1 ) {
						window_anim.draw( 17, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y + i * WINDOW_GRAPH_H );//�E���\��
						window_anim.draw( 14, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y + i * WINDOW_GRAPH_H );
					} else {
						window_anim.draw( 16, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y + i * WINDOW_GRAPH_H );//���\��
						window_anim.draw( 13, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y + i * WINDOW_GRAPH_H );
					}
				} else if ( j == 0 ) {
					window_anim.draw( 9, ( *it ).x, ( *it ).y + i * WINDOW_GRAPH_H );						//���\��
					window_anim.draw( 6, ( *it ).x, ( *it ).y + i * WINDOW_GRAPH_H );
				} else if ( j == ( *it ).w_num - 1 ) {
					window_anim.draw( 11, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y + i * WINDOW_GRAPH_H );	//�E�\��
					window_anim.draw( 8, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y + i * WINDOW_GRAPH_H );
				} else {
					window_anim.draw( 10, ( *it ).x + j * WINDOW_GRAPH_W, ( *it ).y + i * WINDOW_GRAPH_H );	//���\��
				}

			}
		}
	}

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
}