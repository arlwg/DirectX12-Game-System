#include "TitleState.hpp"
#include "World.hpp"
#include "Game.hpp"


TitleState::TitleState(StateStack* stack, Context* context)
	: State(stack, context)
	, mShowText(true)
	, mBackgroundSprite(nullptr)
	//, mText(nullptr)
{
	//Clear items, and resources.
	mContext->mGame->mAllRitems.clear();
	mContext->mGame->mOpaqueRitems.clear();
	mContext->mGame->mFrameResources.clear();
	// Build our materials
	mContext->mGame->BuildMaterials();

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mContext->mGame));
	mBackgroundSprite = backgroundSprite.get();
	mBackgroundSprite->setScale(103.0, 13.0, 350.0);
	mBackgroundSprite->setPosition(0, -0.3f, 0);

	mSceneGraph->attachChild(std::move(backgroundSprite));

	mSceneGraph->build();

	mSceneGraph->build();
	for (auto& e : mContext->mGame->mAllRitems)
		mContext->mGame->mOpaqueRitems.push_back(e.get());

	mContext->mGame->BuildFrameResources();

}

void TitleState::draw()
{
	/*for (int i = 0; i < 1000; i++)
	{
		std::wstring text = L"Here";

		::OutputDebugString(text.c_str());
	}*/
	mSceneGraph->draw();
}

bool TitleState::update(const GameTimer& gt)
{
	/*std::wstring text = L"Here";

	::OutputDebugString(text.c_str());*/
	mSceneGraph->update(gt);
	return true;
}

bool TitleState::handleEvent(Command& event)
{
	/*requestStackPop();

	requestStackPush(States::Menu);*/

	/*std::wstring text = L"Here";
		
	::OutputDebugString(text.c_str());*/

	return true;

}

void TitleState::buildState()
{
}


