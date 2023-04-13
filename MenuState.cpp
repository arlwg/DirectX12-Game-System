#include "MenuState.h"
#include "World.hpp"
#include "Game.hpp"


MenuState::MenuState(StateStack* stack, Context* context)
	: State(stack, context)
	, mScreenTex(nullptr)

{
	buildState();
}

void MenuState::draw()
{
	mSceneGraph->draw();
}

bool MenuState::update(const GameTimer& gt)
{
	mSceneGraph->update(gt);
	return true;
}

bool MenuState::handleEvent(WPARAM btnState)
{
	/*requestStackPop();

	requestStackPush(States::Menu);*/
	if (btnState == '1')
	{
		requestStackPop();

		requestStackPush(States::Game);
	}
	else if (btnState == '2')
	{
		PostQuitMessage(0);
	}
	/*std::wstring text = L"Here";

	::OutputDebugString(text.c_str());*/

	return true;

}

void MenuState::buildState()
{
	/// //////////////////////////////////////////////////
	mContext->mGame->mAllRitems.clear();				//
	mContext->mGame->mOpaqueRitems.clear();				// Clear all items
	mContext->mGame->mFrameResources.clear();			//  
	mContext->mGame->BuildMaterials();					//
	/// //////////////////////////////////////////////////


	std::unique_ptr<ScrenTexture> screen(new ScrenTexture(ScrenTexture::Menu, mContext->mGame));
	mScreenTex = screen.get();
	mScreenTex->setPosition(0, 3.15, -2);//mCamera.SetPosition(0, 4.3f, -2.9f);
	mScreenTex->setWorldRotation(-0.6, 0, 0);
	mScreenTex->setScale(2.2f, 1.0f, 1.3f);
	mSceneGraph->attachChild(std::move(screen));




	//////////////////////////////////////////////////////////////////
	mSceneGraph->build();											//
	for (auto& e : mContext->mGame->mAllRitems)						//
		mContext->mGame->mOpaqueRitems.push_back(e.get());			// Build all items
	//
	mContext->mGame->BuildFrameResources();							//
	//////////////////////////////////////////////////////////////////
}




