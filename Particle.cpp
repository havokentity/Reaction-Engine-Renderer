/*
=========================================================================================
	class Particle
=========================================================================================
*/

#include "Client.h"
#include "Particle.h"
#include "CoreEngine.h"

namespace DifferentialArts
{

	/*
	==============================================
		PointAttractor Class
	==============================================
	*/
	PointAttractor::PointAttractor(void)
	{
		this->mass = 1.0f;
		this->pos = Math::Vector3(0.0f, 0.0f, 0.0f);
		this->minDistance = 1.0f;
		this->maxDistance = 1000.0f;
		this->frictionCoefficient = 0.0f;
	}

	PointAttractor::PointAttractor(float m, const Math::Vector3 &p, float minDist, float maxDist,
								   float friction)
	{
		this->minDistance = minDist * minDist;
		this->maxDistance = maxDist * maxDist;
		this->mass = m;
		this->pos = p;
		this->frictionCoefficient = friction;
	}

	void PointAttractor::SetMinDistance(float md)
	{
		this->minDistance = md * md;
	}

	void PointAttractor::SetMaxDistance(float md)
	{
		this->maxDistance = md * md;
	}

	float PointAttractor::getMinDistance(void)
	{
		return 1/(Math::rsqrt(this->minDistance));
	}

	float PointAttractor::getMaxDistance(void)
	{
		return 1/(Math::rsqrt(this->maxDistance));
	}


	/*
	==============================================
		Particle Class inherited from Mass Class
	==============================================
	*/

	Particle::Particle(void):Mass()
	{

	}


	Particle::Particle(float m, const Math::Vector3 &pos, const Math::Vector3 &vel,
					   const Math::Vector3 &color, const Math::Vector3 &colorDecay,
					   const Math::Vector3 &min_color, float life, float size,
					   float sizeDecay, float min_size, float alpha, float angle,
					   float angleDecay, float alphaDecay, float min_alpha,
					   const Math::Vector3 &scale, const Math::Vector3 &scaleDecay,
					   const Math::Vector3 &min_scale, float alphaDecayStart,
					   float scaleDecayStart, float sizeDecayStart,
					   float colorDecayStart, float angleDecayStart):Mass(m, pos, vel)
	{		
			this->life = life;
			this->color = color;
			this->scale = scale;
			this->initial_life = life;
			this->size = size;
			this->alpha = alpha;
			this->alphaDecay = alphaDecay;
			this->colorDecay = colorDecay;
			this->scaleDecay = scaleDecay;
			this->sizeDecay = sizeDecay;
			this->min_size = min_size;
			this->min_alpha = min_alpha;
			this->min_color = min_color;
			this->min_scale = min_scale;
			this->angle = angle;
			this->angleDecay = angleDecay;

			this->alphaDecayStart = alphaDecayStart;
			this->scaleDecayStart = scaleDecayStart;
			this->sizeDecayStart = sizeDecayStart;
			this->colorDecayStart = colorDecayStart;
			this->angleDecayStart = angleDecayStart;
	}	


	void Particle::set(const Particle &p)
	{
			this->angle = p.angle;
			this->angleDecay = p.angleDecay;		
			this->setM(p.m);
			this->pos = p.pos;
			this->vel = p.vel;
			this->life = p.life;
			this->color = p.color;
			this->scale = p.scale;
			this->initial_life = p.life;
			this->size = p.size;
			this->alpha = p.alpha;
			this->alphaDecay = p.alphaDecay;
			this->colorDecay = p.colorDecay;
			this->scaleDecay = p.scaleDecay;
			this->sizeDecay = p.sizeDecay;
			this->min_size = p.min_size;
			this->min_alpha = p.min_alpha;
			this->min_color = p.min_color;
			this->min_scale = p.min_scale;	

			this->alphaDecayStart = p.alphaDecayStart;
			this->scaleDecayStart = p.scaleDecayStart;
			this->sizeDecayStart = p.sizeDecayStart;
			this->colorDecayStart = p.colorDecayStart;
			this->angleDecayStart = p.angleDecayStart;
	}


