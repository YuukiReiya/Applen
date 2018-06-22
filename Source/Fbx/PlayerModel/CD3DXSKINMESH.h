#pragma once
#include <stdio.h>
#include <windows.h>
#include <d3dx9.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include "../../Device/DeviceManager.h"
#include <DirectXMath.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#define MAX_BONES 255

struct BBOX
{
	D3DXVECTOR3 Max;
	D3DXVECTOR3 Min;
	D3DXVECTOR3 AxisX;
	D3DXVECTOR3 AxisY;
	D3DXVECTOR3 AxisZ;
	FLOAT LengthX;
	FLOAT LengthY;
	FLOAT LengthZ;
	D3DXVECTOR3 Pos;
	D3DXMATRIX BindPos;
	D3DXMATRIX World;
	BBOX()
	{
		ZeroMemory(this, sizeof(BBOX));
		AxisX = D3DXVECTOR3(1, 0, 0);
		AxisY = D3DXVECTOR3(0, 1, 0);
		AxisZ = D3DXVECTOR3(0, 0, 1);
	}
};

//�V�F�[�_�[�ɓn���l
struct SHADER_SKIN_GLOBAL0
{	
	D3DXVECTOR4 vLightDir;//���C�g����
	D3DXVECTOR4 vEye;//�J�����ʒu
};

struct SHADER_SKIN_GLOBAL1
{
	D3DXMATRIX mW;//���[���h�s��
	D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
	D3DXVECTOR4 vAmbient;//�A���r�G���g��
	D3DXVECTOR4 vDiffuse;//�f�B�t���[�Y�F
	D3DXVECTOR4 vSpecular;//���ʔ���
};

struct SHADER_GLOBAL_BONES
{
	D3DXMATRIX mBone[MAX_BONES];
	SHADER_GLOBAL_BONES()
	{
		for(int i=0;i<MAX_BONES;i++)
		{
			D3DXMatrixIdentity(&mBone[i]);
		}
	}
};

//�I���W�i���@�}�e���A���\����
struct MY_SKINMATERIAL
{
	CHAR szName[110];
	D3DXVECTOR4 Ka;//�A���r�G���g
	D3DXVECTOR4 Kd;//�f�B�t���[�Y
	D3DXVECTOR4 Ks;//�X�y�L�����[
	CHAR szTextureName[512];//�e�N�X�`���[�t�@�C����
	ID3D11ShaderResourceView* pTexture;
	DWORD dwNumFace;//���̃}�e���A���ł���|���S����
	MY_SKINMATERIAL()
	{
		ZeroMemory(this,sizeof(MY_SKINMATERIAL));
	}
	~MY_SKINMATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};

//���_�\����
struct MY_SKINVERTEX
{
	D3DXVECTOR4 vPos;//���_�ʒu
	D3DXVECTOR3 vNorm;//���_�@��
	D3DXVECTOR2 vTex;//UV���W
	UINT bBoneIndex[4];//�{�[���@�ԍ�
	float bBoneWeight[4];//�{�[���@�d��
	MY_SKINVERTEX()
	{
		ZeroMemory(this,sizeof(MY_SKINVERTEX));
	}
};

//�{�[���\����
struct BONE
{
	D3DXMATRIX mBindPose;//�����|�[�Y�i�����ƕς��Ȃ��j
	D3DXMATRIX mNewPose;//���݂̃|�[�Y�i���̓s�x�ς��j
	DWORD dwNumChild;//�q�̐�
	int iChildIndex[100];//�����̎q�́h�C���f�b�N�X" 50�܂�
	CHAR Name[256];

	BONE()
	{
		ZeroMemory(this,sizeof(BONE));
		D3DXMatrixIdentity(&mBindPose);
		D3DXMatrixIdentity(&mNewPose);
	}
};
//
//
struct CD3DXSKINMESH_INIT
{
	HWND hWnd;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
};

//�h���t���[���\���� (���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
struct MYFRAME: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};
//�h�����b�V���R���e�i�[�\����(
//�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD dwWeight;//�d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
	DWORD dwBoneNum;//�{�[���̐�
	LPD3DXBUFFER pBoneBuffer;//�{�[���E�e�[�u��
	D3DXMATRIX** ppBoneMatrix;//�S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
	D3DXMATRIX* pBoneOffsetMatrices;//�t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[	

	//	D3D11�}�e���A��
	MY_SKINMATERIAL *pD3D11Material;
	ID3D11Buffer* *ppIndexBuffer;
	ID3D11Buffer* pVertexBuffer;
};
//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
//ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
class MY_HIERARCHY: public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY(){}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
		CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );	
};

//
//
//
class D3DXPARSER
{
public:

