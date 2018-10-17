/*
	Please refer 3DMATH.H for this source files "notes"

	Quaternion class from nehe's quarternion tutorial and other maths books

*/

#include "3DMATH.h"

namespace DifferentialArts
{
	namespace Math
	{
		/*
		===============================================================
			class Vector3
		===============================================================
		*/		

		/*
		RX_UTIL_API inline Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
		{
#ifdef SSE
			Vector3 returnVector;

		
			__asm
			{       
					// Load pointers into CPU regs
					mov			eax, v1                      
					mov			ebx, v2

					movups		xmm0, [eax]                 // Move unaligned vectors to SSE regs
					movups		xmm1, [ebx]

					addps		xmm0, xmm1                   // Add vector elements
					movups		[returnVector], xmm0      // Save the return vector
			}

			return Vector3(_mm_add_ps(v1.m128, v2.m128));
#else
			return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
#endif
		}

		RX_UTIL_API inline Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
		{
#ifdef SSE2
			Vector3 returnVector;

			__asm
			{       
					// Load pointers into CPU regs
					mov			eax, this                      
					mov			ebx, v

					movups		xmm0, [eax]                 // Move unaligned vectors to SSE regs
					movups		xmm1, ebx

					subps		xmm0, xmm1                   // Add vector elements
					movups		[returnVector], xmm0      // Save the return vector
			}

			return returnVector;
#else
			return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
#endif
		}

		RX_UTIL_API inline Vector3 operator*(const Vector3 &v, const float s)
		{
#ifdef SSE2
			Vector3 returnVector;	

			__m128 scalar = _mm_set1_ps(s);

			__asm
			{
				mov			eax, this
				movups		xmm0, [eax]
				mulps		xmm0, scalar
				movups		[returnVector],	xmm0
			}

			return returnVector;
#else
			return Vector3(v.x * s, v.y * s, v.z * s);
#endif
		}		
*/	

		/*////////////////////////////////////////////////////////////////////////////
		//																	        //
		//	This function is used to calculate the inverse magnitude of a vector	//
		//																			//
		*/////////////////////////////////////////////////////////////////////////////

		RX_UTIL_API __forceinline float iMagnitude_f3(const float &x, const float &y, const float &z)
		{
			float invmag = rsqrt((x*x)+ 
								 (y*y)+
								 (z*z));

			return invmag;
		}

		/*////////////////////////////////////////////////////////////////////
		//																	//
		//	This function is used to calculate the Normal(unit) of a vector	//
		//																	//
		*/////////////////////////////////////////////////////////////////////

		RX_UTIL_API void __forceinline Normalize_f3(float &x, float &y, float &z)
		{	
			float invmag = rsqrt((x*x)+ 
								 (y*y)+
								 (z*z));

			/*
			if(invmag <= 0)
			{
				return Vector3(0, 0, 0);
			}
			*/
			
			x *= invmag;
			y *= invmag;
			z *= invmag;;
		}		

		/*////////////////////////////////////////////////////////////////////
		//																	//
		//	This function calculate's the Normal of a polygon(triangle)	    //
		//																	//
		*/////////////////////////////////////////////////////////////////////

		RX_UTIL_API __forceinline Vector3 Normal(Vector3 vPoly[])
		{
			Vector3 vector1 = vPoly[2] - vPoly[0];
			Vector3 vector2 = vPoly[1] - vPoly[0];

			Vector3 vNormal = vector1.Cross(vector2);

			return vNormal.Normalize();
		}				

		/*////////////////////////////////////////////////////////////////////
		//																	//
		//	This function calculate's a plane distance from origin    	    //
		//																	//
		*/////////////////////////////////////////////////////////////////////

		RX_UTIL_API __forceinline float PlaneDistance(const Vector3 &vNormal, const Vector3 &vPoint)
		{
			float distance = 0;

			distance = -((vNormal.x * vPoint.x) + (vNormal.y * vPoint.y) + (vNormal.z * vPoint.z));

			return distance;
		}

