#pragma once

enum class GameDataType
{
	None = -1,
};

struct GameData
{

};

typedef GameData* LPGameData;

struct GameSendData
{
	GameDataType DataType{ GameDataType::None };
	LPGameData   GameData{ nullptr };
};
