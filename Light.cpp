/*
	Description: Main AbstractLight Base Object

	classes contained are:

	class AbstractLight
*/

#include "Light.h"

namespace DifferentialArts
{
	/*
	=====================================================================================
		class AbstractLight
	=====================================================================================
	*/
	AbstractLight::AbstractLight()
	{
		this->pos = Math::Vector3(0, 0, 0);
		this->direction = Math::Vector3(0, 0, 0);
		
		this->diffuse = Math::Vector3(1, 1, 1);
		this->specular = Math::Vector3(1, 1, 1);
		this->ambient = Math::Vector3(0.1f, 0.1f, 0.1f);

		this->attenuation = Math::Vector3(0.001f, 0.003f, 0.0004f);
		this->spot = false;
		this->bDeferred = false;

		this->mIndex = 0;
	}

	void AbstractLight::Assign(const AbstractLight &m)
	{
		this->pos = m.pos;
		this->direction = m.direction;
		
		this->diffuse = m.diffuse;
		this->specular = m.specular;
		this->ambient = m.ambient;

		this->attenuation = m.attenuation;
		this->spot = m.spot;;
		this->bEnabled = m.bEnabled;
		this->specularExponent = m.specularExponent;
	}


	AbstractLight::~AbstractLight()
	{

	}

	void AbstractLight::setAttenuation(Math::Vector3 &x)
	{
		this->attenuation = x;
	}

	void AbstractLight::setSpot(bool &x)
	{
		this->spot = x;
	}

	Math::Vector3 AbstractLight::getAttenuation(void)
	{
		return this->attenuation;
	}

	bool AbstractLight::getSpot(void)
	{
		return this->spot;
	}

	/*
	=====================================================================================
		class EngineLight
	=====================================================================================
	*/

	EngineLight::EngineLight()
	{
		this->pos = Math::Vector3(0, 0, 0);
		this->direction = Math::Vector3(0, 0, 0);
		
		this->diffuse = Math::Vector3(1, 1, 1);
		this->specular = Math::Vector3(1, 1, 1);
		this->ambient = Math::Vector3(0.1f, 0.1f, 0.1f);

		this->attenuation = Math::Vector3(0.001f, 0.003f, 0.0004f);
		this->spot = false;
		this->specularExponent = 8;
		this->bEnabled = true;

		this->mIndex = 0;
	}

	EngineLight::EngineLight(int index)
	{
		this->pos = Math::Vector3(0, 0, 0);
		this->direction = Math::Vector3(0, 0, 0);
		
		this->diffuse = Math::Vector3(1, 1, 1);
		this->specular = Math::Vector3(1, 1, 1);
		this->ambient = Math::Vector3(0.1f, 0.1f, 0.1f);

		this->attenuation = Math::Vector3(0.001f, 0.003f, 0.0004f);
		this->spot = false;
		this->specularExponent = 8;
		this->bEnabled = false;

		if(index < 0)
		{
			index = 0;
		} else if(index > 7)
		{
			index = 7;
		}

		this->mIndex = index;
	}

	EngineLight::~EngineLight()
	{

	}

	void EngineLight::UpdateAPILight(bool passObjSpcViaMultiTexcoord)
	{
		this->UpdateAPILight_pos_only();

		this->UpdateAPILight_diffuse_only();
		this->UpdateAPILight_specular_only();
		this->UpdateAPILight_ambient_only();
		this->UpdateAPILight_Enabled();

		this->UpdateAPILight_attenuation_only();
	}

	void EngineLight::UpdateAPILight_Enabled(void)
	{
		switch(this->mIndex)
		{
		case 0:
			if(this->bEnabled)		{			glEnable(GL_LIGHT0);		}
			else					{			glDisable(GL_LIGHT0);		}
			break;
		case 1:
			if(this->bEnabled)		{			glEnable(GL_LIGHT1);		}
			else					{			glDisable(GL_LIGHT1);		}
			break;
		case 2:
			if(this->bEnabled)		{			glEnable(GL_LIGHT2);		}
			else					{			glDisable(GL_LIGHT2);		}
			break;
		case 3:
			if(this->bEnabled)		{			glEnable(GL_LIGHT3);		}
			else					{			glDisable(GL_LIGHT3);		}
			break;
		case 4:
			if(this->bEnabled)		{			glEnable(GL_LIGHT4);		}
			else					{			glDisable(GL_LIGHT4);		}
			break;
		case 5:
			if(this->bEnabled)		{			glEnable(GL_LIGHT5);		}
			else					{			glDisable(GL_LIGHT5);		}
			break;
		case 6:
			if(this->bEnabled)		{			glEnable(GL_LIGHT6);		}
			else					{			glDisable(GL_LIGHT6);		}
			break;
		case 7:
			if(this->bEnabled)		{			glEnable(GL_LIGHT7);		}
			else					{			glDisable(GL_LIGHT7);		}
			break;
		default:
			break;
		}
	}

