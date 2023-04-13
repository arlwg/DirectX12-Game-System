#pragma once
#include "State.h"
#include "SpriteNode.h"
#include "Common/d3dApp.h"
#include "ScreenTexture.h"
#include "SceneNode.hpp"
#include "Aircraft.hpp"

class GameState : public State
{
public:
	GameState(StateStack* stack, Context* context);

	virtual void		draw();
	virtual bool		update(const GameTimer& gt) override;
	virtual bool		handleEvent(WPARAM btnState) override;
	virtual void		buildState() override;

private:
	float								GenerateRandomNumber(float lower, float upper);
	void                                ResetEnemyPositions(Aircraft* aircraft, float lower, float upper);
	void								BackGroundMovement(const GameTimer& gt);
	void								EnemiesMovement(const GameTimer& gt);
	void								CheckAircraftBounds();
	void								Input(const GameTimer& gt);
	void								SaveCurrentPosition();
	void								LoadSavedPositions();

private:
	enum Layer
	{
		Background,
		Air,
		LayerCount
	};


private:


	XMFLOAT4							mWorldBounds;
	XMFLOAT2		    				mSpawnPosition;
	float								mScrollSpeed;
	Aircraft* mPlayerAircraft;
	SpriteNode* mBackground;
	Aircraft* mEnemy;
	Aircraft* mEnemy2;

	XMFLOAT3 playerVelocity;
	float playerSpeed = 5.f;
	float enemySpeed = 5.0f;
	float backgroundSpeed = 10.0f;
};

