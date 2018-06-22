//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dx9.h>
#include <d3dCompiler.h>
#include "../../Device/DeviceManager.h"
#include "CD3DXSKINMESH.h"

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

class PlayerModel
{
private:
	int motionNum;
	float motionPos;
	float speed;
	
public:
	PlayerModel() 
	{ 
		motionNum = 0;
		motionPos = 0.0f;
	}
	~PlayerModel() { 
		SAFE_RELEASE(m_pBlendState);
		SAFE_RELEASE(m_pSampleLinear);
		SAFE_DELETE(m_pSkinMesh);
	}

	ID3D11BlendState* m_pBlendState;
	ID3D11SamplerState* m_pSampleLinear;
	CD3DXSKINMESH* m_pSkinMesh;

	void Init();
	void Render();
	void SetMotionNum(const int num) 
	{ 
		motionNum = num;
		motionPos = 0.0f;

		if (num == 1)
		{
			speed = 0.026f;
			motionPos = 1.2f;
		}
		else if (num == 0)
		{
			speed = 0.026f;
		}
		else
		{
			speed = 0.013f;
		}
		
	};
	void SetPos(float x, float y, float z)
	{
		m_pSkinMesh->m_Pos = D3DXVECTOR3(x, y, z);
	}
	void SetScale(float x, float y, float z)
	{
		m_pSkinMesh->m_Scale = D3DXVECTOR3(x, y, z);
	}
	void SetRot(float x,float y,float z)
	{
		m_pSkinMesh->m_Rot = D3DXVECTOR3(x,y,z);
	}
	bool EndSpinAnimation() 
	{ 
		if (motionPos >= 1.2f) return true;
		return false;
	};
};
