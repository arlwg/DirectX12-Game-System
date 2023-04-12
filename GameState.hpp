#pragma once
#include "Aircraft.hpp"
#include "CommandQueue.h"
#include "State.h"
#include "SpriteNode.h"
#include "Common/d3dApp.h"
#include "ScreenTexture.h"
class GameState : public State
{
public:
    GameState(StateStack* stack, Context* context);

    virtual void		draw();
    virtual bool		update(const GameTimer& gt) override;
    virtual bool		handleEvent(Command& event) override;
    virtual void		buildState() override;


//--------------------------------------------------------------


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
private:

    ScrenTexture* mScreenTex;
};

