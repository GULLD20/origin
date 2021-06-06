#include "../pch.h"

#include "PauseState.h"

#include <Keyboard.h>
#include <CommonStates.h>

#include "../DebugObj/DebugFont.h"
#include "GameContext.h"
#include "GameStateManager.h"



PauseState::PauseState()
	:GameState()
{
}



PauseState::~PauseState()
{
}



void PauseState::Initialize()
{
}



void PauseState::Update(DX::StepTimer timer)
{
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	if (keyState.IsKeyDown(DirectX::Keyboard::Z))
	{
		GameStateManager* gameStateManager = GameContext<GameStateManager>().Get();
		gameStateManager->RequestState("Title");
	}

	if (keyState.IsKeyDown(DirectX::Keyboard::X))
	{
		GameStateManager* gameStateManager = GameContext<GameStateManager>().Get();
		gameStateManager->PopState();
	}
}



void PauseState::Render()
{
	GameContext<DirectX::SpriteBatch>::Get()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, GameContext<DirectX::CommonStates>::Get()->NonPremultiplied());

	DebugFont* debugFont = DebugFont::GetInstance();
	debugFont->print(300, 280, L"[Z] Title   [X] Play");
	debugFont->draw();

	GameContext<DirectX::SpriteBatch>::Get()->End();
}



void PauseState::Finalize()
{

}
