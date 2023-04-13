#include "GameState.hpp"
#include "Game.hpp"
#include <random>
#include <iostream>

GameState::GameState(StateStack* stack, Context* context)
	: State(stack, context)
	, mPlayerAircraft(nullptr)
	, mEnemy(nullptr)
	, mEnemy2(nullptr)
	, mBackground(nullptr)
	, mWorldBounds(-1.5f, 1.5f, 200.0f, 0.0f) //Left, Right, Down, Up
	, mSpawnPosition(0.f, 0.f)
	, mScrollSpeed(1.0f)
	, playerVelocity(0,0,0)

{
	buildState();
}

void GameState::draw()
{
	mSceneGraph->draw();
}

bool GameState::update(const GameTimer& gt)
{
	const float deltaTime = gt.DeltaTime();
	//mPlayerAircraft->setVelocity(mPlayerAircraft->getVelocity());
	mSceneGraph->update(gt);
	CheckAircraftBounds();
	BackGroundMovement(gt);
	EnemiesMovement(gt);
	Input(gt);
	mPlayerAircraft->move(XMFLOAT3(playerVelocity.x,playerVelocity.y, 0));
	mPlayerAircraft->setPosition(mPlayerAircraft->getWorldPosition().x, (mPlayerAircraft->getWorldPosition().y + ((playerVelocity.z * deltaTime)/4)), mPlayerAircraft->getWorldPosition().z);

	mSceneGraph->update(gt);

	return true;
}

bool GameState::handleEvent(WPARAM btnState)
{	
	if (btnState == 'P')
	{
		requestStackPush(States::Title);
	}
	return true;
}

void GameState::buildState()
{
	/// //////////////////////////////////////////////////
	mContext->mGame->mAllRitems.clear();				//
	mContext->mGame->mOpaqueRitems.clear();				// Clear all items
	mContext->mGame->mFrameResources.clear();			//  
	mContext->mGame->BuildMaterials();					//
	/// //////////////////////////////////////////////////

	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mContext->mGame));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(0, 0.1, -.5f);
	mPlayerAircraft->setScale(1.4f, 0.8f, 0.9f);
	mSceneGraph->attachChild(std::move(player));


	std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mContext->mGame));
	mEnemy = enemy1.get();
	mEnemy->setPosition(-1.5f, 0.1f, 2);
	mEnemy->setScale(0.9f, 0.9f, 0.9f);
	mEnemy->setWorldRotation(0, XM_PI, 0);
	mSceneGraph->attachChild(std::move(enemy1));

	std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mContext->mGame));
	mEnemy2 = enemy2.get();
	mEnemy2->setPosition(1.5f, 0.1, 2);
	mEnemy->setScale(0.9f, 0.9f, 0.9f);
	mEnemy2->setWorldRotation(0, XM_PI, 0);
	mSceneGraph->attachChild(std::move(enemy2));

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mContext->mGame));
	mBackground = backgroundSprite.get();
	//mBackground->setScale(10.0, 1.0, 350.0);
	mBackground->setScale(10.0, 1.0, 350.0);
	mBackground->setPosition(0, -0.3f, -mBackground->getWorldScale().z / 10);
	mSceneGraph->attachChild(std::move(backgroundSprite));


	//////////////////////////////////////////////////////////////////
	mSceneGraph->build();											//
	for (auto& e : mContext->mGame->mAllRitems)						//
		mContext->mGame->mOpaqueRitems.push_back(e.get());			// Build all items
	//
	mContext->mGame->BuildFrameResources();							//
	//////////////////////////////////////////////////////////////////
}

float GameState::GenerateRandomNumber(float lower, float upper)
{
	std::mt19937 generator(static_cast<unsigned int>(std::time(0)));
	std::uniform_real_distribution<float> distribution(lower, upper);

	return distribution(generator);
}

void GameState::ResetEnemyPositions(Aircraft* aircraft, float lower, float upper)
{
	if (aircraft->getWorldPosition().z <= -5)
	{
		aircraft->setPosition(GenerateRandomNumber(lower, upper), 0.1f, GenerateRandomNumber(8, 10));
	}
}

