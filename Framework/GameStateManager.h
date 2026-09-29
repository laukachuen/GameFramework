#pragma once

#include "GameState.h"
#include <memory>

class GameStateManager
{
private:
    std::unique_ptr<GameState> m_currentState;
    std::unique_ptr<GameState> m_pendingState;

    Keyboard& m_kbd;
    Graphics& m_gfx;

    void ProcessPendingState()
    {
        if (m_pendingState)
        {
            if (m_currentState)
            {
                m_currentState->Finalize();
            }

            m_currentState = std::move(m_pendingState);

            if (m_currentState)
            {
                m_currentState->Initialize();
            }
        }
    }

public:
    GameStateManager(Keyboard& k, Graphics& g)
        : m_kbd(k), m_gfx(g)
    {}

    Context GetContext()
    {
        return Context{ m_kbd, m_gfx, *this };
    }

    void RequestStateChange(std::unique_ptr<GameState> nextState)
    {
        m_pendingState = std::move(nextState);
    }

    void Update(float dt)
    {
        if (m_currentState)
        {
            m_currentState->Update(dt);
        }
        ProcessPendingState();
    }

    void Draw()
    {
        if (m_currentState) 
        {
            m_currentState->Draw();
        }
    }
};