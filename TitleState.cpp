#include "TitleState.hpp"
#include "World.hpp"
#include "Game.hpp"


TitleState::TitleState(StateStack* stack, Context* context)
	: State(stack, context)
	, mScreenTex(nullptr)

{
	buildState();
}

void TitleState::draw()
{
	mSceneGraph->draw();
}

bool TitleState::update(const GameTimer& gt)
{
	mSceneGraph->update(gt);
	return true;
}

bool TitleState::handleEvent(WPARAM btnState)
{
	/*requestStackPop();

	requestStackPush(States::Menu);*/

	/*std::wstring text = L"Here";
		
	::OutputDebugString(text.c_str());*/

	return true;

}

void TitleState::buildState()
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




