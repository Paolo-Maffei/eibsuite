#ifndef __CONTENT_GENERATOR_HEADER__
#define __CONTENT_GENERATOR_HEADER__

//location of javascripts, gifs, etc.
#define HOSTED_FILES_LOCATION	"http://eibsuite.googlepages.com/"

#include "CString.h"
#include "StatsDB.h"
#include "Html.h"

#define ADD_TO_CONTENT(x) content += x

class CContentGenerator
{
public:
	CContentGenerator();
	virtual ~CContentGenerator();

	//the pages
	static void Page_Main(CDataBuffer& content,const CString& domain, const CString& error_message);

	static void Page_HistoryPerFunction(CDataBuffer& content, CEibAddress& function, CEIBObjectRecord& record, const CString& error_message);
	static void Form_HistoryPerFunction(CDataBuffer& content, const CString& error_message);

	static void Form_SendCommandToEIB(CDataBuffer& content, const CString& error_message);
	static void Page_AckCommandToEIB(CDataBuffer& content, bool successful, const CString& error_message);

	static void Form_ScheduleCommand(CDataBuffer& content, const CString& error_message);
	static void Page_AckScheduleCommand(CDataBuffer& content);

	static void Page_HistoryGlobal(CDataBuffer& content, CStatsDB& db, const CString& error_message);
	static void Page_UnAuthorizedAction(CDataBuffer& content);
	static void Generate_Error_Line(CDataBuffer& content, const CString& error_message);

private:
	static void AddLinkToMainPage(CDataBuffer& content);
	static void AddLink(CDataBuffer& content, const CString& target_url, const CString& display_text = "", bool close_tag = true);
	static void AddImage(CDataBuffer& content, const CString& source_url, int width, int height, int border = 0, const CString& alternative_text = "");
	static void AddHeader(CDataBuffer& content, const int heading_level, const CString& heading_text);
	static void AddInputTag(CDataBuffer& content, const CString&  type, const CString& name, const CString& value, const CString& id = "", int maxlength = 0, int size = 0);
	static void AddFormTag(CDataBuffer& content, const CString& action, const CString& method = "post");
	static void AddScriptTag(CDataBuffer& content, const CString& language, const CString& type, const CString& source);
	static void AddEibRecordTable(CDataBuffer& content, CEibAddress& function, CEIBObjectRecord& record);
};

#endif

