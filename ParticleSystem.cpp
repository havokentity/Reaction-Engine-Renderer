/*
	Description: Particle system source file

	classes contained are:

	class ParticleSystem
*/

#include "ParticleSystem.h"
#include "CoreEngine.h"

namespace DifferentialArts
{
	/*
	==============================================
		ParticleSystem Class 
	==============================================
	*/

	ParticleSystem::ParticleSystem()
	{
		/*
		if(!base_particle)
		{
			base_particle = new Particle();
		}
		*/

		this->fCurrentElapsedTimeSinceLastReleaseOfParticle = 0;
		this->current = 0;
		loaded = false;
		bFastForwarded = false;
		stopped = false;
		this->timesPlayed = 0;
		this->totalTimesPlayed = 0;
		this->final = false;
		this->systemLife = 0.0f;
		this->fSystemAge = 0.0f;
		this->bRunOnce = false;
		particleVertices = 0;
	}

	bool ParticleSystem::PrepareVB(void)
	{
		particleVertices = new ParticleVB[this->totalParticles * 4];

		if(!particleVertices)
		{
			return false;
		}

		if(!this->psVB.Initialize(this->totalParticles * 4, particleVertices,
			VertexBuffer::BufferType::DYNAMIC_SEND, sizeof(ParticleVB), 1))
		{
			return false;
		}

		return true;
	}

	ParticleSystem::ParticleSystem(float mass, const Math::Vector3 &pos,
					const Math::Vector3 &vel, unsigned int totalParticles,
					unsigned int uiTexture, float fReleaseRate, unsigned int numberToRelease,
					float friction, float fastForward, const Math::Vector3 &gravity,
					const Math::Vector3 &wind, const Math::Vector3 &randomForce,
					const Math::Vector3 &randomPointOfOrigin, bool chaos,
					Particle* particle_config)
	{
		/*
		if(!base_particle)
		{
			base_particle = new Particle();
		}
		*/

		this->friction = friction;	
		this->vel = vel;
		this->pos = pos;
		this->gravity = gravity;
			
		this->fReleaseRate = fReleaseRate;
		this->numberToRelease = numberToRelease;
		this->fCurrentElapsedTimeSinceLastReleaseOfParticle = 0;
		this->uiTexture = uiTexture;
		this->wind = wind;
		this->current = 0;
		this->totalParticles = totalParticles;
		this->randomForce = randomForce;
		this->chaos = chaos;
		//this->mass = mass;
		this->setMass(mass);
		this->randomPointOfOrigin = randomPointOfOrigin;
		this->fastForward = fastForward;
		loaded = true;
		bFastForwarded = false;
		stopped = false;
		this->timesPlayed = 0;
		this->totalTimesPlayed = 0;
		this->final = false;
		this->systemLife = 0.0f;
		this->fSystemAge = 0.0f;
		this->bRunOnce = false;

		if(!bFastForwarded && fastForward > 0)
		{
			fastForwardSystem(fastForward);
			bFastForwarded = true;
		}

		this->particle.clear();

		if(!PrepareVB())
		{
			myGlobalGame->mlog.LogError(LString("Error creating a psystem!"));
		}

		this->setBaseParticle(particle_config);
	}

