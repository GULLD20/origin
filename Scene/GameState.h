#pragma once

#include "../StepTimer.h"

class GameState
{
	public:
		GameState() = default;

	public:
		virtual ~GameState() = default;


	public:
		virtual void Initialize() = 0;
		virtual void Update(DX::StepTimer timer) = 0;
		virtual void Render() = 0;
		virtual void Finalize() = 0;
};
