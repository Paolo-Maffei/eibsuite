#include "StringTokenizer.h"

StringTokenizer::StringTokenizer(const CString& str, const CString& delim)
{

   if ((str.GetLength() == 0) || (delim.GetLength() == 0)) return;

   _token_str = str;
   _delim     = delim;

  /*
     Remove sequential delimiter
  */
   int curr_pos = 0;

   while(true)
   {
      if ((curr_pos = _token_str.Find(delim,curr_pos)) != static_cast<int>(string::npos))
      {
         curr_pos += _delim.GetLength();

         while(_token_str.Find(_delim,curr_pos) == curr_pos)
         {
            _token_str.Erase(curr_pos,_delim.GetLength());
         }
      }
      else
       break;
   }

   /*
     Trim leading delimiter
   */
   if (_token_str.Find(_delim,0) == 0)
   {
      _token_str.Erase(0,_delim.GetLength());
   }

   /*
     Trim ending delimiter
   */
   curr_pos = 0;
   if ((curr_pos = _token_str.RFind(_delim)) != static_cast<int>(string::npos))
   {
      if (curr_pos != (_token_str.GetLength() - _delim.GetLength())) return;
	  _token_str.Erase(_token_str.GetLength() - _delim.GetLength(),_delim.GetLength());
   }

}


int StringTokenizer::CountTokens()
{

   unsigned int prev_pos = 0;
   int num_tokens        = 0;

   if (_token_str.GetLength() > 0)
   {
      num_tokens = 0;

      unsigned int curr_pos = 0;
      while(true)
      {
         if ((curr_pos = _token_str.Find(_delim,curr_pos)) != string::npos)
         {
            num_tokens++;
            prev_pos  = curr_pos;
            curr_pos += _delim.GetLength();
         }
         else
          break;
      }
      return ++num_tokens;
   }
   else
   {
      return 0;
   }

}


bool StringTokenizer::HasMoreTokens()
{
   return (_token_str.GetLength() > 0);
}


CString StringTokenizer::NextToken()
{

   if (_token_str.GetLength() == 0)
     return "";

   CString  tmp_str = "";
   unsigned int pos = _token_str.Find(_delim,0);

   if (pos != string::npos)
   {
      tmp_str   = _token_str.SubString(0,pos);
      _token_str = _token_str.SubString(pos + _delim.GetLength(),_token_str.GetLength()-pos - 1);
   }
   else
   {
      tmp_str   = _token_str.SubString(0,_token_str.GetLength());
      _token_str.Clear();
   }

   return tmp_str;
}


int StringTokenizer::NextIntToken()
{
   return NextToken().ToInt();
}

int64 StringTokenizer::NextInt64Token()
{
	
	return NextToken().ToInt64();
}

double StringTokenizer::NextFloatToken()
{
   return NextToken().ToDouble();
}


CString StringTokenizer::NextToken(const CString& delimiter)
{
   if (_token_str.GetLength() == 0)
     return "";

   CString  tmp_str = "";
   unsigned int pos = _token_str.Find(delimiter,0);

   if (pos != string::npos)
   {
      tmp_str   = _token_str.SubString(0,pos);
	  _token_str = _token_str.SubString(pos + delimiter.GetLength(),_token_str.GetLength() - pos);
   }
   else
   {
      tmp_str   = _token_str.SubString(0,_token_str.GetLength());
      _token_str = "";
   }

   return tmp_str;
}


CString StringTokenizer::RemainingString()
{
   return _token_str;
}


CString StringTokenizer::FilterNextToken(const CString& filterStr)
{
   CString  tmp_str = NextToken();
   unsigned int currentPos = 0;

   while((currentPos = tmp_str.Find(filterStr,currentPos)) != string::npos)
   {
      tmp_str.Erase(currentPos,filterStr.GetLength());
   }

   return tmp_str;
}
