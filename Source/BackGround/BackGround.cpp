#include "BackGround.h"
#include "../StageLoader/StageLoader.h"
#include "../GameObject//Block/BlockBase.h"

using namespace std;

void BackGround::Init()
{
	offset = {
		1.82f,	/*!< x */
	-1.82f,	/*!< y */
	};

	leftTop = {
		-5.25f,	/*!< x */
		3.35f,	/*!< y */
	};


	back = std::make_shared<TakeModelData>("Back");
	grid = StageLoader::GetInstance().GetLineCSV();
}

void BackGround::Render()
{
	DirectX::XMFLOAT3 tempPos;
	int i, j;
	for (i = 0; i < grid.x; ++i)
	{
		for (j = 0; j < grid.y; ++j)
		{
			tempPos = { offset.x*i + leftTop.x, offset.y*j + leftTop.y,1.0f };
			back->Get()->SetModelPos(tempPos);
			back->Render();
		}
	}
}