		/*////////////////////////////////////////////////////////////////////
		//																	//
		//	This function calculate's if a line collided with a plane	    //
		//																	//
		*/////////////////////////////////////////////////////////////////////

		RX_UTIL_API __forceinline bool IntersectedPlane(Vector3 vTriangle[], Vector3 vLine[], Vector3 &vNormal, float &originDis)
		{
			float d1 = 0, d2  = 0;

			vNormal = Normal(vTriangle);

			originDis = PlaneDistance(vNormal, vTriangle[0]);

			d1 = ((vNormal.x * vLine[0].x) + 
				  (vNormal.y * vLine[0].y) +
				  (vNormal.z * vLine[0].z) + originDis);
			
			d2 = ((vNormal.x * vLine[1].x) + 
				  (vNormal.y * vLine[1].y) +
				  (vNormal.z * vLine[1].z) + originDis);

			if(d1 * d2 >= 0)
				return false;

			return true;
		}

		/*/////////////////////////////////////////////////////////////////////////
		//																		 //
		//	Returns the point of intersection of a line that intersects a  plane //
		//																		 //
		*//////////////////////////////////////////////////////////////////////////

		RX_UTIL_API __forceinline Vector3 IntersectionPoint(const Vector3 &vNormal, Vector3 vLine[], float distance)
		{
			Vector3 vLineDir, vPoint;

			double numerator, denominator, dist = 0.0;

			vLineDir = vLine[1] - vLine[0];
			vLineDir = vLineDir.Normalize();

			numerator = -(vNormal.x * vLine[0].x +
						  vNormal.y * vLine[0].y +
						  vNormal.z * vLine[0].z + distance);

			denominator = vNormal.Dot(vLineDir);

			if(denominator == 0.0)
				return vLine[0];

			dist = numerator/denominator;

			vPoint.x = (float)(vLine[0].x + (vLineDir.x * dist));
			vPoint.y = (float)(vLine[0].y + (vLineDir.y * dist));
			vPoint.z = (float)(vLine[0].z + (vLineDir.z * dist));

			return vPoint;
		}

		/*/////////////////////////////////////////////////////////////////////////
		//																		 //
		//	Check to see if a point is inside a polygon							 //
		//																		 //
		*//////////////////////////////////////////////////////////////////////////

		RX_UTIL_API __forceinline bool InsidePolygon(const Vector3 &vPointOfIntersection, Vector3 vPoly[], long vertexCount)
		{
			const double ERR_FACTOR = 0.9999;
			double angle = 0.0;
			Vector3 vA, vB;

			for(int i = 0; i < vertexCount; i++)
			{
				vA = vPoly[i] - vPointOfIntersection;
				vB = vPoly[(i+1) % vertexCount] - vPointOfIntersection;
				angle += AngleBetweenVectors(vA, vB);
			}

			if(angle >= (ERR_FACTOR * (2.0 * M_PI)))
				return true;

			return false;
		}

		/*/////////////////////////////////////////////////////////////////////////
		//																		 //
		//	Check to see if a line intersected a polygon						 //
		//																		 //
		*//////////////////////////////////////////////////////////////////////////

		RX_UTIL_API __forceinline bool IntersectedPolygon(Vector3 vPoly[], Vector3 vLine[], int vertexCount)
		{
			Vector3 vNormal;
			float originDistance = 0.0f;

			if(!IntersectedPlane(vPoly, vLine, vNormal, originDistance))
				return false;

			Vector3 vIntersectPoint = IntersectionPoint(vNormal, vLine, originDistance);

			if(InsidePolygon(vIntersectPoint, vPoly, vertexCount))
				return true;

			return false;
		}

		/*/////////////////////////////////////////////////////////////////////////
		//																		 //
		//	For rotation a vector about an arbitrary axis						 //
		//																		 //
		*//////////////////////////////////////////////////////////////////////////

