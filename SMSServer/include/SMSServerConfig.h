/*! \file SMSServerConfig.h
    \brief Defines CServerConfig
    
	This is the header file for CServerConfig

*/
#ifndef __SMS_CONF_HEADER__
#define __SMS_CONF_HEADER__

#include "ConfigFile.h"
#include "CException.h"
using namespace std;

#define GENERAL_BLOCK_NAME "SMS-GENERAL"
#define SMS_CONF_FILE_NAME "SMS.conf"

/*! 
	\class CServerConfig
	\brief Defines the server configuration file class

	This class inherits from CConfigFile, the general configuration file class.
*/
class CSMSServerConfig : public CConfigFile
{
public:
	/*!Constructor*/
	CSMSServerConfig(void);
	/*!Destructor*/
	virtual ~CSMSServerConfig(void);

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
#include "SMSGeneralConf.h"
#undef CONF_ENTRY

private:

#define CONF_ENTRY(var_type, method_name, user_string, def_val) \
	var_type _##method_name;
#include "SMSGeneralConf.h"
#undef CONF_ENTRY

	void LoadGeneralBlock(list<CConfParam>& params,const CString& file_name);
	void SaveGeneralBlock(CConfigBlock& block);
};

#endif