	bool ParticleSystem::Initialize_from_file(char* strFileName, unsigned int uiTexture)
	{
		char holdingString[256];
		strcpy(this->fileName, strFileName);

		myGlobalGame->mlog.LogInfo(
			LString("Initializing a particle system from file \"%s\"...", this->fileName));	

		std::ifstream fin(this->fileName);

		if(fin.fail())
		{
			myGlobalGame->mlog.LogError(
				LString("There was an error initializing a particle system from file \"%s\"!", this->fileName));

			return false;
		}

		float tempMass;

		fin>>holdingString;
		fin>>holdingString>>tempMass;	
		this->setMass(tempMass);
		fin>>holdingString>>this->friction;
		fin>>holdingString>>this->pos.x>>this->pos.y>>this->pos.z;
		fin>>holdingString>>this->vel.x>>this->vel.y>>this->vel.z;	
		fin>>holdingString>>this->gravity.x>>this->gravity.y>>this->gravity.z;
		fin>>holdingString>>this->wind.x>>this->wind.y>>this->wind.z;
		fin>>holdingString>>this->randomForce.x>>this->randomForce.y>>this->randomForce.z;
		fin>>holdingString>>this->chaos;
		fin>>holdingString>>this->totalParticles;
		fin>>holdingString>>this->numberToRelease;
		fin>>holdingString>>this->fReleaseRate;
		fin>>holdingString>>this->randomPointOfOrigin.x>>this->randomPointOfOrigin.y>>this->randomPointOfOrigin.z;
		fin>>holdingString>>this->fastForward;
		fin>>holdingString>>this->systemLife;
		this->timesPlayed = 0;
		this->totalTimesPlayed = 0;
		this->final = false;
		//this->systemLife = 0.0f;
		this->fSystemAge = 0.0f;
		this->bRunOnce = false;

		this->uiTexture = uiTexture;

		/*
		if(!base_particle)
		{
			base_particle = new Particle();
		}
		*/

		this->fCurrentElapsedTimeSinceLastReleaseOfParticle = 0;
		this->current = 0;
		stopped = false;

		fin.close();

		loaded = true;
		bFastForwarded = false;

		if(!bFastForwarded && fastForward > 0)
		{
			fastForwardSystem(fastForward);
			bFastForwarded = true;
		}

		this->particle.clear();

		if(!PrepareVB())
		{
			return false;
		}

		myGlobalGame->mlog.LogSuccessText(
			LString("A particle system was successfully initialized from file \"%s\"!", this->fileName));

		return true;
	}

	bool ParticleSystem::Initialize_from_file_SETBASE(char* strFileName, unsigned int uiTexture, Particle* bp)
	{
		this->setBaseParticle(bp);

		if(!Initialize_from_file(strFileName, uiTexture))
		{
			return false;
		}	

		return true;
	}


	bool ParticleSystem::InitFromPSystem(ParticleSystem* pSystem)
	{
		//this->mass = pSystem->mass;	
		this->setMass(pSystem->mass);
		this->friction = pSystem->friction;
		this->pos = pSystem->pos;
		this->vel = pSystem->vel;
		this->gravity = pSystem->gravity;
		this->wind = pSystem->wind;
		this->randomForce = pSystem->randomForce;
		this->chaos = pSystem->chaos;
		this->totalParticles = pSystem->totalParticles;
		this->numberToRelease = pSystem->numberToRelease;
		this->fReleaseRate = pSystem->fReleaseRate;
		this->randomPointOfOrigin = pSystem->randomPointOfOrigin;
		this->fastForward = pSystem->fastForward;

		this->timesPlayed = 0;
		this->totalTimesPlayed = 0;
		this->final = false;
		this->systemLife = 0.0f;
		this->fSystemAge = 0.0f;
		this->bRunOnce = false;

		this->uiTexture = pSystem->uiTexture;

		/*
		if(!base_particle)
		{
			base_particle = new Particle();
		}
		*/

		this->setBaseParticle(pSystem->base_particle);

		this->fCurrentElapsedTimeSinceLastReleaseOfParticle = 0;
		this->current = 0;		
		stopped = false;

		loaded = true;
		bFastForwarded = false;

		if(!bFastForwarded && fastForward > 0)
		{
			fastForwardSystem(fastForward);
			bFastForwarded = true;
		}

		this->particle.clear();

		if(!PrepareVB())
		{
			return false;
		}

		return true;
	}

	void ParticleSystem::reset_particle(int particle_id)
	{
		if(particle_id > -1 && particle_id < (int)particle.size())
		{
			/*
			pIterator = particle.begin() + particle_id;
			//pIterator.SetList(particle);
			pIterator += particle_id;

			pIterator->set(*base_particle);
			pIterator->setVel(this->vel);
			pIterator->addPos(this->pos);
			*/
			Particle* pIterator = &particle[particle_id];
			pIterator->set(*base_particle);
			pIterator->setVel(this->vel);
			pIterator->addPos(this->pos);
		}
	}

