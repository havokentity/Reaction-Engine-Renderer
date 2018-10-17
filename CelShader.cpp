
#include "CelShader.h"

namespace DifferentialArts
{
	CelShader::CelShader()
	{
	}

	CelShader::~CelShader()
	{
	}

	void CelShader::Free(void)
	{
		this->toonShader.Free();
		this->diffuseTex.Free();
		this->specularTex.Free();
		this->edgeTex.Free();
		myGlobalGame->mlog.LogSuccessText(LString("Successfully freed a CelShader Object"));
	}

	void CelShader::InitializeDefault(void)
	{
		diffuseTex.LoadTextureImage("Data/Textures/CelShader/diffuse.bmp");
		specularTex.LoadTextureImage("Data/Textures/CelShader/specular.bmp");
		edgeTex.LoadTextureImage("Data/Textures/CelShader/edge.bmp");

		toonShader.LoadVertexProgram("Data/Shaders/CelShader/ToonShader.vcg", 0);
		toonShader.LoadFragmentProgram("Data/Shaders/CelShader/ToonShader.pcg", 0);

		toonShader.AssignParameterToPixelProgram(diffuseSampler1D, "diffuseSampler1D");
		toonShader.AssignParameterToPixelProgram(specularSampler1D, "specularSampler1D");
		toonShader.AssignParameterToPixelProgram(edgeSampler1D, "edgeSampler1D");
		toonShader.AssignParameterToPixelProgram(this->texture1, "useTex1");
		cgGLSetParameter1d(this->texture1, 0);

		toonShader.AssignParameterToPixelProgram(materialDiffuse, "materialDiffuse");
		toonShader.AssignParameterToPixelProgram(materialSpecular, "materialSpecular");
		cgGLSetParameter4f(materialDiffuse, 0.8f, 0.8f, 0.8f, 1.0f);
		cgGLSetParameter4f(materialSpecular, 1.0f, 1.0f, 1.0f, 1.0f);
		
		toonShader.AssignParameterToPixelProgram(specularExponent, "specularExponent");
		cgGLSetParameter1f(specularExponent, 32.0f);

		toonShader.AssignParameterToPixelProgram(ambient, "ambient");
		cgGLSetParameter4f(ambient, 0.1f, 0.1f, 0.1f, 1.0f);

		toonShader.AssignParameterToPixelProgram(constant, "constant");
		cgGLSetParameter1f(constant, 0.001f);

		toonShader.AssignParameterToPixelProgram(linear, "linear");
		cgGLSetParameter1f(linear, 0.003f);

		toonShader.AssignParameterToPixelProgram(quadratic, "quadratic");
		cgGLSetParameter1f(quadratic, 0.004f);
	}

	void CelShader::Initialize(char* textureFolder, char* shaderFolder)
	{
		char diffusePath[256];
		char specularPath[256];
		char edgePath[256];

		char vertexShaderPath[256];
		char pixelShaderPath[256];

		strcpy(diffusePath, textureFolder);
		strcat(diffusePath, "diffuse.bmp");

		strcpy(specularPath, textureFolder);
		strcat(specularPath, "specular.bmp");

		strcpy(edgePath, textureFolder);
		strcat(edgePath, "edge.bmp");

		strcpy(vertexShaderPath, shaderFolder);
		strcat(vertexShaderPath, "ToonShader.vcg");

		strcpy(pixelShaderPath, shaderFolder);
		strcat(pixelShaderPath, "ToonShader.pcg");

		diffuseTex.LoadTextureImage(diffusePath);
		specularTex.LoadTextureImage(specularPath);
		edgeTex.LoadTextureImage(edgePath);

		toonShader.LoadVertexProgram(vertexShaderPath, 0);
		toonShader.LoadFragmentProgram(pixelShaderPath, 0);
		toonShader.CombineVertexAndFragmentPrograms();

		toonShader.AssignParameterToCombinedProgram(diffuseSampler1D, "diffuseSampler1D");
		toonShader.AssignParameterToCombinedProgram(specularSampler1D, "specularSampler1D");
		toonShader.AssignParameterToCombinedProgram(edgeSampler1D, "edgeSampler1D");
		toonShader.AssignParameterToCombinedProgram(this->texture1, "useTex1");
		cgGLSetParameter1d(this->texture1, 0);

		toonShader.AssignParameterToCombinedProgram(materialDiffuse, "materialDiffuse");
		toonShader.AssignParameterToCombinedProgram(materialSpecular, "materialSpecular");
		cgGLSetParameter4f(materialDiffuse, 0.8f, 0.8f, 0.8f, 1.0f);
		cgGLSetParameter4f(materialSpecular, 1.0f, 1.0f, 1.0f, 1.0f);

		toonShader.AssignParameterToCombinedProgram(specularExponent, "specularExponent");
		cgGLSetParameter1f(specularExponent, 32.0f);		

		toonShader.AssignParameterToCombinedProgram(ambient, "ambient");
		cgGLSetParameter4f(ambient, 0.1f, 0.1f, 0.1f, 1.0f);

		toonShader.AssignParameterToCombinedProgram(constant, "constant");
		cgGLSetParameter1f(constant, 0.001f);

		toonShader.AssignParameterToCombinedProgram(linear, "linear");
		cgGLSetParameter1f(linear, 0.003f);

		toonShader.AssignParameterToCombinedProgram(quadratic, "quadratic");
		cgGLSetParameter1f(quadratic, 0.004f);
	}

	void CelShader::SetMaterialDiffuse(float r, float g, float b, float a)
	{
		cgGLSetParameter4f(materialDiffuse, r, g, b, a);
	}

	void CelShader::SetAmbient(float r, float g, float b, float a)
	{
		cgGLSetParameter4f(ambient, r, g, b, a);
	}

	void CelShader::SetMaterialSpecular(float r, float g, float b, float a)
	{
		cgGLSetParameter4f(materialSpecular, r, g, b, a);
	}

	void CelShader::SetShine(float shine)
	{
		cgGLSetParameter1f(specularExponent, shine);
	}

	void CelShader::SetTextureUnit1Usage(bool usage)
	{
		cgGLSetParameter1d(this->texture1, usage);
	}

	void CelShader::Bind(void)
	{
		toonShader.BindCombinedProgram();

		cgGLEnableTextureParameter(diffuseSampler1D);
		cgGLEnableTextureParameter(specularSampler1D);
		cgGLEnableTextureParameter(edgeSampler1D);

		cgGLSetTextureParameter(diffuseSampler1D, diffuseTex.getTexture());
		cgGLSetTextureParameter(specularSampler1D, specularTex.getTexture());
		cgGLSetTextureParameter(edgeSampler1D, edgeTex.getTexture());
	}

	void CelShader::SetCurrentLightingEquation(float fconstant, float flinear, float fquadratic)
	{
		cgGLSetParameter1f(constant, fconstant);
		cgGLSetParameter1f(linear, flinear);
		cgGLSetParameter1f(quadratic, fquadratic);
	}

	void CelShader::UnBind(void)
	{
		cgGLDisableTextureParameter(diffuseSampler1D);
		cgGLDisableTextureParameter(specularSampler1D);
		cgGLDisableTextureParameter(edgeSampler1D);
		toonShader.UnBindCombinedProgram();
	}
}