	void EngineLight::UpdateAPILight_pos_only(bool passObjSpcViaMultiTexcoord)
	{
		float tPos[4];

		tPos[0] = this->pos.x;
		tPos[1] = this->pos.y;
		tPos[2] = this->pos.z;
		tPos[3] = !this->spot;
		
		if(passObjSpcViaMultiTexcoord)
		{
			switch(this->mIndex)
			{
			case 0:
				glLightfv(GL_LIGHT0, GL_POSITION, tPos);	
				glMultiTexCoord4fvARB(GL_TEXTURE4, tPos);
				break;
			case 1:
				glLightfv(GL_LIGHT1, GL_POSITION, tPos);
				glMultiTexCoord4fvARB(GL_TEXTURE5, tPos);
				break;
			case 2:
				glLightfv(GL_LIGHT2, GL_POSITION, tPos);
				glMultiTexCoord4fvARB(GL_TEXTURE6, tPos);
				break;
			case 3:
				glLightfv(GL_LIGHT3, GL_POSITION, tPos);
				glMultiTexCoord4fvARB(GL_TEXTURE7, tPos);
				break;
			case 4:
				glLightfv(GL_LIGHT4, GL_POSITION, tPos);
				glMultiTexCoord4fvARB(GL_TEXTURE8, tPos);
				break;
			case 5:
				glLightfv(GL_LIGHT5, GL_POSITION, tPos);
				glMultiTexCoord4fvARB(GL_TEXTURE9, tPos);
				break;
			case 6:
				glLightfv(GL_LIGHT6, GL_POSITION, tPos);
				glMultiTexCoord4fvARB(GL_TEXTURE10, tPos);
				break;
			case 7:
				glLightfv(GL_LIGHT7, GL_POSITION, tPos);
				glMultiTexCoord4fvARB(GL_TEXTURE11, tPos);
				break;
			default:
				break;
			}
		} else {
			switch(this->mIndex)
			{
			case 0:
				glLightfv(GL_LIGHT0, GL_POSITION, tPos);			
				break;
			case 1:
				glLightfv(GL_LIGHT1, GL_POSITION, tPos);
				break;
			case 2:
				glLightfv(GL_LIGHT2, GL_POSITION, tPos);
				break;
			case 3:
				glLightfv(GL_LIGHT3, GL_POSITION, tPos);
				break;
			case 4:
				glLightfv(GL_LIGHT4, GL_POSITION, tPos);
				break;
			case 5:
				glLightfv(GL_LIGHT5, GL_POSITION, tPos);
				break;
			case 6:
				glLightfv(GL_LIGHT6, GL_POSITION, tPos);
				break;
			case 7:
				glLightfv(GL_LIGHT7, GL_POSITION, tPos);
				break;
			default:
				break;
			}
		}
	}