	/* THIS IS A REDUNDANT METHOD, SHOULDN'T BE USED ANY MORE
		CHECK OUT solve_and_simulate FOR SIMULATION OF PARTICLE 
		SYSTEM
	*/
	void ParticleSystem::solve(void)
	{
		//unsigned int len = this->particle.GetSize();
		//pIterator.SetList(this->particle);		
		for(int i = 0; i < this->particle.size(); i++)
		{	
			if(this->particle[i].life > 0)
			{
				this->particle[i].applyForce(gravity);
				this->particle[i].applyForce(wind);
				this->particle[i].applyForce(this->particle[i].getVel()*-friction);
			} else {
				this->reset_particle(i);
			}
		}
	}

	 /* Initialized the psystem with zero force acting on it*/
	void ParticleSystem::init(void)
	{
		/*
		for(this->pIterator = this->particle.begin(); this->pIterator != this->particle.end(); this->pIterator++)
		{	
			//if((*this->pIterator).life > 0)
			//{
				(*this->pIterator).init();
			//}
		}*/

		//unsigned int len = this->particle.GetSize();
		//pIterator.SetList(this->particle);	

		/*
		for(this->pIterator = this->particle.begin(); this->pIterator != this->particle.end();this->pIterator++)
		{	
			pIterator->init();
		}
		*/


//#define USE_CUSTOM_FANCY_MEMORY_16_BYTE_ALIGNED_VECTOR_HERE
		for(int i = 0; i < particle.size(); i++)
		{
			particle[i].init();
		}
//#undef USE_CUSTOM_FANCY_MEMORY_16_BYTE_ALIGNED_VECTOR_HERE
	}

	void ParticleSystem::setMass(float m)
	{
		if(this->mass <= 0.0f)
		{
			this->mass = 1.0f;
		}

		this->mass = m;
		this->inverseMass = 1.0f / this->mass;
	}



