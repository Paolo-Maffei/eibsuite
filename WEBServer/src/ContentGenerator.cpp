#include "ContentGenerator.h"
#include "Utils.h"
#include "WEBServer.h"
#include "EIBAddress.h"

using namespace EibStack;

CContentGenerator::CContentGenerator(){}
CContentGenerator::~CContentGenerator(){}

void CContentGenerator::Page_Main(CDataBuffer& content,const CString& domain, const CString& error_message)
{

	content.Clear();
	ADD_TO_CONTENT(HTML_OPEN);
	ADD_TO_CONTENT(HTML_HEAD_OPEN);
	ADD_TO_CONTENT("<link rel=\"shortcut icon\" href=\"/favicon.ico\" type=\"image/x-icon\" />");
	ADD_TO_CONTENT(HTML_HEAD_CLOSE);
	
	ADD_TO_CONTENT(HTML_BODY_OPEN);

	AddHeader(content,1,"EIB Web Interface");
	
	Generate_Error_Line(content,error_message);

	/*	
	<ul>
	<li><a href="/?cmd=1">Global History</a><br /><br />
	<li><a href="/?cmd=2">Function History</a><br /><br />
	<li><a href="/?cmd=3">Send Command</a><BR></ul>
*/
	ADD_TO_CONTENT(HTML_BULLET_LIST_OPEN);

		ADD_TO_CONTENT(HTML_BULLET_LIST_ITEM);
		AddLink(content, "/?cmd=1", "Global History");
		ADD_TO_CONTENT(HTML_BREAK_DOUBLE);
		
		ADD_TO_CONTENT(HTML_BULLET_LIST_ITEM);
		AddLink(content, "/?cmd=2", "Function History");
		ADD_TO_CONTENT(HTML_BREAK_DOUBLE);

		ADD_TO_CONTENT(HTML_BULLET_LIST_ITEM);
		AddLink(content, "/?cmd=3", "Send Command");
		ADD_TO_CONTENT(HTML_BREAK_DOUBLE);

		ADD_TO_CONTENT(HTML_BULLET_LIST_ITEM);
		AddLink(content, "/?cmd=4", "Schedule Command");
		ADD_TO_CONTENT(HTML_BREAK_DOUBLE);

	ADD_TO_CONTENT(HTML_BULLET_LIST_CLOSE);

	ADD_TO_CONTENT("<hr />");
	AddLink(content, "http://www.codeplex.com/eibsuite", "EIB Suite homepage");
	ADD_TO_CONTENT(HTML_BREAK);


	ADD_TO_CONTENT(HTML_BODY_CLOSE);
	ADD_TO_CONTENT(HTML_CLOSE);
}

void CContentGenerator::Page_HistoryPerFunction(CDataBuffer& content, CEibAddress& function, CEIBObjectRecord& record, const CString& error_message)
{
	content.Clear();
	content += HTML_OPEN;
	content += HTML_HEAD_OPEN;
	content += HTML_HEAD_CLOSE;
	content += HTML_BODY_OPEN;
	
	AddHeader(content,1,"EIB Web Interface");

	Generate_Error_Line(content,error_message);
	
	AddEibRecordTable(content, function, record);
		
	content += "<hr />";
	AddLinkToMainPage(content);

	content += HTML_BODY_CLOSE;
	content += HTML_CLOSE;

}

