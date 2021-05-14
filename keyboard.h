#pragma once

//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h"

//�L�[�{�[�h�̃w�b�_�t�@�C��

//�}�N����`
#define KEY_KIND_MAX	256	//�K������L�[�̎��

//�L�[�{�[�h�\����
struct KEYBOARD
{
	char TempKeyState[KEY_KIND_MAX];	//���͏�Ԃ��Ǘ�
	int AllKeyState[KEY_KIND_MAX];		//���ׂẴL�[�̓��͎��Ԃ��Ǘ�
	int OldAllKeyState[KEY_KIND_MAX];	//�ȑO�̂��ׂẴL�[�̓��͎����Ԃ��Ǘ�
};

//�O���̃O���[�o���ϐ�
extern KEYBOARD keyboard;

//�v���g�^�C�v�錾
extern VOID AllKeyUpdate(VOID);
extern BOOL KeyDown(int KEY_INPUT_);
extern BOOL KeyUp(int KEY_INPUT_);
extern BOOL KeyClick(int KEY_INPUT_);
extern BOOL KeyDownKeep(int KEY_INPUT_,int millSec);