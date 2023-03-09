#pragma once
#include "SceneNode.hpp"
#include "Aircraft.hpp"
#include "SpriteNode.h"
#include <list>

#include "CommandQueue.h"

class World 
{
public:
	explicit							World(Game* window);
	void								update(const GameTimer& gt);
	void								draw();
	void								Input(const GameTimer& gt);
	void								BackGroundMovement(const GameTimer& gt);
	void								EnemiesMovement(const GameTimer& gt);
	CommandQueue& getCommandQueue();
	float								GenerateRandomNumber(float lower, float upper);
	void                                ResetEnemyPositions(Aircraft* aircraft, float lower, float upper);

	//void								loadTextures();
	void								buildScene();
private:
	void								CheckAircraftBounds();
	void								AircraftScaling();

private:
	enum Layer
	{
		Background,
		Air,
		LayerCount
	};


private:
	Game*								mGame;
	CommandQueue						mCommandQueue;
	SceneNode*							mSceneGraph;
	std::array<SceneNode*, LayerCount>	mSceneLayers;

	XMFLOAT4							mWorldBounds;
	XMFLOAT2		    				mSpawnPosition;
	float								mScrollSpeed;
	Aircraft*							mPlayerAircraft;
	SpriteNode*							mBackground;
	Aircraft*							mEnemy;
	Aircraft*							mEnemy2;

private:
	float enemySpeed = 5.0f;
	float backgroundSpeed = 10.0f;
};
