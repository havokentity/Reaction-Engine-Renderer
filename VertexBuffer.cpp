/*
	Description: Vertex Buffer Object class

	classes contained are:

	class VertexBuffer

	THIS IS OPENGL SPECIFIC
*/


#include "VertexBuffer.h"

namespace DifferentialArts
{
	RX_API int Texcoord2VB_setup_array[1] = { 0 };
	RX_API int Texcoord2VB_texcoord_elements_array[1] = { 2 };

	RX_API int BumpmapVB_setup_array[3] = { 0, 3, 4 };
	RX_API int BumpmapVB_texcoord_elements_array[3] = { 2, 3, 3 };

	RX_API int BumpmapSkinVB_setup_array[5] = { 0, 3, 4, 1, 2 };
	RX_API int BumpmapSkinVB_texcoord_elements_array[5] = { 2, 3, 3, 3, 3 };

	/*
	================================================================================
		VertexBuffer class
	================================================================================
	*/
	void VertexBuffer::StartVertexSetup(void)
	{
		glVertexPointer( 3, GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForVertices) );

		glEnableClientState( GL_VERTEX_ARRAY );
	}

	void VertexBuffer::StartVertexSetupZero(void)
	{
		glVertexPointer( 3, GL_FLOAT, nVertexStride, BUFFER_OFFSET(0) );

		glEnableClientState( GL_VERTEX_ARRAY );
	}

	void VertexBuffer::EndVertexSetup(void)
	{
		glDisableClientState( GL_VERTEX_ARRAY );
	}

	/*
	================================================================================
		VertexBuffer Constructor
	================================================================================
	*/

	VertexBuffer::VertexBuffer(void)
	{
		this->vertexCount = 0;
		this->indexCount = 0;
	}

	/*
	================================================================================
		VertexBuffer Destructor
	================================================================================
	*/
	VertexBuffer::~VertexBuffer(void)
	{
		this->Free();
	}

	/*
	================================================================================
		VertexBuffer Free function, releases the VBO object stored in graphic
		card video memory
	================================================================================
	*/
	void VertexBuffer::Free(void)
	{	
		if(!myGlobalGame->isVBOSupported())
		{
			myGlobalGame->mlog.LogError(LString("VBO is not supported, cannot use any class VertexBuffer objects!"));
			return;
		}

		glDeleteBuffersARB(1, &this->mVBO);
		glDeleteBuffersARB(1, &this->mIndexVBO );

		myGlobalGame->mlog.LogSuccessText(LString("Successfully freed a VBO!"));
	}

	bool VertexBuffer::PrepareIndexedVBO(int countIndices, BufferType ibt, GLuint* indices)
	{
		//Indexed VBO code
		if(countIndices > 0)
		{
			this->indexCount = countIndices;
			//Indice VBO code

			mIndexSize = sizeof(GLuint) * countIndices;
			int nParam_ArrayObjectSize = 0;

			glGenBuffersARB( 1, &this->mIndexVBO );
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, this->mIndexVBO );	
			glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mIndexSize, this->mIndices, ibt );

			glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

			if( nParam_ArrayObjectSize <= 0 )
			{
				myGlobalGame->mlog.LogError(LString("glBufferDataARB failed to allocate any memory for indexed VBO!"));
				return false;
			}

			glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
		} else {
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
		}

		return true;
	}

	void VertexBuffer::Bind(void)
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->mVBO );
	}
	
	void VertexBuffer::UnBind(void)
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );	
	}

	void VertexBuffer::BindIndexed(void)
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->mVBO );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, this->mIndexVBO );
	}
	
	void VertexBuffer::UnBindIndexed(void)
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
	}

	void* VertexBuffer::Lock(bool lockDiscard, LOCK_MODE lm)
	{
		if(lockDiscard)
		{
			glBufferDataARB(GL_ARRAY_BUFFER, this->mSize, 0, this->mvbt);
		}

		return glMapBufferARB(GL_ARRAY_BUFFER_ARB, lm);
	}

	void* VertexBuffer::LockIndex(bool lockDiscard, LOCK_MODE lm)
	{
		if(lockDiscard)
		{
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, this->mSize, 0, this->mvbt);
		}

		return glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER, lm);
	}

	bool VertexBuffer::Unlock(void)
	{		
		return (glUnmapBufferARB(GL_ARRAY_BUFFER_ARB) == GL_TRUE) ? true : false;
	}

	bool VertexBuffer::UnlockIndex(void)
	{
		return (glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB) == GL_TRUE) ? true : false;
	}

	void VertexBuffer::StartVBSetup(void)
	{
		for(int i = 0; i < this->texUnitsUsed; i++)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glTexCoordPointer( offsetArray[i], GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForTextures[i]) );
		}

		glNormalPointer( GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForNormals) );
		glVertexPointer( 3, GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForVertices) );

		for(int i = 0; i < this->texUnitsUsed; i++)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		}

		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_VERTEX_ARRAY );
	}

	void VertexBuffer::EndVBSetup(void)
	{
		for(int i = (this->texUnitsUsed - 1); i >= 0; i--)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}

		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );
	}

	void VertexBuffer::StartVBSetupColor(void)
	{
		for(int i = 0; i < this->texUnitsUsed; i++)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glTexCoordPointer( offsetArray[i], GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForTextures[i]) );
		}

		glNormalPointer( GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForNormals) );
		glVertexPointer( 3, GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForVertices) );
		glColorPointer( 4, GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForColor) );

		for(int i = 0; i < this->texUnitsUsed; i++)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		}

		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
	}

	void VertexBuffer::EndVBSetupColor(void)
	{
		for(int i = (this->texUnitsUsed - 1); i >= 0; i--)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}

		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );
	}

	void VertexBuffer::StartVBSetupNoNormals(void)
	{
		for(int i = 0; i < this->texUnitsUsed; i++)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glTexCoordPointer( offsetArray[i], GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForTextures[i]) );
		}

		glVertexPointer( 3, GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForVerticesNoNormals) );

		for(int i = 0; i < this->texUnitsUsed; i++)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		}

		glEnableClientState( GL_VERTEX_ARRAY );
	}

	void VertexBuffer::EndVBSetupNoNormals(void)
	{
		for(int i = (this->texUnitsUsed - 1); i >= 0; i--)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}

		glDisableClientState( GL_VERTEX_ARRAY );
	}

	void VertexBuffer::StartVBSetupNoNormalsColor(void)
	{
		for(int i = 0; i < this->texUnitsUsed; i++)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glTexCoordPointer( offsetArray[i], GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForTextures[i]) );
		}

		glVertexPointer( 3, GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForVerticesNoNormals) );
		glColorPointer( 4, GL_FLOAT, nVertexStride, BUFFER_OFFSET(nOffsetForVertices) );

		for(int i = 0; i < this->texUnitsUsed; i++)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		}

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
	}

	void VertexBuffer::EndVBSetupNoNormalsColor(void)
	{
		for(int i = (this->texUnitsUsed - 1); i >= 0; i--)
		{
			glClientActiveTextureARB(GL_TEXTURE0 + texSetupArray[i]);
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}

		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );
	}


	void VertexBuffer::SetOffset(int indexmap, int texUnit, int floatOffset)
	{
		if(indexmap < 0 || indexmap >= this->texUnitsUsed)
		{
			myGlobalGame->mlog.LogError(LString("An invalid offset was specified, SetOffset failed!"));
			return;
		}

		texSetupArray[indexmap] = texUnit;
		offsetArray[indexmap] = floatOffset;
	}

	void VertexBuffer::PrepareOffsets(void)
	{
		int i;
		unsigned int sum = 0;
		for(i = 0; i < this->texUnitsUsed; i++)
		{
			nOffsetForTextures[i] = sizeof(GLfloat) * sum;
			sum += offsetArray[i];
		}

		this->nOffsetForNormals = sizeof(GLfloat) * sum;
		this->nOffsetForVerticesNoNormals = sizeof(GLfloat) * sum;
		sum += 3;
		this->nOffsetForVertices = sizeof(GLfloat) * sum;
		sum += 3;
		this->nOffsetForColor = sizeof(GLfloat) * sum;
	}

	/*
	================================================================================
		Initializes a VBO object

		Texcoord2VB
	================================================================================
	*/
	bool VertexBuffer::Initialize(unsigned int count, void* vertices, BufferType bt,
								  int vertexStride, int totalTexUnits,
								  int texUnitArray[], int texOffsetArray[],
								  unsigned int countIndices, GLuint* indices,						
								  BufferType it)
	{
		if(!myGlobalGame->isVBOSupported())
		{
			myGlobalGame->mlog.LogError(LString("VBO is not supported, cannot use any class VertexBuffer objects!"));
			return false;
		}

		this->texUnitsUsed = totalTexUnits;

		if(this->texUnitsUsed >= 8)
		{
			myGlobalGame->mlog.LogError(LString("totalTexUnits should be less than 8, VertexBuffer failed to create!"));
			return false;
		}

		for(int i = 0; i < this->texUnitsUsed; i++)
		{
			texSetupArray[i] = texUnitArray[i];
			offsetArray[i] = texOffsetArray[i];
		}
		
		this->nVertexStride = vertexStride;
		mSize = this->nVertexStride * count;
		int nParam_ArrayObjectSize = 0;

		this->vertexCount = count;
		this->mVertices = vertices;

		this->mvbt = bt;
		this->mibt = it;

		glGenBuffersARB( 1, &this->mVBO );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->mVBO );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, mSize, this->mVertices, bt );

		glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

		if( nParam_ArrayObjectSize <= 0 )
		{
			myGlobalGame->mlog.LogError(LString("glBufferDataARB failed to allocate any memory for VBO!"));
			return false;
		}

		this->PrepareOffsets();

		return PrepareIndexedVBO(countIndices, it, indices);
	}		

	/*
	================================================================================
		Renders a VBO object initialized with

		struct Texcoord2VB data
	================================================================================
	*/
	void VertexBuffer::Render(GLenum mode, unsigned int start, unsigned int total)
	{
		glDrawArrays( mode, start, total );
	}

	void VertexBuffer::Render(GLenum mode)
	{
		glDrawArrays( mode, 0, this->vertexCount);
	}

	/*
	================================================================================
		Renders a VBO object initialized with

		struct Texcoord2VB data with indices
	================================================================================
	*/
	void VertexBuffer::RenderIndexed(GLenum mode, unsigned int start, unsigned int total)
	{
		glDrawElements( mode, total, GL_UNSIGNED_INT, BUFFER_OFFSET(start*sizeof(GLuint)));
	}

	void VertexBuffer::RenderIndexed(GLenum mode)
	{
		glDrawElements( mode, indexCount, GL_UNSIGNED_INT, 0 );
	}

	/*
	================================================================================
		Used to dynamically modify indices of a VBO object

		takes the starting offset and the total number of indices
		to be modified

		<NOTE>
		but doesn't affect the VBO on video card memory

		the data in video card memory is updated the next time
		the render call is called
	================================================================================
	*/
	bool VertexBuffer::UploadIndices(GLuint* indices, unsigned int start, unsigned int total)
	{
		this->mIndices = indices;

		glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*start, sizeof(GLuint)*total, this->mIndices);	

		return true;
	}

	bool VertexBuffer::UploadVertices(void* vertices, unsigned int start, unsigned int total)
	{
		this->mVertices = vertices;

		if(total == indexCount && start == 0)
		{
			glBufferDataARB(GL_ARRAY_BUFFER, this->mSize, this->mVertices, this->mvbt);
		} else {
			glBufferSubDataARB(GL_ARRAY_BUFFER, sizeof(nVertexStride)*start, sizeof(nVertexStride)*total, this->mVertices);
		}

		return true;
	}

	bool VertexBuffer::NewVertexData(void* vertices, unsigned int total)
	{
		this->mVertices = vertices;
		this->vertexCount = total;
		mSize = this->nVertexStride * total;

		int nParam_ArrayObjectSize = 0;

		glBufferDataARB( GL_ARRAY_BUFFER_ARB, mSize, this->mVertices, mvbt );	

		glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

		if( nParam_ArrayObjectSize <= 0 )
		{
			myGlobalGame->mlog.LogError(LString("glBufferDataARB failed to allocate any memory for VBO!"));
			return false;
		}

		return true;
	}

	bool VertexBuffer::NewIndexData(GLuint* indices, unsigned int total)
	{
		this->mIndices = indices;
		this->indexCount = total;
		mIndexSize = sizeof(GLuint) * total;

		int nParam_ArrayObjectSize = 0;

		glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mIndexSize, this->mIndices, mibt );	

		glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

		if( nParam_ArrayObjectSize <= 0 )
		{
			myGlobalGame->mlog.LogError(LString("glBufferDataARB failed to allocate any memory for VBO!"));
			return false;
		}

		return true;
	}

	unsigned int VertexBuffer::getIndexCount(void) const
	{
		return this->indexCount;
	}

	unsigned int VertexBuffer::getVertexCount(void) const
	{
		return this->vertexCount;
	}
};