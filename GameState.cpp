#include "GameState.hpp"
#include "Game.hpp"
#include <random>
#include <iostream>
#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

GameState::GameState(StateStack* stack, Context* context)
	: State(stack, context)
	, mPlayerAircraft(nullptr)
	, mEnemy(nullptr)
	, mEnemy2(nullptr)
	, mBackgroundSprite(nullptr)
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


		mSceneGraph->update(gt);
		CheckAircraftBounds();
		BackGroundMovement(gt);
		EnemiesMovement(gt);
		Input(gt);

	
		mPlayerAircraft->move(XMFLOAT3(playerVelocity.x, playerVelocity.y, 0));
		mPlayerAircraft->setPosition(mPlayerAircraft->getWorldPosition().x, (mPlayerAircraft->getWorldPosition().y + ((playerVelocity.z * deltaTime) / 4)), mPlayerAircraft->getWorldPosition().z);

	
		while (!mCommandQueue.isEmpty())
			mSceneGraph->onCommand(mCommandQueue.pop(), gt);


		CommandQueue& commands = mCommandQueue;
		mContext->mPlayer->handleEvent(commands);
		mContext->mPlayer->handleRealtimeInput(commands);

		mSceneGraph->update(gt);
	
	return true;
}

bool GameState::handleEvent(WPARAM btnState)
{	

		if (btnState == 'P')
		{
			SaveCurrentPosition();
			requestStackPop();
			requestStackPush(States::Pause);
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

	initializePlayer();
	initializeEnemies();
	mBackgroundSprite = initializeSprite("Desert", 200, 1, 350);


	
	LoadSavedPositions();


	//////////////////////////////////////////////////////////////////
	mSceneGraph->build();											//
	for (auto& e : mContext->mGame->mAllRitems)						//
		mContext->mGame->mOpaqueRitems.push_back(e.get());			// Build all items
	//
	mContext->mGame->BuildFrameResources();							//
	//////////////////////////////////////////////////////////////////
}

void GameState::initializePlayer()
{
	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mContext->mGame));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(0, 0.1, -.5f);
	mPlayerAircraft->setScale(1.4f, 0.8f, 0.9f);
	mSceneGraph->attachChild(std::move(player));
}

void GameState::initializeEnemies()
{
	auto createEnemy = [this](float x, float y, float z) {
		std::unique_ptr<Aircraft> enemy(new Aircraft(Aircraft::Raptor, mContext->mGame));
		Aircraft* enemyPtr = enemy.get();
		enemyPtr->setPosition(x, y, z);
		enemyPtr->setScale(0.9f, 0.9f, 0.9f);
		enemyPtr->setWorldRotation(0, XM_PI, 0);
		mSceneGraph->attachChild(std::move(enemy));
		return enemyPtr;
	};

	mEnemy = createEnemy(-1.5f, 0.1f, 2);
	mEnemy2 = createEnemy(1.5f, 0.1, 2);
}


SpriteNode* GameState::initializeSprite(const std::string& textureName, float xScale, float yScale, float zScale)
{
	std::unique_ptr<SpriteNode> sprite(new SpriteNode(mContext->mGame, textureName, xScale, yScale, zScale));
	SpriteNode* spritePointer = sprite.get();
	spritePointer->setScale(xScale, yScale, zScale); // Set the scale using the provided values
	spritePointer->setPosition(0, -0.3f, -spritePointer->getWorldScale().z / 10);
	mSceneGraph->attachChild(std::move(sprite));

	mSpriteNodes.push_back(spritePointer); // Add the sprite to the vector

	return spritePointer;
}

void GameState::LoadSavedPositions()
{
	if (mContext->mGame->ReloadGameState)
	{
		//Load positions for all sprite nodes.
		for (size_t i = 0; i < mSpriteNodes.size() && i < mContext->mGame->spritePositions.size(); ++i)
		{
			mSpriteNodes[i]->setPosition(mContext->mGame->spritePositions[i].x, mContext->mGame->spritePositions[i].y, mContext->mGame->spritePositions[i].z);
		}
		
		mPlayerAircraft->setPosition(mContext->mGame->playerPos.x, mContext->mGame->playerPos.y, mContext->mGame->playerPos.z);
		mEnemy->setPosition(mContext->mGame->enemy1Pos.x, mContext->mGame->enemy1Pos.y, mContext->mGame->enemy1Pos.z);
		mEnemy2->setPosition(mContext->mGame->enemy2Pos.x, mContext->mGame->enemy2Pos.y, mContext->mGame->enemy2Pos.z);
		mContext->mGame->ReloadGameState = false;
	}
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
	mBackgroundSprite->move(0, 0, -backgroundSpeed * deltaTime);

	//once it below a certain point reset it to desired position
	if (mBackgroundSprite->getWorldPosition().z <= -(mBackgroundSprite->getWorldScale().z / 10 * 2))
	{
		mBackgroundSprite->setPosition(0, -0.3f, -mBackgroundSprite->getWorldScale().z / 10);
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

	if (pos.y > 1)
	{
		//push back
		pushPowerY = -5.0f;
	}
	if (pos.y < -0.1)
	{
		//push forward
		pushPowerY = 5.0f;
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
	XMFLOAT3 pusher = XMFLOAT3(pushPowerX, pushPowerY, pushPowerZ);
	mPlayerAircraft->move(pusher);
}


void GameState::Input(const GameTimer& gt)
{
	/*const float deltaTime = gt.DeltaTime();
	*/


	
}

void GameState::SaveCurrentPosition()
{
	// Clear any existing positions in the Game class
	mContext->mGame->spritePositions.clear();

	// Iterate through the vector of sprite nodes and save their positions
	for (SpriteNode* spriteNode : mSpriteNodes)
	{
		mContext->mGame->spritePositions.push_back(spriteNode->getWorldPosition());
	}
	mContext->mGame->playerPos = mPlayerAircraft->getWorldPosition();
	mContext->mGame->enemy1Pos = mEnemy->getWorldPosition();
	mContext->mGame->enemy2Pos = mEnemy2->getWorldPosition();
	mContext->mGame->ReloadGameState = true;
}






