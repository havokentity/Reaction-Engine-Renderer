/*
	classes contained are:

	class Frustum
*/

/*
	Refer graphics gems I
*/

#include "Frustum.h"

namespace DifferentialArts
{

	void NormalizePlane(float frustum[6][4], int side)
	{
		/*
		float magnitude = (float)sqrtf( frustum[side][A] * frustum[side][A] + 
									   frustum[side][B] * frustum[side][B] + 
									   frustum[side][C] * frustum[side][C] );*/

		float invMagnitude = Math::rsqrt(frustum[side][A_COEFFICIENT] * frustum[side][A_COEFFICIENT] + 
									   frustum[side][B_COEFFICIENT] * frustum[side][B_COEFFICIENT] + 
									   frustum[side][C_COEFFICIENT] * frustum[side][C_COEFFICIENT] );

		// Then we divide the plane's values by it's magnitude.
		// This makes it easier to work with.
		/*
		frustum[side][A] /= magnitude;
		frustum[side][B] /= magnitude;
		frustum[side][C] /= magnitude;
		frustum[side][D] /= magnitude; 
		*/

		frustum[side][A_COEFFICIENT] *= invMagnitude;
		frustum[side][B_COEFFICIENT] *= invMagnitude;
		frustum[side][C_COEFFICIENT] *= invMagnitude;
		frustum[side][D_COEFFICIENT] *= invMagnitude; 
	}


	/*
	==============================------------------=======================

	This extracts our frustum from the projection and modelview matrix.

	==============================------------------=======================
	*/

