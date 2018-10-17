/*
	For helping in compiling dll and the reliant game
*/


#ifndef RTYPES_H
#define RTYPES_H

#pragma warning(disable:4251)

#define NOMINMAX
#define RX_OPENGL_API
//#define RX_D3D_API

	#ifndef RX_API		
		#ifdef RX_EXPORT
			#define RX_API __declspec(dllexport)
			#define CLIENT_VERSION "1, 900, 900, 0" 
		#else
			#define RX_API __declspec(dllimport) //Client view		
			
			#ifdef NDEBUG
				#pragma comment(lib, "DruidEngine.lib")
			#else	
				#pragma comment(lib, "DruidEngine_d.lib")
			#endif
		#endif
	#endif

#endif