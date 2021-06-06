#pragma once


#include "GameState.h"
#include "../StepTimer.h"

class PauseState: public GameState
{
	public:
		PauseState();

	public:
		virtual ~PauseState();


	public:
		void Initialize() override;
		void Update(DX::StepTimer timer) override;
		void Render() override;
		void Finalize() override;
};