	void Frustum::CalculateFrustum()
	{    
		float   proj[16];								// This will hold our projection matrix
		float   clip[16];								// This will hold the clipping planes

		glGetFloatv( GL_PROJECTION_MATRIX, proj );
		glGetFloatv( GL_MODELVIEW_MATRIX, clip );

		glPushMatrix();
			glLoadMatrixf(proj);
			glMultMatrixf(clip);
			glGetFloatv(GL_MODELVIEW_MATRIX, clip);
		glPopMatrix();


		// This will extract the RIGHT side of the frustum
		m_Frustum[RIGHT_SIDE_OF_FRUSTUM][A_COEFFICIENT] = clip[ 3] - clip[ 0];
		m_Frustum[RIGHT_SIDE_OF_FRUSTUM][B_COEFFICIENT] = clip[ 7] - clip[ 4];
		m_Frustum[RIGHT_SIDE_OF_FRUSTUM][C_COEFFICIENT] = clip[11] - clip[ 8];
		m_Frustum[RIGHT_SIDE_OF_FRUSTUM][D_COEFFICIENT] = clip[15] - clip[12];

		// Now that we have a normal (A,B,C) and a distance (D) to the plane,
		// we want to normalize that normal and distance.

		// Normalize the RIGHT side
		NormalizePlane(m_Frustum, RIGHT_SIDE_OF_FRUSTUM);

		// This will extract the LEFT side of the frustum
		m_Frustum[LEFT_SIDE_OF_FRUSTUM][A_COEFFICIENT] = clip[ 3] + clip[ 0];
		m_Frustum[LEFT_SIDE_OF_FRUSTUM][B_COEFFICIENT] = clip[ 7] + clip[ 4];
		m_Frustum[LEFT_SIDE_OF_FRUSTUM][C_COEFFICIENT] = clip[11] + clip[ 8];
		m_Frustum[LEFT_SIDE_OF_FRUSTUM][D_COEFFICIENT] = clip[15] + clip[12];

		// Normalize the LEFT side
		NormalizePlane(m_Frustum, LEFT_SIDE_OF_FRUSTUM);

		// This will extract the BOTTOM side of the frustum
		m_Frustum[BOTTOM_SIDE_OF_FRUSTUM][A_COEFFICIENT] = clip[ 3] + clip[ 1];
		m_Frustum[BOTTOM_SIDE_OF_FRUSTUM][B_COEFFICIENT] = clip[ 7] + clip[ 5];
		m_Frustum[BOTTOM_SIDE_OF_FRUSTUM][C_COEFFICIENT] = clip[11] + clip[ 9];
		m_Frustum[BOTTOM_SIDE_OF_FRUSTUM][D_COEFFICIENT] = clip[15] + clip[13];

		// Normalize the BOTTOM side
		NormalizePlane(m_Frustum, BOTTOM_SIDE_OF_FRUSTUM);

		// This will extract the TOP side of the frustum
		m_Frustum[TOP_SIDE_OF_FRUSTUM][A_COEFFICIENT] = clip[ 3] - clip[ 1];
		m_Frustum[TOP_SIDE_OF_FRUSTUM][B_COEFFICIENT] = clip[ 7] - clip[ 5];
		m_Frustum[TOP_SIDE_OF_FRUSTUM][C_COEFFICIENT] = clip[11] - clip[ 9];
		m_Frustum[TOP_SIDE_OF_FRUSTUM][D_COEFFICIENT] = clip[15] - clip[13];

		// Normalize the TOP side
		NormalizePlane(m_Frustum, TOP_SIDE_OF_FRUSTUM);

		// This will extract the BACK side of the frustum
		m_Frustum[BACK_SIDE_OF_FRUSTUM][A_COEFFICIENT] = clip[ 3] - clip[ 2];
		m_Frustum[BACK_SIDE_OF_FRUSTUM][B_COEFFICIENT] = clip[ 7] - clip[ 6];
		m_Frustum[BACK_SIDE_OF_FRUSTUM][C_COEFFICIENT] = clip[11] - clip[10];
		m_Frustum[BACK_SIDE_OF_FRUSTUM][D_COEFFICIENT] = clip[15] - clip[14];

		// Normalize the BACK side
		NormalizePlane(m_Frustum, BACK_SIDE_OF_FRUSTUM);

		// This will extract the FRONT side of the frustum
		m_Frustum[FRONT_SIDE_OF_FRUSTUM][A_COEFFICIENT] = clip[ 3] + clip[ 2];
		m_Frustum[FRONT_SIDE_OF_FRUSTUM][B_COEFFICIENT] = clip[ 7] + clip[ 6];
		m_Frustum[FRONT_SIDE_OF_FRUSTUM][C_COEFFICIENT] = clip[11] + clip[10];
		m_Frustum[FRONT_SIDE_OF_FRUSTUM][D_COEFFICIENT] = clip[15] + clip[14];

		// Normalize the FRONT side
		NormalizePlane(m_Frustum, FRONT_SIDE_OF_FRUSTUM);
	}

	/*
	==============================------------------=======================

	This determines if a point is inside of the frustum

	==============================------------------=======================
	*/

	bool Frustum::PointInFrustum( float x, float y, float z )
	{
		// Go through all the sides of the frustum
		for(int i = 0; i < 6; i++ )
		{
			// Calculate the plane equation and check if the point is behind a side of the frustum
			if(m_Frustum[i][A_COEFFICIENT] * x + m_Frustum[i][B_COEFFICIENT] * y + m_Frustum[i][C_COEFFICIENT] * z + m_Frustum[i][D_COEFFICIENT] <= 0)
			{
				// The point was behind a side, so it ISN'T in the frustum
				return false;
			}
		}

		// The point was inside of the frustum (In front of ALL the sides of the frustum)
		return true;
	}

	/*
	==============================------------------=======================

	This determines if a point is inside of the frustum for an infinite frustum

	==============================------------------=======================
	*/