	/*	Function used for solving and simulating particles in system
		NOTE: it doesn't solve the psystem, see class Example_For_SimulatedParticleSystem
		for solving a psystem with customized forces
	*/
	void ParticleSystem::solve_and_simulate(float dt)
	{
		if(stopped)
		{
			return;
		}

		fCurrentElapsedTimeSinceLastReleaseOfParticle += dt;

		if(this->systemLife > 0.0f)
		{
			fSystemAge += dt;

			if(this->fSystemAge >= this->systemLife)
			{
				this->fSystemAge = 0;
				this->timesPlayed++;
				this->totalTimesPlayed++;

				if(this->bRunOnce)
				{
					if(this->timesPlayed >= 1)
					{					
						this->timesPlayed = 0;
						this->bRunOnce = false;
						this->stop();
					}
				}
			}
		}
			
		Math::Vector3 randomVector = Math::Vector3(0, 0, 0);
		Math::Vector3 scaledRandomForce = Math::Vector3(0, 0, 0);		

		this->vel += (this->systemForce * this->inverseMass) * dt;
		this->pos += this->vel * dt;

		Math::Vector3 dir;
		float magSquared;
		float invSqrt;
		float lerpValue;
			
		//unsigned int size = particle.size() - 1;
		for(int i = 0; i < particle.size(); i++)
		{			
			if(this->particle[i].life > 0)
			{					
				/* RANDOM_VECTOR ISN'T NORMALIZED*/
				if(chaos)
				{	
					randomVector = RANDOM_VECTOR;

					scaledRandomForce.x = randomVector.x * randomForce.x;
					scaledRandomForce.y = randomVector.y * randomForce.y;
					scaledRandomForce.z = randomVector.z * randomForce.z;
				}

				//Solve attractors
				for(int j = 0; j < attractor.size(); j++)
				{
					//F = (GMm / r^2) * rCAP
					dir = this->pos + this->attractor[j].pos - this->particle[i].pos;
					magSquared = dir.MagnitudeSquared();

					if(magSquared < this->attractor[j].minDistance)
					{
						magSquared = this->attractor[j].minDistance;
					}
					
					if(magSquared < this->attractor[j].maxDistance)
					{
						invSqrt = Math::rsqrt(magSquared);
						dir *= invSqrt;

						if(magSquared != 0.0f)
						{
							this->particle[i].force  += dir * ((this->attractor[j].mass * this->particle[i].m)/magSquared);
						}

						if(this->attractor[j].frictionCoefficient == 0.0f)
						{
							this->particle[i].force += (this->particle[i].getVel()*-this->attractor[j].frictionCoefficient);
						}
					}				
				}

				//Solve line attractors
				for(int j = 0; j < lineAttractor.size(); j+=2)
				{
					//F = (GMm / r^2) * rCAP
					Math::Vector3 linePointPos = ClosestPointOnLineOutputLerpValue(this->pos + this->lineAttractor[j].pos,
																				   this->pos + this->lineAttractor[j + 1].pos,
																				   this->particle[i].pos,
																				   &lerpValue);

					dir = linePointPos - this->particle[i].pos;
					magSquared = dir.MagnitudeSquared();

					float minDist = lerp(lerpValue, this->lineAttractor[j].minDistance, this->lineAttractor[j+1].minDistance);

					if(magSquared < minDist)
					{
						magSquared = minDist;
					}

					float maxDist = lerp(lerpValue, this->lineAttractor[j].maxDistance, this->lineAttractor[j+1].maxDistance);

					if(magSquared < maxDist)
					{
						invSqrt = Math::rsqrt(magSquared);
						dir *= invSqrt;

						if(magSquared != 0.0f)
						{
							this->particle[i].force  += dir * ((lerp(lerpValue,
																	 this->lineAttractor[j].mass,
																	 this->lineAttractor[j+1].mass)
																	 * this->particle[i].m)/magSquared);
						}

						float finalFriction = lerp(lerpValue, this->lineAttractor[j].frictionCoefficient, this->lineAttractor[j+1].frictionCoefficient);

						if(finalFriction == 0.0f)
						{
							this->particle[i].force += (this->particle[i].getVel()*-finalFriction);
						}
					}

					
				}
				
				this->particle[i].force += (gravity + wind + scaledRandomForce + this->force);
				this->particle[i].force += (this->particle[i].getVel()*-friction);
				this->particle[i].simulate(dt);
			} else {	
				/* Not using iterators in this loop because of possibility of invalidation */
				this->particle[i] = this->particle[particle.size() - 1];
				particle.pop_back();
				current--;
				i--;
				continue;				
			}			
		}	

		scaledRandomForce = Math::Vector3(0, 0, 0);
		Math::Vector3 randomPosition = Math::Vector3(0, 0, 0);

		Particle* pIterator;
			
		if(fCurrentElapsedTimeSinceLastReleaseOfParticle > fReleaseRate)
		{
			if(current < totalParticles)
			{				
				//for(unsigned int i = 0; i<numberToRelease*(1+(int)(fCurrentElapsedTimeSinceLastReleaseOfParticle-fReleaseRate));i++)
				for(unsigned int i = 0; i<numberToRelease; i++)
				{					
					particle.push_back(*base_particle);	
					//particle.AddNode(*base_particle);
					
					randomPosition.x = (randomPointOfOrigin.x!=0)?RANDOM_NUM * randomPointOfOrigin.x:0;
					randomPosition.y = (randomPointOfOrigin.y!=0)?RANDOM_NUM * randomPointOfOrigin.y:0;
					randomPosition.z = (randomPointOfOrigin.z!=0)?RANDOM_NUM * randomPointOfOrigin.z:0;

					//pIterator = --particle.end();
					pIterator = &particle[particle.size() - 1];
					//pIterator.SetList(particle);
					//pIterator += particle.GetLastIndex();
					pIterator->addPos(this->pos + randomPosition);

					if(!chaos)
					{
						randomVector = RANDOM_VECTOR;

						scaledRandomForce.x = randomVector.x * randomForce.x;
						scaledRandomForce.y = randomVector.y * randomForce.y; 
						scaledRandomForce.z = randomVector.z * randomForce.z;

						pIterator->vel += scaledRandomForce * dt;
						//particle[particle.size() - 1].vel += scaledRandomForce * dt;
					}
					current++;
				}
				fCurrentElapsedTimeSinceLastReleaseOfParticle = 0;
			}
		}

		this->systemForce = Math::Vector3(0.0f, 0.0f, 0.0f);
		this->force = Math::Vector3(0.0f, 0.0f, 0.0f);
	}

	float ParticleSystem::getMass(void)
	{
		return this->mass;
	}

	unsigned int ParticleSystem::getTotalTimesPlayedSinceLastResetTimes(void)
	{
		return this->timesPlayed;
	}

	unsigned int ParticleSystem::getTotalTimesPlayed(void)
	{
		return this->totalTimesPlayed;
	}

