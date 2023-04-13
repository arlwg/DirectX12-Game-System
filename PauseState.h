#pragma once
#include "State.h"
#include "SpriteNode.h"
#include "Common/d3dApp.h"
#include "ScreenTexture.h"
class PauseState : public State
{
public:
	PauseState(StateStack* stack, Context* context);

	virtual void		draw();
	virtual bool		update(const GameTimer& gt) override;
	virtual bool		handleEvent(WPARAM btnState) override;
	virtual void		buildState() override;

private:

	ScrenTexture* mScreenTex;
};

