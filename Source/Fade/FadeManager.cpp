#include "FadeManager.h"
#include "../Input/InputManager/InputManager.h"



FadeManager::FadeManager()
{
	count = fadeFrame = 0;
	addVal = 0.0f;
}


FadeManager::~FadeManager()
{
}

/*!
	@brief	初期化
*/
void FadeManager::Initialize()
{
	pSprite = new SpriteBase;
	pTexData = new TEXTURE_DATA;

	/*! テクスチャ初期化 */
	strcpy_s(pTexData->filePath, "Resource/Texture/Fade.png");
	pTexData->xfPos = { -10,-6,0.1f };
	pTexData->xfRot = { 0,0,0 };
	pTexData->xfScale = { 10,10,1 };
	pTexData->xfSize = { 1980,1080 };
	pTexData->xfColor = { 0,0,0,0 };
	pTexData->isFollow = true;
	pSprite->Init(pTexData);

	eState = State::NONE;
}

/*!
	@brief	更新
*/
void FadeManager::Update()
{
	/*! 更新の必要が無ければ処理しない */
	if (eState == State::NONE) { return; }

	/*! α値の加算・減算 */
	pTexData->xfColor.w += addVal;

	/*! フェード終了判定 */
	bool isEnd = false;

	/*! フェードイン時の処理 */
	if (eState == State::FADE_IN) {

		/*! 0以下になったら 0 にする */
		if (pTexData->xfColor.w <= 0) {
			pTexData->xfColor.w = 0;
			isEnd = true;
		}

	}
	/*! フェードアウト時の処理 */
	else if (eState == State::FADE_OUT) {

		/*! 1以上になったら 1 にする */
		if (pTexData->xfColor.w >= 1) {
			pTexData->xfColor.w = 1;
			isEnd = true;
		}

	}

	/*! フェード終了までリターン */
	if (!isEnd) { return; }

	/*! 状態の初期化 */
	eState = State::NONE;
	count = fadeFrame = 0;
	addVal = 0.0f;
}

/*!
	@brief		フェードイン開始
	@param[in]	フェードフレーム
*/
void FadeManager::StartFadeInOnly(int frame)
{
	eState = State::FADE_IN;
	addVal = -(1 / static_cast<float>(frame));
}

/*!
	@brief		フェードアウト開始
	@param[in]	フェードフレーム
*/
void FadeManager::StartFadeOutOnly(int frame)
{
	eState = State::FADE_OUT;
	addVal = 1 / static_cast<float>(frame);
}


/*!
	@brief	描画
*/
void FadeManager::Render()
{
	pSprite->Render();
}
