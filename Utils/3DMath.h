/*
	Description: Consists of Vector classes and is the 3dMath header

	The classes contained here are,
	class Vector2
	class Vector3
	class quarternion
	
*/


/*
	IMPORTANT NOTE:
	---------------
		Some of  the math operations (including those that work on vectors)
		are not member functions in their respective classes.
		It has been	done Non - OOP

	NOTE: ClassifySphere function was adapted from gametutorials.com	
*/


#ifndef _3DMATH__
#define _3DMATH__

/*! \file 3DMATH.h
    \brief Fast 3D math library for reaction engine
    
    The 3D math library for the Reaction Engine is mostly not object oriented
	in order to run faster than it would otherwise run if it were object
	oriented.
*/
#include "UtilsClient.h"
#include "MemoryInterface.h"

#define SSE

#ifdef SSE

#include <intrin.h>

#pragma intrinsic ( _mm_hadd_ps )

#endif

// Allows you to set your FPU to single precision mode and back. 
// This especially speeds up divisions and square roots. Be careful
// with this instrucktions, some of the optimized functions won't work
// in single precision mode
#define SET_TO_SINGLE_PRECISION _controlfp(_PC_24, MCW_PC);
#define SET_TO_DOUBLE_PRECISION _controlfp(_CW_DEFAULT, 0xfffff);

namespace DifferentialArts
{
	namespace Math
	{
		#undef min
		#undef max

		#ifndef RAND_MAX
		#define RAND_MAX 0x7fff
		#endif

		#define FLT_MIN         1.175494351e-38F        // min positive value
		#define FLT_MAX         3.402823466e+38F        // max value 

		// RANDOM_NUM returns a float in the range -1.0f, 1.0f
		#define RANDOM_NUM      (((float)rand()-(float)rand())/RAND_MAX)
		/*! \def RANDOM_NUM
			\brief A macro that generates a random floating point number in the range [-1, 1]
		*/

		/*
		RANDOM_VECTOR returns a Vector3 whose components (x,y,z) are between -1.0f and 1.0f 
		this is not a normalized vector.
		*/
		#define RANDOM_VECTOR   Math::Vector3(RANDOM_NUM,RANDOM_NUM,RANDOM_NUM)
		/*! \def RANDOM_VECTOR
			\brief A macro that generates a random Vector3 vector
		*/

		#define FREE(p) { if(p) { delete (p); (p) = NULL; } }
		/*! \def FREE(p)
			\brief A macro that frees a pointer by deallocating memory
		*/

		#define lerp(t,a,b) ( ( a ) + ( t ) * (( b ) - ( a )) )
		/*! \def lerp(t,a,b)
			\brief A macro that does linear interpolation
		*/

		//Smallest value constant
		const float EPSILON = 0.001f;
		/*! \var float EPSILON
			\brief A small value
		*/

		//PI GLOBAL DEFINE
		#ifndef M_PI
		#define M_PI 3.1415926535897932	
		/*! \def M_PI
			\brief A define for the value of PI
		*/
		#endif

		//PI Divided by 2 GLOBAL DEFINE
		#ifndef M_PI_BY_2
		#define M_PI_BY_2 1.5707963267948966
		/*! \def M_PI_BY_2
			\brief A define for the value of PI/2
		*/
		#endif

		const float asm_one = 1.f;
		const float asm_half_pi = (float)M_PI_BY_2;

		const float piover180 = float(M_PI)/180; //PI OVER 180, used for converting degree to radian
		const float piover360 = float(M_PI)/360; //PI OVER 180, also used for converting degree to radian

		//Fast reciprocal square root
		RX_UTIL_API inline float rsqrt( float number )
		{
				float number_half = number * .5f;
				long i = * ( long* ) &number;
				//i = 0x5f3759df - ( i >> 1 ); 
				
				//	This will be more accurate for an inverse squareroot,
				//	reference: http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
				
	 			i = 0x5f375a86 - ( i >> 1 );
				number = * ( float* ) &i;
				return number * ( 1.5f - number_half * number * number );
		}
		/*! \fn float rsqrt( float number )
			\brief Calculates reciprocal square root

			\param number The number who's inverse square root is to be found.

			@return An inverse square root
		*/


		/*////////////////////////////////////////////////////////////////////
		//																	//
		//	This is the 2D Vector class called "Vector2"			   	        //
		//																	//
		*/////////////////////////////////////////////////////////////////////

		/*!	\class Vector2
			\brief A 2D vector class
			\author Rajesh Peter Douglas D'Monte

			Use this class if you would like to use 2D vectors.
		*/
		class RX_UTIL_API Vector2
		{
		public:
			//Constructor 1
			Vector2()
			{
				x = 0;
				y = 0;
			}
			//!<Constructor
			/*!< 
				Initializes both components to 0
			*/

			//Constructor 2
			Vector2(float xx, float yy)
			{
				x = xx, y = yy;
			}
			//!<Constructor
			/*!< 
				Initializes both components to the given parameters

				\param xx The x component of the vector
				\param yy The y component of the vector
			*/

			//Operator : + of 2D vector, "Vector2"
			Vector2 operator+(Vector2 &v)
			{
				return Vector2(x + v.x, y + v.y);
			}
			//!<Operator +
			/*!< 
				Adds two vectors

				\param v The supplied vector

				@return The sum of this vector with the supplied vector
			*/

			//Operator : - of 2D vector, "Vector2"
			Vector2 operator-(Vector2 v)
			{
				return Vector2(x - v.x, y - v.y);
			}
			//!<Operator -
			/*!< 
				Subtracts two vectors

				\param v The supplied vector

				@return The subtraction of this vector with the supplied vector
			*/

			//Operator : * of 2D vector, "Vector2"
			Vector2 operator*(float s)
			{
				return Vector2(x * s, y * s);
			}
			//!<Operator *
			/*!< 
				Multiplies a vector with a scalar

				\param s The scalar

				@return A scaled vector
			*/

			//Operator : / of 2D vector, "Vector2"
			Vector2 operator/(float s)
			{
				return Vector2(x / s, y / s);
			}
			//!<Operator /
			/*!< 
				Divides a vector with a scalar

				\param s The scalar

				@return An inverse scaled vector
			*/

			//Our public members
			float x;//!<Floating point x component
			float y;//!<Floating point y component
		};

		/*////////////////////////////////////////////////////////////////////
		//																	//
		//	This is the 3D Vector class called "Vector3"			   	        //
		//																	//
		*/////////////////////////////////////////////////////////////////////

		/*!	\class Vector3
			\brief A 3D vector class
			\author Rajesh Peter Douglas D'Monte

			Use this class if you would like to use 3D vectors.
		*/
		class RX_UTIL_API Vector3 : public BaseMemoryInterface
		{
		public:
			//Constructor 1
			inline Vector3()
			{
#ifdef SSE
				m128 = _mm_setzero_ps();
#else
				x = 0;
				y = 0;
				z = 0;
#endif
			}
			//!<Constructor
			/*!< 
				Initializes all components to 0
			*/

