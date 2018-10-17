/*
	class Particle, class PointAttractor
*/


#ifndef PARTICLE_H
#define PARTICLE_H

/* Remove VC8 warning for using deprecated functions of the CRT */
#pragma warning(disable:4996)

#include <fstream>

#include "Physics.h"

namespace DifferentialArts
{
	/*
	==============================================
		PointAttractor Class
	==============================================
	*/

	/*!	\class PointAttractor
		\brief A point attractor class
		\author Rajesh Peter Douglas D'Monte	

		This class is used inside of the Particle system class for attracting other particles to point mass
	*/
	class RX_API PointAttractor
	{
	public:
		PointAttractor(void);							//!< Default constructor
		PointAttractor(float m, const Math::Vector3 &p,
					   float minDist = 1.0f,
					   float maxDist = 10000.0f,
					   float friction = 0.0f);			//!< Constructor

		void SetMinDistance(float md);					//!< Sets cutin distance for space-time curvature localized for PointAttractor Instance
		void SetMaxDistance(float md);					//!< Sets cutoff distance for space-time curvature localized for PointAttractor Instance

		float getMinDistance(void);						//!< Returns minimum distance.
		float getMaxDistance(void);						//!< Returns maximum distance.

		Math::Vector3 pos;								//!< Position vector of PointAttractor instance
		float mass;										//!< Mass of attractor

		friend class ParticleSystem;

		float frictionCoefficient;

	private:
		float minDistance;								//!< Squared cutin distance for space-time curvature localized for PointAttractor Instance
		float maxDistance;								//!< Squared cutoff distance for space-time curvature localized for PointAttractor Instance
	};

	/*
	==============================================
		Particle Class inherited from Mass Class
	==============================================
	*/

	/*!	\class Particle
		\brief A particle class
		\author Rajesh Peter Douglas D'Monte	

		This class is used inside of the Particle system class
	*/
	class RX_API Particle: public Mass 
	{
	public:			
		float life;				//!< Life of particle	
		
		float size;				//!< Particle size
		float sizeDecay;		//!< Particle size decay constant	
		float min_size;			//!< Particle size floor

		Math::Vector3 scale;				//!< Scale of the particle
		Math::Vector3 scaleDecay;		//!< The decay constant for the particle's scale
		Math::Vector3 min_scale;			//!< Minimum scaling constant
		
		float alpha;			//!< Particle transparency
		float alphaDecay;		//!< Particle transparency rate of decay
		float min_alpha;		//!< Alpha transparency floor		

		Math::Vector3 color;				//!< Particle color
		Math::Vector3 colorDecay;		//!< Particle color rate of decay
		Math::Vector3 min_color;			//!< Particle floor color

		float angle;			//!< Particle texture angle of rotation
		float angleDecay;		//!< Particle angle rate of change	

		float alphaDecayStart;		//!< Alpha decay start in seconds
		float scaleDecayStart;		//!< Scale decay start in seconds
		float sizeDecayStart;		//!< Size decay start in seconds
		float colorDecayStart;		//!< Color decay start in seconds
		float angleDecayStart;		//!< Angle decay start in seconds
		
		Particle(void);		//!< Constructor

		Particle(float m, const Math::Vector3 &pos, const Math::Vector3 &vel,
				  const Math::Vector3 &color, const Math::Vector3 &colorDecay, const Math::Vector3 &min_color,
				  float life, float size, float sizeDecay, float min_size, float alpha,
				  float angle, float angleDecay, float alphaDecay, float min_alpha,
				  const Math::Vector3 &scale, const Math::Vector3 &scaleDecay, const Math::Vector3 &min_scale,
				  float alphaDecayStart, float scaleDecayStart, float sizeDecayStart,
				  float colorDecayStart, float angleDecayStart);//!< Constructor that sets useful properties.

		void set(const Particle &p);		//!< Sets this particle's properties to another particle's properties	

		void addVel(const Math::Vector3 &v);		//!< Adds velocity to the particle
		void addPos(const Math::Vector3 &p);		//!< Add displacement

		bool Initialize_from_file(char* strFileName);	//!< Initialize the particle properties from file

	private:	
		char fileName[256];

		float initial_life;     //!< Initial life of a particle, set internally

		void simulate(float dt);//!< Simulates a particle by a small interval dt

		friend class ParticleSystem;
	};
}

#endif