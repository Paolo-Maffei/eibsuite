#include "cli.h"
#include "Protocol.h"

static void GetInputFromUser(const CString& msg, CString& input)
{
	std::fputs(msg.GetBuffer(), stdout);
	std::getline( std::cin, input.GetSTDString());
}

static bool ValidateBool(const CString& input, bool& val)
{
	if(input.GetLength() > 1){
		return false;
	}
	if(input[0] != 'Y' && input[0] != 'y' && input[0] != 'n' && input[0] != 'N'){
		return false;
	}
	if(input[0] == 'Y' || input[0] == 'y')
		val = true;
	else
		val = false;

	return true;
}

static bool ValidateRangeInt(const CString& input, int& val,int min, int max)
{
	stringstream ss(CString(input).GetBuffer());
	if((ss >> val).fail()){
		return false;
	}
	if(val < min || val > max){
		return false;
	}
	return true;
}

static bool ValidateInt(const CString& input, int& val)
{
	stringstream ss(CString(input).GetBuffer());
	if((ss >> val).fail()){
		return false;
	}
	return true;
}

bool ConsoleCLI::Getint(const CString& msg, int& val, int def)
{
	bool keep_asking;
	do
	{
		CString new_msg(msg);
		new_msg += " [";
		new_msg += def;
		new_msg += "] ";
		CString input;
		GetInputFromUser(new_msg, input);
		if(input.GetLength() == 0){
			val = def;
			return true;
		}
		keep_asking = !ValidateInt(input,val);
		if(keep_asking){
			std::puts("Illegal input. please try again...");
		}
	}while(keep_asking);
	return true;
}

bool ConsoleCLI::GetCString(const CString& msg, CString& val, const CString& def)
{
	CString new_msg(msg);
	new_msg += " [";
	new_msg += def;
	new_msg += "] ";
	CString input;
	GetInputFromUser(new_msg, input);
	if(input.GetLength() == 0){
		val = def;
		return true;
	}
	val = input;
	return true;
}

bool ConsoleCLI::Getbool(const CString& msg, bool& val, bool def)
{
	bool keep_asking;
	do
	{
		CString new_msg(msg);
		new_msg += def ? " [Y] " : " [N] ";
		CString input;
		GetInputFromUser(new_msg, input);
		if(input.GetLength() == 0){
			val = def;
			return true;
		}
		keep_asking = !ValidateBool(input,val);
		if(keep_asking){
			std::puts("Illegal input. please try again...");
		}
	}while(keep_asking);

	return true;
}

template <class T>
bool GetOptionBase(const CString& msg, const map<T,CString>& options, T& val, const T& def)
{
	if(options.size() == 0){
		return false;
	}
	CString input,new_msg;
	int i;
	typename map<T,CString>::const_iterator it;
	bool keep_asking;
	do
	{
		i = 1;
		new_msg = msg;
		new_msg += " [";
		new_msg += def;
		new_msg += "]";
		new_msg += CRLF;
		for(it = options.begin(); it != options.end(); ++it)
		{
			new_msg += "[";
			new_msg += i;
			new_msg += "] ";
			new_msg += it->second;
			new_msg += CRLF;
			++i;
		}
		GetInputFromUser(new_msg, input);
		if(input.GetLength() == 0){
			val = def;
			return true;
		}
		keep_asking = !ValidateRangeInt(input,i, 1, options.size());
		if(keep_asking){
			std::puts("Illegal input. please try again...");
		}
	}while (keep_asking);

	int temp = 1;
	for(it = options.begin(); it != options.end(); ++it)
	{
		if(i == temp){
			val = it->first;
			break;
		}
		++temp;
	}
	return true;
}

bool ConsoleCLI::GetStrOption(const CString& msg, const map<int,CString>& options, int& val, const int& def)
{
	return GetOptionBase<int>(msg,options,val,def);
}

bool ConsoleCLI::GetStrOption(const CString& msg, const map<CString,CString>& options, CString& val, const CString& def)
{
	return GetOptionBase<CString>(msg,options,val,def);
}