			inline Vector3(const Vector3 &v)
			{
#ifdef SSE
				m128 = v.m128;
#else
				x = v.x;
				y = v.y;
				z = v.z;
#endif
			}

#ifdef SSE
			inline Vector3(const __m128 &v)
			{
				m128 = v;
			}
#else
#endif

			//Constructor 2
			inline Vector3(const float xx, const float yy, const float zz)
			{
#ifdef SSE
				m128 = _mm_set_ps(0.0f, zz, yy, xx);
#else
				x = xx;
				y = yy;
				z = zz;
#endif
			}
			//!<Constructor
			/*!< 
				Initializes all components to the given parameters

				\param xx The x component of the vector
				\param yy The y component of the vector
				\param zz The z component of the vector
			*/	

			inline void operator=(const Vector3 &v)
			{
#ifdef SSE
				m128 = v.m128;
#else
				x = v.x;
				y = v.y;
				z = v.z;
#endif
			}

			//Operator : + of 3D vector, "Vector3"
			inline Vector3 operator+(const Vector3 &v) const
			{
#ifdef SSE
				return Vector3(_mm_add_ps(m128, v.m128));
#else
				return Vector3(v.x + x, v.y + y, v.z + z);
#endif
			}
			//!<Operator +
			/*!< 
				Adds two vectors

				\param v The supplied vector

				@return The sum of this vector with the supplied vector
			*/

			//Operator : - of 3D vector, "Vector3"
			inline Vector3 operator-(const Vector3 &v) const
			{
#ifdef SSE
				return Vector3(_mm_sub_ps(m128, v.m128));
#else
				return Vector3(x - v.x, y - v.y, z - v.z);
#endif
			}
			//!<Operator -
			/*!< 
				Subtracts two vectors

				\param v The supplied vector

				@return The subtraction of this vector with the supplied vector
			*/
			
			//Operator : * of 3D vector, "Vector3"
			inline Vector3 operator*(const float &s) const
			{
#ifdef SSE
				return Vector3(_mm_mul_ps(m128, _mm_set1_ps(s)));
#else
				return Vector3(x * s, y * s, z * s);
#endif
			}
			//!<Operator *
			/*!< 
				Multiplies a vector with a scalar

				\param s The scalar

				@return A scaled vector
			*/

			//Operator : / of 3D vector, "Vector3"
			inline Vector3 operator/(const float &s) const
			{
#ifdef SSE
				__m128 _128bitScalar = _mm_set1_ps(1/s);
				return Vector3(_mm_mul_ps(m128, _128bitScalar));
#else
				float one_over_s = 1/s;
				return Vector3(x * one_over_s, y * one_over_s, z * one_over_s);
#endif
			}	
			//!<Operator /
			/*!< 
				Divides a vector with a scalar

				\param s The scalar

				@return An inverse scaled vector
			*/

			//Operator : += of 3D vector, "Vector3"
			inline void operator+=(const Vector3 &v)
			{
#ifdef SSE
				m128 = _mm_add_ps(m128, v.m128);
#else
				x += v.x;
				y += v.y;
				z += v.z;
#endif
			}
			//!<Operator +=
			/*!< 
				Adds two vectors

				\param v The supplied vector

				@return The sum of this vector with the supplied vector
			*/

			//Operator : -= of 3D vector, "Vector3"
			inline void operator-=(const Vector3 &v)
			{
#ifdef SSE
				m128 = _mm_sub_ps(m128, v.m128);
#else
				x -= v.x;
				y -= v.y;
				z -= v.z;
#endif
			}
			//!<Operator -=
			/*!< 
				Subtracts two vectors

				\param v The supplied vector

				@return The subtraction of this vector with the supplied vector
			*/

			//Operator : *= of 3D vector, "Vector3"
			inline void operator*=(const float &s)
			{
#ifdef SSE
				__m128 _128bitScalar = _mm_set1_ps(s);
				m128 = _mm_mul_ps(m128, _128bitScalar);
#else
				x *= s;
				y *= s;
				z *= s;
#endif
			}
			//!<Operator *=
			/*!< 
				Multiplies a vector with a scalar

				\param s The scalar

				@return A scaled vector
			*/

			//Operator : /= of 3D vector, "Vector3"
			inline void operator/=(const float &s)
			{
#ifdef SSE
				__m128 _128bitScalar = _mm_set1_ps(1/s);
				m128 = _mm_mul_ps(m128, _128bitScalar);
#else
				float one_over_s = 1/s;
				x *= one_over_s;
				y *= one_over_s;
				z *= one_over_s;
#endif
			}   
			//!<Operator /=
			/*!< 
				Divides a vector with a scalar

				\param s The scalar

				@return An inverse scaled vector
			*/

			inline Vector3 operator - (void) const
			{		
#ifdef SSE
				return Vector3(_mm_sub_ps(_mm_setzero_ps(), m128));
#else
				return Vector3(-x, -y, -z);
#endif
			}
			//!<Operator -
			/*!< 
				This is a unary operator.

				@return The negative of the vector
			*/		

			inline float Magnitude(void) const
			{
#ifdef SSE
				__m128 a = _mm_mul_ps(m128, m128); //squared xyzw

				// add x, y, z of a and store in first part of b
				//__m128 b = _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,0,0)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1,1,1,1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,2,2,2))));
				__m128 b = _mm_hadd_ps(a, a);
				b = _mm_hadd_ps(b, b);
				__m128 final = _mm_sqrt_ss(b);
				return final.m128_f32[0];
#else
				float invmag = rsqrt((x*x) + (y*y) + (z*z));
				return 1.0f/invmag;
#endif
			}
			//!<Magnitude
			/*!< 
				This method calculates the magnitude of the vector

				@return The magnitude of the vector
			*/	

			inline float InverseMagnitude(void) const
			{
#ifdef SSE
				__m128 a = _mm_mul_ps(m128, m128);

				// horizontal add
				//__m128 b = _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,0,0)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1,1,1,1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,2,2,2))));
				__m128 b = _mm_hadd_ps(a, a);
				b = _mm_hadd_ps(b, b);
				__m128 final = _mm_rsqrt_ss(b);
				return final.m128_f32[0];
#else
				float invmag = rsqrt((x*x) + (y*y) + (z*z));
				return invmag;
#endif
			}
			//!<InverseMagnitude
			/*!< 
				This method calculates the reciprocal magnitude of the vector

				@return The inverse magnitude of the vector
			*/	

			inline float MagnitudeSquared(void) const
			{
#ifdef SSE
				__m128 a = _mm_mul_ps(m128, m128);

				// horizontal add
				//__m128 b = _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,0,0)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1,1,1,1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,2,2,2))));
				__m128 b = _mm_hadd_ps(a, a);
				b = _mm_hadd_ps(b, b);
				return b.m128_f32[0];
#else
				return ((x*x) + (y*y) + (z*z));
#endif
			}
			//!<MagnitudeSquared
			/*!< 
				This method calculates the magnitude of the vector squared

				@return The squared magnitude of the vector
			*/