	void ParticleSystem::operate(float dt)
	{
		if(!loaded)
		{
			return;
		}

		//setInitVel(initialVelocity);
		init();
		//solve();
//#define USE_CUSTOM_FANCY_MEMORY_16_BYTE_ALIGNED_VECTOR_HERE
		solve_and_simulate(dt);
//#undef USE_CUSTOM_FANCY_MEMORY_16_BYTE_ALIGNED_VECTOR_HERE
	}

	/*
	====================================================================================
		Renders the particle system with quads with 4 vertices each
	====================================================================================
	*/
	inline void ParticleSystem::Render(void)
	{
		if(!loaded || stopped)
		{
			 return;
		}

		float m[16];
		float tSize;
		Math::Vector3 tPos = Math::Vector3(0, 0, 0);	
		std::vector<Particle>::iterator tpIterator; 

		/*
		Math::Vector3 point0;
		Math::Vector3 point1;
		Math::Vector3 point2;
		Math::Vector3 point3;
		*/	

		this->maxExtent = Math::Vector3(FLT_MIN, FLT_MIN, FLT_MIN);
		this->minExtent = Math::Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
			
		//FLT_MAX
		glBindTexture(GL_TEXTURE_2D, this->uiTexture);

		glGetFloatv( GL_MODELVIEW_MATRIX, m );

		Math::Vector3 vRight = Math::Vector3( m[0], m[4], m[8] );
		Math::Vector3 vUp = Math::Vector3( m[1], m[5], m[9] );

		Math::Vector3 minus_vRight_minus_vUp	= -vRight - vUp;
		Math::Vector3 vRight_minus_vUp			=  vRight - vUp;
		Math::Vector3 vRight_plus_vUp			=  vRight + vUp;
		Math::Vector3 minus_vRight_plus_vUp		= -vRight + vUp;

		int tempCount = 0;

		Particle* pIterator;

		//for(this->pIterator = this->particle.begin(); this->pIterator != this->particle.end();this->pIterator++)
		for(int  i = 0; i < particle.size(); i++)
		{
			pIterator = &particle[i];

			if(pIterator->life > 0.0f)
			{
				tPos = pIterator->getPos();
				tSize = pIterator->size;
				if(tSize < 0)
				{
					tSize = 0;
				}

				/*
				glColor4f(
						  (this->pIterator->color.x > 0 ) ? this->pIterator->color.x : 0,
						  (this->pIterator->color.y > 0 ) ? this->pIterator->color.y : 0,
						  (this->pIterator->color.z > 0 ) ? this->pIterator->color.z : 0,
						  (this->pIterator->alpha   > 0 ) ? this->pIterator->alpha   : 0
						 );	*/					

				Math::Vector3 vPoint0 = tPos + (minus_vRight_minus_vUp * tSize * pIterator->scale.x );
				Math::Vector3 vPoint1 = tPos + (vRight_minus_vUp * tSize * pIterator->scale.x );
				Math::Vector3 vPoint2 = tPos + (vRight_plus_vUp * tSize * pIterator->scale.y );
				Math::Vector3 vPoint3 = tPos + (minus_vRight_plus_vUp * tSize * pIterator->scale.y );

				maxExtent.x = (vPoint0.x > maxExtent.x) ? vPoint0.x: maxExtent.x;
				maxExtent.x = (vPoint1.x > maxExtent.x) ? vPoint1.x: maxExtent.x;
				maxExtent.x = (vPoint2.x > maxExtent.x) ? vPoint2.x: maxExtent.x;
				maxExtent.x = (vPoint3.x > maxExtent.x) ? vPoint3.x: maxExtent.x;

				maxExtent.y = (vPoint0.y > maxExtent.y) ? vPoint0.y: maxExtent.y;
				maxExtent.y = (vPoint1.y > maxExtent.y) ? vPoint1.y: maxExtent.y;
				maxExtent.y = (vPoint2.y > maxExtent.y) ? vPoint2.y: maxExtent.y;
				maxExtent.y = (vPoint3.y > maxExtent.y) ? vPoint3.y: maxExtent.y;

				maxExtent.z = (vPoint0.z > maxExtent.z) ? vPoint0.z: maxExtent.z;
				maxExtent.z = (vPoint1.z > maxExtent.z) ? vPoint1.z: maxExtent.z;
				maxExtent.z = (vPoint2.z > maxExtent.z) ? vPoint2.z: maxExtent.z;
				maxExtent.z = (vPoint3.z > maxExtent.z) ? vPoint3.z: maxExtent.z;

				minExtent.x = (vPoint0.x < minExtent.x) ? vPoint0.x: minExtent.x;
				minExtent.x = (vPoint1.x < minExtent.x) ? vPoint1.x: minExtent.x;
				minExtent.x = (vPoint2.x < minExtent.x) ? vPoint2.x: minExtent.x;
				minExtent.x = (vPoint3.x < minExtent.x) ? vPoint3.x: minExtent.x;

				minExtent.y = (vPoint0.y < minExtent.y) ? vPoint0.y: minExtent.y;
				minExtent.y = (vPoint1.y < minExtent.y) ? vPoint1.y: minExtent.y;
				minExtent.y = (vPoint2.y < minExtent.y) ? vPoint2.y: minExtent.y;
				minExtent.y = (vPoint3.y < minExtent.y) ? vPoint3.y: minExtent.y;

				minExtent.z = (vPoint0.z < minExtent.z) ? vPoint0.z: minExtent.z;
				minExtent.z = (vPoint1.z < minExtent.z) ? vPoint1.z: minExtent.z;
				minExtent.z = (vPoint2.z < minExtent.z) ? vPoint2.z: minExtent.z;
				minExtent.z = (vPoint3.z < minExtent.z) ? vPoint3.z: minExtent.z;

				/*
				glMatrixMode(GL_TEXTURE);
				glPushMatrix();
				glLoadIdentity();
				glTranslatef(0.5f, 0.5f, 0.0f); // Translate so center of texture is at the texture 
										 // coordinate system's origin
				glRotatef(this->pIterator->angle, 0.0f, 0.0f, 1.0f); // Rotate around the Z-axis 
				glTranslatef(-0.5f, -0.5f, 0.0f); // Translate back	
				*/

				//V1
				this->particleVertices[tempCount].r = (pIterator->color.x > 0 ) ? pIterator->color.x : 0;
				this->particleVertices[tempCount].g = (pIterator->color.y > 0 ) ? pIterator->color.y : 0;
				this->particleVertices[tempCount].b = (pIterator->color.z > 0 ) ? pIterator->color.z : 0;
				this->particleVertices[tempCount].a = 0.5f;//(pIterator->alpha   > 0 ) ? pIterator->alpha   : 0;

				this->particleVertices[tempCount].tu = 0;
				this->particleVertices[tempCount].tv = 0;

				this->particleVertices[tempCount].vx = vPoint0.x;
				this->particleVertices[tempCount].vy = vPoint0.y;
				this->particleVertices[tempCount].vz = vPoint0.z;

				//V2
				this->particleVertices[tempCount + 1].r = (pIterator->color.x > 0 ) ? pIterator->color.x : 0;
				this->particleVertices[tempCount + 1].g = (pIterator->color.y > 0 ) ? pIterator->color.y : 0;
				this->particleVertices[tempCount + 1].b = (pIterator->color.z > 0 ) ? pIterator->color.z : 0;
				//this->particleVertices[tempCount + 1].a = (pIterator->alpha   > 0 ) ? pIterator->alpha   : 0;

				this->particleVertices[tempCount + 1].tu = 1;
				this->particleVertices[tempCount + 1].tv = 0;

				this->particleVertices[tempCount + 1].vx = vPoint1.x;
				this->particleVertices[tempCount + 1].vy = vPoint1.y;
				this->particleVertices[tempCount + 1].vz = vPoint1.z;

				//V3
				this->particleVertices[tempCount + 2].r = (pIterator->color.x > 0 ) ? pIterator->color.x : 0;
				this->particleVertices[tempCount + 2].g = (pIterator->color.y > 0 ) ? pIterator->color.y : 0;
				this->particleVertices[tempCount + 2].b = (pIterator->color.z > 0 ) ? pIterator->color.z : 0;
				//this->particleVertices[tempCount + 2].a = (pIterator->alpha   > 0 ) ? pIterator->alpha   : 0;

				this->particleVertices[tempCount + 2].tu = 1;
				this->particleVertices[tempCount + 2].tv = 1;

				this->particleVertices[tempCount + 2].vx = vPoint2.x;
				this->particleVertices[tempCount + 2].vy = vPoint2.y;
				this->particleVertices[tempCount + 2].vz = vPoint2.z;

				//V4
				this->particleVertices[tempCount + 3].r = (pIterator->color.x > 0 ) ? pIterator->color.x : 0;
				this->particleVertices[tempCount + 3].g = (pIterator->color.y > 0 ) ? pIterator->color.y : 0;
				this->particleVertices[tempCount + 3].b = (pIterator->color.z > 0 ) ? pIterator->color.z : 0;
				//this->particleVertices[tempCount + 3].a = (pIterator->alpha   > 0 ) ? pIterator->alpha   : 0;

				this->particleVertices[tempCount + 3].tu = 0;
				this->particleVertices[tempCount + 3].tv = 1;

				this->particleVertices[tempCount + 3].vx = vPoint3.x;
				this->particleVertices[tempCount + 3].vy = vPoint3.y;
				this->particleVertices[tempCount + 3].vz = vPoint3.z;

				tempCount += 4;

				/*
				glBegin(GL_QUADS);						
					glTexCoord2f(0.0f, 0.0f);
					glVertex3f(vPoint0.x, vPoint0.y, vPoint0.z);

					glTexCoord2f(1.0f, 0.0f);
					glVertex3f(vPoint1.x, vPoint1.y, vPoint1.z);

					glTexCoord2f(1.0f, 1.0f);
					glVertex3f(vPoint2.x, vPoint2.y, vPoint2.z);

					glTexCoord2f(0.0f, 1.0f);
					glVertex3f(vPoint3.x, vPoint3.y, vPoint3.z);	
				glEnd();	

				glPopMatrix();
				glMatrixMode(GL_MODELVIEW);
				*/				
			}
		}	

		this->psVB.Bind();
		this->psVB.NewVertexData(particleVertices, tempCount-4);
		this->psVB.StartVBSetupNoNormalsColor();
		this->psVB.Render(GL_QUADS);
		this->psVB.EndVBSetupNoNormalsColor();
		VertexBuffer::UnBind();
	}

