#include "World.hpp"
#include <random>
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
	mPlayerAircraft->setScale(1.4f, 0.8f, 0.9f);
	mSceneGraph->attachChild(std::move(player));


	std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mGame));
	mEnemy = enemy1.get();
	mEnemy->setPosition(-1.5f, 0.1f, 2);
	mEnemy->setScale(0.9f, 0.9f, 0.9f);
	mEnemy->setWorldRotation(0, XM_PI, 0);
	mSceneGraph->attachChild(std::move(enemy1));

	std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mGame));
	mEnemy2 = enemy2.get();
	mEnemy2->setPosition(1.5f, 0.1, 2);
	mEnemy2->setScale(0.9f, 0.9f, 0.9f);
	mEnemy2->setWorldRotation(0, XM_PI, 0);
	mSceneGraph->attachChild(std::move(enemy2));

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mGame));
	mBackground = backgroundSprite.get();
	mBackground->setScale(10.0, 1.0, 350.0);
	mBackground->setPosition(0, 0, -mBackground->getWorldScale().z /10);
	
	mSceneGraph->attachChild(std::move(backgroundSprite));

	mSceneGraph->build();
}

void World::update(const GameTimer& gt)
{
	mSceneGraph->update(gt);
	Input(gt);
	BackGroundMovement(gt);
	EnemiesMovement(gt);
}
void World::BackGroundMovement(const GameTimer& gt)
{
	const float deltaTime = gt.DeltaTime();
	mBackground->move(0, 0, -backgroundSpeed * deltaTime);

	//once it below a certain point reset it to desired position
	if (mBackground->getWorldPosition().z <= -(mBackground->getWorldScale().z / 10 * 2))
	{
		mBackground->setPosition(0, 0, -mBackground->getWorldScale().z / 10);
	}

}


void World::EnemiesMovement(const GameTimer& gt)
{
	const float deltaTime = gt.DeltaTime();

	mEnemy->move(0, 0, -enemySpeed * deltaTime);
	mEnemy2->move(0, 0, -enemySpeed * deltaTime);

	std::mt19937 generator(static_cast<unsigned int>(std::time(0)));
	std::uniform_real_distribution<float> distribution(-4.5f, 4.5f);
	std::uniform_real_distribution<float> distributionZcheck(-14.0f, -6.0f);
	std::uniform_real_distribution<float> distributionZset(8.0f, 13.0f);

	if (mEnemy->getWorldPosition().z <= GenerateRandomNumber(-14.0f, -6.0f))
	{
		mEnemy->setPosition(GenerateRandomNumber(-4.5, 4.5), 0.1f, GenerateRandomNumber(8, 13));
	}

	if (mEnemy2->getWorldPosition().z <= GenerateRandomNumber(-14.0f, -6.0f))
	{
		mEnemy2->setPosition(GenerateRandomNumber(-4.5, 4.5), 0.1f, GenerateRandomNumber(8, 13));
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
	const float deltaTime = gt.DeltaTime();
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
	}
}
