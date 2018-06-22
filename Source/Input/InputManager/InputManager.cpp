/*!
	@file	InputManager.h
	@date	2018/04/13
	@author	�ԏ� �G�P
	@brief	���͊Ǘ��N���X
*/
#include "InputManager.h"
#include "../Keyboard/Keyboard.h"

/*!
	@brief	���O���
*/
using namespace Keyboard;
using namespace KeyCode;

/*!
	@def	�萔�錾
*/
const int InputManager::RESET_FRAME = 15;

/*!
	@brief	�R���X�g���N�^
*/
InputManager::InputManager()
{
}

/*!
	@brief	�f�X�g���N�^
*/
InputManager::~InputManager()
{
	Finalize();
}

/*!
	@brief	������
*/
void InputManager::Initialize()
{
	/*! 1P�R���g���[���[���g�� */
	pGamePad = new GamePad(GamePad::Index::One);
	/*! �A�����͖h�~�p�t���O */
	isInput = 0;
}

/*!
	@brief	���̓o�b�t�@�X�V
*/
void InputManager::Update()
{
	Keyboard::Update();
	pGamePad->Update();
}

/*!
	@brief	���
*/
void InputManager::Finalize()
{
	if (!pGamePad) {
		delete(pGamePad);
		pGamePad = nullptr;
	}
}

/*!
	@brief	�������͂���Ă����
*/
bool InputManager::GetLeft()
{
	/*! �Q�[���p�b�h�̍��W���C�X�e�B�b�N */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).x < 0) {
		return true;
	}

	/*! �Q�[���p�b�h�̏\���L�[ */
	if (pGamePad->GetArrow(Arrow::HORIZONTAL) < 0) {
		return true;
	}

	/*! �L�[�{�[�h */
	if (GetButton(Keyboard::LEFT)) {
		return true;
	}

	return false;
}

/*!
	@brief	�������͂��ꂽ�u��
*/
bool InputManager::GetLeftDown()
{
	/*! �Q�[���p�b�h�̍��W���C�X�e�B�b�N */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).x < 0.0f) {

		/*! �}�X�N�Ƀq�b�g���Ȃ���Ώ��������X�L�b�v */
		bool isSkip = (isInput & 0b0100) != 0;
		if (isSkip) { return false; }

		/*! �}�X�N�𗧂Ă� */
		isInput = (isInput | 0b0100);
		return true;
	}

	/*! �Q�[���p�b�h�̏\���L�[ */
	if (pGamePad->GetArrowDown(Arrow::HORIZONTAL) < 0) {
		return true;
	}

	/*! �L�[�{�[�h */
	if (Keyboard::GetButtonDown(Keyboard::LEFT)) {
		return true;
	}

	/*! ���̓}�X�N�̃��Z�b�g */
	isInput = (isInput & 0b11111011);

	return false;
}

/*!
	@brief	�E�����͂���Ă����
*/

bool InputManager::GetRight()
{
	/*! �Q�[���p�b�h�̍��W���C�X�e�B�b�N */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).x > 0) {
		return true;
	}

	/*! �Q�[���p�b�h�̏\���L�[ */
	if (pGamePad->GetArrow(Arrow::HORIZONTAL) > 0) {
		return true;
	}

	/*! �L�[�{�[�h */
	if (GetButton(Keyboard::RIGHT)) {
		return true;
	}

	return false;
}

/*!
	@brief	�E�����͂��ꂽ�u��
*/
bool InputManager::GetRightDown()
{
	/*! �Q�[���p�b�h�̍��W���C�X�e�B�b�N */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).x > 0.0f) {

		/*! �}�X�N�Ƀq�b�g���Ȃ���Ώ��������X�L�b�v */
		bool isSkip = (isInput & 0b1000) != 0;
		if (isSkip) { return false; }

		/*! �}�X�N�𗧂Ă� */
		isInput = (isInput | 0b1000);
		return true;
	}

	/*! �Q�[���p�b�h�̏\���L�[ */
	if (pGamePad->GetArrowDown(Arrow::HORIZONTAL) > 0) {
		return true;
	}

	/*! �L�[�{�[�h */
	if (Keyboard::GetButtonDown(Keyboard::RIGHT)) {
		return true;
	}

	/*! ���̓}�X�N�̃��Z�b�g */
	isInput = (isInput & 0b11110111);

	return false;
}

/*!
	@brief	START�L�[���������u��
*/
bool InputManager::GetStartDown()
{
	if (pGamePad->GetButtonDown(Button::START) ||
		Keyboard::GetButtonDown(Keyboard::SPACE)) {
		return true;
	}

	return false;
}

/*!
	@brief	�A�N�V����1�{�^���̉������u��
*/
bool InputManager::GetUpActionDown()
{
	if (pGamePad->GetButtonDown(Button::Y) ||
		Keyboard::GetButtonDown(Keyboard::UP)) {
		return true;
	}
	return false;
}

/*!
	@brief	�A�N�V����2�{�^���̉������u��
*/
bool InputManager::GetDownActionDown()
{
	if (pGamePad->GetButtonDown(Button::A) ||
		Keyboard::GetButtonDown(Keyboard::DOWN)) {
		return true;
	}
	return false;
}

/*!
	@brief	��{�^������
*/
bool InputManager::GetUpDown()
{

	/*! �Q�[���p�b�h�̍��W���C�X�e�B�b�N */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).y > 0.0f) {

		/*! �}�X�N�Ƀq�b�g���Ȃ���Ώ��������X�L�b�v */
		bool isSkip = (isInput & 0b0001) != 0;
		if (isSkip) { return false; }

		/*! �}�X�N�𗧂Ă� */
		isInput = (isInput | 0b0001);
		return true;
	}

	/*! �Q�[���p�b�h�̏\���L�[ */
	if (pGamePad->GetArrowDown(Arrow::VERTICAL) > 0) {
		return true;
	}

	/*! �L�[�{�[�h */
	if (GetButtonDown(Keyboard::UP)) {
		return true;
	}

	/*! ���̓}�X�N�̃��Z�b�g */
	isInput = (isInput & 0b11111110);

	return false;
}

/*!
	@brief	���{�^������
*/
bool InputManager::GetUnderDown()
{

	/*! �Q�[���p�b�h�̍��W���C�X�e�B�b�N */
	if (pGamePad->GetJoyStick(JoyStick::LEFT).y < 0) {
		
		/*! �}�X�N�Ƀq�b�g���Ȃ���Ώ��������X�L�b�v */
		bool isSkip = (isInput & 0b0010) != 0;
		if (isSkip) { return false; }

		/*! �}�X�N�𗧂Ă� */
		isInput = (isInput | 0b0010);
		return true;


	}

	/*! �Q�[���p�b�h�̏\���L�[ */
	if (pGamePad->GetArrowDown(Arrow::VERTICAL) < 0) {
		return true;
	}

	/*! �L�[�{�[�h */
	if (GetButtonDown(Keyboard::DOWN)) {
		return true;
	}

	/*! ���̓}�X�N�̃��Z�b�g */
	isInput = (isInput & 0b11111101);

	return false;
}

/*!
	@brief	����{�^��
*/
bool InputManager::GetDecideDown()
{
	if (pGamePad->GetButtonDown(Button::A) ||
		Keyboard::GetButtonDown(Keyboard::SPACE)||
		Keyboard::GetButtonDown(Keyboard::ENTER)) {
		return true;
	}


	return false;
}

/*!
	@brief	BACK�{�^��
*/
bool InputManager::GetBackDown()
{
	return pGamePad->GetButtonDown(Button::BACK);
}
