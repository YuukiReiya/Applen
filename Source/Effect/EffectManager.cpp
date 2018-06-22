#include "EffectManager.h"
#include "../Device/DeviceManager.h"

/*!
	@brief	名前空間
*/
//using namespace Effekseer;
//using namespace EffekseerRenderer;
//using namespace EffekseerRendererDX11;

/*!
	@def	定数宣言
*/
const int EffectManager::MAX_RENDER_SPRITE = 2000;	/*!< 最大描画スプライト数 */

/*!
	@brief	コンストラクタ
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
	@brief	デストラクタ
*/
EffectManager::~EffectManager()
{
}

/*!
	@brief	初期化
*/
void EffectManager::Init()
{
	auto& device = DeviceManager::GetInstance();

	/*! レンダー作成 */
	pRender = EffekseerRendererDX11::Renderer::Create(
		device.pDevice.Get(),
		device.pDeviceContext.Get(),
		MAX_RENDER_SPRITE
	);

	///*! マネージャー作成 */
//	pInstance = Effekseer::Manager::Create(MAX_RENDER_SPRITE);

	///*! 描画機能設定 */
	//pInstance->SetSpriteRenderer(pRender->CreateSpriteRenderer());
	//pInstance->SetRibbonRenderer(pRender->CreateRibbonRenderer());
	//pInstance->SetRingRenderer(pRender->CreateRingRenderer());
	//pInstance->SetTrackRenderer(pRender->CreateTrackRenderer());
	//pInstance->SetModelRenderer(pRender->CreateModelRenderer());

	///*! テクスチャ読み込み機能設定 */
	//pInstance->SetTextureLoader(pRender->CreateTextureLoader());
	//pInstance->SetModelLoader(pRender->CreateModelLoader());

	// 視点位置を確定
	//pos = Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	//// 投影行列を設定
	//pRender->SetProjectionMatrix(
	//	Effekseer::Matrix44().PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT), 1.0f, 50.0f));

	//// カメラ行列を設定
	//pRender->SetCameraMatrix(
	//	Effekseer::Matrix44().LookAtRH(pos, Effekseer::Vector3D(0.0f, 0.0f, 0.0f), Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));


}