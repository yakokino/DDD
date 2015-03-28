#include "ddd_loader.h"
#include "ddd_map_ui.h"

int DDDMap::loadFiles() throw(...){
	int last = 0;		//残り処理数
	int chk;
	int gh[18];
	if( back_anim.empty() == 1 ){
		DDDLoader::setLoadFile("Media\\Graphic\\back001.jpg");
		chk = DDDLoader::getGraphHandle("Media\\Graphic\\back001.jpg" , 1 );
		if( chk == -1 ) throw(2);						//非同期読み込みがエラーならthrow
		else if( chk >= 0 ) back_anim.setImage(0, chk);	//非同期読み込みが終わっていればアニメーションに追加
		else last++;									//非同期読み込み中なら残り処理＋１
	}
	if( map_anim.empty( MAP_TILE ) == 1 ){
		DDDLoader::setLoadFile("Media\\Graphic\\map_tile.bmp");
		chk = DDDLoader::getGraphHandle("Media\\Graphic\\map_tile.bmp");
		if( chk == -1 ) throw(-2);
		else if( chk >= 0 ) map_anim.setImage(MAP_TILE, chk);
		else last++;
	}
	if( map_anim.empty( MAP_WAY1 ) == 1 ){
		DDDLoader::setLoadFile("Media\\Graphic\\map_way1.bmp");
		chk = DDDLoader::getGraphHandle("Media\\Graphic\\map_way1.bmp");
		if( chk == -1 ) throw(-2);
		else if( chk >= 0 ) map_anim.setImage(MAP_WAY1, chk);
		else last++;
	}
	if( mini_map.empty() == 1 ){
		DDDLoader::setLoadFile("Media\\Graphic\\map_mini.bmp");
		chk = DDDLoader::getGraphHandle("Media\\Graphic\\map_mini.bmp", 10, 10, 1, 9, 5, gh);
		if( chk == -1 ) throw(-2);
		else if( chk >= 0 ){
			for(int i=0 ; i<10 ; i++ ){
				mini_map.setImage(i, gh[i]);
			}
		} else last++;
	}
	return last;
}

void DDDMap::init(){
	for(int i=0 ; i<DDDMAP_H ; i++ ){
		for(int j=0 ; j<DDDMAP_W ; j++ ){
			map_data[i][j] = MAP_TILE;
		}
	}
	map_data[ 0][ 1] = MAP_WAY1;
	map_data[ 1][ 1] = MAP_WAY1;
	map_data[ 9][10] = MAP_WAY1;													//テスト代入(X座標は右側なので注意)
	map_data[10][10] = MAP_WAY1;
	map_data[11][10] = MAP_WAY1;
	map_data[12][10] = MAP_WAY1;
	map_data[10][11] = MAP_WAY1;
	map_data[10][ 9] = MAP_WAY1;
	map_data[ 0][ 9] = MAP_WAY1;

	map_data[13][5] = MAP_WAY1;

	view = false;
}

void DDDMap::setPointXY(int camera_x, int camera_y, int mouse_x, int mouse_y){

	bool map_out = false;
	if( view == false ){
		map_out = true;		//MAP_VIEWコマンドでなければマップ外情報を入力
	} else {
		//ポインタ位置情報更新
		int cx = ( DDDMAP_W - 1 )*MAPTILE_HW - ( DDDMAP_H - 1 )*MAPTILE_HW - camera_x;
		int cy = ( DDDMAP_W - 1 )*MAPTILE_HH + ( DDDMAP_H - 1 )*MAPTILE_HH - camera_y + MAPTILE_HH + MAPTILE_UPSPACE;
		int tmp = (int)( MAPTILE_S * ( mouse_x - cx ) );
		int tmp1 = -tmp + cy;
		int tmp2 =  tmp + cy - MAPTILE_H;
		int tmp3 = tmp1;
		int tmp4 = tmp2;
		
		point_x = -1;
		point_y = -1;
		for(int k=DDDMAP_H ; k>=0 ; k--){
			for(int j=DDDMAP_W ; j>=0 ; j--){
				if( k != DDDMAP_H && j != DDDMAP_W ){
					if( map_data[k][j] != MAP_TILE ){
						cx = j*MAPTILE_HW - k*MAPTILE_HW - camera_x;
						cy = j*MAPTILE_HH + k*MAPTILE_HH - camera_y + MAPTILE_HH + MAPTILE_UPSPACE;
						if( mouse_x > cx && mouse_x < cx + MAPTILE_W ){
							tmp3 -= 4;
							tmp4 -= 4;
						} else {
							continue;
						}
					}
				}
				if( mouse_y >= tmp3 - (DDDMAP_W - 1 - j) * MAPTILE_H ){
					if( mouse_y >= tmp4 - (DDDMAP_H - 1 - k) * MAPTILE_H ){
						point_x = j;
						point_y = k;
						goto getpoint;		//ループを抜ける
					}
				}
				tmp3 = tmp1;
				tmp4 = tmp2;
			}
		}

getpoint:
		if( point_x >= DDDMAP_W || point_y >= DDDMAP_H )map_out = true;		//下側にマップ外ならマップ外情報を入力
	}
	if( map_out == true ){
		//マップ外ならば(-1,-1)にする
		point_x = -1;
		point_y = -1;
	}
}