	void EngineLight::UpdateAPILight_direction_only(void)
	{
		float tPos[3];

		tPos[0] = this->pos.x;
		tPos[1] = this->pos.y;
		tPos[2] = this->pos.z;

		switch(this->mIndex)
		{
		case 0:
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, tPos);
			break;
		case 1:
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, tPos);
			break;
		case 2:
			glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, tPos);
			break;
		case 3:
			glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, tPos);
			break;
		case 4:
			glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, tPos);
			break;
		case 5:
			glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, tPos);
			break;
		case 6:
			glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, tPos);
			break;
		case 7:
			glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, tPos);
			break;
		default:
			break;
		}
	}

	void EngineLight::UpdateAPILight_diffuse_only(void)
	{
		float tTemp[4];

		tTemp[0] = this->diffuse.x;
		tTemp[1] = this->diffuse.y;
		tTemp[2] = this->diffuse.z;
		tTemp[3] = 1;

		switch(this->mIndex)
		{
		case 0:
			glLightfv(GL_LIGHT0, GL_DIFFUSE, tTemp);
			break;
		case 1:
			glLightfv(GL_LIGHT1, GL_DIFFUSE, tTemp);
			break;
		case 2:
			glLightfv(GL_LIGHT2, GL_DIFFUSE, tTemp);
			break;
		case 3:
			glLightfv(GL_LIGHT3, GL_DIFFUSE, tTemp);
			break;
		case 4:
			glLightfv(GL_LIGHT4, GL_DIFFUSE, tTemp);
			break;
		case 5:
			glLightfv(GL_LIGHT5, GL_DIFFUSE, tTemp);
			break;
		case 6:
			glLightfv(GL_LIGHT6, GL_DIFFUSE, tTemp);
			break;
		case 7:
			glLightfv(GL_LIGHT7, GL_DIFFUSE, tTemp);
			break;
		default:
			break;
		}
	}

	void EngineLight::UpdateAPILight_specular_only(void)
	{
		float tTemp[4];

		tTemp[0] = this->specular.x;
		tTemp[1] = this->specular.y;
		tTemp[2] = this->specular.z;
		tTemp[3] = this->specularExponent;

		switch(this->mIndex)
		{
		case 0:
			glLightfv(GL_LIGHT0, GL_SPECULAR, tTemp);
			break;
		case 1:
			glLightfv(GL_LIGHT1, GL_SPECULAR, tTemp);
			break;
		case 2:
			glLightfv(GL_LIGHT2, GL_SPECULAR, tTemp);
			break;
		case 3:
			glLightfv(GL_LIGHT3, GL_SPECULAR, tTemp);
			break;
		case 4:
			glLightfv(GL_LIGHT4, GL_SPECULAR, tTemp);
			break;
		case 5:
			glLightfv(GL_LIGHT5, GL_SPECULAR, tTemp);
			break;
		case 6:
			glLightfv(GL_LIGHT6, GL_SPECULAR, tTemp);
			break;
		case 7:
			glLightfv(GL_LIGHT7, GL_SPECULAR, tTemp);
			break;
		default:
			break;
		}
	}

	void EngineLight::UpdateAPILight_ambient_only(void)
	{
		float tTemp[4];

		tTemp[0] = this->ambient.x;
		tTemp[1] = this->ambient.y;
		tTemp[2] = this->ambient.z;
		tTemp[3] = 1;

		switch(this->mIndex)
		{
		case 0:
			glLightfv(GL_LIGHT0, GL_AMBIENT, tTemp);
			break;
		case 1:
			glLightfv(GL_LIGHT1, GL_AMBIENT, tTemp);
			break;
		case 2:
			glLightfv(GL_LIGHT2, GL_AMBIENT, tTemp);
			break;
		case 3:
			glLightfv(GL_LIGHT3, GL_AMBIENT, tTemp);
			break;
		case 4:
			glLightfv(GL_LIGHT4, GL_AMBIENT, tTemp);
			break;
		case 5:
			glLightfv(GL_LIGHT5, GL_AMBIENT, tTemp);
			break;
		case 6:
			glLightfv(GL_LIGHT6, GL_AMBIENT, tTemp);
			break;
		case 7:
			glLightfv(GL_LIGHT7, GL_AMBIENT, tTemp);
			break;
		default:
			break;
		}
	}

	void EngineLight::UpdateAPILight_attenuation_only(void)
	{
		float tTemp[4];

		tTemp[0] = this->attenuation.x;
		tTemp[1] = this->attenuation.y;
		tTemp[2] = this->attenuation.z;
		tTemp[3] = 1;

		switch(this->mIndex)
		{
		case 0:
			glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, tTemp[0]);
			glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, tTemp[1]);
			glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, tTemp[2]);
			break;
		case 1:
			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, tTemp[0]);
			glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, tTemp[1]);
			glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, tTemp[2]);
			break;
		case 2:
			glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, tTemp[0]);
			glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, tTemp[1]);
			glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, tTemp[2]);
			break;
		case 3:
			glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, tTemp[0]);
			glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, tTemp[1]);
			glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, tTemp[2]);
			break;
		case 4:
			glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, tTemp[0]);
			glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, tTemp[1]);
			glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, tTemp[2]);
			break;
		case 5:
			glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, tTemp[0]);
			glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, tTemp[1]);
			glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, tTemp[2]);
		case 6:
			glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, tTemp[0]);
			glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, tTemp[1]);
			glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, tTemp[2]);
			break;
		case 7:
			glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, tTemp[0]);
			glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, tTemp[1]);
			glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, tTemp[2]);
			break;
		default:
			break;
		}
	}
}