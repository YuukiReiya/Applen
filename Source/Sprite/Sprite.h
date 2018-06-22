/**
* @file		Sprite.h
* @brief	CSV�t�@�C���̃��[�h
* @author	���@����
* @date�@	2018/02/20
*/
#pragma once
#include "SpriteBase.h"
#include <unordered_map>
#include <memory>

typedef std::unordered_map<std::string, std::shared_ptr<SpriteBase>> umap_str_sprite;

class Sprite
{
private:
public:
	Sprite() {};
	~Sprite() {};

	umap_str_sprite InitTex(const char* path);
};