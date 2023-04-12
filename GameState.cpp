#include "GameState.hpp"

#include <random>

#include "World.hpp"
#include "Game.hpp"


GameState::GameState(StateStack* stack, Context* context)
    : State(stack, context)
    , mScreenTex(nullptr)

{
    buildState();
}

void GameState::draw()
{
    mSceneGraph->draw();
}

bool GameState::update(const GameTimer& gt)
{
    /*std::wstring text = L"Here";

    ::OutputDebugString(text.c_str());*/

    CheckAircraftBounds();
    while (!mCommandQueue.isEmpty())
        mSceneGraph->onCommand(mCommandQueue.pop(), gt);
    //Input(gt);
    BackGroundMovement(gt);
    EnemiesMovement(gt);
    AircraftScaling();
    mSceneGraph->update(gt);
    return true;
}

bool GameState::handleEvent(Command& event)
{
    /*requestStackPop();

    requestStackPush(States::Menu);*/

    /*std::wstring text = L"Here";
        
    ::OutputDebugString(text.c_str());*/

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


    std::unique_ptr<ScrenTexture> screen(new ScrenTexture(ScrenTexture::Title, mContext->mGame));
    mScreenTex = screen.get();
    mScreenTex->setPosition(0, 3.1, -2);//mCamera.SetPosition(0, 4.3f, -2.9f);
    mScreenTex->setWorldRotation(-0.6, 0, 0);
    mScreenTex->setScale(2.5f, 1.5f, 1.5f);
    mSceneGraph->attachChild(std::move(screen));




    //////////////////////////////////////////////////////////////////
    mSceneGraph->build();											//
    for (auto& e : mContext->mGame->mAllRitems)						//
        mContext->mGame->mOpaqueRitems.push_back(e.get());			// Build all items
    //
    mContext->mGame->BuildFrameResources();							//
    //////////////////////////////////////////////////////////////////
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

float GameState::GenerateRandomNumber(float lower, float upper)
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(0)));
    std::uniform_real_distribution<float> distribution(lower, upper);

    return distribution(generator);
}

void GameState::ResetEnemyPositions(Aircraft* m_aircraft, float lower, float upper)
{
    if (m_aircraft->getWorldPosition().z <= -5)
    {
        m_aircraft->setPosition(GenerateRandomNumber(lower, upper), 0.1f, GenerateRandomNumber(8, 10));
    }
}




