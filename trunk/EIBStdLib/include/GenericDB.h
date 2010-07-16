#ifndef __GENERIC_DB_HEADER__
#define __GENERIC_DB_HEADER__

#include <map>
#include <fstream>

template<class K,class T>
class CGenericDB
{
public:
	CGenericDB(){};
	virtual ~CGenericDB(){};

	inline bool GetRecord(const K& key, T& record)
	{
		typename map<K,T>::iterator it;
		it = _data.find(key);
		if (it != _data.end()){
			record = it->second;
			return true;
		}
		return false;
	}

	bool AddRecord(const K& key, const T& record)
	{
		typename map<K,T>::iterator it;
		it = _data.find(key);
		if (it == _data.end()){
			_data.insert(pair<K,T>(key,record));
			return true;
		}
		return false;
	}

	bool DeleteRecord(const K& key)
	{
		typename map<K,T>::iterator it;
		it = _data.find(key);
		if (it != _data.end()){
			_data.erase(it);
			return true;
		}
		return false;
	}

	bool EditRecord(const K& key, const T& record)
	{
		if (DeleteRecord(key))
		{
			if(AddRecord(key,record)){
				return true;
			}
		}
		return false;
	}
	
	//will be called for each parameter read from file
	virtual void OnReadParamComplete(T& current_record, const CString& param,const CString& value) = 0;
	//will be called for each record read from file
	virtual void OnReadRecordComplete(T& current_record) = 0;
	//will be called for each record read from file
	virtual void OnReadRecordNameComplete(T& current_record, const CString& record_name) = 0;
	//will be called for each record that to be saved to the file
	virtual void OnSaveRecordStarted(const T& record,CString& record_name, map<CString,CString>& param_values) = 0;

	virtual void Init(const CString& file_name)
	{ 
		_file_name = file_name;
		
		ifstream myfile;
		myfile.open(_file_name.GetBuffer(),ios::in);
		if (myfile.fail())
		{
			ofstream out_file;
			out_file.open(_file_name.GetBuffer(),ios::out|ios::trunc);
			out_file.close();
		}
	}

	void Clear()
	{
		_data.clear();
	}

	bool Load()
	{
		ifstream myfile;
		myfile.open(_file_name.GetBuffer(),ios::in);
		if (myfile.fail()){
			ofstream out_file;
			out_file.open(_file_name.GetBuffer(),ios::out|ios::trunc);
			out_file.close();
			return false;
		}

		CString line;
		CString record_name;
		T record;
		bool first = true;
		while (!myfile.eof())
		{
			int index = string::npos;
			getline(myfile,line.GetSTDString());
			line.Trim();
			if(line.IsEmpty() || line[0] == '#')
			{
				continue;
			}
		
			if(line[0] == '[' && line[line.GetLength() - 1] == ']')
			{
				if(!first)
				{
					OnReadRecordComplete(record);
					record_name.Clear();
				}
				record_name = line.SubString(1,line.GetLength() - 2);
				OnReadRecordNameComplete(record,record_name);
				first = false;
			}
			else if (line[0] == '[' || line[line.GetLength() - 1] == ']'){
				return false;
			}
			else if((index = line.FindFirstOf('=')) != static_cast<int>(string::npos))
			{
				if(index == line.GetLength() -1){
					return false;	
				}
				CString param_name = line.SubString(0,index);
				CString param_value = line.SubString(index + 1,line.GetLength() - index - 1);
				if(!record_name.IsEmpty())
				{
					param_name.Trim();
					param_value.Trim();
					OnReadParamComplete(record,param_name,param_value);
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		if(!record_name.IsEmpty())
		{
			OnReadRecordComplete(record);
		}
		myfile.close();
		return true;
	}

	bool Save()
	{
		ofstream myfile;
		myfile.open(_file_name.GetBuffer(),ios::out|ios::trunc);
		if (myfile.fail()){	
			return false;
		}

		typename map<K,T>::iterator it;

		for ( it=_data.begin() ; it != _data.end(); it++ )
		{
			CString name;
			map<CString,CString> params_values;
			OnSaveRecordStarted(it->second,name,params_values);
			myfile << "[" << name.GetBuffer() << "]" << endl;
			//write the record data
			map<CString,CString>::iterator pv_it;
			for ( pv_it=params_values.begin() ; pv_it != params_values.end(); pv_it++ )
			{
				myfile << pv_it->first.GetBuffer() << " = " << pv_it->second << endl;
			}
			
			myfile << endl;
		}

		myfile.close();
		return true;
	}

protected:
	map<K,T> _data;
	CString _file_name;
};

#endif
