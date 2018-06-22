#include "TitleBackGround.h"
#include "../GameObject//Block/BlockBase.h"

using namespace std;

void TitleBackGround::Init()
{
	offset = {
		1.82f,	/*!< x */
		-1.82f,	/*!< y */
	};

	leftTop = {
		-15.5f,	/*!< x */
		8.2f,	/*!< y */
	};

	back = std::make_shared<TakeModelData>("stage");
}

void TitleBackGround::Render()
{
	DirectX::XMFLOAT3 tempPos;
	int i, j;
	for (i = 0; i < 18; ++i)
	{
		for (j = 0; j < 12; ++j)
		{
			tempPos = { offset.x*i + leftTop.x, offset.y*j + leftTop.y,12.65f };
			back->Get()->SetModelPos(tempPos);

			if ((i > 5 && i < 12) && (j == 1 || j == 2))
			{
			}
			else
			{
				back->Render();
			}
		}
	}
}