	/*
	====================================================================================
		Renders the particle system with the point sprite ARB extension
	====================================================================================
	*/
	inline void ParticleSystem::RenderPointSprites(void)
	{
		if(!loaded || stopped)
		{
			 return;
		}

		//float m[16];
		float tSize;
		Math::Vector3 tPos = Math::Vector3(0, 0, 0);
			
		glBindTexture(GL_TEXTURE_2D, this->uiTexture);

		glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );

		float quadratic[] =  { 0.000000001f, 0.000000001f, 0.000000001f };
		glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

		glEnable(GL_POINT_SPRITE_ARB);

		glBegin(GL_POINTS);

		Particle* pIterator;		

		//for(this->pIterator = this->particle.begin(); this->pIterator != this->particle.end();this->pIterator++)
		for(int  i = 0; i < particle.size(); i++)
		{
			pIterator = &particle[i];

			if(pIterator->life > 0.0f)
			{
				tPos = pIterator->getPos();
				tSize = pIterator->size * pIterator->scale.x;

				float maxSize = 0.0f;
				glGetFloatv(GL_POINT_SIZE_MAX_ARB, &maxSize);

				if(tSize < 0)
				{
					tSize = 0;
				} else if(tSize > maxSize)
				{
					tSize = maxSize;
				}

				glColor4f(
						  (pIterator->color.x > 0 ) ? pIterator->color.x : 0,
						  (pIterator->color.y > 0 ) ? pIterator->color.y : 0,
						  (pIterator->color.z > 0 ) ? pIterator->color.z : 0,
						  (pIterator->alpha   > 0 ) ? pIterator->alpha   : 0
						 );

				/*
				glGetFloatv( GL_MODELVIEW_MATRIX, m );

				Math::Vector3 vRight = Math::Vector3( m[0], m[4], m[8] );
				Math::Vector3 vUp = Math::Vector3( m[1], m[5], m[9] );

				Math::Vector3 vPoint0 = tPos + ((-vRight - vUp) * tSize*pIterator->getScale().x);
				Math::Vector3 vPoint1 = tPos + (( vRight - vUp) * tSize*pIterator->getScale().x);
				Math::Vector3 vPoint2 = tPos + (( vRight + vUp) * tSize*pIterator->getScale().y);
				Math::Vector3 vPoint3 = tPos + ((-vRight + vUp) * tSize*pIterator->getScale().y);
				*/

				/*
				glMatrixMode(GL_TEXTURE);
				glPushMatrix();
				glLoadIdentity();
				glTranslatef(0.5f, 0.5f, 0.0f); // Translate so center of texture is at the texture 
										 // coordinate system's origin
				glRotatef(pIterator->getAngle(), 0.0f, 0.0f, 1.0f); // Rotate around the Z-axis 
				glTranslatef(-0.5f, -0.5f, 0.0f); // Translate back	

				glPopMatrix();
				glMatrixMode(GL_MODELVIEW);*/

				glPointSize(tSize);
				glVertex3f(tPos.x, tPos.y, tPos.z);
					
				/*
				glBegin(GL_QUADS);						
					glTexCoord2f(0.0f, 0.0f);
					glVertex3f(vPoint0.x, vPoint0.y, vPoint0.z);

					glTexCoord2f(1.0f, 0.0f);
					glVertex3f(vPoint1.x, vPoint1.y, vPoint1.z);

					glTexCoord2f(1.0f, 1.0f);
					glVertex3f(vPoint2.x, vPoint2.y, vPoint2.z);

					glTexCoord2f(0.0f, 1.0f);
					glVertex3f(vPoint3.x, vPoint3.y, vPoint3.z);		
				glEnd();	
				*/			
			}
		}		

