/*
	class Mass, Simulation, Spring
*/


#ifndef PHYSICS_H_BLAU_PUNKT
#define PHYSICS_H_BLAU_PUNKT

#include "Client.h"
#include "utils/3DMATH.H"

namespace DifferentialArts
{
	/*
	=======================
		Mass Class
	=======================
	*/
	/*!	\class Mass
		\brief Basic mass class
		\author Rajesh Peter Douglas D'Monte	

		This class should not be used for defining mass objects. This was started
		as an effort to build a custom physics engine. However, the RxEngine team
		found it far more feasible to use an existing physics engine for the Reaction
		Engine. Use the physics engine provided by RxEngine instead. This class
		is still used in some engine internals
	*/
	class RX_API Mass
	{
	public:
		__forceinline Mass(void);									//!< Constructor
		__forceinline Mass(float m);								//!< Constructor that sets the mass
		__forceinline Mass(float m, const Math::Vector3 &pos, const Math::Vector3 &vel);			//!< Constructor that sets mass, position and velocity.

		__forceinline void applyForce(const Math::Vector3 &f);					//!< Applies a force
		__forceinline void init(void);							//!< Initializes the mass object by resetting forces.
		__forceinline void simulate(float dt);					//!< Simulates the mass by a small interval dt
		__forceinline void setVel(const Math::Vector3 &vel);						//!< Sets the velocity
		__forceinline Math::Vector3 getVel(void);							//!< Returns the velocity
		__forceinline void setPos(const Math::Vector3 &pos);						//!< Sets the position
		__forceinline Math::Vector3 getPos(void);							//!< Returns the position
		__forceinline void setM(float m);							//!< Sets the mass 
		__forceinline float getM(void);							//!< Returns the mas
		__forceinline float getInverseM(void);					//!< Returns the inverse mass
		__forceinline Math::Vector3 getForce(void);						//!< Returns the total forces acting on the mass

	protected:
		float inverseMass;
		float m;
		Math::Vector3 pos;
		Math::Vector3 vel;
		Math::Vector3 force;
	};

	/*
	=======================
		Simulation Class
	=======================
	*/

	class RX_API Simulation
	{
	public:
		int numberOfMasses;
		Mass** masses;

		Simulation(int n, float m);
		void release(void);
		void init(void);
		void solve(void);
		Mass* getMass(int n);
		void simulate(float dt);
		void operate(float dt);
	};

	/*
	=======================
		Spring Class
	=======================
	*/

	class RX_API Spring
	{
	public:
		Mass* mass1;
		Mass* mass2;

		float springLength;
		float springConstant;
		float frictionConstant;

		Spring(Mass* mass1, Mass* mass2, float springConstant, float springLength, 
			   float frictionConstant);

		void solve(void);
	};
}

#endif