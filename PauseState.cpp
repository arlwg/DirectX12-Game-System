#include "PauseState.h"
#include "World.hpp"
#include "Game.hpp"


PauseState::PauseState(StateStack* stack, Context* context)
	: State(stack, context)
	, mScreenTex(nullptr)

{
	buildState();

}

void PauseState::draw()
{
	mSceneGraph->draw();
}

bool PauseState::update(const GameTimer& gt)
{
	mSceneGraph->update(gt);
	return true;
}

bool PauseState::handleEvent(WPARAM btnState)
{
	if (btnState == 'R')
	{
		requestStackPop();
		requestStackPush(States::Game);
	}
	if (btnState == 'M')
	{
		requestStackPop();
		requestStackPush(States::Menu);
		mContext->mGame->ReloadGameState = false;
	}
	if (btnState == 'Q')
	{
		PostQuitMessage(0);
	}

	return true;

}

void PauseState::buildState()
{
	/// //////////////////////////////////////////////////
	mContext->mGame->mAllRitems.clear();				//
	mContext->mGame->mOpaqueRitems.clear();				// Clear all items
	mContext->mGame->mFrameResources.clear();			//  
	mContext->mGame->BuildMaterials();					//
	/// //////////////////////////////////////////////////


	std::unique_ptr<ScrenTexture> screen(new ScrenTexture(ScrenTexture::Pause, mContext->mGame));
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




