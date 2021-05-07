//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"	//DxLib���g���Ƃ��͕K�v

//�}�N����`
#define GAME_TITLE	"�Q�[���^�C�g��"	//�Q�[���^�C�g��
#define GAME_WIDTH	1280				//�Q�[����ʂ̕�(�E�B�h�X)
#define GAME_HEIGHT	720					//�Q�[����ʂ̍���(�n�C�g)
#define GAME_COLOR  32					//�Q�[���̐F��

#define GAME_ICON_ID	333				//�Q�[����ICON��ID

#define GAME_WINDOW_BAR 0				//�E�B���h�E�o�[�̎��

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
	SetWindowStyleMode(GAME_WINDOW_BAR);
	SetWaitVSyncFlag(TRUE);
	SetAlwaysRunFlag(TRUE);

	//�c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//���i�̈ʒu�����߂�
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	//�l�p�̑傫�������߂�
	int width = 32;
	int height = 32;

	//�~�̔��a�����߂�
	int radius = 100;

	//���x
	int Speed = 5;
	int XSpeed = Speed;
	int YSpeed = Speed;

	SetDrawScreen(DX_SCREEN_BACK);

	//�������[�v
		while (1)
		{
			//��������L�[�������ꂽ�Ƃ�
			if (CheckHitKeyAll() != 0)	//0�̂Ƃ��ɁA�����L�[�������ꂽ
			{
				break;	//�������[�v�𔲂���
			}

			//���b�Z�[�W���󂯎�葱����
			if (ProcessMessage() != 0)	//-1�̂Ƃ��A�G���[��E�B���h�E������ꂽ
			{
				break;	//�������[�v�𔲂���
			}

			//��ʂ���������
			if (ClearDrawScreen() != 0) { break; }

			//�l�p��`��
			//DrawBox(
			//	X, Y, X + width,Y +  height,
			//	GetColor(255, 0, 0),
			//	TRUE
			//);

			//�~��`��
			DrawCircle(
				X, Y, radius,
				GetColor(0, 255, 0),
				FALSE, 5
			);

			X += XSpeed;

			Y += YSpeed;

			if (X < 0 || X + width > GAME_WIDTH)
			{
				XSpeed = -XSpeed;
			}

			if (Y < 0 || Y + height > GAME_HEIGHT)
			{
				YSpeed = -YSpeed;
			}

			ScreenFlip();
	}


	//�c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;	// �\�t�g�̏I�� 
}