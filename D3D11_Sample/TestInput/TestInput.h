#pragma once

#include <GameLibrary.h>

class TestInput : public GameLibrary::Game
{
public:
	TestInput(const GameLibrary::ProjectSettings& settings);

protected:
	void OnUpdate() noexcept override;
};
