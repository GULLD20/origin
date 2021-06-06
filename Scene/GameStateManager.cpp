#include "../pch.h"

#include "GameStateManager.h"

#include <cassert>

#include "GameState.h"
#include "Utilities.h"

GameStateManager::GameStateManager()
	: m_stateFactories()
	, m_pStates()
	, m_popCount(0)
	, m_nextStateName()
{
}


GameStateManager::~GameStateManager()
{
}


void GameStateManager::SetStartState(const std::string& stateName)
{
	RequestState(stateName);
}



void GameStateManager::Update(DX::StepTimer timer)
{
	if (m_popCount > 0)
	{
		for (int i = 0; i < m_popCount; i++)
		{
			m_pStates.back()->Finalize();
			m_pStates.pop_back();
		}
		m_popCount = 0;
	}


	if (!m_nextStateName.empty())
	{
		ChangeState();
	}


	assert(m_pStates.size() > 0 && "There is no active state.");
	m_pStates.back()->Update(timer);
}



void GameStateManager::Render()
{
	assert(m_pStates.size() > 0 && "There is no active state.");

	for (const GameStatePtr& state : m_pStates)
	{
		state->Render();
	}
}



void GameStateManager::RequestState(const std::string& stateName)
{
	assert(m_stateFactories.count(stateName) == 1 && "A GameState with this name is not registered.");

	m_popCount = m_pStates.size();
	m_nextStateName = stateName;
}



void GameStateManager::PushState(const std::string& stateName)
{
	m_nextStateName = stateName;
}



void GameStateManager::PopState(int count)
{
	assert(count > 0 && "Count is invalid.");

	if (!m_nextStateName.empty())
	{
		m_nextStateName.clear();
		count--;
	}

	m_popCount = Clamp<int>(m_popCount + count, 0, m_pStates.size()-1);
}



void GameStateManager::ChangeState()
{
	m_pStates.push_back(m_stateFactories[m_nextStateName]());
	m_pStates.back()->Initialize();

	m_nextStateName.clear();
}