		glEnd();

		glDisable(GL_POINT_SPRITE_ARB);
	}

	unsigned int ParticleSystem::getActiveParticles(void)
	{
		return this->current;
	}

	void ParticleSystem::applyForce(const Math::Vector3 &f)
	{
		this->force += f;
	}

	void ParticleSystem::applyForceOnEmitter(const Math::Vector3 &f)
	{
		this->systemForce += f;
	}

	void ParticleSystem::setBaseParticle(Particle* particle)
	{
		this->base_particle = particle;
	}

	void ParticleSystem::fastForwardSystem(float seconds)
	{		
		float dt = 0.1f;
		for(float ms = 0; ms<seconds;ms+=dt)
		{
			this->operate(dt);		
		}
	}

	Particle ParticleSystem::getParticle(unsigned int i)
	{
		if(i > -1 && i < (unsigned int)particle.size())
		{
			//std::vector<Particle>::iterator tIterator = particle.begin() + i;
			//return (*tIterator);
			return particle[i];
		}

		return *base_particle;
	}

	void ParticleSystem::toggleStopped(void)
	{
		stopped = !stopped;
	}

	void ParticleSystem::stop(void)
	{
		stopped = true;
	}

	void ParticleSystem::run(void)
	{
		stopped = false;
	}

	void ParticleSystem::runOnce(void)
	{
		if(!stopped)
		{
			return;
		}

		this->bRunOnce = true;
		stopped = false;
	}

	bool ParticleSystem::getstopped(void)
	{
		return stopped;
	}

	void ParticleSystem::setTex(GLuint &tex)
	{
		this->uiTexture = tex;
	}

	void ParticleSystem::resetTimes(void)
	{
		this->timesPlayed = 0;
	}

	/* 
		THIS METHOD IS GIVING SOME TROUBLE, SO DON'T USE IT, FREE SHOULD DO THE TRICK
		ITSELF 
	*/
	void ParticleSystem::Free(void)
	{
		/*
		if(base_particle != 0)
		{
			delete base_particle;
			base_particle = 0;
		}
		*/

		particle.clear();

		//glDeleteTextures(1, &uiTexture);
	}

	void ParticleSystem::ReleaseBaseParticle(void)
	{
		if(base_particle)
		{
			delete base_particle;
			base_particle = NULL;
		}
		
		glDeleteTextures(1, &uiTexture);
	}

	ParticleSystem::~ParticleSystem(void)
	{
		this->Free();	
	}

	float ParticleSystem::GetFastForward(void) const
	{
		return this->fastForward;
	}
}