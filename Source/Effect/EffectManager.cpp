#include "EffectManager.h"
#include "../Device/DeviceManager.h"

/*!
	@brief	���O���
*/
//using namespace Effekseer;
//using namespace EffekseerRenderer;
//using namespace EffekseerRendererDX11;

/*!
	@def	�萔�錾
*/
const int EffectManager::MAX_RENDER_SPRITE = 2000;	/*!< �ő�`��X�v���C�g�� */

/*!
	@brief	�R���X�g���N�^
*/
EffectManager::EffectManager()
{
	pInstance = NULL;
	pRender = NULL;
	pSound = NULL;
	pEffect = NULL;
	handle = -1;
}

/*!
	@brief	�f�X�g���N�^
*/
EffectManager::~EffectManager()
{
}

/*!
	@brief	������
*/
void EffectManager::Init()
{
	auto& device = DeviceManager::GetInstance();

	/*! �����_�[�쐬 */
	pRender = EffekseerRendererDX11::Renderer::Create(
		device.pDevice.Get(),
		device.pDeviceContext.Get(),
		MAX_RENDER_SPRITE
	);

	///*! �}�l�[�W���[�쐬 */
//	pInstance = Effekseer::Manager::Create(MAX_RENDER_SPRITE);

	///*! �`��@�\�ݒ� */
	//pInstance->SetSpriteRenderer(pRender->CreateSpriteRenderer());
	//pInstance->SetRibbonRenderer(pRender->CreateRibbonRenderer());
	//pInstance->SetRingRenderer(pRender->CreateRingRenderer());
	//pInstance->SetTrackRenderer(pRender->CreateTrackRenderer());
	//pInstance->SetModelRenderer(pRender->CreateModelRenderer());

	///*! �e�N�X�`���ǂݍ��݋@�\�ݒ� */
	//pInstance->SetTextureLoader(pRender->CreateTextureLoader());
	//pInstance->SetModelLoader(pRender->CreateModelLoader());

	// ���_�ʒu���m��
	//pos = Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	//// ���e�s���ݒ�
	//pRender->SetProjectionMatrix(
	//	Effekseer::Matrix44().PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT), 1.0f, 50.0f));

	//// �J�����s���ݒ�
	//pRender->SetCameraMatrix(
	//	Effekseer::Matrix44().LookAtRH(pos, Effekseer::Vector3D(0.0f, 0.0f, 0.0f), Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));


}