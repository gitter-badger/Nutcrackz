#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

class IMainGame;

enum class ScreenState
{
	NONE,
	RUNNING,
	EXIT_APPLICATION,
	CHANGE_NEXT,
	CHANGE_PREVIOUS
};

enum class PhysicsMode {
	RIGID,
	DYNAMIC
};

enum class ObjectMode {
	NONE,
	PLAYER,
	PLATFORM,
	PLATFORMPHYSICS,
	LIGHT,
	FINISH
};

enum class SelectionMode {
	SELECT,
	PLACE
};

class IGameScreen
{
public:
	friend class ScreenList;

	IGameScreen()
	{
	}
		
	virtual ~IGameScreen()
	{
	}

	void setObjectMode(int zero = 0) { if (zero == 0) { m_objectMode = ObjectMode::NONE; } }

	//Return the index of the next or previous screen when changing screens
	virtual int getNextScreenIndex() const = 0;
	virtual int getPreviousScreenIndex() const = 0;

	//Called at beginning and end of application
	virtual void build() = 0;
	virtual void destroy() = 0;

	//Called when a screen enters and exits focus
	virtual void onEntry() = 0;
	virtual void onExit() = 0;

	//Called in the main game loop
	virtual void update() = 0;
	virtual void draw() = 0;

	int getScreenIndex() const { return m_scrIndex; }

	void setRunning() {	m_currentState = ScreenState::RUNNING; }

	ScreenState getState() const { return m_currentState; }

	void setParentGame(IMainGame *game) { m_game = game; }

	bool moveSelected;
	bool rotateSelected;
	bool scaleSelected;

	bool playPressed;
	bool stopPressed;
	bool debugPressed;

	bool createBox;
	bool createPhysicsBox;
	bool createPointLight;
	bool createPlayer;

	bool rightMouseButtonPressed;

	ObjectMode m_objectMode = ObjectMode::NONE;
	SelectionMode m_selectMode = SelectionMode::SELECT;

protected:
	int m_scrIndex = -1;

	IMainGame *m_game = nullptr;

	ScreenState m_currentState = ScreenState::NONE;
};