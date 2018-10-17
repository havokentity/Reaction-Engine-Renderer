/*»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
+
+	Author	: Varun R
+	Modified: Rajesh Peter Douglas D'Monte
+	Started	: 15th March, 2008
+	Revision	: 1
+	Comments	: Declarations for the XML based configuration 
+				  file parser.
+	Changes		: Added namespace and changed formatting of docs.
+				  Removed Global "using namespace std".
+
+			Copyright © DifferentialArts 2007-09
  ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««*/


#ifndef	_CfgParser_H_
#define	_CfgParser_H_

#include	"tinyxml.h"
#include	<vector>
#include	<string>
#include	<map>
#include	<iostream>

#include "Client.h"

#pragma	comment(lib, "tinyxml.lib")

namespace DifferentialArts
{
	#define	CFG_VERSION				"A0150308"
	#define	CFG_COMMENT1			"      Auto generated REACTION ENGINE config file. You may modify this file     "
	#define	CFG_COMMENT2			"      but comments will not be preserved when saved by the application.        "
	#define	CFG_COMMENT_DEC			"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+"

	//Recognized tokens//
	#define	TOKEN_CFG				"rxCfg"
	#define	TOKEN_GROUP				"group"
	#define	TOKEN_SETTING			"var"

	//Recognized attributes//
	#define	ATTRIBUTE_VERSION			"version"
	#define	ATTRIBUTE_GROUP_NAME		"id"
	#define	ATTRIBUTE_VAR_NAME		"name"
	#define	ATTRIBUTE_VAR_VALUE		"value"


	/*!
		The main class that manages loading of all configuration XML files.
	*/
	class RX_API CfgParser
	{
	private:
		bool								canParse;			//!<Used internally to authenticate the config file.
		std::map<std::string, std::string>	values;				//!<Map of variables and their values.
		TiXmlDocument						xmlDoc;				//!<The XML document object.

		void	ParseNode(TiXmlNode *node, std::string group);	//!<Recursively parses the XML tree structure.
		
		TiXmlElement	*GetNode(TiXmlNode *node, std::string tag);	//!<Returns a node forming a hierarchy speciffied by the tag.

	public:
		bool	Save(std::string file);								//!<Writes back config data to XML file.
		bool	Load(std::string file);								//!<Opens and parses a configuration file.
		void	Close(void);

		bool	AddVar(std::string tag, std::string value);			//!<Adds a config var to the cfg.
		std::string	GetVar(std::string tag);						//!<Returns a config var.

		void	DumpToOutput(void);									//!<For debugging purposes.

		~CfgParser()
		{	Close();	}
	};
}

#endif