void GameState::BackGroundMovement(const GameTimer& gt)
{
	const float deltaTime = gt.DeltaTime();
	mBackground->move(0, 0, -backgroundSpeed * deltaTime);

	//once it below a certain point reset it to desired position
	if (mBackground->getWorldPosition().z <= -(mBackground->getWorldScale().z / 10 * 2))
	{
		mBackground->setPosition(0, -0.3f, -mBackground->getWorldScale().z / 10);
	}
}

void GameState::EnemiesMovement(const GameTimer& gt)
{
	const float deltaTime = gt.DeltaTime();

	mEnemy->move(0, 0, -enemySpeed * deltaTime);
	mEnemy2->move(0, 0, -enemySpeed * deltaTime);


	ResetEnemyPositions(mEnemy, -4, 1);
	ResetEnemyPositions(mEnemy2, 0, 4);
}

void GameState::CheckAircraftBounds()
{
	auto pos = mPlayerAircraft->getWorldPosition();

	float pushPowerX = 0.0f;
	float pushPowerY = 0.0f;
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

	if (pos.y > 1)
	{
		//push back
		pushPowerY = -4.7f;
	}
	if (pos.y < -0.15)
	{
		//push forward
		pushPowerY = 4.7f;
	}
	XMFLOAT3 pusher = XMFLOAT3(pushPowerX, pushPowerY, 0);
	mPlayerAircraft->move(pusher);
}


void GameState::Input(const GameTimer& gt)
{
	const float deltaTime = gt.DeltaTime();
	if (GetAsyncKeyState('W') & 0x8000)
	{
		bool hit = false;
		if (!hit)
		{
			playerVelocity = XMFLOAT3(playerVelocity.x, playerVelocity.y, +playerSpeed);
			mPlayerAircraft->setWorldRotation(-0.3f, mPlayerAircraft->getWorldRotation().y, mPlayerAircraft->getWorldRotation().z);
		}
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		bool hit = false;
		if (!hit)
		{

			playerVelocity = XMFLOAT3(playerVelocity.x, playerVelocity.y, -playerSpeed);
			mPlayerAircraft->setWorldRotation(0.3f, mPlayerAircraft->getWorldRotation().y, mPlayerAircraft->getWorldRotation().z);
		}
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		bool hit = false;
		if (!hit)
		{
			playerVelocity = XMFLOAT3(-playerSpeed / 2, playerVelocity.y, playerVelocity.z);
			mPlayerAircraft->setWorldRotation(mPlayerAircraft->getWorldRotation().x, -0.1, 0.5);
		}
	}
	if ((GetAsyncKeyState('D') & 0x8000))
	{
		bool hit = false;
		if (!hit)
		{
			playerVelocity = XMFLOAT3(+playerSpeed / 2, playerVelocity.y, playerVelocity.z);
			mPlayerAircraft->setWorldRotation(mPlayerAircraft->getWorldRotation().x, 0.1, -0.5);
		}
	}

	if ((!(GetAsyncKeyState('D') & 0x8000) && !(GetAsyncKeyState('A') & 0x8000)) ||
		((GetAsyncKeyState('D') & 0x8000) && (GetAsyncKeyState('A') & 0x8000)))
	{
		bool hit = false;
		if (!hit)
		{
			playerVelocity = XMFLOAT3(0, playerVelocity.y, playerVelocity.z);
			mPlayerAircraft->setWorldRotation(mPlayerAircraft->getWorldRotation().x, 0, 0);
		}
	}
	if ((!(GetAsyncKeyState('W') & 0x8000) && !(GetAsyncKeyState('S') & 0x8000)) ||
		((GetAsyncKeyState('W') & 0x8000) && (GetAsyncKeyState('S') & 0x8000)))
	{
		bool hit = false;
		if (!hit)
		{
			playerVelocity = XMFLOAT3(playerVelocity.x, playerVelocity.y, 0);
			mPlayerAircraft->setWorldRotation(0, mPlayerAircraft->getWorldRotation().y, mPlayerAircraft->getWorldRotation().z);
		}
	}


	
}




