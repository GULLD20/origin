#pragma once


#include <functional>
#include <memory>
#include <deque>
#include <string>
#include <unordered_map>

#include "../StepTimer.h"

class GameState;

class GameStateManager final
{
	private:
		using GameStatePtr         = std::unique_ptr<GameState>;
		using GameStateStack       = std::deque<GameStatePtr>;
		using GameStateFactory     = std::function<GameStatePtr()>;
		using GameStateFactoryList = std::unordered_map<std::string, GameStateFactory>;

	private:
		template<typename State>
		static GameStatePtr CreateState();

	public:
		GameStateManager();


	public:
		~GameStateManager();


	public:
		template<typename State>
		void RegisterState(const std::string& stateName);

		void SetStartState(const std::string& stateName);

		void Update(DX::StepTimer timer);
		void Render();

		void RequestState(const std::string& stateName);
		void PushState(const std::string& stateName);
		void PopState(int count = 1);

	private:
		void ChangeState();


private:
	GameStateFactoryList m_stateFactories;
	GameStateStack       m_pStates;
	int                  m_popCount;
	std::string          m_nextStateName;
};



template<typename State>
GameStateManager::GameStatePtr GameStateManager::CreateState()
{
	return std::make_unique<State>();
}


template<typename State>
void GameStateManager::RegisterState(const std::string& stateName)
{
	// 1. 静的メンバ関数
	m_stateFactories.insert(std::make_pair(stateName, CreateState<State>));

	// 2. ラムダ式
	//m_stateFactories.insert(std::make_pair(stateName, [](){ return std::make_unique<State>(); }));
}
