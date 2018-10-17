/*
	Description: Consists of Shader class and helpers
	and these classes are used to provide Cg Shader support

	The classes contained here are,

	FOR nVIDIA Cg Only

	class Shader, class Effect
	
*/

#include "Shader.h"

namespace DifferentialArts
{
	Shader::Shader()
	{
		memset(this->mVertexProgramName, 0, sizeof(char)*256);
		memset(this->mPixelProgramName, 0, sizeof(char)*256);
		this->mCGprogram_vertex = 0;
		this->mCGprogram_pixel = 0;
		this->mCGprogram_combined = 0;
	}

	Shader::~Shader()
	{
		//this->Free();
	}

	void Shader::Free(void)
	{
		if(mCGprogram_vertex)
		{
			cgDestroyProgram(mCGprogram_vertex);
		}

		if(mCGprogram_pixel)
		{
			cgDestroyProgram(mCGprogram_pixel);
		}

		if(mCGprogram_combined)
		{
			cgDestroyProgram(mCGprogram_combined);

			if(this->mVertexProgramName && this->mPixelProgramName)
			{
				myGlobalGame->mlog.LogSuccessText(LString("Successfully freed combined vertex program %s and fragment program %s!", this->mVertexProgramName, this->mPixelProgramName)); 
			} else if(this->mVertexProgramName)
			{
				myGlobalGame->mlog.LogSuccessText(LString("Successfully freed combined vertex program %s!", this->mVertexProgramName)); 
			} else if(this->mPixelProgramName)
			{
				myGlobalGame->mlog.LogSuccessText(LString("Successfully freed combined fragment program %s!", this->mPixelProgramName)); 
			}
		} else {
			if(this->mVertexProgramName)
			{
				myGlobalGame->mlog.LogSuccessText(LString("Successfully freed vertex program %s !", this->mVertexProgramName)); 
			}

			if(this->mPixelProgramName)
			{
				myGlobalGame->mlog.LogSuccessText(LString("Successfully freed pixel program %s !", this->mPixelProgramName)); 
			}
		}

		myGlobalGame->mlog.LogSuccessText(LString("Successfully freed a Shader Object!"));
	}

	bool Shader::LoadVertexProgram(char* fileName, char* entryPoint, CGprofile vsProfile, bool bSource)
	{	
		strcpy(this->mVertexProgramName, fileName);
		myGlobalGame->mlog.LogInfo(LString("Loading vertex program %s...", this->mVertexProgramName));
		this->mCGprogram_vertex = cgCreateProgramFromFile(myGlobalGame->getCGContext(),
														  ((bSource == true) ? CG_SOURCE : CG_OBJECT),
														  this->mVertexProgramName,
														  vsProfile,
														  entryPoint, 
														  NULL);

		mVSProfile = vsProfile;

		if(!this->mCGprogram_vertex)
		{
			myGlobalGame->mlog.LogError(LString("Error loading %s vertex program", this->mVertexProgramName));
			return false;
		}

		char errorString[256];
		sprintf(errorString, "Creating vertex program from file %s", this->mVertexProgramName);
		myGlobalGame->checkForCgError(errorString);
		cgGLLoadProgram(this->mCGprogram_vertex);
		sprintf(errorString, "Loading vertex program from file %s", this->mVertexProgramName);
		myGlobalGame->checkForCgError(errorString);
		myGlobalGame->mlog.LogSuccessText(LString("Successfully loaded vertex program %s !", this->mVertexProgramName)); 

		return true;
	}

	bool Shader::LoadFragmentProgram(char* fileName, char* entryPoint, CGprofile fsProfile, bool bSource)
	{
		strcpy(this->mPixelProgramName, fileName);
		myGlobalGame->mlog.LogInfo(LString("Loading fragment program %s ...", this->mPixelProgramName));
		this->mCGprogram_pixel = cgCreateProgramFromFile(myGlobalGame->getCGContext(),
														 ((bSource == true) ? CG_SOURCE : CG_OBJECT),
														 this->mPixelProgramName,
														 fsProfile,
														 entryPoint, 
														 NULL);

		mFSProfile = fsProfile;

		if(!this->mCGprogram_pixel)
		{
			myGlobalGame->mlog.LogError(LString("Error loading %s pixel program", this->mPixelProgramName));
			return false;
		}

		char errorString[256];
		sprintf(errorString, "Creating pixel program from file %s", this->mPixelProgramName);
		myGlobalGame->checkForCgError(errorString);
		cgGLLoadProgram(this->mCGprogram_pixel);
		sprintf(errorString, "Loading pixel program from file %s", this->mPixelProgramName);
		myGlobalGame->checkForCgError(errorString);
		myGlobalGame->mlog.LogSuccessText(LString("Successfully loaded pixel program %s !", this->mPixelProgramName));

		return true;
	}

