#include "EmulatorConfig.h"

CEmulatorConfig::CEmulatorConfig(void)
{
}

CEmulatorConfig::~CEmulatorConfig(void)
{
}


void CEmulatorConfig::Load(const CString& file_name)
{
	Init();
	if(!this->LoadFromFile(file_name))
	{
		throw CEIBException(ConfigFileError,"Config file Not Exist.");
	}
	list<CConfigBlock>::iterator it;
	for (it = _conf.begin();it != _conf.end();it++)
	{
		CConfigBlock& block = (CConfigBlock&)*it;
		if(block.GetName() == GENERAL_BLOCK_NAME) {
			LoadGeneralBlock(block.GetParams(),file_name);
		}
	}

}

bool CEmulatorConfig::Save(const CString& file_name)
{
	list<CConfigBlock>::iterator it_blocks;
	if(_conf.size() == 0){
		CConfigBlock gen_block;
		gen_block.SetName(GENERAL_BLOCK_NAME);
		_conf.insert(_conf.end(),gen_block);
	}

	for ( it_blocks=_conf.begin() ; it_blocks != _conf.end(); it_blocks++ )
	{
		CConfigBlock& block = (CConfigBlock&)*it_blocks;
		if(block.GetName() == GENERAL_BLOCK_NAME){
			SaveGeneralBlock(block);
		}
	}

	return this->SaveToFile(file_name);
}

void CEmulatorConfig::Init()
{
	#define CONF_ENTRY(var_type, method_name, user_string, def_val) \
	this->_##method_name = def_val;	
	#include "EmulatorGeneralConf.h"
	#undef CONF_ENTRY
}

void CEmulatorConfig::SaveGeneralBlock(CConfigBlock& block)
{
	CConfParam pv;
	#define CONF_ENTRY(var_type, method_name, user_string, def_val) \
	ParamToString(pv.GetValue(),this->_##method_name); \
	pv.SetName(user_string); \
	block.Update(pv);
	#include "EmulatorGeneralConf.h"
	#undef CONF_ENTRY
}

void CEmulatorConfig::LoadGeneralBlock(list<CConfParam>& params,const CString& file_name)
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
		#include "EmulatorGeneralConf.h"
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

