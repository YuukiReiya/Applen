#pragma once
#include <memory>
#include <DirectXMath.h>
#include "../Fbx/TakeModelData.h"

class BackGround
{
private:
	DirectX::XMFLOAT2 offset;
	DirectX::XMFLOAT2 leftTop;
	DirectX::XMINT2 grid;
	std::shared_ptr<TakeModelData> back;

public:
	BackGround() {};
	~BackGround() {};

	void Init();
	void Render();
};