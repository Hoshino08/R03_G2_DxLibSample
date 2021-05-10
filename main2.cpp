//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使うときは必要

//マクロ定義
#define GAME_TITLE	"ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH	1280				//ゲーム画面の幅(ウィドス)
#define GAME_HEIGHT	720					//ゲーム画面の高さ(ハイト)
#define GAME_COLOR  32					//ゲームの色域

#define GAME_ICON_ID	333				//ゲームのICONのID

#define GAME_WINDOW_BAR 0				//ウィンドウバーの種類

// プログラムは WinMain から始まります
//Windowsのプログラミング方法 = (WinAPI)で動いている！
//DxLibは、DirectXという、ゲームプログラミングをかんたんに使える仕組み
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE); 
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);						//ウィンドウのタイトルの文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景の色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読込
	SetWindowStyleMode(GAME_WINDOW_BAR);
	SetWaitVSyncFlag(TRUE);
	SetAlwaysRunFlag(TRUE);

	//ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}


	SetDrawScreen(DX_SCREEN_BACK);

	//無限ループ
		while (1)
		{
		

			//メッセージを受け取り続ける
			if (ProcessMessage() != 0)	//-1のとき、エラーやウィンドウが閉じられた
			{
				break;	//無限ループを抜ける
			}

			//画面を消去する
			if (ClearDrawScreen() != 0) { break; }

		

			ScreenFlip();
	}


	//ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;	// ソフトの終了 
}