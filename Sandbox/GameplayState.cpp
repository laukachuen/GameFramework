#include "GameplayState.h"
#include "TitleState.h"

#include "Keyboard.h"
#include "GameStateManager.h"
#include "Graphics.h"

#include <memory>

GameplayState::GameplayState(Context context)
    : GameState(context)
{
}

GameplayState::~GameplayState() = default;

void GameplayState::Initialize()
{
    m_test.LoadBMP("Assets/Test.bmp");
}

void GameplayState::Update(float dt)
{
    if (m_context.kbd.IsKeyTriggered(VK_SPACE))
    {
        m_context.gameStateMgr.RequestStateChange(std::make_unique<TitleState>(m_context));
    }
}

void GameplayState::Draw()
{
    m_context.gfx.GetScreenBuffer().Clear(Color::Black());
    m_context.gfx.GetScreenBuffer().DrawImage(m_test, 300, 300);
}

void GameplayState::Finalize()
{

}