	__forceinline void Shader::EnableVertexShaderSpecificProfile(void)
	{
		myGlobalGame->EnableVertexProfile(this->mVSProfile);
	}

	__forceinline void Shader::EnableFragmentShaderSpecificProfile(void)
	{
		myGlobalGame->EnablePixelProfile(this->mFSProfile);
	}

	__forceinline void Shader::EnableCombinedShaderProfile(void)
	{
		myGlobalGame->EnableVertexProfile(this->mVSProfile);
		myGlobalGame->EnablePixelProfile(this->mFSProfile);
	}

	bool Shader::CombineVertexAndFragmentPrograms(bool bPreserveOriginalPrograms)
	{
		myGlobalGame->mlog.LogInfo(LString("Combining programs..."));

		this->mCGprogram_combined = cgCombinePrograms2(this->mCGprogram_vertex, this->mCGprogram_pixel);

		if(!mCGprogram_combined)
		{
			myGlobalGame->mlog.LogError(LString("There was an error combining a vertex and fragment program!"));
			return false;
		}

		if(!bPreserveOriginalPrograms)
		{
			cgDestroyProgram(this->mCGprogram_vertex);
			cgDestroyProgram(this->mCGprogram_pixel);
		}

		myGlobalGame->mlog.LogInfo(LString("Loading combined program..."));
		cgGLLoadProgram(this->mCGprogram_combined);
		myGlobalGame->checkForCgError("Combined program loading: ");

		myGlobalGame->mlog.LogSuccessText(LString("Successfully combined programs!"));

		return true;
	}

	__forceinline void Shader::BindVertexProgram(void)
	{
		cgGLBindProgram( mCGprogram_vertex );
		//cgGLEnableProfile( myGlobalGame->getCGVertexProfile() );
	}

	__forceinline void Shader::BindPixelProgram(void)
	{
		cgGLBindProgram( mCGprogram_pixel );
		//cgGLEnableProfile( myGlobalGame->getCGPixelProfile() );
	}

	__forceinline void Shader::UnBindLastVertexProgram(void)
	{
		cgGLUnbindProgram( myGlobalGame->getCGVertexProfile() );
		//cgGLDisableProfile( myGlobalGame->getCGVertexProfile() );
	}

	__forceinline void Shader::UnBindLastPixelProgram(void)
	{
		cgGLUnbindProgram( myGlobalGame->getCGPixelProfile() );
		//cgGLDisableProfile( myGlobalGame->getCGPixelProfile() );
	}

	__forceinline void Shader::BindCombinedProgram(void)
	{
		cgGLBindProgram( mCGprogram_combined );
		//this->BindVertexProgram();
		//this->BindPixelProgram();
	}

	__forceinline void Shader::UnBindCombinedProgram(void)
	{
		//cgGLUnbindProgram( myGlobalGame->getCGPixelProfile() );
		//this->UnBindLastVertexProgram();
		//this->UnBindLastPixelProgram();
		cgGLUnbindProgram( myGlobalGame->getCGVertexProfile() );
		cgGLUnbindProgram( myGlobalGame->getCGPixelProfile() );
	}

	void Shader::AssignParameterToPixelProgram(CGparameter &yourParameter, char* shaderVariableName)
	{
		yourParameter = cgGetNamedParameter(this->mCGprogram_pixel, shaderVariableName);
	}

	void Shader::AssignParameterToVertexProgram(CGparameter &yourParameter, char* shaderVariableName)
	{
		yourParameter = cgGetNamedParameter(this->mCGprogram_vertex, shaderVariableName);
	}

	void Shader::AssignParameterToCombinedProgram(CGparameter &yourParameter, char* shaderVariableName)
	{
		yourParameter = cgGetNamedParameter(this->mCGprogram_combined, shaderVariableName);
	}


	/*
	==================================================================

		CGFX effect Class code starts here

	==================================================================
	*/

	Effect::Effect()
	{
		memset(this->mEffectName, 0, sizeof(char)*256);
	}

	Effect::~Effect()
	{
		this->Free();
	}

	void Effect::Free(void)
	{
		if(CG_TRUE == cgIsEffect(this->mCGeffect))
		{
			cgDestroyEffect(this->mCGeffect);

			myGlobalGame->mlog.LogSuccessText(LString("Successfully freed CGeffect %s !", this->mEffectName));
		}
	}

