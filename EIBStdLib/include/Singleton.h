#ifndef __SINGLETON_HEADER__
#define __SINGLETON_HEADER__

template<class T>
class CSingleton
{
public:
	CSingleton()
	{
		static bool created = false;
		if(!created){
			_instance = new T();
			created = true;
		}
	}

	virtual ~CSingleton();
	
	static T& GetInstance()
	{
		return *_instance;
	}

private:
	static T* _instance;
}

#endif