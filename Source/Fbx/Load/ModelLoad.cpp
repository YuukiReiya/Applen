#include "ModelLoad.h"
#include <fstream>

using namespace DirectX;
using namespace std;
using namespace modelstruct;

///////////////////////////////////////////////////////////////////////////////////////////////////
/**
* @fn
* モデルデータを読み込み、構造体に格納
*/
///////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT ModelLoad::Load(const string path)
{
	//独自形式読み込み
	std::ifstream* stream = new ifstream(path, ios::binary);

	if (stream->fail())
	{
		MessageBox(0, L"変換後のファイルが正常に読み込めませんでした", 0, 0);
	}

	char str[256];

	// ファイルタイプ
	stream->read(str, FILE_BYTE);

	unsigned int meshSize = 0;

	// メッシュの数
	stream->read((char*)&meshSize, FILE_BYTE);

	unsigned int i, j, k, l;
	unsigned int Column, Row = Column = 4;
	unsigned int flame;
	unsigned int size = 0;

	for (i = 0; i < meshSize; ++i)
	{
		//メッシュ毎の構造体
		MODEL_MESH tempMesh;

		//一時保存用
		MODEL_MATERIAL tempMat;
		vector<XMFLOAT3> tempPos, tempNorm;
		vector<XMFLOAT2> tempTex;

		while (true)
		{
			// キーワード読み込み
			stream->read(str, FILE_BYTE);

			if (strncmp(str, "VPOS", 4) == 0)
			{
				//頂点数
				stream->read((char*)&size, FILE_BYTE);

				tempMesh.vertexMax = size;

				//頂点
				for (j = 0; j < size; ++j)
				{
					XMFLOAT3 pos;
					stream->read((char*)&pos.x, FILE_BYTE);
					stream->read((char*)&pos.y, FILE_BYTE);
					stream->read((char*)&pos.z, FILE_BYTE);

					tempPos.push_back(pos);
				}
			}
			else if (strncmp(str, "VNOR", 4) == 0)
			{
				//法線数
				stream->read((char*)&size, FILE_BYTE);

				//法線
				for (j = 0; j < size; ++j)
				{
					DirectX::XMFLOAT3 normal;
					stream->read((char*)&normal.x, FILE_BYTE);
					stream->read((char*)&normal.y, FILE_BYTE);
					stream->read((char*)&normal.z, FILE_BYTE);

					tempNorm.push_back(normal);
				}
			}
			else if (strncmp(str, "VUVT", 4) == 0)
			{
				//UV数
				stream->read((char*)&size, FILE_BYTE);

				//UV
				for (j = 0; j < size; ++j)
				{
					DirectX::XMFLOAT2 uv;
					stream->read((char*)&uv.x, FILE_BYTE);
					stream->read((char*)&uv.y, FILE_BYTE);
					uv.y *= -1.0f;//DirectXはyの要素を逆にする

					tempTex.push_back(uv);
				}
			}
			else if (strncmp(str, "LMTR", 4) == 0)
			{
				XMMATRIX mat;

				for (i = 0; i < Column; ++i)
				{
					for (j = 0; j < Row; ++j)
					{
						stream->read((char*)&mat.r[i].m128_f32[j], FILE_BYTE);
					}
				}

				tempMesh.mat = mat;
				tempMesh.material.push_back(tempMat);
			}
			else if (strncmp(str, "MDIF", 4) == 0)
			{
				DirectX::XMFLOAT4 dif;
				stream->read((char*)&dif.x, FILE_BYTE);
				stream->read((char*)&dif.y, FILE_BYTE);
				stream->read((char*)&dif.z, FILE_BYTE);
				stream->read((char*)&dif.w, FILE_BYTE);

				tempMat.xfDiffuse = dif;
			}
			else if (strncmp(str, "MSPE", 4) == 0)
			{
				DirectX::XMFLOAT3 spe;
				stream->read((char*)&spe.x, FILE_BYTE);
				stream->read((char*)&spe.y, FILE_BYTE);
				stream->read((char*)&spe.z, FILE_BYTE);

				tempMat.xfSpecular = spe;
			}
			else if (strncmp(str, "MAMB", 4) == 0)
			{
				DirectX::XMFLOAT3 amb;
				stream->read((char*)&amb.x, FILE_BYTE);
				stream->read((char*)&amb.y, FILE_BYTE);
				stream->read((char*)&amb.z, FILE_BYTE);

				tempMat.xfAmbient = amb;
			}
			else if (strncmp(str, "ANIM", 4) == 0)
			{
				//アニメーションサイズ
				stream->read((char*)&size, FILE_BYTE);

				//アニメーションの種類数
				for (j = 0; j < size; ++j)
				{
					//フレーム数の読み込み
					stream->read((char*)&flame, FILE_BYTE);
					vector<vector<XMFLOAT3>> animFlame;

					//ついでに最大フレーム数を保存しておく
					tempMesh.animationMax.push_back(flame);

					// 頂点数
					unsigned int aniVert;
					stream->read((char*)&aniVert, FILE_BYTE);

					//フレーム数
					for (k = 0; k < flame; ++k)
					{
						vector<XMFLOAT3> animVertex;

						//頂点数だけループ
						for (l = 0; l < aniVert; ++l)
						{
							//頂点
							DirectX::XMFLOAT3 vert;

							stream->read((char*)&vert.x, FILE_BYTE);
							stream->read((char*)&vert.y, FILE_BYTE);
							stream->read((char*)&vert.z, FILE_BYTE);

							animVertex.push_back(vert);
						}

						animFlame.push_back(animVertex);
					}

					tempMesh.animation.push_back(animFlame);
				}
			}
			else if (strncmp(str, "NEXT", 4) == 0)
			{
				break;
			}
		}

		//vertex構造体に格納
		for (j = 0; j < tempMesh.vertexMax; ++j)
		{
			MODEL_VERTEX tempVertex;

			tempVertex.xfPos = tempPos[j];
			tempVertex.xfNorm = tempNorm[j];

			if (tempTex.empty())
			{
				tempVertex.xfTex = { 0.0f,0.0f };
			}
			else
			{
				tempVertex.xfTex = tempTex[j];
			}

			tempMesh.vertex.push_back(tempVertex);
		}

		//最終的な値の格納
		mesh.push_back(tempMesh);
	}

	this->isLoadComplete = true;

	stream->close();
	delete stream;
	stream = nullptr;

	return S_OK;
}