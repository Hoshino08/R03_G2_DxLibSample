//ヘッダファイルの読み込み
#include "game.h"		//ゲーム全体のヘッダファイル
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理

//構造体の定義

//キャラクタの構造体
struct CHARACTOR
{
	int handle = -1;	//画像のハンドル（管理番号）
	char path[255];		//画像の場所（パス）
	int x;				//X位置
	int y;				//Y位置
	int width;			//
	int height;			//

	int speed = 1;

	RECT coll;				//当たり判定の領域（四角）
	BOOL IsDraw = FALSE;	//画像が描画できる？
};


//動画の構造体
struct MOVIE
{
	int handle = -1;	//動画のハンドル
	char path[255];		//動画のパス

	int x;			//X位置
	int y;			//Y位置
	int width;		//幅
	int height;		//高さ

	int Volume = 255;	//ボリューム0～255
};

//グローバル変数
//
GAME_SCENE GameScene;		//
GAME_SCENE OldGameScene;	//
GAME_SCENE NextGameScene;	//

//プレイ動画の背景
MOVIE playMovie;

//プレイヤー
CHARACTOR player;

//ゴール
CHARACTOR Goal;

//画面の切り替え
BOOL IsFadeOut = FALSE;	//フェードアウト
BOOL IsFadeIn = FALSE;	//フェードイン

//シーン切り替え
int fadeTimeMill = 2000;	//切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;
int fadeOutCnt = fadeOutCntInit;
int fadeOutCntMax = fadeTimeMax;

//フェードイン
int fadeInCntInit = fadeTimeMax;
int fadeInCnt = fadeInCntInit;
int fadeInCntMax = fadeTimeMax;

//プロトタイプ宣言
VOID Title(VOID);
VOID TitleProc(VOID);
VOID TitleDraw(VOID);

VOID Play(VOID);
VOID PlayProc(VOID);
VOID PlayDraw(VOID);

VOID End(VOID);
VOID EndProc(VOID);
VOID EndDraw(VOID);

VOID Change(VOID);
VOID ChangeProc(VOID);
VOID ChangeDraw(VOID);

VOID ChangeScene(GAME_SCENE scene);	//シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);	//当たり判定の領域を更新
VOID CollUpdate(CHARACTOR* chara);			//当たり判定

BOOL OnCollRect(RECT a, RECT b);		//矩形と矩形の当たり判定

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
	SetWindowStyleMode(GAME_WINDOW_BAR);				//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);								//ディスプレイの垂直同期を有効化する
	SetAlwaysRunFlag(TRUE);								//ウィンドウをずっとアクティブにする

	//ＤＸライブラリ初期化処理S
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//ダブルバッファリングの有効化
	SetDrawScreen(DX_SCREEN_BACK);


	//最初のシーンは、タイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//
	strcpyDx(playMovie.path, ".\\MOVIE\\Milk.mp4");
	playMovie.handle = LoadGraph(playMovie.path);	//画像の読み込み

	//プレイ動画の背景の読み込み

	//
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//メインのウィンドウハンドル
			playMovie.path,				//メッセージ本文
			"動画読み込みエラー！",		//メッセージタイトル
			MB_OK						//ボタン
		);

		DxLib_End();	//強制終了
		return-1;		//エラー終了
	}

	//
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//
	strcpyDx(player.path, ".\\Image\\Player.png");	//パスのコピー
	player.handle = LoadGraph(player.path);	//画像の読み込み

	//
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//メインのウィンドウハンドル
			player.path,				//メッセージ本文
			"画像読み込みエラー！",		//メッセージタイトル
			MB_OK						//ボタン
		);

		DxLib_End();	//強制終了
		return-1;		//エラー終了
	}

	//画像の幅と高さを設定
	GetGraphSize(player.handle, &player.width, &player.height);

	/*
	//当たり判定を更新する
	CollUpdatePlayer(&player);	//プレイヤーの当たり判定のアドレス
	*/

	//プレイヤーの初期化
	player.x = GAME_WIDTH / 2 - player.width / 2;
	player.y = GAME_HEIGHT / 2 - player.height / 2;
	player.speed = 500;
	player.IsDraw = TRUE;

	//当たり判定を更新する
	CollUpdatePlayer(&player);	//プレイヤーの当たり判定のアドレス

	//ゴールの画像を読み込み
	strcpyDx(Goal.path, ".\\Image\\Goal1.png");	//パスのコピー
	Goal.handle = LoadGraph(Goal.path);	//画像の読み込み

	//
	if (Goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//メインのウィンドウハンドル
			Goal.path,				//メッセージ本文
			"画像読み込みエラー！",		//メッセージタイトル
			MB_OK						//ボタン
		);

		DxLib_End();	//強制終了
		return-1;		//エラー終了
	}

	//画像の幅と高さを設定
	GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

	/*
	//当たり判定を更新する
	CollUpdate(&Goal);	//プレイヤーの当たり判定のアドレス
	*/


	//go-ruの初期化
	Goal.x = 0;
	Goal.y = 20;
	Goal.speed = 500;
	Goal.IsDraw = TRUE;

	/*
	//当たり判定を更新する
	CollUpdate(&Goal);	//プレイヤーの当たり判定のアドレス
	*/

	//無限ループ
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボード入力の更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
 		}

		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//
			break;
		case GAME_SCENE_PLAY:
			Play();				//プレイ画面
			break;
		case GAME_SCENE_END:
			End();				//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();			//切り替え画面
			break;
		default:
			break;
		}
		
		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンに保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//FPSを描画
		FPSDraw();

		//FPSを待つ
		FPSWait();
		
		ScreenFlip();	//ダブルバッファリングした画像を描画
	}

	//終わるときの処理
	DeleteGraph(playMovie.handle);
	DeleteGraph(player.handle);
	DeleteGraph(Goal.handle);

	//ＤＸライブラリ使用の終了処理
	DxLib_End();

	return 0;	// ソフトの終了 
}

	/// <summary>
	///シーンを切り替える
	/// </summary>
	/// <param name="scene">シーン</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;
	IsFadeIn = FALSE;
	IsFadeOut = TRUE;

	return;
	}

	/// <summary>
	/// タイトル画面
	/// </summary>
	VOID Title(VOID)
	{
		TitleProc();	//処理
		TitleDraw();	//描画
		return;
	}

	/// <summary>
	/// タイトル画面の処理
	/// </summary>
	VOID TitleProc(VOID)
	{
		
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{

			//シーン切り替え
		//次のシーンの初期化を行うと楽

		//プレイ画面に切り替え
			ChangeScene(GAME_SCENE_PLAY); 
		}
		return;
	}

	/// <summary>
	/// タイトル画面の描画
	/// </summary>
	VOID TitleDraw(VOID)
	{
		DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
		return;
	}
	

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//処理
	PlayDraw();	//描画
	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