			inline Vector3 Normalize(void) const
			{
#ifdef SSE
				__m128 a = _mm_mul_ps(m128, m128);

				// horizontal add
				//__m128 b = _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,0,0)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1,1,1,1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,2,2,2))));				
				__m128 b = _mm_hadd_ps(a, a);
				b = _mm_hadd_ps(b, b);
				__m128 fv_rsqrt = _mm_rsqrt_ss(b);
				__m128 final = _mm_shuffle_ps(fv_rsqrt, fv_rsqrt, _MM_SHUFFLE(0, 0, 0, 0));

				return Vector3(_mm_mul_ps(m128, final));
#else
				float invmag = rsqrt((x*x) + (y*y) + (z*z));
				return Vector3(x * invmag, y * invmag, z * invmag);
#endif
			}
			//!<Normalize
			/*!< 
				This method returns a normalized version of the vector

				@return A unit vector
			*/	

			inline Vector3 Cross(const Vector3 &v) const
			{
#ifdef SSE
				__m128 a = _mm_shuffle_ps(m128, m128, _MM_SHUFFLE(3, 0, 2, 1));
				__m128 b = _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(3, 1, 0, 2));
				__m128 c = _mm_shuffle_ps(m128, m128, _MM_SHUFFLE(3, 1, 0, 2));
				__m128 d = _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(3, 0, 2, 1));

				__m128 e = _mm_mul_ps(a, b);
				__m128 f = _mm_mul_ps(c, d);

				return Vector3(_mm_sub_ps(e, f));
#else
				Vector3 vCross = Vector3((y*v.z)-(z*v.y), (z*v.x)-(x*v.z), (x*v.y)-(y*v.x));
				return vCross;
#endif
			}
			//!<Cross
			/*!< 
				This method returns a the cross product of vector v with this vector

				@return A vector perpendicular to \a v and \a this
			*/

			inline float Dot(const Vector3 &v) const
			{
#ifdef SSE
				//__m128 a = _mm_mul_ps(m128, v.m128);
				//__m128 b = _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(0,0,0,0)), _mm_add_ss(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1,1,1,1)), _mm_shuffle_ps(a, a, _MM_SHUFFLE(2,2,2,2))));
				//__m128 a = _mm_hadd_ps(m128, v.m128);
				//__m128 b = _mm_hadd_ps(m128, a);
				__m128 a;
				a = _mm_mul_ps(m128, v.m128);
				a = _mm_hadd_ps(a, a);
				a = _mm_hadd_ps(a, a); 

				return a.m128_f32[0];
#else
				return ((x*v.x)+(y*v.y)+(z*v.z));
#endif
			}
			//!<Dot
			/*!< 
				Finds the scalar product of 2 vectors

				@return The scalar product of \a v and \a this
			*/

			

			//Our public members
#ifdef SSE
			union
			{
				__m128 m128;
				struct
				{
					float x, y, z, w;
				};
			};
				
#else
			float x;	//!<Floating point x component
			float y;	//!<Floating point y component
			float z;	//!<Floating point z component				
#endif
		};

#ifdef SSE

		/*!	\class Vector4
			\brief A 4D vector class
			\author Rajesh Peter Douglas D'Monte

			Use this class if you would like to use 4D vectors.
		*/
		class RX_UTIL_API Vector4 : public Vector3
		{
		public:
			//Constructor 1
			inline Vector4()
			{
				m128 = _mm_setzero_ps();
			}
			//!<Constructor
			/*!< 
				Initializes all components to 0
			*/

			inline Vector4(const Vector4 &v)
			{
				m128 = v.m128;
			}

			inline Vector4(const Vector3 &v, float ww)
			{
				m128 = v.m128;
				this->w = ww;
			}

			inline Vector4(const __m128 &v)
			{
				m128 = v;
			}

			//Constructor 2
			inline Vector4(const float xx, const float yy, const float zz, const float ww)
			{
				m128 = _mm_set_ps(ww, zz, yy, xx);
			}

			inline void operator=(const Vector4 &v)
			{
				m128 = v.m128;
			}

			inline void operator=(const Vector3 &v)
			{
				m128 = v.m128;
			}
		};
#else
		class Vector4 : public Vector3
		{
		public:
			float w;

			//Constructor 1
			inline Vector4()
			{
				x = y = z = w = 0;
			}
			//!<Constructor

			inline Vector4(const Vector4 &v)
			{
				x = v.x;
				y = v.y;
				z = v.z;
				w = v.w;
			}

			//Constructor 2
			inline Vector4(const float xx, const float yy, const float zz, const float ww)
			{
				x = xx;
				y = yy;
				z = zz;
				w = ww;
			}

			inline void operator=(const Vector4 &v)
			{
				x = v.x;
				y = v.y;
				z = v.z;
				w = v.w;
			}

			inline void operator=(const Vector3 &v)
			{
				x = v.x;
				y = v.y;
				z = v.z;
			}
		};
