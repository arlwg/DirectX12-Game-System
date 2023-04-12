#pragma once
#include "Entity.hpp"
#include <string>
#include "Category.hpp"
class ScrenTexture :
	public Entity
{
public:
	enum Type
	{
		Title,
		Menu,
		Pause,
	};


public:
	ScrenTexture(Type type, Game* game);
	virtual unsigned int getCategory() const;


private:
	virtual void		drawCurrent() const;
	virtual void		buildCurrent();


private:
	Type				mType;
	std::string			mSprite;
	RenderItem* mScreenRitem;
};
