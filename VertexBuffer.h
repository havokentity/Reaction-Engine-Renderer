/*
	Description: Vertex Buffer Object class

	classes contained are:

	class VertexBuffer

	THIS IS OPENGL SPECIFIC
*/

#ifndef CVBO_H___
#define CVBO_H___

#include "CoreEngine.h"

namespace DifferentialArts
{
	//Single texture unit struct
	/*!
		\struct Texcoord2VB
		\brief Data structure for VBO's.
		
		For a vertex and a single texture coord with vertex normal \n

		\code
		struct Texcoord2VB
		{
			GLfloat tu, tv;
			GLfloat nx, ny, nz;
			GLfloat vx, vy, vz;
		};
		\endcode
	*/
	struct Texcoord2VB
	{
		GLfloat tu, tv;
		GLfloat nx, ny, nz;
		GLfloat vx, vy, vz;
	};	

	//Single texture unit struct sans normal
	/*!
		\struct ParticleVB
		\brief Data structure for VBO's.
		
		For a vertex and a single texture coord without vertex normal \n

		\code
		struct ParticleVB
		{
			GLfloat tu, tv;
			GLfloat vx, vy, vz;
		};
		\endcode
	*/
	struct ParticleVB
	{
		GLfloat tu, tv;
		GLfloat vx, vy, vz;		
		GLfloat r, g, b, a;
	};	

	//Three texture coord struct for Bump mapping
	/*
		This is a special structure for sending tangent, binormal data to the 
		lighting shader for bumpmapping etc 

		NOTE: r, s i've put them after u and v

		TO BE OPTIMIZED: reduce components for binormals and tangents by 1
	*/
	
	//Optimized Three texture coord struct for Bump mapping
	/*!
		\struct BumpmapVB
		\brief Data structure for VBO's. Useful for bumpmapped surfaces.
		
		Look at the code below to understand the structure \n

		\code
		struct BumpmapVB
		{
			GLfloat tu, tv;
			GLfloat tu2, tv2, tr2;  //To be used in tex unit 3
			GLfloat tu3, tv3, tr3;  //To be used in tex unit 4
			GLfloat nx, ny, nz;
			GLfloat vx, vy, vz;
		};
		\endcode
	*/

	struct BumpmapVB
	{
		GLfloat tu, tv;
		GLfloat tu2, tv2, tr2;  //To be used in tex unit 3
		GLfloat tu3, tv3, tr3;  //To be used in tex unit 4
		GLfloat nx, ny, nz;
		GLfloat vx, vy, vz;
	};

	//Optimized Three texture coord struct for Bump mapping and skinning
	/*!
		\struct SkinnedBumpmapVB
		\brief Data structure for VBO's. Useful for bumpmapped and skinned surfaces.
		
		Look at the code below to understand the structure \n

		\code
		struct SkinnedBumpmapVB
		{
			GLfloat tu, tv;
			GLfloat tu2, tv2, tr2;  //To be used in tex unit 3
			GLfloat tu3, tv3, tr3;  //To be used in tex unit 4

			GLfloat tu4, tv4, tr4, ts4;  //To be used in tex unit 1
			GLfloat tu5, tv5, tr5, ts5;  //To be used in tex unit 2

			GLfloat nx, ny, nz;
			GLfloat vx, vy, vz;
		};
		\endcode
	*/
	struct SkinnedBumpmapVB
	{
		GLfloat tu, tv;
		GLfloat tu2, tv2, tr2;  //To be used in tex unit 3
		GLfloat tu3, tv3, tr3;  //To be used in tex unit 4

		GLfloat tu4, tv4, tr4;  //To be used in tex unit 1
		GLfloat tu5, tv5, tr5;  //To be used in tex unit 2

		GLfloat nx, ny, nz;
		GLfloat vx, vy, vz;
	};

	//For shadow object
	/*!
		\struct ShadowVB
		\brief Shadow Data structure for VBO's
		
		This is the most basic vbo with the smallest size for rendering shadows \n

		\code
		struct ShadowVB
		{
			GLfloat vx, vy, vz;
		};
		\endcode
	*/
	struct ShadowVB
	{
		GLfloat vx, vy, vz;
	};