void CContentGenerator::AddEibRecordTable(CDataBuffer& content, CEibAddress& function, CEIBObjectRecord& record)
{
	ADD_TO_CONTENT(HTML_H2_OPEN_ADD_ATTRIBUTES);
	ADD_TO_CONTENT(HTML_ALIGN_CENTER);
	ADD_TO_CONTENT(HTML_TAG_CLOSE);
	ADD_TO_CONTENT(HTML_FONT_OPEN);
	ADD_TO_CONTENT("face=\"sans-serif\" color=\"purple\">");
	ADD_TO_CONTENT("History for device: ");
	ADD_TO_CONTENT(HTML_FONT_CLOSE);
	ADD_TO_CONTENT(HTML_FONT_OPEN);
	ADD_TO_CONTENT("face=\"sans-serif\" color=\"blue\">");
		
	ADD_TO_CONTENT(function.ToString());
		
	ADD_TO_CONTENT(HTML_FONT_CLOSE);
	ADD_TO_CONTENT(HTML_H2_CLOSE);
	

	ADD_TO_CONTENT(HTML_TABLE_OPEN);
	ADD_TO_CONTENT(HTML_ALIGN_CENTER);
	ADD_TO_CONTENT(HTML_BORDER_1);
	ADD_TO_CONTENT(HTML_TAG_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_ROW_OPEN);
	ADD_TO_CONTENT(HTML_TABLE_HEADER_OPEN);
	ADD_TO_CONTENT("Time");
	ADD_TO_CONTENT(HTML_TABLE_HEADER_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_HEADER_OPEN);
	ADD_TO_CONTENT("Value");
	ADD_TO_CONTENT(HTML_TABLE_HEADER_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_ROW_CLOSE);

	//add logical: yes/no
	deque<CEIBRecord>::const_iterator it;
	for(it = record.GetHistory().begin(); it != record.GetHistory().end(); ++it)
	{
		ADD_TO_CONTENT(HTML_TABLE_ROW_OPEN_CENTERED);
		ADD_TO_CONTENT(HTML_TABLE_DATA_OPEN);
		ADD_TO_CONTENT(HTML_FONT_OPEN);
		ADD_TO_CONTENT("face=\"sans-serif\" color=\"purple\" size=\"2\">");
		ADD_TO_CONTENT((*it).GetTime().Format());
		ADD_TO_CONTENT(HTML_FONT_CLOSE);		
		ADD_TO_CONTENT(HTML_TABLE_DATA_CLOSE);

		ADD_TO_CONTENT(HTML_TABLE_DATA_OPEN);
		ADD_TO_CONTENT(HTML_FONT_OPEN);
		ADD_TO_CONTENT("face=\"sans-serif\" color=\"purple\" size=\"2\">");
		ADD_TO_CONTENT((*it).PrintValue());
		ADD_TO_CONTENT(HTML_FONT_CLOSE);		
		ADD_TO_CONTENT(HTML_TABLE_DATA_CLOSE);
		ADD_TO_CONTENT(HTML_TABLE_ROW_CLOSE);
	}

	ADD_TO_CONTENT(HTML_TABLE_CLOSE);
}


void CContentGenerator::Form_HistoryPerFunction(CDataBuffer& content, const CString& error_message)
{
	content.Clear();
	ADD_TO_CONTENT(HTML_OPEN);
	ADD_TO_CONTENT(HTML_HEAD_OPEN);
	ADD_TO_CONTENT(HTML_HEAD_CLOSE);
	ADD_TO_CONTENT(HTML_BODY_OPEN);
	
	AddHeader(content,1,"EIB Web Interface");

	Generate_Error_Line(content,error_message);

	AddFormTag(content,"/");
/*
 <FORM action="/" method="post">
    Enter Function Hex value: <BR>
    <INPUT type="text" name="function">
    <INPUT type="submit" value="Send">
	<input type="hidden" name="cmd" value="2">
 </FORM>
*/
	content += "Enter EIB Address (x/x/x): ";
	content += HTML_BREAK;

	AddInputTag(content,"TEXT",PARAM_NAME_FUNCTION,"","");

	AddInputTag(content,"SUBMIT","","SEND","");

	AddInputTag(content,"HIDDEN",PARAM_NAME_COMMAND,"2","");
	
	content += HTML_FORM_CLOSE;

	content += "<hr />";
	AddLinkToMainPage(content);

}

