/*! \file WEBServerConfig.h
    \brief Defines CServerConfig
    
	This is the header file for CServerConfig

*/
#ifndef __WEB_CONF_HEADER__
#define __WEB_CONF_HEADER__

#include "ConfigFile.h"
#include "CException.h"
using namespace std;

#define GENERAL_BLOCK_NAME "WEB-GENERAL"
#define WEB_CONF_FILE_NAME "WEB.conf"

/*! 
	\class CWEBServerConfig
	\brief Defines the server configuration file class

	This class inherits from CConfigFile, the general configuration file class.
*/
class CWEBServerConfig : public CConfigFile
{
public:
	/*!Constructor*/
	CWEBServerConfig(void);
	/*!Destructor*/
	virtual ~CWEBServerConfig(void);

	/*!
		\fn void Init()
		\brief Initializes server config file

		Parameter values are initialized  to default values
	*/
	void Init();
	/*!
		\fn bool Save(const CString& file_name)
		\brief Saves configuration to file
		\param file_name configuration will be saved to this file
	*/
	bool Save(const CString& file_name);
	/*!
		\fn bool Load(const CString& file_name)
		\brief Loads configuration from file
		\param file_name configuration will be loaded from this filename
	*/
	void Load(const CString& file_name);
	
// Get / Set Methods
#define CONF_ENTRY(var_type, method_name, user_string, def_val) \
	const var_type& Get##method_name() const {return _##method_name;}  \
	void Set##method_name(var_type val) {_##method_name = val;}
#include "WEBGeneralConf.h"
#undef CONF_ENTRY

private:

#define CONF_ENTRY(var_type, method_name, user_string, def_val) \
	var_type _##method_name;
#include "WEBGeneralConf.h"
#undef CONF_ENTRY

	void LoadGeneralBlock(list<CConfParam>& params,const CString& file_name);
	void SaveGeneralBlock(CConfigBlock& block);
};

#endif
