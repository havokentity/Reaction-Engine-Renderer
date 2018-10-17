/*
	Description: Bezier path and bezier curve motion objects

	classes contained are:

	class BezierCurve,

	class CurveEditorUI

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

#include "BezierPath.h"
#include <math.h>

namespace DifferentialArts
{
	/*
	================================================
		class BezierCurve
	================================================
	*/

	BezierCurve::BezierCurve(void)
	{
		this->p1 = Math::Vector3(0, 0, 0);
		this->p2 = Math::Vector3(0, 0, 0);
		this->p3 = Math::Vector3(0, 0, 0);
		this->p4 = Math::Vector3(0, 0, 0);
	}

	BezierCurve::~BezierCurve(void)
	{
	}

	BezierCurve::BezierCurve(const Math::Vector3 &a, const Math::Vector3 &b, const Math::Vector3 &c, const Math::Vector3 &d)
	{
		this->p1 = a;
		this->p2 = b;
		this->p3 = c;
		this->p4 = d;
	}

	Math::Vector3 BezierCurve::pointOnCurve(float t)
	{
		/*
			Bezier curve
			B(t) = P1 * ( 1 - t )^3 + P2 * 3 * t * ( 1 - t )^2 + P3 * 3 * t^2 * ( 1 - t ) + P4 * t^3 
		*/

		Math::Vector3 ret;

		float a = (1-t);
		float e = a * a; // (1-t) ^ 2
		float b = e * a; // (1-t) ^ 3 
		float c = t * t; // t ^ 2
		float d = c * t; // t ^ 3

		ret = p1 * b + p2*3 * t * e + p3*3 * c * a + p4 * d;

		return ret;
	}

	void BezierCurve::debugDrawCurve(float resolution, int secondaryColor, int currentPoint)
	{
		Math::Vector3 temp;

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		int thirdColor = 0;

		glBegin(GL_LINE_STRIP);
			for(float t = 0; t <= 1 + resolution; t += resolution)
			{
				temp = this->pointOnCurve(t);	

				switch(currentPoint)
				{
				case 0:
					if(t < 0.25f)
					{
						thirdColor = 1;
					} else {
						thirdColor = 0;
					}
					break;
				case 1:
					if(t < 0.5f && t > 0.25f)
					{
						thirdColor = 1;
					} else {
						thirdColor = 0;
					}
					break;
				case 2:
					if(t > 0.5f && t < 0.75f)
					{
						thirdColor = 1;
					} else {
						thirdColor = 0;
					}
					break;
				case 3:
					if(t > 0.75f)
					{
						thirdColor = 1;
					} else {
						thirdColor = 0;
					}
					break;
				default:
					thirdColor = 0;
					break;
				}

				glColor3f(1, (float)secondaryColor, (float)thirdColor);
				glVertex3f(temp.x, temp.y, temp.z);
			}
		glEnd();
	}

	/*
	================================================
		class AbstractCurveHead
	================================================
	*/
	void AbstractCurveHead::SetHead(float t)
	{
		this->time = t;
	}

	/*
	================================================
		class AbstractBezierPathStatic
	================================================
	*/

	void AbstractBezierPathStatic::Free(void)
	{
		if(this->curveList)
		{
			delete[] curveList;
			this->curveList = 0;
		}
	}

	bool AbstractBezierPathStatic::Initialize(GLuint bcurves)
	{
		this->curveList = new BezierCurve[bcurves];

		if(!this->curveList)
		{
			myGlobalGame->mlog.LogError(LString("A AbstractBezierPathStatic curve failed to initialize. Insufficient memory!"));
			return false;
		}

		this->curves = bcurves;
		this->maxTime = (float)bcurves;

		return true;
	}

	/*
	================================================
		class AbstractBezierPathDynamic
	================================================
	*/

	int AbstractBezierPathDynamic::AddCurve(const Math::Vector3 &a, const Math::Vector3 &b, const Math::Vector3 &c, const Math::Vector3 &d)
	{
		//this->curveList.AddNode(BezierCurve(a, b, c, d));
		this->curveList.push_back(BezierCurve(a, b, c, d));

		this->curves = this->curveList.size();
		this->maxTime = (float)this->curves;

		return this->curves;
	}

	int AbstractBezierPathDynamic::RemoveCurve(int i)
	{	
		if(curveList.size() <= 0)
		{
			return -666;
		}

		if(i == -1)
		{
			curveList.pop_back();
		}
		else if(i >= 0 && i < (int)curveList.size())
		{
			curveList[i] = curveList[curveList.size() - 1];
			curveList.pop_back();

		}

		this->curves = this->curveList.size();
		this->maxTime = (float)this->curves;

		return this->curves;
	}


	/*
	================================================
		class BezierPathStatic
	================================================
	*/

	BezierPathStatic::BezierPathStatic(void)
	{
		this->curveList = 0;
		this->time = 0;
		this->maxTime = 0;
	}


	BezierPathStatic::~BezierPathStatic(void)
	{
		this->Free();
	}

	Math::Vector3 BezierPathStatic::PlayNext(float dt)
	{
		Math::Vector3 ret;
		int index = int(time);	

		ret = this->curveList[index].pointOnCurve(float(fmodf(this->time, ((index==0)?float(1):float(index)))));

		this->time += dt;

		time = (time>maxTime)?0:time;

		return ret;
	}

	Math::Vector3 BezierPathStatic::PlayTillEnd(float dt)
	{
		Math::Vector3 ret;
		int index = int(time);	

		ret = this->curveList[index].pointOnCurve(float(fmodf(this->time, ((index==0)?float(1):float(index)))));

		this->time += dt;

		time = (time>maxTime)?(maxTime - 0.0001f):time;

		return ret;
	}

	void BezierPathStatic::debugDraw(float resolution, int currentCurve, int currentPoint)
	{
		for(GLuint i = 0; i < this->curves; i++)
		{
			this->curveList[i].debugDrawCurve(resolution, (currentCurve == i)?1:0, currentPoint);
		}
	}

	bool BezierPathStatic::Save(const char *file)
	{
		FILE* fp = 0;

		fp = fopen(file, "wb");

		if(!fp)
		{
			return false;
		}

		fwrite(&this->curves, sizeof(GLuint), 1, fp);

		for(GLuint i = 0; i < this->curves; i++)
		{
			fwrite(&this->curveList[i].p1, sizeof(Math::Vector3), 1, fp);
			fwrite(&this->curveList[i].p2, sizeof(Math::Vector3), 1, fp);
			fwrite(&this->curveList[i].p3, sizeof(Math::Vector3), 1, fp);
			fwrite(&this->curveList[i].p4, sizeof(Math::Vector3), 1, fp);
		}

		fclose(fp);
		
		return true;
	}

	bool BezierPathStatic::Load(const char *file)
	{
		FILE* fp = 0;

		fp = fopen(file, "rb");

		if(!fp)
		{
			return false;
		}

		fread(&this->curves, sizeof(GLuint), 1, fp);

		if(this->curveList)
		{
			delete[] this->curveList;
			this->curveList = 0;
		}

		this->curveList = new BezierCurve[this->curves];

		if(!this->curveList)
		{
			return false;
		}

		for(GLuint i = 0; i < this->curves; i++)
		{
			fread(&this->curveList[i].p1, sizeof(Math::Vector3), 1, fp);
			fread(&this->curveList[i].p2, sizeof(Math::Vector3), 1, fp);
			fread(&this->curveList[i].p3, sizeof(Math::Vector3), 1, fp);
			fread(&this->curveList[i].p4, sizeof(Math::Vector3), 1, fp);
		}

		fclose(fp);

		return true;
	}


	/*
	================================================
		class BezierPathDynamic
	================================================
	*/

	BezierPathDynamic::BezierPathDynamic(void)
	{
		this->time = 0;
		this->maxTime = 0;
		this->curves = 0;
	}


	BezierPathDynamic::~BezierPathDynamic(void)
	{
	}

	Math::Vector3 BezierPathDynamic::PlayNext(float dt)
	{
		if(this->curves == 0)
		{
			return Math::Vector3(0, 0, 0);
		}

		Math::Vector3 ret;
		int index = int(time);	

		ret = this->curveList[index].pointOnCurve(float(fmodf(this->time, ((index==0)?float(1):float(index)))));

		this->time += dt;

		time = (time>maxTime)?0:time;

		return ret;
	}

	Math::Vector3 BezierPathDynamic::PlayTillEnd(float dt)
	{
		if(this->curves == 0)
		{
			return Math::Vector3(0, 0, 0);
		}	

		Math::Vector3 ret;
		int index = int(time);	

		int size = curveList.size();

		if(index >= size)
		{
			index = size - 1;
			time = (float)index;
		}

		ret = this->curveList[index].pointOnCurve(float(fmodf(this->time, ((index==0)?float(1):float(index)))));

		this->time += dt;

		time = (time>maxTime)?(maxTime - 0.0001f):time;

		return ret;
	}

	void BezierPathDynamic::debugDraw(float resolution, int currentCurve, int currentPoint)
	{
		for(GLuint i = 0; i < this->curves; i++)
		{
			this->curveList[i].debugDrawCurve(resolution, (currentCurve == i)?1:0, currentPoint);
		}
	}

	bool BezierPathDynamic::Save(const char *file)
	{
		FILE* fp = 0;

		fp = fopen(file, "wb");

		if(!fp)
		{
			return false;
		}

		fwrite(&this->curves, sizeof(GLuint), 1, fp);

		for(GLuint i = 0; i < this->curves; i++)
		{
			fwrite(&this->curveList[i].p1, sizeof(Math::Vector3), 1, fp);
			fwrite(&this->curveList[i].p2, sizeof(Math::Vector3), 1, fp);
			fwrite(&this->curveList[i].p3, sizeof(Math::Vector3), 1, fp);
			fwrite(&this->curveList[i].p4, sizeof(Math::Vector3), 1, fp);
		}

		fclose(fp);
		
		return true;
	}

	bool BezierPathDynamic::Load(const char *file)
	{
		FILE* fp = 0;

		fp = fopen(file, "rb");

		if(!fp)
		{
			return false;
		}

		fread(&this->curves, sizeof(GLuint), 1, fp);

		this->curveList.clear();

		GLuint tempCurveSize = this->curves;

		for(GLuint i = 0; i < tempCurveSize; i++)
		{
			this->AddCurve(Math::Vector3(0, 0, 0), Math::Vector3(0, 0, 0), Math::Vector3(0, 0, 0), Math::Vector3(0, 0, 0));
		}

		for(GLuint i = 0; i < tempCurveSize; i++)
		{
			fread(&this->curveList[i].p1, sizeof(Math::Vector3), 1, fp);
			fread(&this->curveList[i].p2, sizeof(Math::Vector3), 1, fp);
			fread(&this->curveList[i].p3, sizeof(Math::Vector3), 1, fp);
			fread(&this->curveList[i].p4, sizeof(Math::Vector3), 1, fp);	
		}

		fclose(fp);

		return true;
	}	
}