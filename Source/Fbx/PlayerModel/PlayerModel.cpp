#include "PlayerModel.h"

void PlayerModel::Init()
{
	//アルファブレンド用ブレンドステート作成
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable =false;
	bd.AlphaToCoverageEnable=false;
	bd.RenderTarget[0].BlendEnable=true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	DeviceManager::GetInstance().pDevice->CreateBlendState(&bd, &m_pBlendState);	

	//スキンメッシュクラスに必要な情報を渡し初期化
	CD3DXSKINMESH_INIT si;
	si.hWnd= DeviceManager::GetInstance().hWnd;
	si.pDevice= DeviceManager::GetInstance().pDevice.Get();
	si.pDeviceContext= DeviceManager::GetInstance().pDeviceContext.Get();

	m_pSkinMesh=new CD3DXSKINMESH;
	m_pSkinMesh->Init(&si);
	m_pSkinMesh->CreateFromX((CHAR*)"Resource/Model/ringo_ani2.X");

}

void PlayerModel::Render()
{
	// 60/4800 (フレーム数/4800で想定していたスピードになる)
	static float motionMax[4] = { 2.625f,1.625f,1.825f,1.625f };

	m_pSkinMesh->m_pD3dxMesh->ChangeAnimSet(motionNum);

	//再生地点をセット
	LPD3DXANIMATIONCONTROLLER anim = m_pSkinMesh->m_pD3dxMesh->m_pAnimController;

	//再生地点の設定
	anim->SetTrackPosition(0, motionPos);
	
	//レンダリング
	m_pSkinMesh->Render();

	//再生地点の更新
	if (motionPos < motionMax[motionNum])
	{
		motionPos += speed;
	}
	else
	{
		if (motionNum != 2)
		{
			motionNum = 2;
		}
		motionPos = 0;
	}
}