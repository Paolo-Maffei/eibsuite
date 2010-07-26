#include "ServerConfig.h"

CServerConfig::CServerConfig(void) : _load_ok(false)
{
}

CServerConfig::~CServerConfig(void)
{
}


bool CServerConfig::Load(const CString& file_name)
{
	_load_ok = false;
	Init();
	
	if(!this->LoadFromFile(file_name))
	{
		throw CEIBException(ConfigFileError,"Config file Not Exist.");
	}
	list<CConfigBlock>::iterator it;
	for (it = _conf.begin();it != _conf.end();it++)
	{
		CConfigBlock& block = (CConfigBlock&)*it;
		if(block.GetName() == GENERAL_BLOCK_NAME)
		{
			LoadGeneralBlock(block.GetParams(),file_name);
		}
	}
	_load_ok = true;
	return true;
}

bool CServerConfig::Save(const CString& file_name)
{
	if(!_load_ok){
		//we didn't load the file properly --> we should change it but let the user do it
		return true;
	}

	list<CConfigBlock>::iterator it_blocks;
	if(_conf.size() == 0){
		CConfigBlock gen_block;
		CString b_name = GENERAL_BLOCK_NAME;
		gen_block.SetName(b_name);
		_conf.insert(_conf.end(),gen_block);
	}

	for ( it_blocks=_conf.begin() ; it_blocks != _conf.end(); it_blocks++ )
	{
		CConfigBlock& block = (CConfigBlock&)*it_blocks;
		if(block.GetName() == GENERAL_BLOCK_NAME)
		{
			SaveGeneralBlock(block);
		}
	}

	this->SaveToFile(file_name);

	return true;
}

void CServerConfig::Init()
{
	#define CONF_ENTRY(var_type, method_name, user_string, def_val) \
	this->_##method_name = def_val;	
	#include "GeneralConf.h" 
	#undef CONF_ENTRY
}

void CServerConfig::SaveGeneralBlock(CConfigBlock& block)
{
	CConfParam pv;
	#define CONF_ENTRY(var_type, method_name, user_string, def_val) \
	ParamToString(pv.GetValue(),this->_##method_name); \
	pv.SetName(user_string); \
	block.Update(pv);
	#include "GeneralConf.h"
	#undef CONF_ENTRY
	
}

void CServerConfig::LoadGeneralBlock(list<CConfParam>& params, const CString& file_name)
{
	list<CConfParam>::iterator it;
	for (it = params.begin();it != params.end();it++)
	{
		CConfParam& pv = (CConfParam&)*it;
		#define CONF_ENTRY(var_type, method_name, user_string, def_val) \
		if (pv.GetName() == user_string){  \
			ParamFromString(pv.GetValue(), this->_##method_name);	\
			pv.SetValid(true);\
		}
		#include "GeneralConf.h"
		#undef CONF_ENTRY
	}

	for (it = params.begin();it != params.end();it++)
	{
		CConfParam& pv = (CConfParam&)*it;
		if(!pv.IsValid()){
			CString err_str = "Unknown parameter \"";
			err_str += pv.GetName();
			err_str += "\" in file ";
			err_str += file_name;
			throw CEIBException(ConfigFileError,err_str);
		}
	}
}

void CServerConfig::ToXml(CDataBuffer& xml_str)
{
	//CXmlNode conf_node = _doc.RootElement().InsertChild(CONFIG_XML);

	#define CONF_ENTRY(var_type, method_name, user_string, def_val) \
	{CXmlElement elem = _doc.RootElement().InsertChild(user_string);\
	elem.SetValue(this->Get##method_name());}
	#include "GeneralConf.h"
	#undef CONF_ENTRY

	_doc.ToString(xml_str);
}

void CServerConfig::FromXml(const CDataBuffer& xml_str)
{
}