	bool Effect::LoadEffect(char* fileName)
	{
		strcpy(this->mEffectName, fileName);

		myGlobalGame->mlog.LogInfo(LString("Loading CGeffect %s...", this->mEffectName));

		this->mCGeffect = cgCreateEffectFromFile(myGlobalGame->getCGContext(), fileName, NULL);

		if(!this->mCGeffect)
		{
			const char * pszErrors = cgGetLastListing(myGlobalGame->getCGContext());

			int len = strlen(pszErrors);

			if(len < 1024)
			{
				myGlobalGame->mlog.LogError(LString("CGeffect parse error %s", pszErrors));
			} else {
				char temp[1024];
				for(int i = 0; i < len; i+= 1024)
				{
					for(int j = i; j < 1024; j++)
					{
						temp[j] = pszErrors[i + j];
					}

					myGlobalGame->mlog.LogError(LString("CGeffect parse error %s", temp));
				}
			}

			myGlobalGame->mlog.LogError(LString("Error loading %s CGeffect", this->mEffectName));

			return false;
		}

		if(!this->obtainValidatedTechnique())
		{
			myGlobalGame->mlog.LogError(LString("Error loading %s CGeffect", this->mEffectName));
			return false;
		}

		//Maps all pass names to a pass ID number
		int index = 0;

		CGpass tempPass = cgGetFirstPass(this->mTechnique);

		while(NULL != tempPass)
		{	
			mPassNameIDMap.insert(PassIDPair(cgGetPassName(tempPass), index++));
			tempPass = cgGetNextPass(tempPass);
		}

		myGlobalGame->mlog.LogSuccessText(LString("Successfully loaded CGeffect %s !", this->mEffectName));

		return true;
	}
		
	bool Effect::obtainValidatedTechnique(void)
	{
		this->mTechnique = 0;
		this->mTechnique = cgGetFirstTechnique(this->mCGeffect);

		while (this->mTechnique)
		{
			if (cgValidateTechnique(this->mTechnique) == CG_FALSE)
			{
				myGlobalGame->mlog.LogError(LString("Technique %s did not validate", cgGetTechniqueName(this->mTechnique)));
				this->mTechnique = cgGetNextTechnique(this->mTechnique);
			} else {
				myGlobalGame->mlog.LogSuccessText(LString("Technique %s was validated successfully!",
					cgGetTechniqueName(this->mTechnique)));
				return true;
			}		
		}

		return false;
	}

	void Effect::AssignParameterToEffect(CGparameter &yourParameter, char* effectVariableName)
	{
		yourParameter = cgGetNamedEffectParameter(this->mCGeffect, effectVariableName);
	}

	void Effect::AssignPassToEffect(CGpass &yourPass, char* effectPassName)
	{
		yourPass = cgGetNamedPass(this->mTechnique, effectPassName);
	}

	__forceinline void Effect::ResetLastPass(void)
	{
		cgResetPassState(this->mPass);
	}

	__forceinline void Effect::ResetPass(CGpass &pass)
	{
		cgResetPassState(pass);
	}

	__forceinline void Effect::BindPass(CGpass &pass)
	{
		cgSetPassState(pass);
	}

	__forceinline void Effect::BindFirstPass(void)
	{
		this->mPass = cgGetFirstPass(this->mTechnique);
		cgSetPassState(this->mPass);
		//myGlobalGame->mlog.LogError(LString("%s", cgGetPassName(this->mPass)));
		
	}

	__forceinline void Effect::BindNextPass(void)
	{
		this->mPass = cgGetNextPass(this->mPass);
		cgSetPassState(this->mPass);
	}

	__forceinline void Effect::BindNamedPass(char *passName)
	{
		this->mPass = cgGetNamedPass(this->mTechnique, passName);
		cgSetPassState(this->mPass);
	}

	CGpass Effect::getCurrentPass(void)
	{
		return this->mPass;
	}

	const char* Effect::getCurrentPassName(void)
	{
		return cgGetPassName(this->mPass);
	}

	void Effect::CreateSharedParameter(CGparameter &yourMainSharedParameter, CGtype type)
	{
		yourMainSharedParameter = cgCreateParameter(myGlobalGame->getCGContext(), type); 
	}

	void Effect::ConnectSharedParameter(CGparameter &yourMainSharedParameter, CGparameter &connectee)
	{		
		cgConnectParameter(yourMainSharedParameter, connectee); 
	}
}