	bool Frustum::PointInFrustumInfinite( float x, float y, float z )
	{
		if(m_Frustum[0][A_COEFFICIENT] * x + m_Frustum[0][B_COEFFICIENT] * y + m_Frustum[0][C_COEFFICIENT] * z + m_Frustum[0][D_COEFFICIENT] <= 0)
		{
				// The point was behind a side, so it ISN'T in the frustum
			return false;
		}

		if(m_Frustum[1][A_COEFFICIENT] * x + m_Frustum[1][B_COEFFICIENT] * y + m_Frustum[1][C_COEFFICIENT] * z + m_Frustum[1][D_COEFFICIENT] <= 0)
		{
				// The point was behind a side, so it ISN'T in the frustum
			return false;
		}

		if(m_Frustum[2][A_COEFFICIENT] * x + m_Frustum[2][B_COEFFICIENT] * y + m_Frustum[2][C_COEFFICIENT] * z + m_Frustum[2][D_COEFFICIENT] <= 0)
		{
				// The point was behind a side, so it ISN'T in the frustum
			return false;
		}

		if(m_Frustum[3][A_COEFFICIENT] * x + m_Frustum[3][B_COEFFICIENT] * y + m_Frustum[3][C_COEFFICIENT] * z + m_Frustum[3][D_COEFFICIENT] <= 0)
		{
				// The point was behind a side, so it ISN'T in the frustum
			return false;
		}

		if(m_Frustum[5][A_COEFFICIENT] * x + m_Frustum[5][B_COEFFICIENT] * y + m_Frustum[5][C_COEFFICIENT] * z + m_Frustum[5][D_COEFFICIENT] <= 0)
		{
				// The point was behind a side, so it ISN'T in the frustum
			return false;
		}

		// The point was inside of the frustum (In front of ALL the sides of the frustum)
		return true;
	}

	/*
	==============================------------------=======================

	This determines if a sphere is inside of our frustum by it's center and radius.

	==============================------------------=======================
	*/

	bool Frustum::SphereInFrustum( float x, float y, float z, float radius )
	{
		// Go through all the sides of the frustum
		for(int i = 0; i < 6; i++ )	
		{
			// If the center of the sphere is farther away from the plane than the radius
			if( m_Frustum[i][A_COEFFICIENT] * x + m_Frustum[i][B_COEFFICIENT] * y + m_Frustum[i][C_COEFFICIENT] * z + m_Frustum[i][D_COEFFICIENT] <= -radius )
			{
				// The distance was greater than the radius so the sphere is outside of the frustum
				return false;
			}
		}
		
		// The sphere was inside of the frustum!
		return true;
	}


	/*
	==============================------------------=======================

	This determines if a cube is in or around our frustum by it's center and 1/2 it's length

	==============================------------------=======================
	*/

