/*
	classes contained are:

	class Frustum
*/

#ifndef _FRUSTUM_H__
#define _FRUSTUM_H__

#pragma comment(lib, "opengl32.lib")

#include "Client.h"
#include <windows.h>
#include <gl/gl.h>

#include "utils/3DMATH.H"

namespace DifferentialArts
{

	/*! \file Frustum.h
		\brief Contains the frustum class and its related defines
	    
		This file contains the frustum class and defines that are used
		by the frustum class.
	*/

	/* Frustum sides */
	#define RIGHT_SIDE_OF_FRUSTUM	0
	/*! \def RIGHT_SIDE_OF_FRUSTUM
		\brief The right side of the frustum
	*/

	#define LEFT_SIDE_OF_FRUSTUM	1
	/*! \def LEFT_SIDE_OF_FRUSTUM
		\brief The left side of the frustum
	*/
	#define BOTTOM_SIDE_OF_FRUSTUM	2
	/*! \def BOTTOM_SIDE_OF_FRUSTUM
		\brief The top side of the frustum
	*/
	#define TOP_SIDE_OF_FRUSTUM		3
	/*! \def TOP_SIDE_OF_FRUSTUM
		\brief The top side of the frustum
	*/
	#define BACK_SIDE_OF_FRUSTUM	4
	/*! \def BACK_SIDE_OF_FRUSTUM
		\brief The back side of the frustum
	*/
	#define FRONT_SIDE_OF_FRUSTUM	5
	/*! \def FRONT_SIDE_OF_FRUSTUM
		\brief The front side of the frustum
	*/

	/* PlaneData sides */
	#define A_COEFFICIENT	0
	/*! \def A
		\brief Coefficient of x of the plane equation of a side of the frustum
	*/
	#define B_COEFFICIENT	1
	/*! \def B
		\brief Coefficient of y of the plane equation of a side of the frustum
	*/
	#define C_COEFFICIENT	2
	/*! \def C
		\brief Coefficient of z of the plane equation of a side of the frustum
	*/
	#define D_COEFFICIENT	3
	/*! \def D
		\brief The distance from the origin of a given plane of the frustum
	*/

	// This will allow us to create an object to keep track of our frustum
	/*! \class Frustum
		\brief Frustum class

		This class is responsible for all frustum related calculations
		in the engine.
	*/
	class RX_API Frustum
	{

	public:

		//! Call this every time the camera moves to update the frustum
		void CalculateFrustum();

		//! This takes a 3D point and returns \a true if it's inside of the frustum
		bool PointInFrustum(float x, float y, float z);

		//! This takes a 3D point and returns \a true if it's inside of the frustum for an infinite frustum
		bool PointInFrustumInfinite(float x, float y, float z);

		//! This takes a 3D point and a radius and returns \a true if the sphere is inside of the frustum
		bool SphereInFrustum(float x, float y, float z, float radius);

		//! This takes the center and half the length of the cube.
		bool CubeInFrustum( float x, float y, float z, float size );

		//! Checks to see if a quad tree type cuboid is in frustum
		bool CuboidInFrustum( float x, float z, float size, float h );

		//! Checks to see if an cuboid is in frustum
		bool AACuboidInFrustum( float x, float y, float z, const Math::Vector3 &mins, const Math::Vector3 &maxs);

		//! Checks to see if a frustum intersects a frustum
		bool FrustumInFrustum(Frustum &f);

		//!< Calculates the 8 corner points of a frustum
		void CalculateFrustumCorners(Math::Vector3 pPoints[8], Math::Vector3 &vPos, Math::Vector3 &vView,
									 Math::Vector3 &vUp, float fNear, float fFar, float fScale,
									 float fFOV, float fAspect);

		//! This holds the A B C and D values for each plane of the frustum.
		float m_Frustum[6][4];

	private:
		
		__forceinline Math::Vector3 GetIntersectionPoint(int plane1, int plane2, int plane3);

		//POINTS
		Math::Vector3 firstFrustumCorners[8];

		//EDGES
		Math::Vector3 edge[12][2];

		//TEMP POLYGON
		Math::Vector3 tempPolygon[4];
	};

}


#endif
