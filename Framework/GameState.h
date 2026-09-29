#pragma once

class Keyboard;
class GameStateManager;
class Graphics;

struct Context
{
	Keyboard& kbd;
	Graphics& gfx;
	GameStateManager& gameStateMgr;
};

class GameState
{
protected:
	Context m_context;

public:
	GameState(Context context)
		:m_context(context)
	{}

	virtual ~GameState() = default;

	virtual void Initialize() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;
};