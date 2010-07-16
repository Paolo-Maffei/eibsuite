#include "URLEncoding.h"

using namespace std;

CString URLEncoder::Encode(const CString& str)
{
	int len = str.GetLength();
	char* buff = new char[len + 1];
	strcpy(buff,str.GetBuffer());
	CString ret;
	for(int i=0;i<len;i++)
	{
		if(IsOrdinaryChar(buff[i]))
		{
			ret += buff[i];
		}else if(buff[i] == ' ')
		{
			ret += "+";
		}else 
		{
			char tmp[6];
			sprintf(tmp,"%%%x",buff[i]);
			ret += tmp;
		}
	}
	delete[] buff;
	return ret;
}

bool URLEncoder::IsOrdinaryChar(char c)
{
	char ch = tolower(c);
	if(ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd' || ch == 'e' 
		|| ch == 'f' || ch == 'g' || ch == 'h' || ch == 'i' || ch == 'j' 
		|| ch == 'k' || ch == 'l' || ch == 'm' || ch == 'n' || ch == 'o' 
		|| ch == 'p' || ch == 'q' || ch == 'r' || ch == 's' || ch == 't' 
		|| ch == 'u' || ch == 'v' || ch == 'w' || ch == 'x' || ch == 'y' 
		|| ch == 'z' || ch == '0' || ch == '1' || ch == '2' || ch == '3' 
		|| ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' 
		|| ch == '9') {
		return true;
	}
	return false;
}

CString URLEncoder::Decode(const CString& str)
{
	int len = str.GetLength();
	char* buff = new char[len + 1];
	strcpy(buff,str.GetBuffer());
	string ret;
	for(int i=0; i<len; i++) 
	{
		if(buff[i] == '+') {
			ret += " ";
		}
		else if(buff[i] == '%')
		{
			char tmp[4];
			char hex[4];			
			hex[0] = buff[++i];
			hex[1] = buff[++i];
			hex[2] = '\0';		
			sprintf(tmp,"%c",ConvertToDec(hex));
			ret += tmp;
		}
		else 
		{
			ret += buff[i];
		}
	}
	delete[] buff;
	return ret;
}

int URLEncoder::ConvertToDec(const char* hex) {
	char buff[12];
	sprintf(buff,"%s",hex);
	int ret = 0;
	int len = strlen(buff);
	for(int i=0;i<len;i++) {
		char tmp[4];
		tmp[0] = buff[i];
		tmp[1] = '\0';
		GetAsDec(tmp);
		int tmp_i = atoi(tmp);
		int rs = 1;
		for(int j=i;j<(len-1);j++) {
			rs *= 16;
		}
		ret += (rs * tmp_i);
	}
	return ret;
}

void URLEncoder::GetAsDec(char* hex)
{
	char tmp = tolower(hex[0]);
	if(tmp == 'a') 
	{
		strcpy(hex,"10");
	}
	else if(tmp == 'b')
	{
		strcpy(hex,"11");
	}
	else if(tmp == 'c')
	{
		strcpy(hex,"12");
	}
	else if(tmp == 'd')
	{
		strcpy(hex,"13");
	}
	else if(tmp == 'e')
	{
		strcpy(hex,"14");
	}
	else if(tmp == 'f')
	{
		strcpy(hex,"15");
	}
}