void DDDMap::actionCommand( COMMAND com, int no ){
	if( com == MAP_VIEW ){
		view = true;
		return;
	}
	view = false;
}

void DDDMap::draw(int camera_x, int camera_y){
	//背景表示
	back_anim.draw( 0, 0, 0, 1.1 );
	//back_anim.draw( 0, -350, -100 );
	
	DrawFormatString(230,0,0,"(%2d,%2d)",point_x,point_y);		//とりあえず仮に座標表示

	
	//マップ表示
	for(int i=0 ; i<DDDMAP_H ; i++ ){
		for(int j=0 ; j<DDDMAP_W ; j++ ){
			int kx = j*MAPTILE_HW - i*MAPTILE_HW - camera_x;
			int ky = j*MAPTILE_HH + i*MAPTILE_HH - camera_y;
			if( map_data[i][j] == MAP_TILE ){
				SetDrawBlendMode( DX_BLENDMODE_ALPHA, 80 );
			}
			if( point_x == j && point_y == i ){
				SetDrawBright( 50, 150, 255 );
			}
			map_anim.draw(map_data[i][j], kx, ky);
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
			SetDrawBright( 255, 255, 255 );
		}
	}

	//ミニマップ表示
	for(int i=0 ; i<DDDMAP_H ; i++ ){
		for(int j=0 ; j<DDDMAP_W ; j++ ){
			mini_map.draw(map_data[i][j], 920 + j*5 - i*5, 520 + j*3 + i*3);
		}
	}
	
	//ミニマップエリア表示
	int x = 920+(int)( ((double)camera_x)/9.6 );
	int y = 520-1+(int)( ((double)camera_y)/8.2 );
	DrawBox( x, y, x+106, y+80, GetColor(255,0,0), FALSE );

	
	
}

//================================================================================================================

int DDDUI::loadFiles(){



	int last = 0;		//残り処理数
	int chk;
	int gh[18];
	if( ui_anim.empty() == 1 ){
		DDDLoader::setLoadFile("Media\\Graphic\\UserInterface.bmp");
		chk = DDDLoader::getGraphHandle("Media\\Graphic\\UserInterface.bmp");
		if( chk == -1 ) throw(-3);						//非同期読み込みがエラーならthrow
		else if( chk >= 0 ) ui_anim.setImage(0, chk);	//非同期読み込みが終わっていればアニメーションに追加
		else last ++;
	}
	if( ui_anim.empty(2) == 1 ){
		DDDLoader::setLoadFile("Media\\Graphic\\phase_button.bmp");
		chk = DDDLoader::getGraphHandle("Media\\Graphic\\phase_button.bmp", 6, 6, 1, 48, 28, gh);
		if( chk == -1 ) throw(-3);
		else if( chk >= 0 ){
			for(int i=0 ; i<6 ; i++ ){
				ui_anim.setImage( i+1, gh[i] );
			}
		} else last++;
	}
	if( window_anim.empty() == 1 ){
		DDDLoader::setLoadFile("Media\\Graphic\\window.bmp");
		chk = DDDLoader::getGraphHandle("Media\\Graphic\\window.bmp", 18, 6, 3, WINDOW_GRAPH_W, WINDOW_GRAPH_H, gh);
		if( chk == -1 ) throw(-3);
		else if( chk >= 0 ){
			for(int i=0 ; i<18 ; i++ ){
				window_anim.setImage( i, gh[i] );
			}
		} else last++;
	}
	return last;

}