VOID PlayProc(VOID)
{
	/*
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}
	*/

	//
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.y -= player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.y += player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.x -= player.speed * fps.DeltaTime;
	}

	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.x += player.speed * fps.DeltaTime;
	}

	//当たり判定を更新する
	CollUpdatePlayer(&player);

	//ゴールの当たり判定を更新する
	CollUpdate(&Goal);

	//プレイヤーがゴールに当たった時は
	if (OnCollRect(player.coll, Goal.coll) == TRUE)
	{
		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}

	//処理を強制終了
	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
VOID PlayDraw(VOID)
{
	//
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//再生する
		SeekMovieToGraph(playMovie.handle, 0);	//シークバーを最初に戻す
		PlayMovieToGraph(playMovie.handle);		//動画の再生
	}
	//動画を描画(画面に合わせて画像を引き伸ばす)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//ぷれいやーを描画
	if (player.IsDraw == TRUE)
	{
		//
		DrawGraph(player.x, player.y, player.handle,TRUE);

		//
		if (GAME_DEBUG == TRUE)
		{
			//
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}
	
	if (Goal.IsDraw == TRUE)
	{
		//ゴールを描画
		DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);

		//画像を描画
		if (GAME_DEBUG == TRUE)
		{
			//
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0)); 
	return;
}

/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();	//処理
	EndDraw();	//描画
	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
VOID EndProc(VOID)
{
	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をここで行うと楽

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}
	
	return;
}

/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//処理
	ChangeDraw();	//描画
	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//カウンタを減らす
		}
		else
		{
			//フェードイン処理が終わった

			fadeInCnt = fadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン処理終了
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//カウンタを増やす
		}
		else
		{
			//フェードアウト処理が終わった

			fadeOutCnt = fadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;				//フェードアウト処理終了
		}
	}

	//切り替え処理終了
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインしていない、フェードアウトもしていないとき
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のゲームシーン更新
	}
	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;

}


/// <summary>
///	当たり判定の領域更新
/// </summary>
/// <param name="scene">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;

	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
///	当たり判定の領域更新
/// </summary>
/// <param name="scene">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;

	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="a">矩形a</param>
/// <param name="b">矩形b</param>
/// <returns></returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if (
		a.left < b.right
		&& a.right > b.left
		&& a.top < b.bottom
		&& a.bottom > b.top
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

