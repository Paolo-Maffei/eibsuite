#ifndef __TRANSLATION_TABLE_HEADER__
#define __TRANSLATION_TABLE_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "CException.h"

using namespace std;

template<class L,class R>
class CTanslationTable
{

public:
	CTanslationTable(){};
	virtual ~CTanslationTable(){};

	bool InsertEntry(const L& left, const R& right)
	{
		if(IsExistLeft(left) || IsExistRight(right)){
			return false;
		}

		pair<typename map<L,R>::iterator,bool> ret1;
		ret1 = _left_table.insert(pair<L,R>(left,right));

		pair<typename map<R,L>::iterator,bool> ret2;
		ret2 = _right_table.insert(pair<R,L>(right,left));

		if(ret1.second && ret2.second)
		{
			(*ret1.first).second = (R)(*ret2.first).first;
			(*ret2.first).second = (L)(*ret1.first).first;
			return true;
		}
		return false;
	}

	bool FindByLeft(const L& key, R& value)
	{
		typename map<L,R>::iterator it;
		it = _left_table.find(key);
		if (it != _left_table.end()){
			value = it->second;
			return true;
		}
		return false;
	}

	bool FindByRight(const R& key, L& value)
	{
		typename map<R,L>::iterator it;
		it = _right_table.find(key);
		if (it != _right_table.end()){
			value = it->second;
			return true;
		}
		return false;
	}

	bool IsExistLeft(const L& key)
	{
		typename map<L,R>::iterator it;
		it = _left_table.find(key);
		if (it != _left_table.end()){
			return true;
		}
		return false;
	}

	bool IsExistRight(const R& key)
	{
		typename map<R,L>::iterator it;
		it = _right_table.find(key);
		if (it != _right_table.end()){
			return true;
		}
		return false;
	}

	int GetLength() const
	{ 
		ASSERT(_right_table.size() == _left_table.size());
		return _right_table.size();
	}



protected:
	map<L,R> _left_table;
	map<R,L> _right_table;
};

#endif
