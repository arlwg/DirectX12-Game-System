#include "World.hpp"
World::World(Game* game)
	: mSceneGraph(new SceneNode(game))
	, mGame(game)
	, mPlayerAircraft(nullptr)
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
	mPlayerAircraft->setScale(0.5, 0.5, 0.5);
	mSceneGraph->attachChild(std::move(player));

	std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mGame));
	auto raptor = enemy1.get();
	raptor->setPosition(-1.5f, 0.1f, 2);
	raptor->setScale(0.7f, 0.7f, 0.7f);
	raptor->setWorldRotation(0, XM_PI, 0);
	mSceneGraph->attachChild(std::move(enemy1));

	std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mGame));
	auto raptor2 = enemy2.get();
	raptor2->setPosition(1.5f, 0.1, 2);
	raptor2->setScale(0.7f, 0.7f, 0.7f);
	raptor2->setWorldRotation(0, XM_PI, 0);
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