		RX_UTIL_API void RotateVector(Vector3* v, float angle, float x, float y, float z)
		{
			Vector3 vView, vNewView;

			vView = *v;

			float sinTheta = sin(angle);
			float cosTheta = cos(angle);

			vNewView.x =  (cosTheta + (1 - cosTheta) * x * x)     * vView.x;
			vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta) * vView.y;
			vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta) * vView.z;

			vNewView.y =  ((1 - cosTheta) * x * y + z * sinTheta) * vView.x;
			vNewView.y += (cosTheta + (1 - cosTheta) * y * y)     * vView.y;
			vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta) * vView.z;

			vNewView.z =  ((1 - cosTheta) * x * z - y * sinTheta) * vView.x;
			vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta) * vView.y;
			vNewView.z += (cosTheta + (1 - cosTheta) * z * z)     * vView.z;	

			*v = vNewView;
		}

		/*
		====================================
			Quaternion CLASS OBJECT
		====================================
		*/

		Quaternion::Quaternion()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 0;
		}

		Quaternion::Quaternion(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		void Quaternion::CreateFromAxisAngle(float vx, float vy, float vz, float radians)
		{
			float sinThetaByTwo = sin( radians / 2.0f );
			this->w = cos( radians / 2.0f );

			// Calculate the x, y and z of the Quaternion
			this->x = vx * sinThetaByTwo;
			this->y = vy * sinThetaByTwo;
			this->z = vz * sinThetaByTwo;
		}

		void Quaternion::returnMatrix(float *pMatrix)
		{
			if(!pMatrix)
			{
				return;
			}
			
			pMatrix[0] = 1.0f - 2.0f * ( y * y + z * z );
			pMatrix[1] = 2.0f * (x * y + z * w);
			pMatrix[2] = 2.0f * (x * z - y * w);
			pMatrix[3] = 0.0f;
			
			pMatrix[4] = 2.0f * ( x * y - z * w );
			pMatrix[5] = 1.0f - 2.0f * ( x * x + z * z );
			pMatrix[6] = 2.0f * (z * y + x * w );
			pMatrix[7] = 0.0f;

			pMatrix[8] = 2.0f * ( x * z + y * w );
			pMatrix[9] = 2.0f * ( y * z - x * w );
			pMatrix[10] = 1.0f - 2.0f * ( x * x + y * y );
			pMatrix[11] = 0.0f;

			pMatrix[12] = 0;
			pMatrix[13] = 0;
			pMatrix[14] = 0;
			pMatrix[15] = 1.0f;
		}

		void Quaternion::createQuaternionFromMatrix44(float* pMatrix)
		{
			if(!pMatrix)
			{
				return;
			}

			float matrixTrace = pMatrix[0] + pMatrix[5] + pMatrix[10] + 1;
			float temp;

			if(matrixTrace > 0.00000001f)
			{
				temp = 0.5f * rsqrt(matrixTrace);

				this->w = 0.25f/temp;
				this->x = (pMatrix[9] - pMatrix[6]) * temp;
				this->y = (pMatrix[2] - pMatrix[8]) * temp;
				this->z = (pMatrix[4] - pMatrix[1]) * temp;
			} else {
				if(pMatrix[0] > pMatrix[5] && pMatrix[0] > pMatrix[10])
				{
					temp = 0.5f * rsqrt(1.0f + pMatrix[0] - pMatrix[5] - pMatrix[10]);

					this->x = 0.25f / temp;
					this->y = (pMatrix[4] + pMatrix[1]) * temp;
					this->z = (pMatrix[2] + pMatrix[8]) * temp;
					this->w = (pMatrix[9] - pMatrix[6]) * temp;	
				}else if ( pMatrix[5] > pMatrix[10] ) 
				{
					temp  = 0.5f * rsqrt(1.0f + pMatrix[5] - pMatrix[0] - pMatrix[10]);

					this->x = (pMatrix[4] + pMatrix[1]) * temp;
					this->y = 0.25f / temp;
					this->z = (pMatrix[9] + pMatrix[6]) * temp;
					this->w = (pMatrix[2] - pMatrix[8]) * temp;
				} else {	
					temp  = 0.5f * rsqrt( 1.0f + pMatrix[10] - pMatrix[0] - pMatrix[5] );

					this->x = (pMatrix[2] + pMatrix[8] ) * temp;
					this->y = (pMatrix[9] + pMatrix[6] ) * temp;
					this->z = 0.25f / temp;
					this->w = (pMatrix[4] - pMatrix[1] ) * temp;
				}
			}
		}

		void Quaternion::createQuaternionFromMatrix33(float* pMatrix)
		{
			if(!pMatrix)
			{
				return;
			}

			float matrixTrace = pMatrix[0] + pMatrix[4] + pMatrix[8] + 1;
			float temp;

			if(matrixTrace > 0.00000001f)
			{
				temp = 0.5f * rsqrt(matrixTrace);

				this->w = 0.25f/temp;
				this->x = (pMatrix[7] - pMatrix[5]) * temp;
				this->y = (pMatrix[2] - pMatrix[6]) * temp;
				this->z = (pMatrix[3] - pMatrix[1]) * temp;
			} else {
				if(pMatrix[0] > pMatrix[4] && pMatrix[0] > pMatrix[8])
				{
					temp = 0.5f * rsqrt(1.0f + pMatrix[0] - pMatrix[4] - pMatrix[8]);

					this->x = 0.25f / temp;
					this->y = (pMatrix[3] + pMatrix[1]) * temp;
					this->z = (pMatrix[2] + pMatrix[6]) * temp;
					this->w = (pMatrix[7] - pMatrix[5]) * temp;	
				}else if ( pMatrix[4] > pMatrix[8] ) 
				{
					temp  = 0.5f * rsqrt(1.0f + pMatrix[4] - pMatrix[0] - pMatrix[8]);

					this->x = (pMatrix[3] + pMatrix[1]) * temp;
					this->y = 0.25f / temp;
					this->z = (pMatrix[7] + pMatrix[5]) * temp;
					this->w = (pMatrix[2] - pMatrix[6]) * temp;
				} else {	
					temp  = 0.5f * rsqrt( 1.0f + pMatrix[8] - pMatrix[0] - pMatrix[4] );

					this->x = (pMatrix[2] + pMatrix[6] ) * temp;
					this->y = (pMatrix[7] + pMatrix[5] ) * temp;
					this->z = 0.25f / temp;
					this->w = (pMatrix[3] - pMatrix[1] ) * temp;
				}
			}
		}

		Quaternion Quaternion::operator *(Quaternion q)
		{
			Quaternion r;

			r.w = w*q.w - x*q.x - y*q.y - z*q.z;
			r.x = w*q.x + x*q.w + y*q.z - z*q.y;
			r.y = w*q.y + y*q.w + z*q.x - x*q.z;
			r.z = w*q.z + z*q.w + x*q.y - y*q.x;

			return r;
		}

		Quaternion Quaternion::conjugate(void)
		{
			Quaternion ret;

			ret.x = -this->x;
			ret.y = -this->y;
			ret.z = -this->z;
			ret.w = this->w;

			return ret;
		}

		void Quaternion::QuaternionRotateVector(Vector3* v, float radians, float axisX, float axisY, float axisZ)
		{
			float s;
			Quaternion rot, tQuaternionVec, ret;

			rot.w = cos(radians/2);

			s = sin(radians/2);

			rot.x = axisX * s;
			rot.y = axisY * s;
			rot.z = axisZ * s;

			tQuaternionVec.x = v->x;
			tQuaternionVec.y = v->y;
			tQuaternionVec.z = v->z;
			tQuaternionVec.w = 0;

			ret = (rot * tQuaternionVec) * rot.conjugate();

			v->x = ret.x;
			v->y = ret.y;
			v->z = ret.z;

		}					
	}
}