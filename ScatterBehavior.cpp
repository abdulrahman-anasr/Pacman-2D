#include "ScatterBehavior.h"



ScatterBehavior::ScatterBehavior() {}

glm::ivec2 ScatterBehavior::calculateTargetTile(GhostType type)
{
	
	if (type == BLINKY)
	{
		return glm::ivec2(1, 26);
	}
	else if (type == PINKY)
	{
		return glm::ivec2(1, 2);
	}
	else if (type == INKY)
	{
		return glm::ivec2(35, 26);
	}
	else
	{
		return glm::ivec2(35, 1);
	}

}