	//For shadow Skinned object
	/*!
		\struct SkinnedShadowVB
		\brief Shadow Data structure for skinned VBO's
		
		This VBO structure is used for rendering skinned VBO meshes \n

		\code
		struct SkinnedShadowVB
		{
			GLfloat w1, w2, w3;
			GLuint ji1, ji2, ji3;

			GLfloat vx, vy, vz;	
		};
		\endcode
	*/
	struct SkinnedShadowVB
	{
		GLfloat w1, w2, w3;
		GLfloat ji1, ji2, ji3;

		GLfloat vx, vy, vz;	
	};

	extern RX_API int Texcoord2VB_setup_array[1];
	extern RX_API int Texcoord2VB_texcoord_elements_array[1];

	extern RX_API int BumpmapVB_setup_array[3];
	extern RX_API int BumpmapVB_texcoord_elements_array[3];

	extern RX_API int BumpmapSkinVB_setup_array[5];
	extern RX_API int BumpmapSkinVB_texcoord_elements_array[5];

	/*
		RX_API int Texcoord2VB_setup_array[1] = { 0 };
		RX_API int Texcoord2VB_texcoord_elements_array[1] = { 2 };

		RX_API int BumpmapVB_setup_array[3] = { 0, 3, 4 };
		RX_API int BumpmapVB_texcoord_elements_array[3] = { 2, 3, 3 };

		RX_API int BumpmapSkinVB_setup_array[5] = { 0, 3, 4, 1, 2 };
		RX_API int BumpmapSkinVB_texcoord_elements_array[5] = { 2, 3, 3, 3, 3 };
	*/

	/*!	\class VertexBuffer
		\brief Class for handling vertex buffer objects with TexcoordVBN vertex structure
		\author Rajesh Peter Douglas D'Monte	

		This class is only used to handle VBO's (Vertex Buffer objects) for the
		vertex structure contained in TexcoordVBN

		In the render functions, GLenum mode can be GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_QUAD_STRIP etc or other primitive assemblies
	*/	
	class RX_API VertexBuffer
	{
	public:
		VertexBuffer(void); //!< Constructor
		~VertexBuffer(void);//!< Deconstructor. Used Free for freeing data.

		/*! \enum VertexBufferType
			\brief Vertex buffer memory access/storage hint
		*/
		/*! \enum IndexBufferType
			\brief Index buffer memory access/storage hint
		*/
		enum RX_API BufferType
		{
			STATIC_SEND				= GL_STATIC_DRAW_ARB,
			DYNAMIC_SEND			= GL_DYNAMIC_DRAW_ARB,
			STREAM_SEND				= GL_STREAM_DRAW_ARB,
			STATIC_READ				= GL_STATIC_READ_ARB,
			DYNAMIC_READ			= GL_DYNAMIC_READ_ARB,
			STREAM_READ				= GL_STREAM_READ_ARB,
			STATIC_SEND_AND_READ	= GL_STATIC_COPY_ARB,
			DYNAMIC_SEND_AND_READ	= GL_DYNAMIC_COPY_ARB,
			STREAM_SEND_AND_READ	= GL_STREAM_COPY_ARB,
		};

		/*! \enum LOCK_MODE
			\brief Locked vertex buffer data access mode
		*/
		enum LOCK_MODE
		{
			READ					= GL_READ_ONLY_ARB,
			WRITE					= GL_WRITE_ONLY_ARB,
			READ_AND_WRITE			= GL_READ_WRITE_ARB,
		};

		bool Initialize(unsigned int count, void* vertices, BufferType bt = BufferType::STATIC_SEND,
						int vertexStride = sizeof(Texcoord2VB),
						int totalTexUnits = 1,
						int texUnitArray[] = Texcoord2VB_setup_array,
						int texOffsetArray[] = Texcoord2VB_texcoord_elements_array,
						unsigned int countIndices = 0, GLuint* indices = 0,						
						BufferType it = BufferType::STATIC_SEND);
		//!< Initializes the Vertex Buffer Object	

		/*	Dont' forget to start VertexBuffer setup first*/
		/* GLenum mode can be GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_QUAD_STRIP etc or other primitive assemblies */
		__forceinline void Render(GLenum mode, unsigned int start, unsigned int total); //!<Render VBO with limits
		__forceinline void RenderIndexed(GLenum mode, unsigned int start, unsigned int total); //!<Render VBO with limits

