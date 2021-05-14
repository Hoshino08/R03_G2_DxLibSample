#pragma once

//ヘッダファイルの読み込み
#include "DxLib.h"

//キーボードのヘッダファイル

//マクロ定義
#define KEY_KIND_MAX	256	//習得するキーの種類

//キーボード構造隊
struct KEYBOARD
{
	char TempKeyState[KEY_KIND_MAX];	//入力状態を管理
	int AllKeyState[KEY_KIND_MAX];		//すべてのキーの入力時間を管理
	int OldAllKeyState[KEY_KIND_MAX];	//以前のすべてのキーの入力時時間を管理
};

//外部のグローバル変数
extern KEYBOARD keyboard;

//プロトタイプ宣言
extern VOID AllKeyUpdate(VOID);
extern BOOL KeyDown(int KEY_INPUT_);
extern BOOL KeyUp(int KEY_INPUT_);
extern BOOL KeyClick(int KEY_INPUT_);
extern BOOL KeyDownKeep(int KEY_INPUT_,int millSec);