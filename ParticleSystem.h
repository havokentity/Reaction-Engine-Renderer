/*
	class ParticleSystem
*/


#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "utils/AlignedObjectArray.h"
//#include <boost/typeof/std/vector.hpp>
#include "CoreEngine.h"
#include "Particle.h"
#include "VertexBuffer.h"
#include "utils/Containers.h"


namespace DifferentialArts
{
	/*
	==============================================
		ParticleSystem Class 
	==============================================
	*/

	/*!	\class ParticleSystem
		\brief A particle system class
		\author Rajesh Peter Douglas D'Monte	

		This class is used to run particle systems in the Reaction Engine
	*/
	class RX_API ParticleSystem : public BaseMemoryInterface
	{
	public:
		/*
		static void* AllocateParticleMemory()
		{
			return _mm_malloc(sizeof(Particle), 16);
		}//!< Allocates byte aligned memory for a particle.


		static void DeAllocateParticleMemory(Particle* memoryOfParticle)
		{
			_mm_free(memoryOfParticle);
		}//!< De allocates byte aligned memory for a particle.*/

		/* Variables */
		Math::Vector3 pos;							//!< Particle system position
		Math::Vector3 vel;							//!< Particle system velocity
		Math::Vector3 gravity;						//!< Gravity vector acting on particle system
		Math::Vector3 wind;							//!< Wind vector acting on particle system
		Math::Vector3 randomPointOfOrigin;			//!< Random point of origin for particle
		unsigned int totalParticles;		//!< Particle system service ceiling
		float fReleaseRate;					//!< Particle system service release rate in seconds
		unsigned int numberToRelease;		//!< Particle system service pass ceiling
		Math::Vector3 randomForce;					//!< Particle system random force magnitude
		float systemLife;					//!< Particle system life
		bool chaos;							//!< Enables/Disables chaos in the system
		float friction;						//!< Friction applied on each particle

		Math::Vector3 minExtent;						//!< Min extent of bounding box
		Math::Vector3 maxExtent;						//!< max extent of bounding box

		VertexBuffer psVB;
		ParticleVB* particleVertices;

		float GetFastForward(void) const;				//!< Return fast forward in seconds

		/* Methods */
		ParticleSystem();					//!< Constructor

		ParticleSystem(
						float mass,
						const Math::Vector3 &pos, const Math::Vector3 &vel,
						unsigned int totalParticles,
						unsigned int uiTexture,
						float fReleaseRate,
						unsigned int numberToRelease,
						float friction,
						float fastForward,
						const Math::Vector3 &gravity,
						const Math::Vector3 &wind, 
						const Math::Vector3 &randomForce,
						const Math::Vector3 &randomPointOfOrigin, 
						bool chaos,
						Particle* particle_config
						);	 				//!< Configures the particle system

		~ParticleSystem();					//!< Destructor

		bool Initialize_from_file(char* strFileName, unsigned int uiTexture);
		//!< Initializes the particle system from file

		bool Initialize_from_file_SETBASE(char* strFileName, unsigned int uiTexture, Particle* bp);
		//!< Initializes the particle system from file and sets the basic particle to another particle

		bool InitFromPSystem(ParticleSystem* pSystem);
		//!< Initializes a particle system from a particle system

		void reset_particle(int particle_id);
		//!< Resets a particle with id number	to the base particle properties
		
		/*!<
			Function used for solving and simulating particles in system
			NOTE: it doesn't solve the psystem, see class Example_For_SimulatedParticleSystem
			for solving a psystem with customized forces
		*/
		void operate(float dt);							//!< Calls init()  and solve_and_simulate()
		void applyForce(const Math::Vector3 &f);						//!< Applies a force f on the system
		void applyForceOnEmitter(const Math::Vector3 &f);				//!< Applies a force on the particle system emitter
		void setMass(float m);							//!< Sets the emitter points mass		
		float getMass(void);							//!< Returns the mass of the particle system

		inline virtual void Render(void);				//!< Renders the particle system with reference to a camera
		inline virtual void RenderPointSprites(void);	//!< Renders the particle system with the ARB point sprite extension (Upto 4 times faster)
		unsigned int getActiveParticles(void);			//!< Returns the total number of active particles
		
		void setBaseParticle(Particle* particle);		//!< Sets the base particle	
		Particle getParticle(unsigned int i);			//!< Returns the particle at position i

		void fastForwardSystem(float seconds);			//!< Fast forwards the system by a certain amount seconds
		void toggleStopped(void);						//!< Toggles the stop state of the particle system
		void stop(void);								//!< Stops the particle system
		void run(void);									//!< Runs the particle system
		void runOnce(void);								//!< Runs the particle system only once
		bool getstopped(void);							//!< Returns true if particle system is stopped
		void setTex(GLuint &tex);						//!< Sets texture to be used by particle system
		void resetTimes(void);							//!< Reset timesPlayed to 0

		AlignedObjectArray<PointAttractor> attractor;			//!< Point gravitational attractor
		AlignedObjectArray<PointAttractor> lineAttractor;		//!< Supply attractors in pairs

		unsigned int getTotalTimesPlayedSinceLastResetTimes(void);
		//!< Returns total times particle system played since resetTimes() was last called
		unsigned int getTotalTimesPlayed(void);
		//!< Returns total times particle system played 


		/* 
			THIS METHOD IS GIVING SOME TROUBLE, SO DON'T USE IT, FREE SHOULD DO THE TRICK
			ITSELF 
		*/
		void Free(void);								//!< Releases the particles of the particle system resources.

		void ReleaseBaseParticle(void);					//!< Releases the base particle			
		
	protected:
		bool bRunOnce;	
		bool stopped;
		float fSystemAge;


//#define USE_CUSTOM_FANCY_MEMORY_16_BYTE_ALIGNED_VECTOR_HERE
		AlignedObjectArray<Particle> particle;
		//__declspec(align(16)) AlignedObjectArray<__declspec(align(16)) Particle>::iterator pIterator;
//#undef USE_CUSTOM_FANCY_MEMORY_16_BYTE_ALIGNED_VECTOR_HERE

		Math::Vector3 force, systemForce;			
		float mass;
		float inverseMass;

		float fCurrentElapsedTimeSinceLastReleaseOfParticle;
		
		Particle* base_particle;
		unsigned int current;	
		unsigned int uiTexture;	
		float fastForward;
		bool loaded;
		bool bFastForwarded;
		char fileName[256];

		bool final;

		bool PrepareVB(void);

		void init(void);					//!< First stage of operate
		void solve_and_simulate(float dt);	//!< Protected: Solves and simulates the system. See detailed description
		void solve(void);					//!< Redundant. See detailed description
		/*!<
			THIS IS A REDUNDANT METHOD, SHOULDN'T BE USED ANY MORE
			CHECK OUT solve_and_simulate FOR SIMULATION OF PARTICLE 
			SYSTEM
		*/

		unsigned int timesPlayed;//!< Number of times the particle system has played
		unsigned int totalTimesPlayed;//!< Total times the particle system played excluding resets	
	};
}

#endif