void CContentGenerator::Form_SendCommandToEIB(CDataBuffer& content, const CString& error_message)
{
	/*
	<FORM ACTION="/" METHOD="post">
	<TABLE BORDER="1">
	  <TR>
		<TH>Destination Address:</TH>
		<TD>
			<INPUT TYPE="TEXT" NAME="dst_addr">
		</TD>
	  </TR>
	  <TR>
		<TH>APCI (value):</TH>
		<TD>
			<INPUT TYPE="TEXT" NAME="apci">
		</TD>
	  </TR>
	</TABLE>
	<input type="hidden" name="cmd" value="3">
	<INPUT TYPE="SUBMIT" VALUE="Send">
	</FORM>
*/
	content.Clear();
	content += HTML_OPEN;
	content += HTML_HEAD_OPEN;
	content += HTML_HEAD_CLOSE;
	content += HTML_BODY_OPEN;
	
	AddHeader(content,1,"EIB Web Interface");

	Generate_Error_Line(content,error_message);

	content += "Enter Function and value in Hex format (Use the 0x perfix): ";
	content += HTML_BREAK;

	AddFormTag(content,"/");

	content += HTML_TABLE_OPEN;
	content += HTML_BORDER_1;
	content += HTML_TAG_CLOSE;

	content += HTML_TABLE_ROW_OPEN;
	content += HTML_TABLE_HEADER_OPEN;
	content += "Destination Address:";
	content += HTML_TABLE_HEADER_CLOSE;
	content += HTML_TABLE_DATA_OPEN;
	AddInputTag(content,"TEXT",PARAM_NAME_FUNCTION_DEST_ADDRESS,"","",6,20);
	content += HTML_TABLE_DATA_CLOSE;
	content += HTML_TABLE_ROW_CLOSE;

	content += HTML_TABLE_ROW_OPEN;
	content += HTML_TABLE_HEADER_OPEN;
	content += "APCI (value):";
	content += HTML_TABLE_HEADER_CLOSE;
	content += HTML_TABLE_DATA_OPEN;
	AddInputTag(content,"TEXT",PARAM_NAME_FUNCTION_APCI,"0x","",20,20);
	content += HTML_TABLE_DATA_CLOSE;
	content += HTML_TABLE_ROW_CLOSE;
	
	
	content += HTML_TABLE_CLOSE;
	AddInputTag(content,"HIDDEN",PARAM_NAME_COMMAND,PARAM_COMMAND_VALUE_SEND_COMMAND,"");
	AddInputTag(content,"SUBMIT","","SEND","");
	content += HTML_FORM_CLOSE;	

	content += HTML_BODY_CLOSE;
	content += HTML_CLOSE;
}
void CContentGenerator::Page_AckCommandToEIB(CDataBuffer& content, bool successful, const CString& error_message)
{
	content.Clear();
	content += HTML_OPEN;
	content += HTML_HEAD_OPEN;
	content += HTML_HEAD_CLOSE;
	content += HTML_BODY_OPEN;
	
	AddHeader(content,1,"EIB Web Interface");

	Generate_Error_Line(content,error_message);

	content += HTML_H2_OPEN;
	content += successful ? "Command sent successfuly" : "Command was not sent to eib server";
	content += HTML_H2_CLOSE;	

	content += "<hr />";
	AddLinkToMainPage(content);

	content += HTML_BODY_CLOSE;
	content += HTML_CLOSE;
}

void CContentGenerator::Page_HistoryGlobal(CDataBuffer& content, CStatsDB& statsDB, const CString& error_message)
{
	
	map<CEibAddress,CEIBObjectRecord> db = statsDB.GetData();
	map<CEibAddress,CEIBObjectRecord>::iterator stats_it;

	content.Clear();
	content += HTML_OPEN;
	content += HTML_HEAD_OPEN;
	content += HTML_HEAD_CLOSE;
	content += HTML_BODY_OPEN;
	
	AddHeader(content,1,"EIB Web Interface");

	Generate_Error_Line(content,error_message);

	//CEIBObjectRecord& record;
	for(stats_it = db.begin(); stats_it != db.end(); ++stats_it)
	{		
		CEibAddress addr = (*stats_it).first;
		AddEibRecordTable(content, addr, (*stats_it).second);
	}	
	
	if (db.size() == 0 )
	{
		content += HTML_H3_OPEN_ADD_ATTRIBUTES;
		content += HTML_ALIGN_CENTER;
		content += HTML_TAG_CLOSE;
		content += HTML_FONT_OPEN;
		content += "face=\"sans-serif\" color=\"purple\">";
		content += "EIB History is empty";
		content += HTML_FONT_CLOSE;
		content += HTML_H3_CLOSE;
	}	

	content += "<hr />";
	AddLinkToMainPage(content);
	
	content += HTML_BODY_CLOSE;
	content += HTML_CLOSE;
}


void CContentGenerator::Generate_Error_Line(CDataBuffer& content, const CString& error_message)
{
	if (error_message.GetLength()>0)
	{
		content += "<hr/>";
		content += HTML_FONT_OPEN;
		content += "face=\"sans-serif\" color=\"red\" size=\"2\">";
		content += "Error: ";
		content += error_message;
		content += HTML_FONT_CLOSE;	
		content += "<hr/><br />";
	}
}