#endif


		RX_UTIL_API void RotateVector(Vector3* v, float angle, float x, float y, float z);
		/*! \fn void RotateVector(Vector3* v, float angle, float x, float y, float z)
			\brief Rotates a vector about an angle around the given axis

			\param v The vector to be rotated
			\param angle The angle of rotation
			\param x The x direction of the axis
			\param y The y direction of the axis
			\param z The z direction of the axis
		*/	

		//Normalize a vector
		RX_UTIL_API void __forceinline Normalize_f3(float &x, float &y, float &z);
		/*! \fn Vector3 Normalize(float &x, float &y, float &z)
			\brief Normalizes a given vector provided as floats

			\param x X component of the vector
			\param y Y component of the vector
			\param z Z component of the vector

			@return Nothing, all parameters are passed as references
		*/

		//Range compress to 1

		/*////////////////////////////////////////////////////////////////////
		//																	//
		//	This function is used generate a Range compressed normal		//
		//																	//
		*/////////////////////////////////////////////////////////////////////
		RX_UTIL_API inline Vector3 RangeCompress(const Vector3 &v)
		{
			Vector3 returnVector = v;

			float invmag = rsqrt((v.x*v.x)+ 
								 (v.y*v.y)+
								 (v.z*v.z));

			returnVector.x *= invmag;
			returnVector.y *= invmag;
			returnVector.z *= invmag;

			
			returnVector += Vector3(1.0f, 1.0f, 1.0f);	
			returnVector *= 0.5f;

			return returnVector;
		}
		/*! \fn Vector3 RangeCompress(Vector3 v)
			\brief Range compresses a given vector for usage in a texture

			This function normalizes the vector before range compressing

			\param v The vector to be range compressed

			@return A range compressed vector
		*/

		//Find the inverse magnitude of a vector 
		RX_UTIL_API __forceinline float iMagnitude_f3(const float &x, const float &y, const float &z);
		/*! \fn float iMagnitude(Vector3 v)
			\brief Finds the inverse magnitude of the given vector (fast) supplied as 3 floats

			\param x X component of the vector
			\param y Y component of the vector
			\param z Z component of the vector

			@return Nothing, all parameters are passed as references
		*/		

		//Find a normal to a polygon
		RX_UTIL_API __forceinline Vector3 Normal(Vector3 vPoly[]);
		/*! \fn float iMagnitude(Vector3 v)
			\brief Finds the normal of a polygon

			\param vPoly The points of a triangle

			@return A normal to the polygon
		*/

		//Find the closest point on a line when compared to another external point
		RX_UTIL_API inline Vector3 ClosestPointOnLine(const Vector3 &vA, const Vector3 &vB, const Vector3 &vPoint)
		{
			Vector3 vVector1 = vPoint - vA;	
			Vector3 vVector2 = (vB - vA);
			float one_over_d = vVector2.InverseMagnitude();

			vVector2 = (vB - vA)*one_over_d;

			float t = vVector2.Dot(vVector1);

			if(t <= 0)
				return vA;

			if(t >= 1/one_over_d)
				return vB;

			Vector3 vVector3 = vVector2 * t;

			Vector3 closestPoint = vA + vVector3;

			return closestPoint;
		}
		/*! \fn Vector3 ClosestPointOnLine(Vector3 vA, Vector3 vB, Vector3 vPoint)
			\brief Finds the closest point on a line to a given point

			\param vA One end of the line
			\param vB The other end of the line
			\param vPoint The point to be tested against

			@return The the closest point
		*/

		//Find the closest point on a line when compared to another external point
		RX_UTIL_API inline Vector3 ClosestPointOnLineOutputLerpValue(const Vector3 &vA, const Vector3 &vB, const Vector3 &vPoint, float* lerpValue)
		{
			Vector3 vVector1 = vPoint - vA;	
			Vector3 vVector2 = (vB - vA);
			float one_over_d = vVector2.InverseMagnitude();

			vVector2 = (vB - vA)*one_over_d;

			float t = vVector2.Dot(vVector1);

			if(t <= 0)
			{
				(*lerpValue) = 0.0f;
				return vA;
			}

			if(t >= 1/one_over_d)
			{
				(*lerpValue) = 1.0f;
				return vB;
			}

			Vector3 closestPoint = vA + vVector2 * t;

			(*lerpValue) = t * one_over_d;

			return closestPoint;
		}
		/*! \fn Vector3 ClosestPointOnLineOutputLerpValue(Vector3 vA, Vector3 vB, Vector3 vPoint, float* t)
			\brief Finds the closest point on a line to a given point and stores the lerp value in a pointer

			\param vA One end of the line
			\param vB The other end of the line
			\param vPoint The point to be tested against
			\param lerpValue A pointer to which the linear interpolant value is stored.

			@return The the closest point
		*/


		//find the distance between plane and origin
		RX_UTIL_API __forceinline float PlaneDistance(const Vector3 &vNormal, const Vector3 &vPoint);
		/*! \fn float PlaneDistance(Vector3 vNormal, Vector3 vPoint)
			\brief Finds the distance between plane and origin

			\param vNormal Normal of the plane
			\param vPoint Any point lying on the plane

			@return The plane distance from origin
		*/

		//Check to see if a ray collided with a plane
		RX_UTIL_API __forceinline bool IntersectedPlane(Vector3 vTriangle[], Vector3 vLine[], Vector3 &vNormal, float &originDis);
		/*! \fn bool IntersectedPlane(Vector3 vTriangle[], Vector3 vLine[], Vector3 &vNormal, float &originDis)
			\brief Test to see if a ray intersected with a plane

			\param vTriangle Three points on the plane
			\param vLine Two points representing the ray

			@return true if the ray intersected the plane

			\param vNormal The normal of the plane
			\param originDis The distance the plane is from the origin
		*/

		//Returns the point of intersection of a line that intersects a  plane
		RX_UTIL_API __forceinline Vector3 IntersectionPoint(const Vector3 &vNormal, Vector3 vLine[], float distance);
		/*! \fn Vector3 IntersectionPoint(Vector3 vNormal, Vector3 vLine[], float distance)
			\brief Finds the point of intersection of a line and a plane

			\param vNormal The normal of the plane
			\param vLine Two points representing the line
			\param distance The distance the plane is from origin

			@return The intersection point
		*/

		//	Check to see if a point is inside a polygon	
		RX_UTIL_API __forceinline bool InsidePolygon(const Vector3 &vPointOfIntersection, Vector3 vPoly[], long vertexCount);
		/*! \fn bool InsidePolygon(Vector3 vPointOfIntersection, Vector3 vPoly[], long vertexCount)
			\brief Finds the point is inside a polygon

			Note: Works only for polygons if all internal angles of the polygon is less than 180 degrees (PI radians)

			\param vPointOfIntersection The point on the plane of the polygon
			\param vPoly The polygon
			\param vertexCount The number of vertices in the polygon

			@return true if the point is inside the polygon
		*/

		//	Check to see if a line intersected a polygon
		RX_UTIL_API __forceinline bool IntersectedPolygon(Vector3 vPoly[], Vector3 vLine[], int vertexCount);
		/*! \fn bool IntersectedPolygon(Vector3 vPoly[], Vector3 vLine[], int vertexCount)
			\brief Finds if a line intersected a polygon

			\param vPoly The polygon 
			\param vLine Two points representing a line
			\param vertexCount The number of vertices in the polygon

			@return true if the point is inside the polygon
		*/

		//Return radian from degree
		RX_UTIL_API inline float DegreeToRadian(float x)
		{
			return float(x*piover180);
		}
		/*! \fn float DegreeToRadian(float x)
			\brief Converts degrees to radians

			\param x A degree

			@return A radian
		*/

		//Return the absolute value of a number
		RX_UTIL_API __forceinline float abs(float n)
		{
				*(long*)&n &= 0x7fffffff;	// clear bit 31
				return n;					// return value
		}
		/*! \fn float abs(float n)
			\brief Finds the absolute value for a given number

			\param n The given number

			@return The absolute value
		*/

		
		RX_UTIL_API inline float min(float a, float b)
		{
			return (a < b? a : b);
		}
		/*!  \fn float min(float a, float b)
			\brief Returns the minimum of \a a and \a b			
		*/

		RX_UTIL_API inline float max(float a, float b)
		{
			return (a > b? a : b);
		}
		/*! \fn float max(float a, float b)
			\brief Returns the maximum of \a a and \a b
		*/

		RX_UTIL_API inline float step(float a, float x)
		{
			return (float)(x >= a);
		}
		/*! \fn float step(float a, float x)
			\brief Returns 1 if x >= a, otherwise returns 0 (step function)
		*/

		/*
		=============================================

			Fast Trig functions

		=============================================
		*/

		RX_UTIL_API inline float sin(float r)
		{
				__asm
				{
						fld r	
						fsin
				}
		}

		/*! \fn sin(float r)
			\brief Fast sine function
		*/
		RX_UTIL_API inline float cos(float r)
		{
				__asm
				{
						fld r	
						fcos
				}
		}
		/*! \fn cos(float r)
			\brief Fast cosine function
		*/
		RX_UTIL_API inline float tan(float r)
		{
				__asm
				{
						fld r	
						fsin
						fld r
						fcos
						fdiv
				}
		}
		/*! \fn tan(float r)
			\brief Fast tan function
		*/


		/*
		=============================================
			arc cos function on the fpu
		=============================================
		*/
		RX_UTIL_API inline float acos(float r)
		{
			__asm
			{
				fld r // r0 = r
				fld r // r1 = r0, r0 = r
				fmul r // r0 = r0 * r
				fsubr asm_one // r0 = r0 - 1.f
				fsqrt // r0 = sqrtf( r0 )
				fchs // r0 = - r0
				fdiv // r0 = r1 / r0
				fld1 // {{ r0 = atan( r0 )
				fpatan // }}
				fadd asm_half_pi // r0 = r0 + pi / 2
			} // returns r0
		}

		/*
		=============================================
			arc sin function on the fpu
		=============================================
		*/
		RX_UTIL_API inline float asin(float r)
		{
			__asm
			{
				fld r // r0 = r
				fld r // r1 = r0, r0 = r
				fmul r // r0 = r0 * r
				fsubr asm_one // r0 = r0 - 1.f
				fsqrt // r0 = sqrtf( r0 )
				fdiv // r0 = r1 / r0
				fld1 // {{ r0 = atan( r0 )
				fpatan // }}
			} // returns r0
		}
		/*! \fn asin(float r)
			\brief Fast arc sine function
		*/

		/*
		=============================================
			arc tan function on the fpu
		=============================================
		*/
		RX_UTIL_API inline float atan(float r)
		{
			__asm
			{
				fld r // r0 = r
				fld1 // {{ r0 = atan( r0 )
				fpatan // }}
			} // returns r0
		}
		/*! \fn atan(float r)
			\brief Fast arc tan function
		*/

		//Clampe routine
		/*
		=============================================
			for clamping value
		=============================================
		*/
		RX_UTIL_API inline float clamp(float x, float a, float b)
		{
			return x < a ? a : (x > b ? b : x);
		}
		/*! \fn float clamp(float x, float a, float b)
			\brief Fast clamp function
		*/

		RX_UTIL_API inline Vector3 mix(Vector3 &color0, Vector3 &color1, float f)
		{
			return color0 * (1-f) + color1 * f;
		}
		/*! \fn Vector3 mix(Vector3 color0, Vector3 color1, float f)
			\brief Returns a mixture between 2 colors or vectors based on fraction f
		*/

		RX_UTIL_API inline float pulse(float a, float b, float x)
		{
			return (step(a, x) - step(b, x));
		}
		/*! \fn float pulse(float a, float b, float x)
			\brief Returns a pulse (1.0f) that starts when x = a and ends at x = b
		*/

		RX_UTIL_API inline float smoothstep(float a, float b, float x)
		{
			if (x < a)
				return 0;		
			if (x >= b)
				return 1;

			x = (x - a)/(b - a);
			return (x*x * (3 - 2*x));
		}
		/*! \fn float smoothstep(float a, float b, float x)
			\brief Returns a smooth curve value which is 0 at \a a and 1 at \a b and whose slope is 0 at \a a and \a b
		*/

		RX_UTIL_API inline float mod(float x, float a)
		{
			int n = (int)(x/a);
			x -= n*a;
			if (x < 0)
				x += a;

			return x;
		}	
		/*! \fn float mod(float x, float a)
			\brief The periodic modulus function
		*/

		RX_UTIL_API inline int floor(float &x)
		{
			return ((int)x - (x < 0 && x != (int)x));
		}
		/*! \fn int floor(float x)
			\brief Returns the floor of a floating point number
		*/

		RX_UTIL_API inline int ceil(float &x)
		{
			return ((int)x + (x > 0 && x != (int)x));
		}
		/*! \fn int ceil(float x)
			\brief Returns the ceiling of a floating point number
		*/

		// Coefficients of basis matrix for n-spline
		#define CROO -0.5f
		#define CR01 1.5f
		#define CR02 -1.5f
		#define CR03 0.5f
		#define CR10 1.0f
		#define CR11 -2.5f
		#define CR12 2.0f
		#define CR13 -0.5f
		#define CR20 -0.5f
		#define CR21 0.0f
		#define CR22 0.5f
		#define CR23 0.0f
		#define CR30 0.0f
		#define CR31 1.0f
		#define CR32 0.0f
		#define CR33 0.0f

		RX_UTIL_API inline float spline(float x, int totalPoints, float *points)
		{
			int span;
			int nspans = totalPoints - 3;
			float cO, cl, c2, c3; // coefficients of the cubic
			if (nspans < 1)
			{
				// illegal, must have atleast 4 points on the spline dude!!! :D
				//fprintf(stderr, �Spline has too few knots.\n�);
				return 0;
			}
			// Find the appropriate 4-point span of the spline.
			x = Math::clamp(x, 0, 1) * nspans;

			span = (int) x;

			if (span >= nspans)
			{
				span = nspans;
			}
			x -= span;
			points += span;

			// Evaluate the span cubic at x using Horner�s rule
			c3 = CROO*points[0] + CR01*points[1] + CR02*points[2] + CR03*points[3];
			c2 = CR10*points[0] + CR11*points[1] + CR12*points[2] + CR13*points[3];
			cl = CR20*points[0] + CR21*points[1] + CR22*points[2] + CR23*points[3];
			cO = CR30*points[0] + CR31*points[1] + CR32*points[2] + CR33*points[3];

			return ((c3*x + c2)*x + cl)*x + cO;
		}
		/*! \fn float spline(float x, int totalPoints, float *points)
			\brief Evaluates a spline, minimum 4 points required, use smoothstep for 3 points
		*/

		//Find the angle between two vectors
		RX_UTIL_API inline float AngleBetweenVectors(const Vector3 &v1, const Vector3 &v2)
		{
			float dotProduct = v1.Dot(v2);

			float mag = v1.InverseMagnitude() * v2.InverseMagnitude();

			float angle = acos(dotProduct * mag);

			//if(_isnan(angle))
			//	return 0;

			return angle;
		}
		/*! \fn double AngleBetweenVectors(Vector3 v1, Vector3 v2)
			\brief Finds the angle of the shortest path between two vectors

			\param v1 The first vector
			\param v2 The second vector

			@return The angle between the two vectors
		*/

		/*
		====================================
			Quaternion CLASS OBJECT (Quaternionernion class)
		====================================
		*/
		/*!	\class Quaternion
			\brief A Quaternionernion class
			\author Rajesh Peter Douglas D'Monte

			Use this class if you would like to use Quaternionernions. Detailed descriptions of methods
			are provided when necessary.
		*/
		class RX_UTIL_API Quaternion
		{
		public:	
			Quaternion();
			//!<Constructor
			/*!< 
				Initializes all components to 0
			*/
			Quaternion(float x, float y, float z, float w);
			//!<Constructor
			/*!< 
				Initializes all components to the given parameters

				\param x The x component of the vector
				\param y The y component of the vector
				\param z The z component of the vector
				\param w The w component of the vector
			*/

			void CreateFromAxisAngle(float vx, float vy, float vz, float radians);//!<Creates a Quaternionernion from an axis and an angle
			void returnMatrix(float *pMatrix);//!<Returns the respective matrix of the Quaternionernion through pMatrix
			void createQuaternionFromMatrix33(float* pMatrix);//!<Creates a Quaternionernion from a 3x3 matrix
			void createQuaternionFromMatrix44(float* pMatrix);//!<Creates a Quaternionernion from a 4x4 matrix
			Quaternion operator *(Quaternion q);//!<Multiplies two Quaternionernions
			Quaternion conjugate(void);//!<Returns the conjugate of the Quaternionernion
			void QuaternionRotateVector(Vector3* v, float radians, float axisX, float axisY, float axisZ);//Rotates a vector using Quaternionernions
			

			float x;//!<Floating point x component
			float y;//!<Floating point y component
			float z;//!<Floating point z component
			float w;//!<Floating point w component
		};

		//////////////////////////////////////////////////////////////////////////////
		//
		// class Matrix
		//
		//  Written by Zvi Devir, Intel MSL
		//
		//////////////////////////////////////////////////////////////////////////////

		//
		//   Copyright (c) 2001 Intel Corporation.
		//
		// Permition is granted to use, copy, distribute and prepare derivative works 
		// of this library for any purpose and without fee, provided, that the above 
		// copyright notice and this statement appear in all copies.  
		// Intel makes no representations about the suitability of this library for 
		// any purpose, and specifically disclaims all warranties. 
		// See LEGAL.TXT for all the legal information.
		//

		//SSE additions by Rajesh Peter Douglas D'Monte
		//Rajesh Peter Douglas D'Monte added a few more methods
		//Copyright (c) 2009 - 10 DifferentialArts


		#pragma pack(push,16) /* Must ensure class & union 16-_B aligned */

		class Matrix
		{
		public:

#ifdef SSE
			union
			{
				struct
				{
					__m128 m1;
					__m128 m2;
					__m128 m3;
					__m128 m4;
				};
				struct
				{
					float  _11, _12, _13, _14;
					float  _21, _22, _23, _24;
					float  _31, _32, _33, _34;
					float  _41, _42, _43, _44;
				};

				
			};

#else

			struct
			{
				float  _11, _12, _13, _14;
				float  _21, _22, _23, _24;
				float  _31, _32, _33, _34;
				float  _41, _42, _43, _44;
			};
#endif			

			float	ogl[16];

			//operator float* () const {return (float*) this;}
			//operator const float* () const {return (const float*) this;}

		// Constructors and convertions...
			Matrix() {}
			Matrix( float _11, float _12, float _13, float _14,
					  float _21, float _22, float _23, float _24,
					  float _31, float _32, float _33, float _34,
					  float _41, float _42, float _43, float _44 );

			//<NOTE> NO bound checks done!!! </NOTE>
			float& operator() (int i, int j) {
				return *(((float *)&_11) + (i<<2)+j);
			}

			Matrix& operator= (const Matrix& _A);

			Matrix  operator *  (Matrix&);
			Matrix& operator *= (const Matrix&);
			Matrix& operator *= (float);
			Matrix& operator /= (float);
			Matrix& operator += (const Matrix&);

			void Transpose(void);
			float Inverse();
			float *GetMatrix(void);
			float *GetGLMatrix(void);
			void SetFromOGLFormat(float *mat);
			void SetDirectOGLFormat(float *mat);

			inline void SetFirstRow(const Vector4 &v)
			{
#ifdef SSE
				this->m1 = v.m128;
#else	
				this->_11 = v.x; 
				this->_12 = v.y;
				this->_13 = v.z;
				this->_14 = v.w;
#endif
			}

			inline void SetSecondRow(const Vector4 &v)
			{
#ifdef SSE
				this->m2 = v.m128;
#else	
				this->_21 = v.x; 
				this->_22 = v.y;
				this->_23 = v.z;
				this->_24 = v.w;
#endif
			}

			inline void SetThirdRow(const Vector4 &v)
			{
#ifdef SSE
				this->m3 = v.m128;
#else	
				this->_31 = v.x; 
				this->_32 = v.y;
				this->_33 = v.z;
				this->_34 = v.w;
#endif
			}

			inline void SetFourthRow(const Vector4 &v)
			{
#ifdef SSE
				this->m4 = v.m128;
#else	
				this->_41 = v.x; 
				this->_42 = v.y;
				this->_43 = v.z;
				this->_44 = v.w;
#endif
			}

			// Other Constructors:
			void IdentityMatrix();
		};


		inline Matrix::Matrix( float d11, float d12, float d13, float d14,
								   float d21, float d22, float d23, float d24,
								   float d31, float d32, float d33, float d34,
								   float d41, float d42, float d43, float d44 )
		{
#ifdef SSE
			m1 = _mm_set_ps(d14, d13, d12, d11);
			m2 = _mm_set_ps(d24, d23, d22, d21);
			m3 = _mm_set_ps(d34, d33, d32, d31);
			m4 = _mm_set_ps(d44, d43, d42, d41);
#else
			_11 = d11; _12 = d12; _13 = d13; _14 = d14;
			_21 = d21; _22 = d22; _23 = d23; _24 = d24;
			_31 = d31; _32 = d32; _33 = d33; _34 = d34;
			_41 = d41; _42 = d42; _43 = d43; _44 = d44;
#endif
		}

		inline Matrix& Matrix::operator = (const Matrix& _A)
		{
#ifdef SSE
			m1 = _mm_set_ps(_A._14, _A._13, _A._12, _A._11);
			m2 = _mm_set_ps(_A._24, _A._23, _A._22, _A._21);
			m3 = _mm_set_ps(_A._34, _A._33, _A._32, _A._31);
			m4 = _mm_set_ps(_A._44, _A._43, _A._42, _A._41);
#else
			_11 = _A._11; _12 = _A._12; _13 = _A._13; _14 = _A._14;
			_21 = _A._21; _22 = _A._22; _23 = _A._23; _24 = _A._24;
			_31 = _A._31; _32 = _A._32; _33 = _A._33; _34 = _A._34;
			_41 = _A._41; _42 = _A._42; _43 = _A._43; _44 = _A._44;			
#endif
			return *this;
		}

		inline Matrix& Matrix::operator *= (const Matrix& _A)
		{
#ifdef SSE
			m1 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(0,0,0,0)), _A.m1), _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(1,1,1,1)), _A.m2)), _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(2,2,2,2)), _A.m3)), _mm_mul_ps(_mm_shuffle_ps(m1, m1, _MM_SHUFFLE(3,3,3,3)), _A.m4));
			m2 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(0,0,0,0)), _A.m1), _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(1,1,1,1)), _A.m2)), _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(2,2,2,2)), _A.m3)), _mm_mul_ps(_mm_shuffle_ps(m2, m2, _MM_SHUFFLE(3,3,3,3)), _A.m4));
			m3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(0,0,0,0)), _A.m1), _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(1,1,1,1)), _A.m2)), _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(2,2,2,2)), _A.m3)), _mm_mul_ps(_mm_shuffle_ps(m3, m3, _MM_SHUFFLE(3,3,3,3)), _A.m4));
			m4 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(0,0,0,0)), _A.m1), _mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(1,1,1,1)), _A.m2)), _mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(2,2,2,2)), _A.m3)), _mm_mul_ps(_mm_shuffle_ps(m4, m4, _MM_SHUFFLE(3,3,3,3)), _A.m4));
