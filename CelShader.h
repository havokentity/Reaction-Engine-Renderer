/*
	Description: Toon shader class/helpers

	The classes contained here are,
	class CelShader
	
	NOTE: Please set total lights to be used in shader file directly
*/

#ifndef CELLSHADER_H___
#define CELLSHADER_H___

#include "CoreEngine.h"
#include "Shader.h"

namespace DifferentialArts
{

	/*!	\class CelShader
		\brief Cel shader effect helper class
		\author Rajesh Peter Douglas D'Monte

		For basic cel shading and interaction with the cel shading shaders.
		This class is very old and doesn't use the new Reaction Engine features.
		You should probably write your own cel shader my modifying Reaction Engines
		new lighting engine. Reaction Engine's Lighting Effect engine can easily be modded
		to do celshading with the new advanced features it supports. 
	*/
	class RX_API CelShader
	{
	public:
		CelShader(); //!< Constructor
		~CelShader(); //!< Deconstructor

		void InitializeDefault(void); //!< Initializes the cel shader/* Default CelShader texture folder is set to 'Data/Textures/CelShader/'*/
		void Initialize(char* textureFolder, char* shaderFolder); //!< Initialize by specifying folders

		void SetMaterialDiffuse(float r, float g, float b, float a); //!< Set material diffuse
		void SetMaterialSpecular(float r, float g, float b, float a); //!< Set material specular
		void SetAmbient(float r, float g, float b, float a);//!< Set ambient
		void SetTextureUnit1Usage(bool usage); //!< Enable texture unit 1 usage
		void SetShine(float shine); //!< Set specular power
		void SetCurrentLightingEquation(float fconstant, float flinear, float fquadratic); //!< Set lighting equation

		void Bind(void); //!< Bind shader
		void UnBind(void); //!< Unbind shader

		void Free(void); //!< Release resources allocated to CelShader object

		Shader toonShader; //!< The toon shader object

	private:
		CGparameter diffuseSampler1D, specularSampler1D, edgeSampler1D; //Samplers
		CGparameter materialDiffuse, materialSpecular; //Material Properties
		CGparameter ambient;
		CGparameter specularExponent, constant, linear, quadratic, texture1;

		Texture1D diffuseTex, specularTex, edgeTex;
		
	};
}

#endif