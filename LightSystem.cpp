/*
	Description: Main Engine/Wrapper class

	classes contained are:

	class LightSystem
*/


#include "LightSystem.h"

namespace DifferentialArts
{
	LightSystem::LightSystem(void)
	{
	}

	LightSystem::~LightSystem(void)
	{

	}

	bool LightSystem::Initialize(void)
	{
		for(int i = 0; i < RX_CUSTOM_MAX_LIGHTS; i++)
		{
			this->light[i] = EngineLight(i);
			this->light[i].bEnabled = false;
		}

		return true;
	}
}