#else
			float t11, t12, t13, t14;
			float t21, t22, t23, t24;
			float t31, t32, t33, t34;
			float t41, t42, t43, t44;

			t11 = _11*_A._11 + _12*_A._21 + _13*_A._31 + _14*_A._41;
			t12 = _11*_A._12 + _12*_A._22 + _13*_A._32 + _14*_A._42;
			t13 = _11*_A._13 + _12*_A._23 + _13*_A._33 + _14*_A._43;
			t14 = _11*_A._14 + _12*_A._24 + _13*_A._34 + _14*_A._44;

			t21 = _21*_A._11 + _22*_A._21 + _23*_A._31 + _24*_A._41;
			t22 = _21*_A._12 + _22*_A._22 + _23*_A._32 + _24*_A._42;
			t23 = _21*_A._13 + _22*_A._23 + _23*_A._33 + _24*_A._43;
			t24 = _21*_A._14 + _22*_A._24 + _23*_A._34 + _24*_A._44;

			t31 = _31*_A._11 + _32*_A._21 + _33*_A._31 + _34*_A._41;
			t32 = _31*_A._12 + _32*_A._22 + _33*_A._32 + _34*_A._42;
			t33 = _31*_A._13 + _32*_A._23 + _33*_A._33 + _34*_A._43;
			t34 = _31*_A._14 + _32*_A._24 + _33*_A._34 + _34*_A._44;

			t41 = _41*_A._11 + _42*_A._21 + _43*_A._31 + _44*_A._41;
			t42 = _41*_A._12 + _42*_A._22 + _43*_A._32 + _44*_A._42;
			t43 = _41*_A._13 + _42*_A._23 + _43*_A._33 + _44*_A._43;
			t44 = _41*_A._14 + _42*_A._24 + _43*_A._34 + _44*_A._44;

			_11 = t11; _12 = t12; _13 = t13; _14 = t14;
			_21 = t21; _22 = t22; _23 = t23; _24 = t24;
			_31 = t31; _32 = t32; _33 = t33; _34 = t34;
			_41 = t41; _42 = t42; _43 = t43; _44 = t44;
