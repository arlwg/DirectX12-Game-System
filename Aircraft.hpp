#pragma once
#include "Entity.hpp"
#include <string>
#include "Category.hpp"
class Aircraft :
    public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
	};


public:
	Aircraft(Type type, Game* game);
	virtual unsigned int getCategory() const;


private:
	virtual void		drawCurrent() const;
	virtual void		buildCurrent();


private:
	Type				mType;
	std::string			mSprite;
	RenderItem* mAircraftRitem;
};
