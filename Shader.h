/*
	Description: Consists of Shader class and helpers
	and these classes are used to provide Cg Shader support

	The classes contained here are,

	FOR nVIDIA Cg Only

	class Shader, class Effect
	
*/

#ifndef SHADER_H
#define SHADER_H

/* CG LIBRARIES */
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgGL.lib ")

/* CG HEADERS */
#include <Cg/Cg.h>
#include <Cg/cgGL.h>

#include "CoreEngine.h"
#include "Client.h"

#include <map>
#include <string>

namespace DifferentialArts
{
	typedef std::map<std::string, int> PassIDMap; //!<Pass ID Map typedef
	typedef std::pair<std::string, int> PassIDPair; //!<Pass ID pair typedef 

	/*
	NOTE: INITIALIZE SHADERS FIRST IN ENGINE BEFORE USING class Shader, 

	refer class CoreEngine in CoreEngine.h
	*/

	/*!	\class Shader
		\brief Handles NVIDIA Cg Shaders
		\author Rajesh Peter Douglas D'Monte

		Use this class for using Cg programs in your application.
		It is highly recommended you use Effect instead of Shader for all
		shaders.
	*/
	class RX_API Shader
	{
	public:
		Shader(); //!<Constructor
		~Shader(); //!<Deconstructor. Call Free instead.

		void Free(void); //!<Free the shader. Deconstructor doesn't release shader memory
		bool LoadVertexProgram(char* fileName, char* entryPoint, CGprofile vsProfile = myGlobalGame->getCGVertexProfile(), bool bSource = true); 
		//!<Loads a vertex program, entryPoint is the entry function of the shader program, specify NULL for main, specify the profile in the third parameter, specify if the program is cg source in the 4th parameter
		bool LoadFragmentProgram(char* fileName, char* entryPoint, CGprofile fsProfile = myGlobalGame->getCGPixelProfile(), bool bSource = true);
		//!<Loads a fragment program, entryPoint is the entry function of the shader program, specify NULL for main, specify the profile in the third parameter, specify if the program is cg source in the 4th parameter
		
		bool CombineVertexAndFragmentPrograms(bool bPreserveOriginalPrograms = false);
		//!< Combines a vertex and fragment program

		__forceinline void BindVertexProgram(void); //!< Binds the vertex program
		__forceinline void BindPixelProgram(void); //!< Binds the fragment program
		__forceinline void UnBindLastVertexProgram(void); //!< Unbinds the last vertex program
		__forceinline void UnBindLastPixelProgram(void); //!< Unbinds the last fragment program

		__forceinline void EnableVertexShaderSpecificProfile(void); //!< Enables the right vertex profile for this vertex program
		__forceinline void EnableFragmentShaderSpecificProfile(void); //!< Enables the right fragment profile for this vertex program
		__forceinline void EnableCombinedShaderProfile(void); //!< Enables the right vertex and fragment profile for the combined shader program

		__forceinline void BindCombinedProgram(void); //!< Bind the combined program, make sure you call Shader::CombineVertexAndFragmentPrograms() first.
		__forceinline void UnBindCombinedProgram(void); //!< Unbinds the combined program, make sure you call Shader::CombineVertexAndFragmentPrograms() first.

		void AssignParameterToPixelProgram(CGparameter &yourParameter, char* shaderVariableName);
		//!<Assigns a parameter to the fragment program
		void AssignParameterToVertexProgram(CGparameter &yourParameter, char* shaderVariableName);
		//!<Assigns a parameter to the vertex program
		void AssignParameterToCombinedProgram(CGparameter &yourParameter, char* shaderVariableName);
		//!<Assigns a parameter to the combined program, make sure you call Shader::CombineVertexAndFragmentPrograms() first.	

	protected:
		CGprogram   mCGprogram_vertex;
		CGprogram   mCGprogram_pixel;
		CGprogram   mCGprogram_combined;

		char mVertexProgramName[256];
		char mPixelProgramName[256];

		CGprofile mVSProfile;
		CGprofile mFSProfile;
	};

	/*
		This class is used to handle CGFX files,
		you can always inherit from this class, to make more robust effects
	*/

	/*!	\class Effect
		\brief Handles NVIDIA CgFX effect files
		\author Rajesh Peter Douglas D'Monte

		Use this class for using NVIDIA CgFX effect files in you programs.
		It is highly recommended you use Effect instead of Shader for all
		shaders.
	*/
	class RX_API Effect
	{
	public:
		Effect();//!<Constructor
		~Effect();//!<Deconstructor

		bool LoadEffect(char* fileName); //!< Loads a CgFX effect
		void Free(void); //!< Release resources for the effect object. Deconstructor doesn't do the job.

		__forceinline void ResetLastPass(void); //!< Reset the last pass. Can be likened to Shader::UnBindAllOfLastPrograms()
		__forceinline void ResetPass(CGpass &pass);//!< Reset the specified pass
		__forceinline void BindPass(CGpass &pass);//!< Binds the specified pass
		__forceinline void BindNamedPass(char* passName); //!< Bind a pass by refering to it by its name.
		__forceinline void BindFirstPass(void); //!<Bind the first pass in the effect
		__forceinline void BindNextPass(void);// !< Bind the next pass

		//This validates most CGeffect's techniques in order and uses first one that works
		bool obtainValidatedTechnique(void);
		//!< Returns true if a valid technique was found in the effect

		void AssignParameterToEffect(CGparameter &yourParameter, char* effectVariableName);
		//!< Assign a parameter to an effect
		void AssignPassToEffect(CGpass &yourPass, char* effectPassName);
		//!< Assign a pass to an effect

		CGpass getCurrentPass(void);
		//!< Returns the currently bound pass in the effect

		const char* getCurrentPassName(void);
		//!< Returns the currently bound pass name in the effect

		PassIDMap mPassNameIDMap;
		//!< Stores a ID number for each pass

		unsigned int mSceneManagerConvenienceID;
		//!< Don't modify. This is a public so that if your an expert user, you can modify as you see fit

		static void CreateSharedParameter(CGparameter &yourMainSharedParameter, CGtype type = CG_FLOAT);
		//!< Static function that creates a shared shader/effect parameter

		static void ConnectSharedParameter(CGparameter &yourMainSharedParameter, CGparameter &connectee);
		//!< Static function that connects the \a connectee parameter to another CGparameter

		static inline void DestroySharedParameter(CGparameter &yourSharedParameter)
		{
			cgDestroyParameter(yourSharedParameter);
		}
		//!< Destroys a shared CG parameter created using CreateSharedParameter

	protected:
		CGeffect mCGeffect;
		CGtechnique mTechnique;
		CGpass mPass;

		char mEffectName[256];
	};
}

#endif
