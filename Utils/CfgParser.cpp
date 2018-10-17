/*»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
+
+	Author	: Varun R
+	Started	: 15th March, 2008
+	Revision	: 1.0 (16th April, 2008)
+	Comments	: XML based configuration file parser code.
+
+			Copyright © Differential Arts 2007-09
  ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««*/


#include	"CfgParser.h"

namespace DifferentialArts
{
	bool	CfgParser::Load(std::string file)
	{
		values.clear();
		if(! xmlDoc.LoadFile(file.c_str()))
			return false;

		canParse = false;
		ParseNode(& xmlDoc, "");
		xmlDoc.Clear();

		return true;
	}


	void	CfgParser::Close(void)
	{
		xmlDoc.Clear();
		values.clear();
	}


	void	CfgParser::ParseNode(TiXmlNode *node, std::string group)
	{
		if(node ->Type() == TiXmlNode::ELEMENT)
		{
			if(! _strcmpi(node ->Value(), TOKEN_CFG) && 
				! _strcmpi((node ->ToElement()) ->Attribute(ATTRIBUTE_VERSION), CFG_VERSION))	//A <cfg> tag.
				canParse = true;
			else if(! _strcmpi(node ->Value(), TOKEN_GROUP) && canParse)		//A config group.
			{
				group += std::string((node ->ToElement()) ->Attribute(ATTRIBUTE_GROUP_NAME));
				group += std::string(".");
			}
			else if(! _strcmpi(node ->Value(), TOKEN_SETTING) && canParse)	//A variable.
			{
				std::string key = group;
				key += (node ->ToElement()) ->Attribute(ATTRIBUTE_VAR_NAME);
				
				AddVar(key, std::string((node ->ToElement()) ->Attribute(ATTRIBUTE_VAR_VALUE)));
			}
		}

		for(node = node ->FirstChild(); node; node = node ->NextSibling())
			ParseNode(node, group);
	}


	void	CfgParser::DumpToOutput(void)
	{
		std::map<std::string, std::string>::iterator	itr = values.begin();

		std::cout<<"--------------- Debug Dump------------------"<<std::endl<<std::endl;
		for(; itr!= values.end(); itr++)
			std::cout<<"\t["<<itr ->first<<"] = "<<itr ->second<<std::endl;

		std::cout<<std::endl<<"--------------------------------------------"<<std::endl;
	}


	bool	CfgParser::Save(std::string file)
	{
		xmlDoc.Clear();	//Erase previously loaded data, if present.

		//Create the document skeleton.
		TiXmlDeclaration	*dec;
		TiXmlElement	*cfg;
		TiXmlComment	*com1, *com2, *dec1, *dec2;

		dec1	= new TiXmlComment(CFG_COMMENT_DEC);
		com1	= new TiXmlComment(CFG_COMMENT1);
		com2	= new TiXmlComment(CFG_COMMENT2);
		dec2	= new TiXmlComment(CFG_COMMENT_DEC);

		com1 ->SetCondenseWhiteSpace(false);
		com2 ->SetCondenseWhiteSpace(false);

		if(! com1 || ! com2 || !dec1 || ! dec2)
			return false;
		xmlDoc.LinkEndChild(dec1);
		xmlDoc.LinkEndChild(com1);
		xmlDoc.LinkEndChild(com2);
		xmlDoc.LinkEndChild(dec2);

		dec = new TiXmlDeclaration("1.0", "", "");
		if(! dec)
		{
			xmlDoc.Clear();
			return false;
		}
		xmlDoc.LinkEndChild(dec);

		cfg = new TiXmlElement(TOKEN_CFG);
		if(! cfg)
		{
			xmlDoc.Clear();
			return false;
		}
		cfg ->SetAttribute(ATTRIBUTE_VERSION, CFG_VERSION);
		xmlDoc.LinkEndChild(cfg);

		//Iterate through the map and store all the values.
		std::map<std::string, std::string>::iterator	itr = values.begin();

		for(; itr!= values.end(); itr++)
		{
			TiXmlElement *node = GetNode(cfg, itr ->first);

			if(! node)
			{
				xmlDoc.Clear();
				return false;
			}

			node ->SetAttribute(ATTRIBUTE_VAR_VALUE, itr ->second.c_str());
		}
			
		if(! xmlDoc.SaveFile(file.c_str()))
			return false;

		xmlDoc.Clear();

		return true;
	}


	/*!
		Returns the pointer to the element matching the given hierarchy
		if found, else create the necessary nodes.

		@param	node	The 'rxCfg' element under which all the configs are.
	*/
	TiXmlElement	*CfgParser::GetNode(TiXmlNode *node, std::string tag)
	{
		if(! node)
			return NULL;

		while(1)
		{
			size_t	pos;
			if((pos = tag.find(".")) == std::string::npos)	//This is the variable name.
			{
				//Add the node.
				TiXmlElement *var = new TiXmlElement(TOKEN_SETTING);
				if(! var)
					return NULL;

				node ->LinkEndChild(var);
				var ->SetAttribute(ATTRIBUTE_VAR_NAME, tag.c_str());
				//Value will be set in the caller.

				return var;
			}

			//There's still a group so isolate it.
			std::string group = tag.substr(0, pos);
			tag = tag.substr(pos+1);

			//Find the group element.
			bool	createNew = true;
			TiXmlNode	*cur = NULL;

			while( (cur = node ->IterateChildren(TOKEN_GROUP, cur)) )
				if(std::string(cur ->ToElement() ->Attribute(ATTRIBUTE_GROUP_NAME)) == group)	//Found it!
				{
					createNew = false;
					node = cur;
					break;
				}

			if(createNew)	//Not found
			{
				//Create and attach one.
				TiXmlElement *grp = new TiXmlElement(TOKEN_GROUP);
				if(! grp)
					return NULL;

				node ->LinkEndChild(grp);
				grp ->SetAttribute(ATTRIBUTE_GROUP_NAME, group.c_str());

				node = grp;
			}
		}


		return NULL;
	}


	bool	CfgParser::AddVar(std::string tag, std::string value)
	{
		for(unsigned int i=0; i<tag.length(); i++)
			tag[i] = tolower(tag[i]);

		values[tag] = value;

		return true;
	}

	std::string	CfgParser::GetVar(std::string tag)
	{
		std::map<std::string, std::string>::const_iterator	itr;

		//We don't want case sensitivity.
		for(unsigned int i=0; i<tag.length(); i++)
			tag[i] = tolower(tag[i]);

		if( (itr = values.find(tag)) == values.end() )
			return	"<undefined>";

		return itr ->second;
	}
}