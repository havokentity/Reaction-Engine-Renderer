#include "Physics.h"

namespace DifferentialArts
{
	/*
	=======================
		Mass Class
	=======================
	*/

	__forceinline Mass::Mass(void)
	{
		this->setM(1.0f);
		pos = Math::Vector3(0, 0, 0);
		vel = Math::Vector3(0, 0, 0);
		force = Math::Vector3(0, 0, 0);
	}

	__forceinline Mass::Mass(float m)
	{
		this->setM(m);
		pos = Math::Vector3(0, 0, 0);
		vel = Math::Vector3(0, 0, 0);
		force = Math::Vector3(0, 0, 0);
	}

	__forceinline Mass::Mass(float m, const Math::Vector3 &pos, const Math::Vector3 &vel)
	{
		this->setM(m);
		this->pos = pos;
		this->vel = vel;
		this->force = Math::Vector3(0, 0, 0);
	}

	__forceinline void Mass::applyForce(const Math::Vector3 &f)
	{
		force += f;
	}

	__forceinline void Mass::init(void)
	{
		force.x = 0;
		force.y = 0;
		force.z = 0;
	}

	__forceinline void Mass::simulate(float dt)
	{
		vel += (force/m) * dt;
		pos += vel * dt;
	}

	__forceinline void Mass::setVel(const Math::Vector3 &vel)
	{
		this->vel = vel;
	}

	__forceinline Math::Vector3 Mass::getVel(void)
	{
		return this->vel;
	}

	__forceinline void Mass::setPos(const Math::Vector3 &pos)
	{
		this->pos = pos;
	}

	__forceinline Math::Vector3 Mass::getPos(void)
	{
		return this->pos;
	}

	__forceinline void Mass::setM(float m)
	{
		if(this->m <= 0.0f)
		{
			m = 1.0f;
		}

		this->m = m;
		this->inverseMass = 1/this->m;
	}

	__forceinline float Mass::getM(void)
	{
		return this->m;
	}

	__forceinline float Mass::getInverseM(void)
	{
		return this->inverseMass;
	}


	__forceinline Math::Vector3 Mass::getForce(void)
	{
			return force;
	}

	/*
	=======================
		Simulation Class
	=======================
	*/

	Simulation::Simulation(int n, float m)
	{
		numberOfMasses = n;
		masses = new Mass*[n];
		for(int i=0;i<numberOfMasses;i++)
		{		
			masses[i] = new Mass(m);
		}
	}

	void Simulation::release(void)
	{
		for(int i=0;i<numberOfMasses;i++)
		{
			delete masses[i];
			masses[i] = 0;
		}
		delete masses;
		masses = 0;
	}

	void Simulation::init(void)
	{
		for(int i=0;i<numberOfMasses;i++)
		{			
			masses[i]->init();
		}
	}

	void Simulation::solve(void)
	{

	}

	Mass* Simulation::getMass(int n)
	{
		if(n < 0 || n >=numberOfMasses)
			return 0;

		return masses[n];
	}

	void Simulation::simulate(float dt)
	{
		for(int i=0;i<numberOfMasses;i++)
		{			
			masses[i]->simulate(dt);
		}
	}

	void Simulation::operate(float dt)
	{		
			init();
			solve();
			simulate(dt);
	}

	/*
	=======================
		Spring Class
	=======================
	*/

	Spring::Spring(Mass* mass1, Mass* mass2, float springConstant, float springLength, 
			   float frictionConstant)
	{
		this->mass1 = mass1;
		this->mass2 = mass2;

		this->springLength = springLength;
		this->springConstant = springConstant;
		this->frictionConstant = frictionConstant;
	}

	void Spring::solve(void)
	{
		Math::Vector3 force;
		Math::Vector3 springVector = mass1->getPos() - mass2->getPos();

		float inverseR = springVector.InverseMagnitude();
		float r = 1/inverseR;

		force += (springVector*inverseR) * (r-springLength) * -springConstant;
		force += -(mass1->getVel() - mass2->getVel()) * frictionConstant;

		mass1->applyForce(force);
		mass2->applyForce(-force);
	}
}