	MY_HIERARCHY cHierarchy;
	MY_HIERARCHY* m_pHierarchy;
	LPD3DXFRAME m_pFrameRoot;
	MYMESHCONTAINER* m_pContainer;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	LPD3DXANIMATIONSET m_pAnimSet[100];//100�܂ł̃A�j���[�V�����Z�b�g�ɑΉ�
	HRESULT LoadMeshFromX(LPDIRECT3DDEVICE9,LPSTR);
	void BuildAllMesh(D3DXFRAME* pFrame);
	HRESULT AllocateBoneMatrix(  LPD3DXMESHCONTAINER  );
	HRESULT AllocateAllBoneMatrices( LPD3DXFRAME  );
	VOID UpdateFrameMatrices(LPD3DXFRAME , LPD3DXMATRIX );
	
	
	//
	int GetNumVertices();
	int GetNumFaces();
	int GetNumMaterials();
	int GetNumUVs();
	int GetNumBones();
	int GetNumBoneVertices(int iBoneIndex);
	DWORD GetBoneVerticesIndices(int iBoneIndex,int iIndexInGroup);
	double GetBoneVerticesWeights(int iBoneIndex,int iIndexInGroup);
	D3DXVECTOR3 GetVertexCoord(DWORD iIndex);
	D3DXVECTOR3 GetNormal(DWORD iIndex);
	D3DXVECTOR2 GetUV(DWORD iIndex);
	int GetIndex(DWORD iIndex);
	D3DXVECTOR4 GetAmbient(int iIndex);
	D3DXVECTOR4 GetDiffuse(int iIndex);
	D3DXVECTOR4 GetSpecular(int iIndex);
	CHAR* GetTexturePath(int index);
	float GetSpecularPower(int iIndex);
	int GeFaceMaterialIndex(int iFaceIndex);
	int GetFaceVertexIndex(int iFaceIndex,int iIndexInFace);
	D3DXMATRIX GetBindPose(int iBoneIndex);
	D3DXMATRIX GetNewPose(int iBoneIndex);
	CHAR* GetBoneName(int iBoneIndex);
	void ChangeAnimSet(int index);

	
};

//
//class CD3DXSKINMESH
//CD3DXSKINMESH �I���W�i�����b�V���N���X
class CD3DXSKINMESH
{
public:
	BBOX m_BBox[MAX_BONES];
	HWND m_hWnd;	

	//Dx9
	LPDIRECT3D9 m_pD3d9;
	LPDIRECT3DDEVICE9 m_pDevice9;
	//Dx11
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11SamplerState* m_pSampleLinear;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11Buffer* m_pConstantBufferBone;
	ID3D11Buffer* m_pLineVertexBuffer;
	ID3D11Buffer* m_pLineConstantBuffer;
	ID3D11InputLayout* m_pLineVertexLayout;
	ID3D11VertexShader* m_pLineVertexShader;
	ID3D11PixelShader* m_pLinePixelShader;

	//���b�V��
	D3DXMATRIX Scale, Rot, Tran;
	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
	D3DXVECTOR3 m_Eye;
	D3DXPARSER* m_pD3dxMesh;
	DWORD m_dwNumVert;
	DWORD m_dwNumFace;
	DWORD m_dwNumUV;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;
	MY_SKINMATERIAL* m_pMaterial;
	DWORD m_dwNumMaterial;
	D3DXVECTOR3 m_Pos;
	float m_Yaw;
	float m_Pitch;
	float m_Roll;
	D3DXVECTOR3 m_Scale,m_Rot;
	D3DXMATRIX m_World;
	D3DXMATRIX m_CameraTPS;

	//�{�[��
	int m_iNumBone;
	BONE* m_BoneArray;
	
	//���\�b�h
	CD3DXSKINMESH();
	~CD3DXSKINMESH();
	HRESULT Init(CD3DXSKINMESH_INIT* si);
	HRESULT CreateIndexBuffer(DWORD dwSize,int* pIndex,ID3D11Buffer** ppIndexBuffer);
	void Render();
	HRESULT CreateFromX(CHAR* szFileName);
	HRESULT ReadSkinInfo(MY_SKINVERTEX*);
	//void RecursiveSetNewPoseMatrices(BONE* pBone,D3DXMATRIX* pmParent);
	void SetNewPoseMatrices(int frame);
	D3DXMATRIX GetBindPoseMatrix(int index);
	D3DXMATRIX GetCurrentPoseMatrix(int index);
	CHAR* GetBoneNames(int iBoneIndex);
	void RenderRay(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3 RayLength);
	void DrawFrame(LPD3DXFRAME pFrameRoot);
	void LoadResource(LPD3DXFRAME pFrameRoot);
};