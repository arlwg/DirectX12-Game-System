
#pragma once
#include "Entity.hpp"

class SpriteNode :
	public Entity
{
public:
	SpriteNode(Game* game, const std::string& textureName, float xScale, float yScale, float zScale); // Added textureName and scale parameters

	private:
	virtual void		drawCurrent() const;
	virtual void		buildCurrent();

	std::string textureName; // New member variable to store textureName
	float xScale;
	float yScale;
	float zScale;     
};