	bool Frustum::CubeInFrustum( float x, float y, float z, float size )
	{
		for(int i = 0; i < 6; i++ )
		{
			if(m_Frustum[i][A_COEFFICIENT] * (x - size) + m_Frustum[i][B_COEFFICIENT] * (y - size) + m_Frustum[i][C_COEFFICIENT] * (z - size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + size) + m_Frustum[i][B_COEFFICIENT] * (y - size) + m_Frustum[i][C_COEFFICIENT] * (z - size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x - size) + m_Frustum[i][B_COEFFICIENT] * (y + size) + m_Frustum[i][C_COEFFICIENT] * (z - size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + size) + m_Frustum[i][B_COEFFICIENT] * (y + size) + m_Frustum[i][C_COEFFICIENT] * (z - size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x - size) + m_Frustum[i][B_COEFFICIENT] * (y - size) + m_Frustum[i][C_COEFFICIENT] * (z + size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + size) + m_Frustum[i][B_COEFFICIENT] * (y - size) + m_Frustum[i][C_COEFFICIENT] * (z + size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x - size) + m_Frustum[i][B_COEFFICIENT] * (y + size) + m_Frustum[i][C_COEFFICIENT] * (z + size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + size) + m_Frustum[i][B_COEFFICIENT] * (y + size) + m_Frustum[i][C_COEFFICIENT] * (z + size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;

			// If we get here, it isn't in the frustum
			return false;
		}

		return true;
	}

	bool Frustum::CuboidInFrustum( float x, float z, float size, float h)
	{
		float y = 0, sizeY = h;

		for(int i = 0; i < 6; i++ )
		{
			if(m_Frustum[i][A_COEFFICIENT] * (x - size) + m_Frustum[i][B_COEFFICIENT] * (y) + m_Frustum[i][C_COEFFICIENT] * (z - size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + size) + m_Frustum[i][B_COEFFICIENT] * (y ) + m_Frustum[i][C_COEFFICIENT] * (z - size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x - size) + m_Frustum[i][B_COEFFICIENT] * (y + sizeY) + m_Frustum[i][C_COEFFICIENT] * (z - size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + size) + m_Frustum[i][B_COEFFICIENT] * (y + sizeY) + m_Frustum[i][C_COEFFICIENT] * (z - size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x - size) + m_Frustum[i][B_COEFFICIENT] * (y) + m_Frustum[i][C_COEFFICIENT] * (z + size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + size) + m_Frustum[i][B_COEFFICIENT] * (y) + m_Frustum[i][C_COEFFICIENT] * (z + size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x - size) + m_Frustum[i][B_COEFFICIENT] * (y + sizeY) + m_Frustum[i][C_COEFFICIENT] * (z + size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + size) + m_Frustum[i][B_COEFFICIENT] * (y + sizeY) + m_Frustum[i][C_COEFFICIENT] * (z + size) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;

			// If we get here, it isn't in the frustum
			return false;
		}

		return true;
	}

	bool Frustum::AACuboidInFrustum( float x, float y, float z, const Math::Vector3 &mins, const Math::Vector3 &maxs)
	{
		/*
		for(int i = 0; i < 6; i++ )
		{
			if(m_Frustum[i][A] * (x - mins.x) + m_Frustum[i][B] * (y - mins.y) + m_Frustum[i][C] * (z - mins.z) + m_Frustum[i][D] > 0)
			   continue;
			if(m_Frustum[i][A] * (x + maxs.x) + m_Frustum[i][B] * (y - mins.y) + m_Frustum[i][C] * (z - mins.z) + m_Frustum[i][D] > 0)
			   continue;
			if(m_Frustum[i][A] * (x - mins.x) + m_Frustum[i][B] * (y + maxs.y) + m_Frustum[i][C] * (z - mins.z) + m_Frustum[i][D] > 0)
			   continue;
			if(m_Frustum[i][A] * (x + maxs.x) + m_Frustum[i][B] * (y + maxs.y) + m_Frustum[i][C] * (z - mins.z) + m_Frustum[i][D] > 0)
			   continue;
			if(m_Frustum[i][A] * (x - mins.x) + m_Frustum[i][B] * (y - mins.y) + m_Frustum[i][C] * (z + maxs.z) + m_Frustum[i][D] > 0)
			   continue;
			if(m_Frustum[i][A] * (x + maxs.x) + m_Frustum[i][B] * (y - mins.y) + m_Frustum[i][C] * (z + maxs.z) + m_Frustum[i][D] > 0)
			   continue;
			if(m_Frustum[i][A] * (x - mins.x) + m_Frustum[i][B] * (y + maxs.y) + m_Frustum[i][C] * (z + maxs.z) + m_Frustum[i][D] > 0)
			   continue;
			if(m_Frustum[i][A] * (x + maxs.x) + m_Frustum[i][B] * (y + maxs.y) + m_Frustum[i][C] * (z + maxs.z) + m_Frustum[i][D] > 0)
			   continue;

			// If we get here, it isn't in the frustum
			return false;
		}
		*/

		for(int i = 0; i < 6; i++ )
		{
			if(m_Frustum[i][A_COEFFICIENT] * (x - mins.x) + m_Frustum[i][B_COEFFICIENT] * (y - mins.y) + m_Frustum[i][C_COEFFICIENT] * (z - mins.z) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + maxs.x) + m_Frustum[i][B_COEFFICIENT] * (y - mins.y) + m_Frustum[i][C_COEFFICIENT] * (z - mins.z) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x - mins.x) + m_Frustum[i][B_COEFFICIENT] * (y + maxs.y) + m_Frustum[i][C_COEFFICIENT] * (z - mins.z) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + maxs.x) + m_Frustum[i][B_COEFFICIENT] * (y + maxs.y) + m_Frustum[i][C_COEFFICIENT] * (z - mins.z) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x - mins.x) + m_Frustum[i][B_COEFFICIENT] * (y - mins.y) + m_Frustum[i][C_COEFFICIENT] * (z + maxs.z) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + maxs.x) + m_Frustum[i][B_COEFFICIENT] * (y - mins.y) + m_Frustum[i][C_COEFFICIENT] * (z + maxs.z) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x - mins.x) + m_Frustum[i][B_COEFFICIENT] * (y + maxs.y) + m_Frustum[i][C_COEFFICIENT] * (z + maxs.z) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;
			if(m_Frustum[i][A_COEFFICIENT] * (x + maxs.x) + m_Frustum[i][B_COEFFICIENT] * (y + maxs.y) + m_Frustum[i][C_COEFFICIENT] * (z + maxs.z) + m_Frustum[i][D_COEFFICIENT] > 0)
			   continue;

