//ヘッダーファイルの読み込み
#include "DxLib.h"	//DxLibを使うときは必要

#include "keyboard.h"

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

	//ＤＸライブラリ初期化処理S
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//ダブルバッファリングの有効化
	SetDownScreen(DX_SOREEN_BACK);

	//円の中心点
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;
	//円の半径
	int radius = 50;

	//無限ループ
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//キー入力
		if (keyDown(KEY_INPUT_UP) == TRUE)
		{
			Y--;
		}

		if (KeyDown(KEY_INPUT_DOWN) == TRUE)
		{
			Y++;
		}
		if (keyDown(KEY_INPUT_LEFT) == TRUE)
		{
			X--;
		}

		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
		{
			X++;
		}

		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);

		ScreenFlip();	//ダブルバッファリングした画像を描画
	}



	//ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;	// ソフトの終了 
}