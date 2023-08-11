#include "World.hpp"
#include <random>

#include "CommandQueue.h"
#include <iostream>

World::World(Game* game)
	: mSceneGraph(new SceneNode(game))
	, mGame(game)
	, mPlayerAircraft(nullptr)
	, mEnemy(nullptr)
	, mEnemy2(nullptr)
	, mBackground(nullptr)
	, mWorldBounds(-1.5f, 1.5f, 200.0f, 0.0f) //Left, Right, Down, Up
	, mSpawnPosition(0.f, 0.f)
	, mScrollSpeed(1.0f)
{
}



void World::draw()
{
	mSceneGraph->draw();
}

void World::buildScene()
{
	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mGame));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(0, 0.1, 0.0);
	mPlayerAircraft->setScale(0.0f, 0.0f, 0.0f);
	mSceneGraph->attachChild(std::move(player));


	std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mGame));
	mEnemy = enemy1.get();
	mEnemy->setPosition(-1.5f, 0.1f, 2);
	mEnemy->setScale(0.0f, 0.0f, 0.0f);
	mEnemy->setWorldRotation(0, XM_PI, 0);
	mSceneGraph->attachChild(std::move(enemy1));

	std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mGame));
	mEnemy2 = enemy2.get();
	mEnemy2->setPosition(1.5f, 0.1, 2);
	mEnemy2->setScale(0.0f, 0.0f, 0.0f);
	mEnemy2->setWorldRotation(0, XM_PI, 0);
	mSceneGraph->attachChild(std::move(enemy2));

	/*std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mGame));
	mBackground = backgroundSprite.get();
	//mBackground->setScale(10.0, 1.0, 350.0);
	mBackground->setScale(0.0,0.0, 0.0);
	mBackground->setPosition(0, -0.3f, -mBackground->getWorldScale().z /10);
	mSceneGraph->attachChild(std::move(backgroundSprite));*/

	mSceneGraph->build();
}

void World::CheckAircraftBounds()
{
	auto pos = mPlayerAircraft->getWorldPosition();
	
	float pushPowerX = 0.0f;
	float pushPowerZ = 0.0f;
	if (pos.x > 1.93356)
	{
		//push to the left
		pushPowerX = -5.0f;
	}
	if (pos.x < -1.80393)
	{
		//push to the right
		pushPowerX = 5.0f;
	}

	if (pos.z > 1)
	{
		//push back
		pushPowerZ = -5.0f;
	}
	if (pos.z < -1.3977)
	{
		//push forward
		pushPowerZ = 5.0f;
	}
	XMFLOAT3 pusher = XMFLOAT3(pushPowerX, 0, pushPowerZ);
	mPlayerAircraft->move(pusher);
}

void World::AircraftScaling()
{
	if (mPlayerAircraft->moveRight)
	{
		mPlayerAircraft->setWorldRotation(0, 0.1, -0.5);
	}
	if (mPlayerAircraft->moveLeft)
	{
		mPlayerAircraft->setWorldRotation(0, -0.1, 0.5);
	}
	if ((!mPlayerAircraft->moveLeft && !mPlayerAircraft->moveRight) || (mPlayerAircraft->moveLeft && mPlayerAircraft->moveRight) )
	{
		mPlayerAircraft->setWorldRotation(0, 0, 0);
	}
}

void World::update(const GameTimer& gt)
{
	
	//mPlayerAircraft->setVelocity(mPlayerAircraft->getVelocity());
	mSceneGraph->update(gt);
	CheckAircraftBounds();
	while (!mCommandQueue.isEmpty())
		mSceneGraph->onCommand(mCommandQueue.pop(), gt);
	//Input(gt);
	BackGroundMovement(gt);
	EnemiesMovement(gt);
	AircraftScaling();
}
void World::BackGroundMovement(const GameTimer& gt)
{
	const float deltaTime = gt.DeltaTime();
	mBackground->move(0, 0, -backgroundSpeed * deltaTime);

	//once it below a certain point reset it to desired position
	if (mBackground->getWorldPosition().z <= -(mBackground->getWorldScale().z / 10 * 2))
	{
		mBackground->setPosition(0, -0.3f, -mBackground->getWorldScale().z / 10);
	}

}

void World::EnemiesMovement(const GameTimer& gt)
{
	const float deltaTime = gt.DeltaTime();

	mEnemy->move(0, 0, -enemySpeed * deltaTime);
	mEnemy2->move(0, 0, -enemySpeed * deltaTime);

	
	ResetEnemyPositions(mEnemy, -4, 1);
	ResetEnemyPositions(mEnemy2, 0, 4);

}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::ResetEnemyPositions(Aircraft* m_aircraft, float lower, float upper)
{
	if (m_aircraft->getWorldPosition().z <= -5)
	{
		m_aircraft->setPosition(GenerateRandomNumber(lower, upper), 0.1f, GenerateRandomNumber(8, 10));
	}
}

float World::GenerateRandomNumber(float lower, float upper)
{
	std::mt19937 generator(static_cast<unsigned int>(std::time(0)));
	std::uniform_real_distribution<float> distribution(lower, upper);

	return distribution(generator);
}


void World::Input(const GameTimer& gt)
{
	/*const float deltaTime = gt.DeltaTime();
	if (GetAsyncKeyState('W') & 0x8000)
	{
		bool hit = false;
		if (!hit)
		{
			mPlayerAircraft->move(0, 0, playerSpeed * deltaTime);
		}
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		bool hit = false;
		if (!hit)
		{
			mPlayerAircraft->move(0, 0, -playerSpeed * deltaTime);
		}
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		bool hit = false;
		if (!hit)
		{
			mPlayerAircraft->move(-playerSpeed * deltaTime, 0, 0);
		}
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		bool hit = false;
		if (!hit)
		{
			mPlayerAircraft->move(playerSpeed * deltaTime, 0, 0);
		}
	}*/
}