void DDDUI::createWindow(int x, int y, int w, int h, WINDOW_TYPE type, WINDOW_TAG tag){
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

void DDDUI::deleteWindow(WINDOW_TAG tag){
	std::vector<WINDOW_DATA>::iterator it = window_list.begin();
	while( it != window_list.end() ){
		if( (*it).tag == tag ){
			it = window_list.erase( it );
		} else {
			it++;
		}
	}
}

void DDDUI::setCommand(int x, int y, int w, int h, COMMAND com, int no){
	COMMAND_DATA command_data;
	command_data.x1 = x;
	command_data.y1 = y;
	command_data.x2 = x + w;
	command_data.y2 = y + h;
	command_data.command = com;
	command_data.no = no;
	command_list.push_back( command_data );
}

void DDDUI::deleteCommand( COMMAND com ){
	std::vector<COMMAND_DATA>::iterator it = command_list.begin();
	while( it != command_list.end() ){
		if( (*it).command == com ){
			it = command_list.erase( it );
		} else {
			it++;
		}
	}

}

void DDDUI::getCommandPos( COMMAND com, int no, int *x, int *y ){
	std::vector<COMMAND_DATA>::iterator it = command_list.begin();
	while( it != command_list.end() ){
		if( (*it).command == com && (*it).no == no ){
			*x = (*it).x1;
			*y = (*it).y1;
			break;
		}
		it++;
	}
}

void DDDUI::setCommandPos( COMMAND com, int no, int x, int y ){
	std::vector<COMMAND_DATA>::iterator it = command_list.begin();
	while( it != command_list.end() ){
		if( (*it).command == com && (*it).no == no ){
			int height = (*it).y2 - (*it).y1;
			int wide   = (*it).x2 - (*it).x1;
			(*it).x1 = x;
			(*it).y1 = y;
			(*it).x2 = x + wide;
			(*it).y2 = y + height;
			break;
		}
		it++;
	}
}

COMMAND DDDUI::checkCommand(int x, int y, int *no){
	std::vector<COMMAND_DATA>::iterator it;
	it = command_list.end();
	while( it != command_list.begin() ){
		//リストの後ろからコマンドをチェックする（リストの後ろのほうが優先度が高い）
		it--;
		if( (*it).x1 < x && (*it). y1 < y && (*it).x2 > x && (*it).y2 > y ){
			*no = (*it).no;
			return (*it).command;		//コマンドが登録されている位置ならばコマンドを返す
		}
	}
	*no = -1;
	return NO_COMMAND;
}



void DDDUI::draw( PHASE phase ){
	//全体UI表示
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, DDDUI_ALPHA );
	ui_anim.draw(0, 0, 0);
	//フェイズ表示
	for(int i=0 ; i<6 ; i++ ){
		if( phase == MAIN_PHASE && ( i == ATTACK_PHASE || i == END_PHASE )  ||
				phase == ATTACK_PHASE && ( i == MAIN_PHASE2 || i == END_PHASE ) ||
					phase == MAIN_PHASE2 && i == END_PHASE || phase == i ){
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		}
		
		else {
			SetDrawBlendMode( DX_BLENDMODE_ALPHA, PHASE_ALPHA );
		}
		ui_anim.draw( i + 1, PHASE_POSX + i * PHASE_SPACE, PHASE_POSY );
	}
	
	SetDrawBlendMode( DX_BLENDMODE_ALPHA, 150 );
	//ウィンドウリスト表示
	std::vector<WINDOW_DATA>::iterator it = window_list.begin();
	for( it = window_list.begin() ; it != window_list.end() ; it++ ){
		for(int i=0 ; i<(*it).h_num ; i++ ){
			for(int j=0 ; j<(*it).w_num ; j++ ){
				if( i == 0 ){
					if( j == 0 ){
						window_anim.draw( 3, (*it).x, (*it).y );						//左上表示
						window_anim.draw( 0, (*it).x, (*it).y );
					} else if( j == (*it).w_num - 1 ){
						window_anim.draw( 5, (*it).x + j * WINDOW_GRAPH_W, (*it).y );	//右上表示
						window_anim.draw( 2, (*it).x + j * WINDOW_GRAPH_W, (*it).y );
					} else {
						window_anim.draw( 4, (*it).x + j * WINDOW_GRAPH_W, (*it).y );	//上表示
						window_anim.draw( 1, (*it).x + j * WINDOW_GRAPH_W, (*it).y );
					}

				} else if( i == (*it).h_num - 1 ){
					if( j == 0 ){
						window_anim.draw( 15, (*it).x, (*it).y + i * WINDOW_GRAPH_H );	//左下表示
						window_anim.draw( 12, (*it).x, (*it).y + i * WINDOW_GRAPH_H );
					} else if( j == (*it).w_num - 1 ){
						window_anim.draw( 17, (*it).x + j * WINDOW_GRAPH_W, (*it).y + i * WINDOW_GRAPH_H );//右下表示
						window_anim.draw( 14, (*it).x + j * WINDOW_GRAPH_W, (*it).y + i * WINDOW_GRAPH_H );
					} else {
						window_anim.draw( 16, (*it).x + j * WINDOW_GRAPH_W, (*it).y + i * WINDOW_GRAPH_H );//下表示
						window_anim.draw( 13, (*it).x + j * WINDOW_GRAPH_W, (*it).y + i * WINDOW_GRAPH_H );
					}
				} else if( j == 0 ){
					window_anim.draw( 9, (*it).x, (*it).y + i * WINDOW_GRAPH_H );						//左表示
					window_anim.draw( 6, (*it).x, (*it).y + i * WINDOW_GRAPH_H );
				} else if( j == (*it).w_num - 1 ){
					window_anim.draw( 11, (*it).x + j * WINDOW_GRAPH_W, (*it).y + i * WINDOW_GRAPH_H );	//右表示
					window_anim.draw( 8, (*it).x + j * WINDOW_GRAPH_W, (*it).y + i * WINDOW_GRAPH_H );
				} else {
					window_anim.draw( 10, (*it).x + j * WINDOW_GRAPH_W, (*it).y + i * WINDOW_GRAPH_H );	//中表示
				}

			}
		}
	}
	
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
}