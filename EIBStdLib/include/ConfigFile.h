/*! \file ConfigFile.h
    \brief ConfigFile Class - Source file

	This is The header file for Buffer class. This Class serves as the base calss for all type of
	various configuration files used by the EIB Network components.

*/

#ifndef __CONFIG_FILE_HEADER__
#define __CONFIG_FILE_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "Directory.h"

using namespace std;

/*! \class CConfParam
	\brief Config Param Class
*/
class EIB_STD_EXPORT CConfParam
{
public:
	/*!Constructor*/
	CConfParam() : _valid(false){};
	/*!Destructor*/
	virtual ~CConfParam(){};

	/*!
	\brief Get Method
	\fn CString& GetName() const
	\return CString& the name of the config parameter
	*/
	const CString& GetName(){ return _name;}
	/*!
	\brief Get Method
	\fn CString& GetValue()
	\return CString& the vlaue of the config parameter
	*/
	CString& GetValue(){ return _value;}
	/*!
	\brief Get Method
	\fn CString& GetComments()
	\return CString& the vlaue of the config parameter comments (comments above the parameter)
	*/
	CString GetComments() { return _comments; }
	/*!
	\brief Set Method
	\fn void SetName(const CString& name)
	\param name - the name of the config parameter
	*/
	void SetName(const CString& name){_name = name; }
	/*!
	\brief Set Method
	\fn void SetName(const char* name)
	\param name - the name of the parameter
	*/
	void SetName(const char* name) { _name = name;}
	/*!
	\brief Set Method
	\fn void SetValue(const CString& value)
	\param value - the value of the config parameter
	*/
	void SetValue(const CString& value){_value = value; }
	/*!
        \brief Set Method
        \fn void SetComments(const CString& comments)
        \param comments - the comments above the config parameter
        */
	void SetComments(const CString& comments) { _comments = comments; }

	void SetValid(bool val) { _valid = val;}
	bool IsValid() { return _valid;}

private:
	CString _name;
	CString _value;
	CString _comments;
	bool _valid;
};

/*! \class CConfigBlock
	\brief Config Block Class
*/
class EIB_STD_EXPORT CConfigBlock
{
public:
	/*!Constructor*/
	CConfigBlock(){};
	/*!Destructor*/
	virtual ~CConfigBlock(){};

	/*!
	\brief Get Method
	\fn CString& GetName()
	\return CString& the name of the config parameter
	*/
	CString& GetName() { return _block_name;}
	/*!
	\brief Set Method
	\fn void SetName(CString& name)
	\param name - the name of the config parameter
	*/
	void SetName(const CString& name) { _block_name = name;}
	/*!
	\brief Set Method
	\fn list<CConfParam>& GetParams()
	\return list of CConfParam assgined withi the Block.
	*/
	list<CConfParam>& GetParams() { return _params;}

	/*!
	\brief Method to determine whether a parameter already exist in the list
	\fn bool IsExist(const CConfParam& param)
	\return true if the parameter already exist in the list
	*/
	bool IsExist(CConfParam& param);

private:
#ifdef WIN32
#pragma warning( disable: 4251 )
#endif
	list<CConfParam> _params;
#ifdef WIN32
#pragma warning( default: 4251 )
#endif
	CString _block_name;
};

/*! \class CConfigFile
	\brief Config File Class
*/
class EIB_STD_EXPORT CConfigFile
{

protected:
	/*!Constructor*/
	CConfigFile(void){};
	/*!Destructor*/
	virtual ~CConfigFile(void){};

	/*!
	\brief Method to save the current configuration into a text file
	\fn bool SaveToFile(const CString& file_name)
	\param file_name - the file name to save to.
	\return bool - If saving was successful or not
	*/
	bool SaveToFile(const CString& file_name);
	/*!
	\brief Method to load the current configuration from a text file
	\fn bool LoadFromFile(const CString& file_name)
	\param file_name - the file name to load from.
	\return bool - If loading was successful or not
	*/
	bool LoadFromFile(const CString& file_name);

	bool IsExist(CConfigBlock& block);

	void ParamFromString(const CString& val, bool& member );
	void ParamFromString(const CString& val, int& member );
	void ParamFromString(const CString& val, CString& member);
	void ParamFromString(const CString& val, double& member);
	void ParamFromString(const CString& val, unsigned int& member);
	void ParamFromString(const CString& val, short& member);
	void ParamFromString(const CString& val, unsigned short& member);

	void ParamToString(CString& val, bool& member );
	void ParamToString(CString& val, int& member );
	void ParamToString(CString& val, CString& member);
	void ParamToString(CString& val, double& member);
	void ParamToString(CString& val, unsigned int& member);
	void ParamToString(CString& val, short& member);
	void ParamToString(CString& val, unsigned short& member);

private:
	list<CConfigBlock>& GetConfig() { return _conf;}

protected:
#ifdef WIN32
#pragma warning( disable: 4251 )
#endif
	list<CConfigBlock> _conf;
#ifdef WIN32
#pragma warning( default: 4251 )
#endif
};

#endif
