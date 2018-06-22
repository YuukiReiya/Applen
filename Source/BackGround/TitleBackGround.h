#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Fbx/TakeModelData.h"

class TitleBackGround
{
private:
	DirectX::XMFLOAT2 offset;
	DirectX::XMFLOAT2 leftTop;
	std::shared_ptr<TakeModelData> back;

public:
	TitleBackGround() {};
	~TitleBackGround() {};

	void Init();
	void Render();
};