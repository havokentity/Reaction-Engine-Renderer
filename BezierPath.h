/*
	Description: Bezier path and bezier curve motion objects

	classes contained are:

	class BezierCurve,

	class AbstractCurveHead,
				|
				--------> class AbstractBezierPathParentStatic
				|								|
				|								-------> class AbstractBezierPathStatic,
				|													|
				|													-------> class BezierPathStatic,
				|			
				|
				|
				|
				--------> class AbstractBezierPathParentDynamic
									|					
									-------> class AbstractBezierPathDynamic
														|
														---------> class BezierPathDynamic

============================================================================================================
	WARNING: Use only CurveEditorUI, BezierPathDynamic,
			 BezierPathStatic classes and if you want BezierCurve
============================================================================================================
*/


#ifndef RX_BEZIER_PATH_H_
#define RX_BEZIER_PATH_H_

#include "CoreEngine.h"
#include "utils/AlignedObjectArray.h"
#include "utils/3DMATH.H"
//#include <vector>
#include "Client.h"

namespace DifferentialArts
{
	/*
	================================================
		class BezierCurve
	================================================
	*/
	/*!	\class BezierCurve
		\brief Bezier curve class
		\author Rajesh Peter Douglas D'Monte	

		Basic bezier curve class. Used internally.
	*/
	class RX_API BezierCurve
	{
	public:
		BezierCurve(void);//!<Constructor
		BezierCurve(const Math::Vector3 &a, const Math::Vector3 &b = Math::Vector3(0, 0, 0), const Math::Vector3 &c = Math::Vector3(0, 0, 0), const Math::Vector3 &d = Math::Vector3(0, 0, 0));//!<Constructor with 4 control points
		~BezierCurve(void);//!< Destructor

		Math::Vector3 pointOnCurve(float t);//!< Returns a point on the curve

		Math::Vector3 p1; //!< First control point for the bezier curve
		Math::Vector3 p2; //!< Second control point for the bezier curve
		Math::Vector3 p3; //!< Third control point for the bezier curve
		Math::Vector3 p4; //!< Fourth control point for the bezier curve

		void debugDrawCurve(float resolution = 0.04f, int secondaryColor = 0, int currentPoint = 0); //!< For debuggin purposes. Max 1, min approx 0
	};

	/*
	================================================
		class AbstractCurveHead
	================================================
	*/

	/*!	\class AbstractCurveHead
		\brief Curve set
		\author Rajesh Peter Douglas D'Monte	

		Basic bezier curve set with no curve information. Used internally.
	*/
	class RX_API AbstractCurveHead : public BaseMemoryInterface
	{
	public:
		void SetHead(float t); //!< Sets the head of the curve play time

	protected:
		float time;
		float maxTime;
		GLuint curves;
	};

	/*
	================================================
		class AbstractBezierPathParentStatic
	================================================
	*/

	/*!	\class AbstractBezierPathParentStatic
		\brief Static bezier curve path class
		\author Rajesh Peter Douglas D'Monte	

		Static bezier curve path with a static list of curves. Used internally.
	*/
	class RX_API AbstractBezierPathParentStatic: public AbstractCurveHead
	{
	public:
		BezierCurve* curveList; //!< Bezier curve list
	};

	/*
	================================================
		class AbstractBezierPathParentDynamic
	================================================
	*/
	/*!	\class AbstractBezierPathParentDynamic
		\brief Dynamic bezier curve path class
		\author Rajesh Peter Douglas D'Monte	

		Dynamic bezier curve path with a Dynamic vector of curves. Used internally.
	*/
	class RX_API AbstractBezierPathParentDynamic: public AbstractCurveHead
	{
	protected:
		AlignedObjectArray<BezierCurve> curveList;//!< Bezier curve vector. Unaccessible.
	};

	/*
	================================================
		class AbstractBezierPathStatic
	================================================
	*/
	/*!	\class AbstractBezierPathStatic
		\brief Static bezier curve path class
		\author Rajesh Peter Douglas D'Monte	

		Static bezier curve path class with setable init properties. Used internally.
	*/
	class RX_API AbstractBezierPathStatic: public AbstractBezierPathParentStatic
	{
	public:
		bool Initialize(GLuint bcurves); //!< Initialize the curve path
		void Free(void);//!< Frees resources allocated to the curve path
	};

	/*
	================================================
		class AbstractBezierPathDynamic
	================================================
	*/
	/*!	\class AbstractBezierPathDynamic
		\brief Dynamic bezier curve path class
		\author Rajesh Peter Douglas D'Monte	

		Dynamic bezier curve path class with ability to add and remove curves. Used internally.
	*/
	class RX_API AbstractBezierPathDynamic: public AbstractBezierPathParentDynamic
	{
	public:
		int AddCurve(const Math::Vector3 &a, const Math::Vector3 &b, const Math::Vector3 &c, const Math::Vector3 &d); //!< Add curves
		int RemoveCurve(int i = -1); //!< Remove curves
	};

	/*
	================================================
		class BezierPathStatic
	================================================
	*/
	/*!	\class BezierPathStatic
		\brief Static bezier curve path FINAL class
		\author Rajesh Peter Douglas D'Monte	

		Static bezier curve path class with complete user control to play curves, load
		and save curve set data etc.
	*/
	class RX_API BezierPathStatic: public AbstractBezierPathStatic
	{
	public:
		BezierPathStatic(void);//!<Constructor
		~BezierPathStatic(void);	//!< Destructor
		
		void debugDraw(float resolution = 0.04f, int currentCurve = -1, int currentPoint = 0);
		//!< For debugging purposes. Draws the curve.
		Math::Vector3 PlayNext(float dt); //!< Play the curve by an interval dt
		Math::Vector3 PlayTillEnd(float dt);	 //!< Play the curve till the end by an interval dt

		bool Load(const char* file); //!< Loads a curve set
		bool Save(const char* file); //!< Saves a curve set
	};

	/*
	================================================
		class BezierPathDynamic
	================================================
	*/
	/*!	\class BezierPathDynamic
		\brief Dynamic bezier curve path FINAL class
		\author Rajesh Peter Douglas D'Monte	

		Dynamic bezier curve path class with complete user control to play curves, load
		and save curve set data etc.
	*/
	class RX_API BezierPathDynamic: public AbstractBezierPathDynamic
	{
	public:
		BezierPathDynamic(void);//!<Constructor
		~BezierPathDynamic(void);	//!< Destructor	
		
		void debugDraw(float resolution = 0.04f, int currentCurve = -1, int currentPoint = 0);
		//!< For debugging purposes. Draws the curve.
		Math::Vector3 PlayNext(float dt); //!< Play the curve by an interval dt
		Math::Vector3 PlayTillEnd(float dt);	//!< Play the curve till the end by an interval dt

		bool Load(const char* file); //!< Loads a curve set
		bool Save(const char* file); //!< Saves a curve set

		friend class CurveEditorUI;
	};
}


#endif