void CContentGenerator::AddLinkToMainPage(CDataBuffer& content)
{
	content += HTML_LINK_OPEN;
	content += "href=";
	content += HTML_QUOTE;
	content += "http://";
	CString domain = CWEBServer::GetInstance().GetDomain();
	content += domain;
	content += HTML_QUOTE;
	content += HTML_TAG_CLOSE;
	content += "WEB Server homepage";
	content += HTML_LINK_CLOSE;
	content += HTML_BREAK;
}
void CContentGenerator::AddImage(CDataBuffer& content, 
								 const CString& source_url, 
								 int width, 
								 int height, 
								 int border, 
								 const CString& alternative_text)
{
	ADD_TO_CONTENT(HTML_IMAGE_OPEN);
	ADD_TO_CONTENT(HTML_SOURCE_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(source_url);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(HTML_WIDTH_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(CString(width));
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(HTML_HEIGHT_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(CString(height));
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(HTML_BORDER_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(CString(border));
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(HTML_ALT_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(alternative_text);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(HTML_TAG_CLOSE);
	ADD_TO_CONTENT(HTML_LINK_CLOSE);
}
void CContentGenerator::AddLink(CDataBuffer& content, const CString& target_url, const CString& display_text, bool close_tag)
{
	ADD_TO_CONTENT(HTML_LINK_OPEN);
	ADD_TO_CONTENT("href=");
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(target_url);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(HTML_TAG_CLOSE);
	if (display_text.GetLength() > 0)
	{		
		ADD_TO_CONTENT(display_text);		
	}
	if (close_tag)
	{
		ADD_TO_CONTENT(HTML_LINK_CLOSE);	
	}
}

void CContentGenerator::AddHeader(CDataBuffer& content, const int heading_level, const CString& heading_text)
{
	// <h1>text</h1>
	ADD_TO_CONTENT("<h");
	ADD_TO_CONTENT(CString(heading_level));
	ADD_TO_CONTENT(">");
	ADD_TO_CONTENT(heading_text);
	ADD_TO_CONTENT("</h");
	ADD_TO_CONTENT(CString(heading_level));
	ADD_TO_CONTENT(HTML_TAG_CLOSE);
}

void CContentGenerator::AddInputTag(CDataBuffer& content, 
									const CString& type, 
									const CString& name, 
									const CString& value, 
									const CString& id,  
									int maxlength, 
									int size)
{
	//  <input type="Text" id="demo1" maxlength="25" size="25">
	//	or
	//	<INPUT TYPE="TEXT" NAME="dst_addr">
	//	or
	//	<INPUT TYPE="HIDDEN" NAME="cmd" VALUE="3">
	//	or
	//	<INPUT TYPE="SUBMIT" VALUE="Send">
	
	ADD_TO_CONTENT(HTML_INPUT_OPEN);
	ADD_TO_CONTENT(HTML_TYPE_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(type);
	ADD_TO_CONTENT(HTML_QUOTE);		
	if (name.GetLength() > 0)
	{
		ADD_TO_CONTENT(HTML_NAME_EQUALS);
		ADD_TO_CONTENT(HTML_QUOTE);
		ADD_TO_CONTENT(name);
		ADD_TO_CONTENT(HTML_QUOTE);
	}
	if (value.GetLength() > 0)
	{
		ADD_TO_CONTENT(HTML_VALUE_EQUALS);
		ADD_TO_CONTENT(HTML_QUOTE);
		ADD_TO_CONTENT(value);
		ADD_TO_CONTENT(HTML_QUOTE);
	}
	if (id.GetLength() > 0)
	{
		ADD_TO_CONTENT(HTML_ID_EQUALS);
		ADD_TO_CONTENT(HTML_QUOTE);
		ADD_TO_CONTENT(id);
		ADD_TO_CONTENT(HTML_QUOTE);
	}
	//TODO: compare type with uppercase too
	if ((maxlength > 0) && ((type == "password") || (type == "text")))
	{
		ADD_TO_CONTENT(HTML_MAXLENGTH_EQUALS);
		ADD_TO_CONTENT(HTML_QUOTE);
		ADD_TO_CONTENT(CString(maxlength));
		ADD_TO_CONTENT(HTML_QUOTE);
	}
	//TODO: compare type with uppercase too
	if ((size > 0) && (type != "hidden"))
	{
		ADD_TO_CONTENT(HTML_SIZE_EQUALS);
		ADD_TO_CONTENT(HTML_QUOTE);
		ADD_TO_CONTENT(CString(size));
		ADD_TO_CONTENT(HTML_QUOTE);
	}
	ADD_TO_CONTENT(HTML_TAG_CLOSE);	
}


void CContentGenerator::AddFormTag(CDataBuffer& content, const CString& action, const CString& method)
{
	//<FORM action="/" method="post">
	ADD_TO_CONTENT(HTML_FORM_OPEN);
	ADD_TO_CONTENT(HTML_ACTION_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(action);
	ADD_TO_CONTENT(HTML_QUOTE);
		
	ADD_TO_CONTENT(HTML_METHOD_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(method);
	ADD_TO_CONTENT(HTML_QUOTE);

	ADD_TO_CONTENT(HTML_TAG_CLOSE);
}
void CContentGenerator::AddScriptTag(CDataBuffer& content, const CString& language, const CString& type, const CString& source)
{	
	//	"<script language="javascript" type="text/javascript" src="http://eibsuite.googlepages.com/datetimepicker.js"></script>"
	ADD_TO_CONTENT(HTML_SCRIPT_TAG_OPEN);
	ADD_TO_CONTENT(HTML_LANGUAGE_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(language);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(HTML_TYPE_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(type);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(HTML_SOURCE_EQUALS);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(source);
	ADD_TO_CONTENT(HTML_QUOTE);
	ADD_TO_CONTENT(HTML_TAG_CLOSE);
	ADD_TO_CONTENT(HTML_SCRIPT_TAG_CLOSE);		
}

void CContentGenerator::Form_ScheduleCommand(CDataBuffer& content, const CString& error_message)
{
	CString file_location(HOSTED_FILES_LOCATION);

	content.Clear();
	ADD_TO_CONTENT(HTML_OPEN);
	ADD_TO_CONTENT(HTML_HEAD_OPEN);
	
	AddScriptTag(content, "javascript", "text/javascript", file_location + "datetimepicker.js");
	
	ADD_TO_CONTENT(HTML_HEAD_CLOSE);
	ADD_TO_CONTENT(HTML_BODY_OPEN);
	AddHeader(content,1,"EIB Web Interface");
	Generate_Error_Line(content,error_message);

	ADD_TO_CONTENT("Enter EIB Destination Address and value: ");
	ADD_TO_CONTENT(HTML_BREAK);
	
	AddFormTag(content,"/");
	
	ADD_TO_CONTENT(HTML_TABLE_OPEN);
	ADD_TO_CONTENT(HTML_BORDER_1);
	ADD_TO_CONTENT(HTML_TAG_CLOSE);

	ADD_TO_CONTENT(HTML_TABLE_ROW_OPEN);
	ADD_TO_CONTENT(HTML_TABLE_HEADER_OPEN);
	ADD_TO_CONTENT("Destination Address:");
	ADD_TO_CONTENT(HTML_TABLE_HEADER_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_DATA_OPEN);
	AddInputTag(content,"TEXT",PARAM_NAME_FUNCTION_DEST_ADDRESS,"","",6,20);
	ADD_TO_CONTENT(HTML_TABLE_DATA_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_ROW_CLOSE);

	ADD_TO_CONTENT(HTML_TABLE_ROW_OPEN);
	ADD_TO_CONTENT(HTML_TABLE_HEADER_OPEN);
	ADD_TO_CONTENT("APCI (value):");
	ADD_TO_CONTENT(HTML_TABLE_HEADER_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_DATA_OPEN);
	AddInputTag(content,"TEXT",PARAM_NAME_FUNCTION_APCI,"0x","",20,20);
	ADD_TO_CONTENT(HTML_TABLE_DATA_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_ROW_CLOSE);
	
	ADD_TO_CONTENT(HTML_TABLE_ROW_OPEN);
	ADD_TO_CONTENT(HTML_TABLE_HEADER_OPEN);
	ADD_TO_CONTENT("Select Date/Time");
	ADD_TO_CONTENT(HTML_TABLE_HEADER_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_DATA_OPEN);
	AddInputTag(content,"TEXT",PARAM_NAME_FUNCTION_DATETIME,"","id1",20,20);
	AddLink(content,"javascript:NewCal('id1','ddmmmyyyy',true,24,'dropdown',true)","",false);
	AddImage(content, file_location + "cal.gif",16,16,0,"Pick a date");
	ADD_TO_CONTENT(HTML_LINK_CLOSE);
	ADD_TO_CONTENT("pick a date..");
	ADD_TO_CONTENT(HTML_TABLE_DATA_CLOSE);
	ADD_TO_CONTENT(HTML_TABLE_ROW_CLOSE);

	ADD_TO_CONTENT(HTML_TABLE_CLOSE);
	AddInputTag(content,"HIDDEN",PARAM_NAME_COMMAND,PARAM_COMMAND_VALUE_SCHEDULE_COMMAND);
	AddInputTag(content,"SUBMIT","","SCHEDULE IT");
	ADD_TO_CONTENT(HTML_FORM_CLOSE);	

	ADD_TO_CONTENT("<hr />");
	CWEBServer::GetInstance().GetCollector()->ScheduleToHtml(content);

	ADD_TO_CONTENT(HTML_BODY_CLOSE);
	ADD_TO_CONTENT(HTML_CLOSE);
}
void CContentGenerator::Page_AckScheduleCommand(CDataBuffer& content)
{
}
