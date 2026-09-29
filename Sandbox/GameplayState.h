#pragma once
#include "GameState.h"
#include <memory>
#include "Image.h"

class Keyboard;
class GameStateManager;
class Graphics;

class GameplayState : public GameState
{
public:
    GameplayState(Context context);
    ~GameplayState();

    void Initialize() override;
    void Update(float dt) override;
    void Draw() override;
    void Finalize() override;

private:
    Image m_test;
};