	void Particle::simulate(float dt)
	{		
		if(life <= 0)
		{
			//life = initial_life;
			return;
		}

		vel += force * this->inverseMass * dt;
		pos += vel * dt;
		life -= dt;

		if(life <= alphaDecayStart)
		{
			alpha -= alphaDecay*dt;
		}

		if(life <= sizeDecayStart)
		{
			size -= sizeDecay*dt;
		}

		if(life <= colorDecayStart)
		{
			color -= colorDecay * dt;	
		}

		if(life <= scaleDecayStart)
		{
			scale -= scaleDecay * dt;
		}

		if(life <= angleDecayStart)
		{
			angle += angleDecay * dt;		

			angle = (angle > 359) ? 0 : angle;
		}
		
		color.x = (color.x < min_color.x) ? min_color.x : color.x;
		color.y = (color.y < min_color.y) ? min_color.y : color.y;
		color.z = (color.z < min_color.z) ? min_color.z : color.z;

		scale.x = (scale.x < min_scale.x) ? min_scale.x : scale.x;
		scale.y = (scale.y < min_scale.y) ? min_scale.y : scale.y;
		scale.z = (scale.z < min_scale.z) ? min_scale.z : scale.z;

		alpha   = (alpha   < min_alpha  ) ? min_alpha   : alpha;
		size    = (size    < min_size   ) ? min_size    : size;		
	}


	void Particle::addVel(const Math::Vector3 &v)
	{
		this->vel += v;
	}


	void Particle::addPos(const Math::Vector3 &p)
	{
		this->pos += p;
	}


	bool Particle::Initialize_from_file(char* strFileName)
	{	
		strcpy(this->fileName, strFileName);

		myGlobalGame->mlog.LogInfo(LString("Initializing a particle from file \"%s\"...", this->fileName));

		char holdingString[256];

		std::ifstream fin(fileName);

		if(fin.fail())
		{
			myGlobalGame->mlog.LogError(
				LString("There was an error initializing a particle from file \"%s\"!", this->fileName));

			return false;
		}

		float tempMass;

		fin>>holdingString;
		fin>>holdingString>>tempMass;

		this->setM(tempMass);

		fin>>holdingString>>this->pos.x>>this->pos.y>>this->pos.z;
		fin>>holdingString>>this->vel.x>>this->vel.y>>this->vel.z;		
		fin>>holdingString>>this->life;
		fin>>holdingString>>this->color.x>>this->color.y>>this->color.z;
		fin>>holdingString>>this->colorDecay.x>>this->colorDecay.y>>this->colorDecay.z;
		fin>>holdingString>>this->min_color.x>>this->min_color.y>>this->min_color.z;
		fin>>holdingString>>this->size;
		fin>>holdingString>>this->sizeDecay;
		fin>>holdingString>>this->min_size;
		fin>>holdingString>>this->alpha;		
		fin>>holdingString>>this->alphaDecay;
		fin>>holdingString>>this->min_alpha;
		fin>>holdingString>>this->scale.x>>this->scale.y>>this->scale.z;
		fin>>holdingString>>this->scaleDecay.x>>this->scaleDecay.y>>this->scaleDecay.z;
		fin>>holdingString>>this->min_scale.x>>this->min_scale.y>>this->min_scale.z;
		fin>>holdingString>>this->angle;
		fin>>holdingString>>this->angleDecay;

		fin>>holdingString>>this->alphaDecayStart;
		fin>>holdingString>>this->sizeDecayStart;
		fin>>holdingString>>this->colorDecayStart;
		fin>>holdingString>>this->scaleDecayStart;
		fin>>holdingString>>this->angleDecayStart;

		this->initial_life = life;

		fin.close();

		myGlobalGame->mlog.LogSuccessText(
			LString("A particle was successfully initialized from file \"%s\"!", this->fileName));

		return true;
	}

}
