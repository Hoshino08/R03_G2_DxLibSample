//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

//�\���̂̒�`

//�L�����N�^�̍\����
struct CHARACTOR
{
	int handle = -1;	//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];		//�摜�̏ꏊ�i�p�X�j
	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//
	int height;			//

	int speed = 1;

	RECT coll;				//�����蔻��̗̈�i�l�p�j
	BOOL IsDraw = FALSE;	//�摜���`��ł���H
};


//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;			//X�ʒu
	int y;			//Y�ʒu
	int width;		//��
	int height;		//����

	int Volume = 255;	//�{�����[��0�`255
};

//�O���[�o���ϐ�
//
GAME_SCENE GameScene;		//
GAME_SCENE OldGameScene;	//
GAME_SCENE NextGameScene;	//

//�v���C����̔w�i
MOVIE playMovie;

//�v���C���[
CHARACTOR player;

//�S�[��
CHARACTOR Goal;

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;	//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;	//�t�F�[�h�C��

//�V�[���؂�ւ�
int fadeTimeMill = 2000;	//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;
int fadeOutCnt = fadeOutCntInit;
int fadeOutCntMax = fadeTimeMax;

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;
int fadeInCnt = fadeInCntInit;
int fadeInCntMax = fadeTimeMax;

//�v���g�^�C�v�錾
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

VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);			//�����蔻��

BOOL OnCollRect(RECT a, RECT b);		//��`�Ƌ�`�̓����蔻��

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O���@ = (WinAPI)�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O�����񂽂�Ɏg����d�g��
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����Ǎ�
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L��������
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	//�c�w���C�u��������������S
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�_�u���o�b�t�@�����O�̗L����
	SetDrawScreen(DX_SCREEN_BACK);


	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//
	strcpyDx(playMovie.path, ".\\MOVIE\\Milk.mp4");
	playMovie.handle = LoadGraph(playMovie.path);	//�摜�̓ǂݍ���

	//�v���C����̔w�i�̓ǂݍ���

	//
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
			playMovie.path,				//���b�Z�[�W�{��
			"����ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK						//�{�^��
		);

		DxLib_End();	//�����I��
		return-1;		//�G���[�I��
	}

	//
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//
	strcpyDx(player.path, ".\\Image\\Player.png");	//�p�X�̃R�s�[
	player.handle = LoadGraph(player.path);	//�摜�̓ǂݍ���

	//
	if (player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
			player.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK						//�{�^��
		);

		DxLib_End();	//�����I��
		return-1;		//�G���[�I��
	}

	//�摜�̕��ƍ�����ݒ�
	GetGraphSize(player.handle, &player.width, &player.height);

	/*
	//�����蔻����X�V����
	CollUpdatePlayer(&player);	//�v���C���[�̓����蔻��̃A�h���X
	*/

	//�v���C���[�̏�����
	player.x = GAME_WIDTH / 2 - player.width / 2;
	player.y = GAME_HEIGHT / 2 - player.height / 2;
	player.speed = 500;
	player.IsDraw = TRUE;

	//�����蔻����X�V����
	CollUpdatePlayer(&player);	//�v���C���[�̓����蔻��̃A�h���X

	//�S�[���̉摜��ǂݍ���
	strcpyDx(Goal.path, ".\\Image\\Goal1.png");	//�p�X�̃R�s�[
	Goal.handle = LoadGraph(Goal.path);	//�摜�̓ǂݍ���

	//
	if (Goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
			Goal.path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK						//�{�^��
		);

		DxLib_End();	//�����I��
		return-1;		//�G���[�I��
	}

	//�摜�̕��ƍ�����ݒ�
	GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

	/*
	//�����蔻����X�V����
	CollUpdate(&Goal);	//�v���C���[�̓����蔻��̃A�h���X
	*/


	//go-ru�̏�����
	Goal.x = 0;
	Goal.y = 20;
	Goal.speed = 500;
	Goal.IsDraw = TRUE;

	/*
	//�����蔻����X�V����
	CollUpdate(&Goal);	//�v���C���[�̓����蔻��̃A�h���X
	*/

	//�������[�v
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
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
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}
		
		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[���ɕۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS��`��
		FPSDraw();

		//FPS��҂�
		FPSWait();
		
		ScreenFlip();	//�_�u���o�b�t�@�����O�����摜��`��
	}

	//�I���Ƃ��̏���
	DeleteGraph(playMovie.handle);
	DeleteGraph(player.handle);
	DeleteGraph(Goal.handle);

	//�c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;	// �\�t�g�̏I�� 
}

	/// <summary>
	///�V�[����؂�ւ���
	/// </summary>
	/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;
	IsFadeIn = FALSE;
	IsFadeOut = TRUE;

	return;
	}

	/// <summary>
	/// �^�C�g�����
	/// </summary>
	VOID Title(VOID)
	{
		TitleProc();	//����
		TitleDraw();	//�`��
		return;
	}

	/// <summary>
	/// �^�C�g����ʂ̏���
	/// </summary>
	VOID TitleProc(VOID)
	{
		
		if (KeyClick(KEY_INPUT_RETURN) == TRUE)
		{

			//�V�[���؂�ւ�
		//���̃V�[���̏��������s���Ɗy

		//�v���C��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_PLAY); 
		}
		return;
	}

	/// <summary>
	/// �^�C�g����ʂ̕`��
	/// </summary>
	VOID TitleDraw(VOID)
	{
		DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
		return;
	}
	

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//����
	PlayDraw();	//�`��
	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	/*
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
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

	//�����蔻����X�V����
	CollUpdatePlayer(&player);

	//�S�[���̓����蔻����X�V����
	CollUpdate(&Goal);

	//�v���C���[���S�[���ɓ�����������
	if (OnCollRect(player.coll, Goal.coll) == TRUE)
	{
		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	//�����������I��
	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(playMovie.handle, 0);	//�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);		//����̍Đ�
	}
	//�����`��(��ʂɍ��킹�ĉ摜�������L�΂�)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//�Ղꂢ��[��`��
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
		//�S�[����`��
		DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);

		//�摜��`��
		if (GAME_DEBUG == TRUE)
		{
			//
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0)); 
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();	//����
	EndDraw();	//�`��
	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}
	
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//����
	ChangeDraw();	//�`��
	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�𑝂₷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;				//�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}
	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;

}


/// <summary>
///	�����蔻��̗̈�X�V
/// </summary>
/// <param name="scene">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;

	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
///	�����蔻��̗̈�X�V
/// </summary>
/// <param name="scene">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;

	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="a">��`a</param>
/// <param name="b">��`b</param>
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