			// If we get here, it isn't in the frustum
			return false;
		}

		return true;
	}

	__forceinline Math::Vector3 Frustum::GetIntersectionPoint(int plane1, int plane2, int plane3)
	{
		Math::Vector3 n; //Numerator of the above equation
		float d; //Denominator of the above equation

		Math::Vector3 n1, n2, n3;
		float d1, d2, d3;

		Math::Vector3 n2crossn3;
		Math::Vector3 n3crossn1;
		Math::Vector3 n1crossn2;

		n1 = Math::Vector3(m_Frustum[plane1][A_COEFFICIENT],
				  m_Frustum[plane1][B_COEFFICIENT],
				  m_Frustum[plane1][C_COEFFICIENT]);

		d1 = m_Frustum[plane1][D_COEFFICIENT];

		n2 = Math::Vector3(m_Frustum[plane2][A_COEFFICIENT],
				  m_Frustum[plane2][B_COEFFICIENT],
				  m_Frustum[plane2][C_COEFFICIENT]);

		d2 = m_Frustum[plane2][D_COEFFICIENT];

		n3 = Math::Vector3(m_Frustum[plane3][A_COEFFICIENT],
				  m_Frustum[plane3][B_COEFFICIENT],
				  m_Frustum[plane3][C_COEFFICIENT]);

		d3 = m_Frustum[plane3][D_COEFFICIENT];

		n2crossn3 = n2.Cross(n3);
		n3crossn1 = n3.Cross(n1); 
		n1crossn2 = n1.Cross(n2);
		
		n = (n2crossn3) * d1 + (n3crossn1) * d2 + (n1crossn2) * d3;
		d = n1.Dot(n2crossn3);

		Math::Vector3 ptOfIntersection = n/d; //No divide by zero check, frustum planes are never ||

		return ptOfIntersection;
	}

	/*
	==============================------------------=======================

	This determines if a frustum intersected a frustum


					d1 ( N2 * N3 ) + d2 ( N3 * N1 ) + d3 ( N1 * N2 )
	P = 	-------------------------------------------------------------------------
  							N1 . ( N2 * N3 )

	==============================------------------=======================
	*/
	bool Frustum::FrustumInFrustum(Frustum &f)
	{
		//POINTS
		//FRONT PLANE
		//Top left front point
		firstFrustumCorners[0] = f.GetIntersectionPoint(LEFT_SIDE_OF_FRUSTUM,
												 TOP_SIDE_OF_FRUSTUM,
												 FRONT_SIDE_OF_FRUSTUM);

		//Top right front point
		firstFrustumCorners[1] = f.GetIntersectionPoint(RIGHT_SIDE_OF_FRUSTUM,
												 TOP_SIDE_OF_FRUSTUM,
												 FRONT_SIDE_OF_FRUSTUM);

		//Bottom left front point
		firstFrustumCorners[2] = f.GetIntersectionPoint(LEFT_SIDE_OF_FRUSTUM,
													BOTTOM_SIDE_OF_FRUSTUM,
													FRONT_SIDE_OF_FRUSTUM);

		//Bottom right front point
		firstFrustumCorners[3] = f.GetIntersectionPoint(RIGHT_SIDE_OF_FRUSTUM,
													 BOTTOM_SIDE_OF_FRUSTUM,
													 FRONT_SIDE_OF_FRUSTUM);

		//BACK PLANE
		//Top left back point
		firstFrustumCorners[4] = f.GetIntersectionPoint(LEFT_SIDE_OF_FRUSTUM,
												 TOP_SIDE_OF_FRUSTUM,
												 BACK_SIDE_OF_FRUSTUM);

		//Top right back point
		firstFrustumCorners[5] = f.GetIntersectionPoint(RIGHT_SIDE_OF_FRUSTUM,
												 TOP_SIDE_OF_FRUSTUM,
												 BACK_SIDE_OF_FRUSTUM);

		//Bottom left back point
		firstFrustumCorners[6] = f.GetIntersectionPoint(LEFT_SIDE_OF_FRUSTUM,
													BOTTOM_SIDE_OF_FRUSTUM,
													BACK_SIDE_OF_FRUSTUM);

		//Bottom right back point
		firstFrustumCorners[7] = f.GetIntersectionPoint(RIGHT_SIDE_OF_FRUSTUM,
													 BOTTOM_SIDE_OF_FRUSTUM,
													 BACK_SIDE_OF_FRUSTUM);

		//Check if any corner pts are in this frustum
		for(int  i = 0; i < 8; i++)
		{
			if(this->PointInFrustum(firstFrustumCorners[i].x,
									firstFrustumCorners[i].y,
									firstFrustumCorners[i].z))
			{
				return true;
			}
		}

		//EDGES

		//SET 0
		//Front Left edge
		this->edge[0][0] = firstFrustumCorners[0];
		this->edge[0][1] = firstFrustumCorners[2];

		//Front right edge
		this->edge[1][0] = firstFrustumCorners[1];
		this->edge[1][1] = firstFrustumCorners[3];

		//Back Left edge
		this->edge[2][0] = firstFrustumCorners[4];
		this->edge[2][1] = firstFrustumCorners[6];

		//Back right edge
		this->edge[3][0] = firstFrustumCorners[5];
		this->edge[3][1] = firstFrustumCorners[7];

		//SET 1
		this->edge[4][0] = firstFrustumCorners[0];
		this->edge[4][1] = firstFrustumCorners[4];

		this->edge[5][0] = firstFrustumCorners[1];
		this->edge[5][1] = firstFrustumCorners[5];

		this->edge[6][0] = firstFrustumCorners[2];
		this->edge[6][1] = firstFrustumCorners[6];

		this->edge[7][0] = firstFrustumCorners[3];
		this->edge[7][1] = firstFrustumCorners[7];

		//SET 2
		this->edge[8][0] = firstFrustumCorners[4];
		this->edge[8][1] = firstFrustumCorners[5];

		this->edge[9][0] = firstFrustumCorners[6];
		this->edge[9][1] = firstFrustumCorners[7];

		this->edge[10][0] = firstFrustumCorners[0];
		this->edge[10][1] = firstFrustumCorners[1];

		this->edge[11][0] = firstFrustumCorners[2];
		this->edge[11][1] = firstFrustumCorners[3];

		//NEXT FRUSTUM POINTS
		//POINTS
		//FRONT PLANE
		//Top left front point
		firstFrustumCorners[0] = GetIntersectionPoint(LEFT_SIDE_OF_FRUSTUM,
												 TOP_SIDE_OF_FRUSTUM,
												 FRONT_SIDE_OF_FRUSTUM);

		//Top right front point
		firstFrustumCorners[1] = GetIntersectionPoint(RIGHT_SIDE_OF_FRUSTUM,
												 TOP_SIDE_OF_FRUSTUM,
												 FRONT_SIDE_OF_FRUSTUM);

		//Bottom left front point
		firstFrustumCorners[2] = GetIntersectionPoint(LEFT_SIDE_OF_FRUSTUM,
													BOTTOM_SIDE_OF_FRUSTUM,
													FRONT_SIDE_OF_FRUSTUM);

		//Bottom right front point
		firstFrustumCorners[3] = GetIntersectionPoint(RIGHT_SIDE_OF_FRUSTUM,
													 BOTTOM_SIDE_OF_FRUSTUM,
													 FRONT_SIDE_OF_FRUSTUM);

		//BACK PLANE
		//Top left back point
		firstFrustumCorners[4] = GetIntersectionPoint(LEFT_SIDE_OF_FRUSTUM,
												 TOP_SIDE_OF_FRUSTUM,
												 BACK_SIDE_OF_FRUSTUM);

		//Top right back point
		firstFrustumCorners[5] = GetIntersectionPoint(RIGHT_SIDE_OF_FRUSTUM,
												 TOP_SIDE_OF_FRUSTUM,
												 BACK_SIDE_OF_FRUSTUM);

		//Bottom left back point
		firstFrustumCorners[6] = GetIntersectionPoint(LEFT_SIDE_OF_FRUSTUM,
													BOTTOM_SIDE_OF_FRUSTUM,
													BACK_SIDE_OF_FRUSTUM);

		//Bottom right back point
		firstFrustumCorners[7] = GetIntersectionPoint(RIGHT_SIDE_OF_FRUSTUM,
													 BOTTOM_SIDE_OF_FRUSTUM,
													 BACK_SIDE_OF_FRUSTUM);

		//POLYGON vs EDGES
		//POLYGON 1 (FRONT)
		tempPolygon[0] = firstFrustumCorners[0];
		tempPolygon[1] = firstFrustumCorners[1];
		tempPolygon[2] = firstFrustumCorners[2];
		tempPolygon[3] = firstFrustumCorners[3];

		for(int i = 0; i < 12; i++)
		{
			if(IntersectedPolygon(tempPolygon, edge[i], 4))
			{
				return true;
			}
		}

		//POLYGON 2 (BACK)
		tempPolygon[0] = firstFrustumCorners[4];
		tempPolygon[1] = firstFrustumCorners[5];
		tempPolygon[2] = firstFrustumCorners[6];
		tempPolygon[3] = firstFrustumCorners[7];

		for(int i = 0; i < 12; i++)
		{
			if(IntersectedPolygon(tempPolygon, edge[i], 4))
			{
				return true;
			}
		}

		//POLYGON 3 (LEFT)
		tempPolygon[0] = firstFrustumCorners[0];
		tempPolygon[1] = firstFrustumCorners[2];
		tempPolygon[2] = firstFrustumCorners[6];
		tempPolygon[3] = firstFrustumCorners[4];

		for(int i = 0; i < 12; i++)
		{
			if(IntersectedPolygon(tempPolygon, edge[i], 4))
			{
				return true;
			}
		}

		//POLYGON 4 (RIGHT)
		tempPolygon[0] = firstFrustumCorners[1];
		tempPolygon[1] = firstFrustumCorners[3];
		tempPolygon[2] = firstFrustumCorners[7];
		tempPolygon[3] = firstFrustumCorners[5];

		for(int i = 0; i < 12; i++)
		{
			if(IntersectedPolygon(tempPolygon, edge[i], 4))
			{
				return true;
			}
		}

		//POLYGON 5 (TOP)
		tempPolygon[0] = firstFrustumCorners[0];
		tempPolygon[1] = firstFrustumCorners[4];
		tempPolygon[2] = firstFrustumCorners[5];
		tempPolygon[3] = firstFrustumCorners[1];

		for(int i = 0; i < 12; i++)
		{
			if(IntersectedPolygon(tempPolygon, edge[i], 4))
			{
				return true;
			}
		}

		//POLYGON 6 (BOTTOM)
		tempPolygon[0] = firstFrustumCorners[2];
		tempPolygon[1] = firstFrustumCorners[6];
		tempPolygon[2] = firstFrustumCorners[7];
		tempPolygon[3] = firstFrustumCorners[3];

		for(int i = 0; i < 12; i++)
		{
			if(IntersectedPolygon(tempPolygon, edge[i], 4))
			{
				return true;
			}
		}

		return false;
	}

	/*
	===========================================================================
		Calculate 8 frustum corners
	===========================================================================
	*/

	void Frustum::CalculateFrustumCorners(Math::Vector3 pPoints[8], Math::Vector3 &vPos,
										  Math::Vector3 &vView, Math::Vector3 &vUp, float fNear,
										  float fFar, float fScale,
										  float fFOV, float fAspect)
	{		
		Math::Vector3 vZ = vView - vPos;
		vZ = vZ.Normalize();

		Math::Vector3 vX;
		vX = vUp.Cross(vZ);
		vX = vX.Normalize();

		Math::Vector3 vY;
		vY = vZ.Cross(vX);		
		  
		float fNearPlaneHeight = Math::tan((fFOV) * Math::piover180 * 0.5f) * fNear;
		float fNearPlaneWidth = fNearPlaneHeight * fAspect;

		float fFarPlaneHeight = Math::tan((fFOV) * Math::piover180 * 0.5f) * fFar;
		float fFarPlaneWidth = fFarPlaneHeight * fAspect;

		Math::Vector3 vNearPlaneCenter = vPos + (vZ * fNear);
		Math::Vector3 vFarPlaneCenter = vPos + (vZ * fFar);

		//vX *= fNearPlaneWidth;
		//vY *= fNearPlaneWidth;

		Math::Vector3 vXNearWidth = vX * fNearPlaneWidth;
		Math::Vector3 vXFarWidth = vX * fFarPlaneWidth;

		Math::Vector3 vYNearHeight = vY * fNearPlaneHeight;
		Math::Vector3 vYFarHeight = vY * fFarPlaneHeight;

		Math::Vector3 xNearMinusYNear = (vXNearWidth) - (vYNearHeight);
		Math::Vector3 xNearPlusYNear = (vXNearWidth) + (vYNearHeight);

		Math::Vector3 xFarMinusYFar = (vXFarWidth) - (vYFarHeight);
		Math::Vector3 xFarPlusYFar = (vXFarWidth) + (vYFarHeight);

		pPoints[0] = (vNearPlaneCenter - xNearMinusYNear);
		pPoints[1] = (vNearPlaneCenter - xNearPlusYNear);
		pPoints[2] = (vNearPlaneCenter + xNearPlusYNear);
		pPoints[3] = (vNearPlaneCenter + xNearMinusYNear);

		pPoints[4] = (vFarPlaneCenter - xFarMinusYFar);
		pPoints[5] = (vFarPlaneCenter - xFarPlusYFar);
		pPoints[6] = (vFarPlaneCenter + xFarPlusYFar);
		pPoints[7] = (vFarPlaneCenter + xFarMinusYFar);

		Math::Vector3 vCenter;//(0.0f, 0.0f, 0.0f);
		for(int i = 0; i < 8; i++) 
			vCenter = vCenter + pPoints[i];

		vCenter /= 8.0f;

		for(int i = 0; i < 8; i++) 
			pPoints[i] = pPoints[i] + ((pPoints[i]-vCenter)*(fScale-1.0f));
	}

}