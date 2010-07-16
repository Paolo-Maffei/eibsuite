#include "HttpSession.h"

CSessionManager::CSessionManager()
{
	JTCSynchronized sync(*this);

	srand((unsigned int)time(NULL));
	_container.clear();
}

CSessionManager::~CSessionManager()
{
}

void CSessionManager::RegenerateSessionID(const CString& current,CString& new_session)
{
	if (!RemoveSession(current)){
		throw CEIBException(GeneralError,"Session not found.");
	}
	GenerateNewSession(EMPTY_STRING,new_session);
}

void CSessionManager::GenerateNewSession(const CString& user_name,CString& session_id)
{
	JTCSynchronized sync(*this);

	CString tmp = user_name;
	tmp += "_";
	CTime now;
	now.SetNow();
	tmp += now.GetTime();

	CDigest digest;
	session_id = digest.HashString(tmp);

	CHttpSessionData sd;
	sd._session_id = session_id;

	_container.insert(pair<CString,CHttpSessionData>(session_id,sd));
}

bool CSessionManager::RemoveSession(const CString& session_id)
{
	JTCSynchronized sync(*this);

	map<CString,CHttpSessionData>::const_iterator it;
	it = _container.find(session_id);
	if (it != _container.end()){
		_container.erase(session_id);
		return true;
	}
	return false;
}

bool CSessionManager::FindSession(const CString& session_id)
{
	JTCSynchronized sync(*this);

	map<CString,CHttpSessionData>::iterator it;
	it = _container.find(session_id);
	if (it != _container.end() && !it->second.IsExpired()){
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

CHttpSessionData::CHttpSessionData(int timeout)
{
	_expiration_time.SetNow();
	_expiration_time += DEFAULT_SESSION_TIMEOUT * 60; //move timeout to future (in minuets)
}

CHttpSessionData::~CHttpSessionData()
{
}

bool CHttpSessionData::IsExpired() const
{
	CTime now;
	now.SetNow();

	if (_expiration_time > now){
		return false;
	}
	return true;
}
