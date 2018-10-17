/*
	For helping in compiling dll and the reliant game
*/


#ifndef R_UTIL_TYPES_H
#define R_UTIL_TYPES_H

#pragma warning(disable:4251)

#define NOMINMAX
#define RX_OPENGL_API
//#define RX_D3D_API

	#ifndef RX_UTIL_API		
		#ifdef RX_UTIL_EXPORT
			#define RX_UTIL_API __declspec(dllexport)
			#define CLIENT_VERSION "1, 860, 890, 0" 
		#else
			#define RX_UTIL_API __declspec(dllimport) //Client view		
			
			#ifdef NDEBUG
				#pragma comment(lib, "DruidUtilities.lib")
			#else	
				#pragma comment(lib, "DruidUtilities_d.lib")
			#endif
		#endif
	#endif

#endif