		__forceinline void Render(GLenum mode); //!<Render VBO
		__forceinline void RenderIndexed(GLenum mode); //!<Render VBO
		
		bool UploadVertices(void* vertices, unsigned int start, unsigned int total);//!< Modifies Dynamic vertices. Make sure your VB object is bound first.
		bool UploadIndices(GLuint* indices, unsigned int start, unsigned int total);//!< Modifies Dynamic indices. Make sure your VB object is bound first.

		bool NewVertexData(void* vertices, unsigned int total); //!< Resizes the vertex buffer object
		bool NewIndexData(GLuint* indices, unsigned int total); //!< Resizes the vertex buffer object

		/*
			In order to Manually control client state for Vertex Buffer Objects
			please look at the CoreEngine.h containing functions for controlling 
			client state

			REFER: EnableClientVBOState, UnBindLastBuffer and DisableClientVBOState
		*/	

		void* Lock(bool lockDiscard, LOCK_MODE lm = LOCK_MODE::READ_AND_WRITE); 
		//!< Returns a pointer to the buffer's data. Make sure VB is bound first

		void* LockIndex(bool lockDiscard, LOCK_MODE lm = LOCK_MODE::READ_AND_WRITE); 
		//!< Returns a pointer to the buffer's index data. Make sure VB is bound first

		bool Unlock(void);
		//!< Unlocks the vertex buffer object. Returns false if the data became corrupt

		bool UnlockIndex(void);
		//!< Unlocks the vertex buffer object indices. Returns false if the data became corrupt

		__forceinline void Bind();	//!< Binds the vertex buffer object
		static void UnBind();  //!< Binds NULL vertex buffer object
		__forceinline void BindIndexed();	//!< Binds the vertex buffer object with index buffer bound
		static void UnBindIndexed();  //!< Binds NULL vertex buffer object with index buffer bound		
		__forceinline unsigned int getIndexCount(void) const; //!< Returns total indices
		__forceinline unsigned int getVertexCount(void) const; //!< Returns total vertices

		void Free(void); //!< Releases resouces allocated to the VBO

		void StartVBSetup(void); //!< Call this function before rendering
		void EndVBSetup(void);	  //!< Call this function after rendering

		void StartVBSetupColor(void); //!< Call this function before rendering and enable color pointer
		void EndVBSetupColor(void);	  //!< Call this function after rendering and enable color pointer

		void StartVBSetupNoNormals(void); //!< Call this function before rendering, excludes normals
		void EndVBSetupNoNormals(void);	  //!< Call this function after rendering, excludes normals

		void StartVBSetupNoNormalsColor(void); //!< Call this function before rendering, excludes normals and enable color pointer
		void EndVBSetupNoNormalsColor(void);   //!< Call this function after rendering, excludes normals and enable color pointer

		void StartVertexSetup(void); //!< This Vertex buffer setup is used for vertices only, Eg: Shadow meshes 
		void EndVertexSetup(void);	//!< End single vertex, VertexBuffer setup.

		void StartVertexSetupZero(void); 
		//!< This Vertex buffer setup is used for vertices only and has a buffer offset of zero, Eg: Shadow meshes. Useful for single vertex structures

		void SetOffset(int indexmap, int texUnit, int floatOffset); //!< Maps a texunit/coord to offset
		void PrepareOffsets(void); //!< Prepares the memory offsets for the vbo after the offsets have been set

		BufferType mvbt; //!< VB type, change this, and the next time you change data using NewVertexData, the new memory settings will take effect
		BufferType mibt;  //!< VB index buffer type, change this, and the next time you change data using NewIndexData, the new memory settings will take effect

	protected:		
		GLuint mIndexVBO;
		GLuint* mIndices;		
		unsigned int indexCount; 
		unsigned int mIndexSize;

		GLuint mVBO;
		void* mVertices;	
		unsigned int vertexCount;
		unsigned int mSize;		

		int nVertexStride;
		int nOffsetForNormals;
		int nOffsetForVertices;
		int nOffsetForColor;
		int nOffsetForVerticesNoNormals;
		int nOffsetForTextures[8];		

		int offsetArray[8];
		int texSetupArray[8];
		int texUnitsUsed;

		bool PrepareIndexedVBO(int countIndices, BufferType ibt, GLuint* indices);				
	};
}

#endif