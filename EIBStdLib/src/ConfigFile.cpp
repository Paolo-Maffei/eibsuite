#include "ConfigFile.h"
#include "Globals.h"

bool CConfigBlock::IsExist(const CConfParam& param)
{
	list<CConfParam>::iterator it_params = GetParams().begin();
	while (it_params != GetParams().end())
	{
		if((*it_params).GetName() == param.GetName()){
			return true;
		}
		it_params++;
	}
	return false;
}

void CConfigBlock::Update(const CConfParam& param)
{
	if(!IsExist(param)){
		_params.insert(_params.end(), param);
		return;
	}

	list<CConfParam>::iterator it_params = GetParams().begin();
	while (it_params != GetParams().end())
	{
		if((*it_params).GetName() == param.GetName()){
			it_params->SetValue(param.GetValueConst());
			break;
		}
		it_params++;
	}
}

bool CConfigFile::LoadFromFile(const CString &file_name)
{
	//If conf directory is not exist -- we sure that we can't find the file and return false
	if(!CDirectory::IsExist(CURRENT_CONF_FOLDER)){
		return false;
	}

	ifstream myfile;
	myfile.open((CURRENT_CONF_FOLDER + file_name).GetBuffer(),ios::in);
	if (myfile.fail()){
		return false;
	}

	CString line;
	int line_num = 1;
	CConfigBlock block;
	CConfParam conf_param;
	block.GetName().Clear();
	while (!myfile.eof())
	{
		int index = (int)string::npos;
		getline(myfile,line.GetSTDString());
		line.Trim();
		if(line.GetLength() < 2){
			continue;
		}
		if(line[0] == '[' && line[line.GetLength() - 1] == ']')
		{
			if(!block.GetName().IsEmpty() && !IsExist(block))
			{
				_conf.insert(_conf.end(),block);
			}
			block.GetName().Clear();
			CString block_str = line.SubString(1,line.GetLength() - 2);
			block.SetName(block_str);
		}
		else if (line[0] == '[' || line[line.GetLength() - 1] == ']')
		{
			throw CEIBException(FileError,"Error in file: %s Line: %d - Empty block is not allowed",file_name.GetBuffer(),line_num);
		}
		else if (line[0] == '#')
		{
			//Make sure the parameter was not inserted to the structure yet - and also validate it
			if(!block.IsExist(conf_param))
			{
				CString comnt = conf_param.GetComments();
				comnt += "\n";
				comnt += line;
				conf_param.SetComments(comnt);
			}
		}
		else if((index = line.FindFirstOf('=')) != static_cast<int>(string::npos))
		{
			if(index == line.GetLength() -1){
				throw CEIBException(FileError,"Error in file: %s Line: %d - param \"%s\" missing value (empty)",file_name.GetBuffer(),line_num,line.GetBuffer());
			}
			CString name = line.SubString(0,index);
			CString value = line.SubString(index + 1,line.GetLength() - index - 1);
			if(!block.GetName().IsEmpty())
			{
				name.Trim();
				value.Trim();
				conf_param.SetName(name);
				conf_param.SetValue(value);
				if(!block.IsExist(conf_param)){
					block.GetParams().insert(block.GetParams().end(),conf_param);
				}
				//clear the parameter (name, value, comment)
				conf_param.SetComments(EMPTY_STRING);
				conf_param.SetName(EMPTY_STRING);
				conf_param.SetValue(EMPTY_STRING);
			}
			else
			{
				throw CEIBException(FileError,"Error in file: %s Line: %d - param is not under legal block",file_name.GetBuffer(),line_num);
			}
		}
		else
		{
			throw CEIBException(FileError,"Error in file: %s Line: %d - Unknown line format",file_name.GetBuffer(),line_num);
		}

		++line_num;
	}

	if(!block.GetName().IsEmpty() && !IsExist(block))
	{
		_conf.insert(_conf.end(),block);
	}

	myfile.close();

	return true;
}

bool CConfigFile::IsExist(CConfigBlock& block)
{
	list<CConfigBlock>::iterator it = _conf.begin();
	while (it != _conf.end())
	{
		if((*it).GetName() == block.GetName()){
			return true;
		}
		it++;
	}
	return false;
}

bool CConfigFile::SaveToFile(const CString &file_name)
{
	if(!CDirectory::IsExist(CURRENT_CONF_FOLDER))
    {
    	if(!CDirectory::Create(CURRENT_CONF_FOLDER)){
        	return false;
        }
    }

	ofstream myfile;
	myfile.open((CURRENT_CONF_FOLDER + file_name).GetBuffer(),ios::out|ios::trunc);
	if (myfile.fail()){
		return false;
	}

	list<CConfigBlock>::iterator it_blocks;

	for ( it_blocks=_conf.begin() ; it_blocks != _conf.end(); it_blocks++ )
	{
		CConfigBlock tmp = (CConfigBlock)*it_blocks;

		myfile << "[" << tmp.GetName() << "]" << endl;

		list<CConfParam>::iterator it_params = tmp.GetParams().begin();

		while (it_params != tmp.GetParams().end())
		{
			if((*it_params).GetComments().GetLength() > 0)
			{
				myfile << (*it_params).GetComments().GetBuffer() << endl;
			}
			myfile << (*it_params).GetName().GetBuffer() << " = " << (*it_params).GetValue().GetBuffer() << endl;
			it_params++;
		}

		myfile << endl;
	}

	myfile.close();
	return true;
}

void CConfigFile::ParamFromString(const CString& val,bool& member)
{
	member = val.ToBool();
}

void CConfigFile::ParamFromString(const CString& val,int& member)
{
	member = val.ToInt();
}

void CConfigFile::ParamFromString(const CString& val,unsigned int& member)
{
	member = val.ToUInt();
}

void CConfigFile::ParamFromString(const CString& val,double& member)
{
	member = val.ToDouble();
}

void CConfigFile::ParamFromString(const CString& val,short& member)
{
	member = val.ToShort();
}

void CConfigFile::ParamFromString(const CString& val,unsigned short& member)
{
	member = val.ToUShort();
}

void CConfigFile::ParamFromString(const CString& val,CString& member)
{
	member = val;
}

void CConfigFile::ParamToString(CString& val,bool& member)
{
	CString tmp(member);
	val = tmp;

}

void CConfigFile::ParamToString(CString& val,int& member)
{
	val = member;
}

void CConfigFile::ParamToString(CString& val,unsigned int& member)
{
	val = member;
}

void CConfigFile::ParamToString(CString& val,double& member)
{
	val = member;
}

void CConfigFile::ParamToString(CString& val,short& member)
{
	val = member;
}

void CConfigFile::ParamToString(CString& val,unsigned short& member)
{
	val = member;
}

void CConfigFile::ParamToString(CString& val,CString& member)
{
	val = member;
}
