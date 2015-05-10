#include "ddd_mouse.h"

void DDDMouse::getInput()
{
	GetMousePoint( &mouse_data.nx, &mouse_data.ny );					//ポインタ位置取得
	mouse_wheel = GetMouseWheelRotVol();		//マウスホイール回転量取得

	int input = GetMouseInput();
	if ( input & MOUSE_INPUT_LEFT ) {
		if ( pressed_frames[MOUSE_LEFT] < 0 ) pressed_frames[MOUSE_LEFT] = 0;
		pressed_frames[MOUSE_LEFT]++;
	} else {
		if ( pressed_frames[MOUSE_LEFT] > 0 ) pressed_frames[MOUSE_LEFT] = 0;
		pressed_frames[MOUSE_LEFT] -= 1;
	}
	if ( input & MOUSE_INPUT_RIGHT ) {
		if ( pressed_frames[MOUSE_RIGHT] < 0 ) pressed_frames[MOUSE_RIGHT] = 0;
		pressed_frames[MOUSE_RIGHT]++;
	} else {
		if ( pressed_frames[MOUSE_RIGHT] > 0 ) pressed_frames[MOUSE_RIGHT] = 0;
		pressed_frames[MOUSE_RIGHT]--;
	}
	if ( input & MOUSE_INPUT_MIDDLE ) {
		if ( pressed_frames[MOUSE_MIDDLE] < 0 ) pressed_frames[MOUSE_MIDDLE] = 0;
		pressed_frames[MIDDLE_CLICK]++;
	} else {
		if ( pressed_frames[MOUSE_MIDDLE] > 0 ) pressed_frames[MOUSE_MIDDLE] = 0;
		pressed_frames[MOUSE_MIDDLE]--;
	}
	mouse_data.dis_x = lclick_x - mouse_data.nx;
	mouse_data.dis_y = lclick_y - mouse_data.ny;
	lclick_x = mouse_data.nx;
	lclick_y = mouse_data.ny;

	checkCommand();		//mouse_dataのcommandとnoを代入
	checkMouseState();	//mouse_dataのaction等を代入
}

void DDDMouse::checkCommand()
{
	std::vector<COMMAND_DATA>::iterator it;
	it = command_list.end();
	while ( it != command_list.begin() ) {
		//リストの後ろからコマンドをチェックする（リストの後ろのほうが優先度が高い）
		it--;
		if ( ( *it ).x1 < mouse_data.nx && ( *it ).y1 < mouse_data.ny && ( *it ).x2 > mouse_data.nx && ( *it ).y2 > mouse_data.ny ) {
			mouse_data.command_no = ( *it ).no;
			mouse_data.command = ( *it ).command;		//コマンドが登録されている位置ならばコマンドを返す
			return;
		}
	}
	mouse_data.command_no = -1;
	mouse_data.command = NO_COMMAND;
}

void DDDMouse::checkMouseState()
{
	if ( pressed_frames[MOUSE_LEFT] == 1 ) {

		click_x = mouse_data.nx;	//クリック開始時のポインタ位置
		click_y = mouse_data.ny;

		mouse_data.push_command = mouse_data.command;	//マウス左ボタンプッシュダウンされた場所のコマンドとコマンドナンバーを記憶する
		mouse_data.push_no = mouse_data.command_no;

		//プッシュダウンアクション
		mouse_data.action = LEFT_PUSH_DOWN;

		//ddd_dice.actionCommand( push_com, 1000 + phase * 100 + no );		//プッシュダウンされたらナンバーに1000足してコマンド実行
		//ddd_card.actionCommand( push_com, 1000 + phase * 100 + no );
		//actionCommand( push_com, 1000 + no );

	} else if ( pressed_frames[MOUSE_LEFT] > 1 ) {
		mouse_data.action = LEFT_DRAG;
	} else if ( pressed_frames[MOUSE_LEFT] == -1 ) {
		if ( mouse_data.push_command != NO_COMMAND && mouse_data.push_command == mouse_data.command && mouse_data.push_no == mouse_data.command_no ) {
			//マウス左ボタンが離された時、クリックされた時のコマンドと一致しているなら

			//クリックアクション実行
			mouse_data.action = LEFT_CLICK;

			//ddd_dice.actionCommand( push_com, 2000 + phase * 100 + no );			//ナンバーに2000足してコマンド実行
			//ddd_card.actionCommand( push_com, 2000 + phase * 100 + no );
			//actionCommand( push_com, 2000 + no );
		} else {
			mouse_data.action = LEFT_PUSH_UP;
		}
		mouse_data.push_command = NO_COMMAND;

	} else if ( pressed_frames[MOUSE_RIGHT] == 1 ) {
		mouse_data.action = RIGHT_PUSH_DOWN;
	} else if ( pressed_frames[MOUSE_RIGHT] > 1 ) {
		mouse_data.action = RIGHT_DRAG;
	} else if ( pressed_frames[MOUSE_RIGHT] == -1 ) {
		mouse_data.action = RIGHT_PUSH_UP;
	} else {												//TODO:中クリックの追記
		mouse_data.action = OVER;
	}
	//ddd_map.actionCommand( ncom );
	//ddd_dice.actionCommand( ncom, phase * 100 + no );						//マウスオーバーされた時にナンバーそのままでコマンド実行
	//ddd_card.actionCommand( ncom, phase * 100 + no );
	//actionCommand( push_com, no );
	
}


void DDDMouse::setCommand( int x, int y, int w, int h, COMMAND com, int no )
{
	COMMAND_DATA command_data;
	command_data.x1 = x;
	command_data.y1 = y;
	command_data.x2 = x + w;
	command_data.y2 = y + h;
	command_data.command = com;
	command_data.no = no;
	command_list.push_back( command_data );
}

void DDDMouse::deleteCommand( COMMAND com )
{
	std::vector<COMMAND_DATA>::iterator it = command_list.begin();
	while ( it != command_list.end() ) {
		if ( ( *it ).command == com ) {
			it = command_list.erase( it );
		} else {
			it++;
		}
	}

}

void DDDMouse::getCommandPos( COMMAND com, int no, int *x, int *y )
{
	std::vector<COMMAND_DATA>::iterator it = command_list.begin();
	while ( it != command_list.end() ) {
		if ( ( *it ).command == com && ( *it ).no == no ) {
			*x = ( *it ).x1;
			*y = ( *it ).y1;
			break;
		}
		it++;
	}
}

void DDDMouse::setCommandPos( COMMAND com, int no, int x, int y )
{
	std::vector<COMMAND_DATA>::iterator it = command_list.begin();
	while ( it != command_list.end() ) {
		if ( ( *it ).command == com && ( *it ).no == no ) {
			int height = ( *it ).y2 - ( *it ).y1;
			int wide = ( *it ).x2 - ( *it ).x1;
			( *it ).x1 = x;
			( *it ).y1 = y;
			( *it ).x2 = x + wide;
			( *it ).y2 = y + height;
			break;
		}
		it++;
	}
}