#endif

			return *this;
		}

		inline Matrix Matrix::operator * (Matrix &_A) 
		{
			Matrix res = (*this);

			res *= _A;
			return res;
		}

		inline Matrix& Matrix::operator *= (float d)
		{
#ifdef SSE
			m1 = _mm_mul_ps(m1, _mm_set1_ps(d));
			m2 = _mm_mul_ps(m2, _mm_set1_ps(d));
			m3 = _mm_mul_ps(m3, _mm_set1_ps(d));
			m4 = _mm_mul_ps(m4, _mm_set1_ps(d));
#else
			_11 *= d; _12 *= d; _13 *= d; _14 *= d;
			_21 *= d; _22 *= d; _23 *= d; _24 *= d;
			_31 *= d; _32 *= d; _33 *= d; _34 *= d;
			_41 *= d; _42 *= d; _43 *= d; _44 *= d;
#endif

			return *this;
		}

		inline Matrix& Matrix::operator /= (float d)
		{
			d = 1.0f/d;

#ifdef SSE
			m1 = _mm_mul_ps(m1, _mm_set1_ps(d));
			m2 = _mm_mul_ps(m2, _mm_set1_ps(d));
			m3 = _mm_mul_ps(m3, _mm_set1_ps(d));
			m4 = _mm_mul_ps(m4, _mm_set1_ps(d));
#else
			_11 *= d; _12 *= d; _13 *= d; _14 *= d;
			_21 *= d; _22 *= d; _23 *= d; _24 *= d;
			_31 *= d; _32 *= d; _33 *= d; _34 *= d;
			_41 *= d; _42 *= d; _43 *= d; _44 *= d;
#endif

			return *this;
		}

		inline Matrix& Matrix::operator += (const Matrix& _A)
		{
#ifdef SSE
			m1 = _mm_add_ps(m1, _A.m1);
			m2 = _mm_add_ps(m2, _A.m2);
			m3 = _mm_add_ps(m3, _A.m3);
			m4 = _mm_add_ps(m4, _A.m4);
#else
			_11 += _A._11; _12 += _A._12; _13 += _A._13; _14 += _A._14; 
			_21 += _A._21; _22 += _A._22; _23 += _A._23; _24 += _A._24; 
			_31 += _A._31; _32 += _A._32; _33 += _A._33; _34 += _A._34; 
			_41 += _A._41; _42 += _A._42; _43 += _A._43; _44 += _A._44; 
#endif

			return *this;
		}

		#define	MATRIX_SWAP(a, b, t)	{	t=a; a=b; b=t;	}

		inline void	Matrix::Transpose(void)
		{
#ifdef SSE
			_MM_TRANSPOSE4_PS(m1, m2, m3, m4);
#else
			float t;

			MATRIX_SWAP(_12, _21, t);
			MATRIX_SWAP(_13, _31, t);
			MATRIX_SWAP(_14, _41, t);
			MATRIX_SWAP(_24, _42, t);
			MATRIX_SWAP(_34, _43, t);
			MATRIX_SWAP(_23, _32, t);
#endif
		}

		#undef	MATRIX_SWAP

		inline void Matrix::IdentityMatrix() 
		{			 
			_11 = _22 = _33 = _44 = 1.0;
			_12 = _13 = _14 = _21 = 0.0;
			_23 = _24 = _31 = _32 = 0.0;
			_34 = _41 = _42 = _43 = 0.0;
		}

		inline float *Matrix::GetMatrix(void)
		{			
			ogl[0] = _11;	ogl[1] = _21;	ogl[2] = _31;	ogl[3] = _41;
			ogl[4] = _12;	ogl[5] = _22;	ogl[6] = _32;	ogl[7] = _42;
			ogl[8] = _13;	ogl[9] = _23;	ogl[10] = _33;	ogl[11] = _43;
			ogl[12] = _14;	ogl[13] = _24;	ogl[14] = _34;	ogl[15] = _44;
			

			/*
			ogl[0] = _11;	ogl[4] = _21;	ogl[8] = _31;	ogl[12] = _41;
			ogl[1] = _12;	ogl[5] = _22;	ogl[9] = _32;	ogl[13] = _42;
			ogl[2] = _13;	ogl[6] = _23;	ogl[10] = _33;	ogl[14] = _43;
			ogl[3] = _14;	ogl[7] = _24;	ogl[11] = _34;	ogl[15] = _44;
			*/

			return	ogl;
		}

		inline float *Matrix::GetGLMatrix(void)
		{			
			ogl[0] = _11;	ogl[4] = _21;	ogl[8] = _31;	ogl[12] = _41;
			ogl[1] = _12;	ogl[5] = _22;	ogl[9] = _32;	ogl[13] = _42;
			ogl[2] = _13;	ogl[6] = _23;	ogl[10] = _33;	ogl[14] = _43;
			ogl[3] = _14;	ogl[7] = _24;	ogl[11] = _34;	ogl[15] = _44;

			return	ogl;
		}

		inline void	Matrix::SetFromOGLFormat(float *mat)
		{
#ifdef SSE
			m1 = _mm_set_ps(mat[12], mat[8], mat[4], mat[0]);
			m2 = _mm_set_ps(mat[13], mat[9], mat[5], mat[1]);
			m3 = _mm_set_ps(mat[14], mat[10], mat[6], mat[2]);
			m4 = _mm_set_ps(mat[15], mat[11], mat[7], mat[3]);
#else
			_11 = mat[0];	_21 = mat[1];	_31 = mat[2];	_41 = mat[3];
			_12 = mat[4];	_22 = mat[5];	_32 = mat[6];	_42 = mat[7];
			_13 = mat[8];	_23 = mat[9];	_33 = mat[10];	_43 = mat[11];
			_14 = mat[12];	_24 = mat[13];	_34 = mat[14];	_44 = mat[15];
#endif
		}

		inline void	Matrix::SetDirectOGLFormat(float *mat)
		{
#ifdef SSE
			m1 = _mm_set_ps(mat[3], mat[2], mat[1], mat[0]);
			m2 = _mm_set_ps(mat[7], mat[6], mat[5], mat[4]);
			m3 = _mm_set_ps(mat[11], mat[10], mat[9], mat[8]);
			m4 = _mm_set_ps(mat[15], mat[14], mat[13], mat[12]);
#else
			_11 = mat[0];	_21 = mat[1];	_31 = mat[2];	_41 = mat[3];
			_12 = mat[4];	_22 = mat[5];	_32 = mat[6];	_42 = mat[7];
			_13 = mat[8];	_23 = mat[9];	_33 = mat[10];	_43 = mat[11];
			_14 = mat[12];	_24 = mat[13];	_34 = mat[14];	_44 = mat[15];
#endif
		}

		inline float Matrix::Inverse() 
		{ 
			float dB = _13*_24 - _14*_23;
			float dC = _31*_42 - _32*_41;

			float DC11 = _44*_31 - _34*_41;
			float DC12 = _44*_32 - _34*_42;
			float DC21 = _33*_41 - _43*_31;
			float DC22 = _33*_42 - _43*_32;

			float dD = _33*_44 - _34*_43;
			float dA = _11*_22 - _12*_21;

			float AB11 = _22*_13 - _12*_23;
			float AB12 = _22*_14 - _12*_24;
			float AB21 = _11*_23 - _21*_13;
			float AB22 = _11*_24 - _21*_14;
			float tr = AB11*DC11 + AB12*DC21 + AB21*DC12 + AB22*DC22;
			float det = dA*dD + dB*dC - tr;

			float CAB11 = _31*AB11 + _32*AB21;
			float CAB12 = _31*AB12 + _32*AB22;
			float CAB21 = _41*AB11 + _42*AB21;
			float CAB22 = _41*AB12 + _42*AB22;

			float iD11 = _33*dA - CAB11;
			float iD12m = CAB12 - _34*dA;
			float iD21m = CAB21 - _43*dA;
			float iD22 = _44*dA - CAB22;

			float BDC11 = _13*DC11 + _14*DC21;
			float BDC12 = _13*DC12 + _14*DC22;
			float BDC21 = _23*DC11 + _24*DC21;
			float BDC22 = _23*DC12 + _24*DC22;

			float iA11 = _11*dD - BDC11;
			float iA12m = BDC12 - _12*dD;
			float iA21m = BDC21 - _21*dD;
			float iA22 = _22*dD - BDC22;

			float rd = 1.0f/det;

			float DBA11 = _33*AB22 - _34*AB21;
			float DBA12 = _34*AB11 - _33*AB12;
			float DBA21 = _43*AB22 - _44*AB21;
			float DBA22 = _44*AB11 - _43*AB12;

			float ACD11 = _11*DC22 - _12*DC21;
			float ACD12 = _12*DC11 - _11*DC12;
			float ACD21 = _21*DC22 - _22*DC21;
			float ACD22 = _22*DC11 - _21*DC12;

			float iB11 = _31*dB - DBA11;
			float iB12m = DBA12 - _32*dB;
			float iB21m = DBA21 - _41*dB;
			float iB22 = _42*dB - DBA22;
		    
			float iC11 = _13*dC - ACD11;
			float iC12m = ACD12 - _14*dC;
			float iC21m = ACD21 - _23*dC;
			float iC22 = _24*dC - ACD22;

			_11 = iA22 * rd;
			_12 = iA12m * rd;
			_21 = iA21m * rd;
			_22 = iA11 * rd;
		    
			_13 = iB22 * rd;
			_14 = iB12m * rd;
			_23 = iB21m * rd;
			_24 = iB11 * rd;
		    
			_31 = iC22 * rd;
			_32 = iC12m * rd;
			_41 = iC21m * rd;
			_42 = iC11 * rd;
		    
			_33 = iD22 * rd;
			_34 = iD12m * rd;
			_43 = iD21m * rd;
			_44 = iD11 * rd;

			return det;
		}


		#pragma pack(pop)
	}

	//Author: Rajesh Peter Douglas D'Monte
	static
	inline
	Math::Vector4 operator * (const Math::Matrix& m, const Math::Vector4& v)
	{
		//return Math::Vector4(
		//	_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(m.m1, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(0,0,0,0))), _mm_mul_ps(m.m2, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(1,1,1,1)))), _mm_mul_ps(m.m3, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(2,2,2,2)))), _mm_mul_ps(m.m4, _mm_shuffle_ps(v.m128, v.m128, _MM_SHUFFLE(3,3,3,3))))
		//);

		__m128 p1 = _mm_mul_ps(m.m1, v.m128);
		__m128 p2 = _mm_mul_ps(m.m2, v.m128);
		__m128 p3 = _mm_mul_ps(m.m3, v.m128);
		__m128 p4 = _mm_mul_ps(m.m4, v.m128);

		p1 = _mm_hadd_ps(p1, p1);
		p1 = _mm_hadd_ps(p1, p1);

		p2 = _mm_hadd_ps(p2, p2);
		p2 = _mm_hadd_ps(p2, p2);

		p3 = _mm_hadd_ps(p3, p3);
		p3 = _mm_hadd_ps(p3, p3);

		p4 = _mm_hadd_ps(p4, p4);
		p4 = _mm_hadd_ps(p4, p4);

		return Math::Vector4(p1.m128_f32[0], p2.m128_f32[0], p3.m128_f32[0], p4.m128_f32[0]);
	}
}

#endif

//End of 3DMATH.H
