#ifndef __HTML_HEADER__
#define __HTML_HEADER__



//basic html tags
#define HTML_OPEN "<html>"
#define HTML_CLOSE "</html>"

#define HTML_HEAD_OPEN "<head>"
#define HTML_HEAD_CLOSE "</head>"

#define HTML_BODY_OPEN "<body>"
#define HTML_BODY_CLOSE "</body>"

#define HTML_H1_OPEN "<h1>"
#define HTML_H1_CLOSE "</h1>"
#define HTML_H2_OPEN "<h2>"
#define HTML_H2_OPEN_ADD_ATTRIBUTES "<h2 "
#define HTML_H2_CLOSE "</h2>"
#define HTML_H3_OPEN_ADD_ATTRIBUTES "<h3 "
#define HTML_H3_CLOSE "</h3>"

#define HTML_LINK_OPEN "<a "
#define HTML_LINK_CLOSE "</a>"

// html form-related tags
#define HTML_FORM_OPEN			"<form "
#define HTML_FORM_CLOSE			"</form>"
#define HTML_ACTION_EQUALS		" action="
#define HTML_METHOD_EQUALS		" method="
#define HTML_METHOD_EQUALS_POST " method=\"post\""
#define HTML_METHOD_EQUALS_GET	" method=\"get\""
#define HTML_INPUT_OPEN			"<INPUT "
#define HTML_TYPE_EQUALS_TEXT	" type=\"text\""
#define HTML_TYPE_EQUALS_SUBMIT " type=\"submit\""
#define HTML_TYPE_EQUALS_HIDDEN " type=\"hidden\""
#define HTML_TYPE_EQUALS_BUTTON " type=\"button\""
#define HTML_TYPE_EQUALS		" type="
#define HTML_NAME_EQUALS		" name="
#define HTML_VALUE_EQUALS		" value="
#define HTML_ID_EQUALS			" id="
#define HTML_MAXLENGTH_EQUALS	" maxlength="
#define HTML_SIZE_EQUALS		" size="
#define HTML_INPUT_SUBMIT_SEND  "<INPUT TYPE=\"SUBMIT\" VALUE=\"Send\">"

// html script-related tags
#define HTML_SCRIPT_TAG_OPEN		"<script"
#define HTML_SCRIPT_TAG_CLOSE		"</script>"
#define HTML_LANGUAGE_EQUALS		" language="
#define HTML_SOURCE_EQUALS			" src="

//html image-related tags
#define HTML_IMAGE_OPEN				"<img "
#define HTML_IMAGE_CLOSE			" />"
#define HTML_WIDTH_EQUALS			" width="
#define HTML_HEIGHT_EQUALS			" height="
#define HTML_BORDER_EQUALS			" border="
#define HTML_ALT_EQUALS				" alt="

// html table-related tags
#define HTML_TABLE_OPEN				"<table "
#define HTML_TABLE_CLOSE			"</table> "
#define HTML_TABLE_ROW_OPEN			"<TR>"
#define HTML_TABLE_ROW_CLOSE		"</TR>"
#define HTML_TABLE_ROW_OPEN_CENTERED	"<TR align=\"center\">"
#define HTML_TABLE_HEADER_OPEN		"<TH>"
#define HTML_TABLE_HEADER_CLOSE		"</TH>"
#define HTML_TABLE_DATA_OPEN		"<TD>"
#define HTML_TABLE_DATA_CLOSE		"</TD>"


#define HTML_BREAK				"<BR>"
#define HTML_BREAK_DOUBLE		"<BR><BR>"

//html list-related tags
#define HTML_BULLET_LIST_OPEN	"<ul>";
#define HTML_BULLET_LIST_CLOSE	"</ul>";
#define HTML_BULLET_LIST_ITEM	"<li>";

#define HTML_FONT_OPEN			"<font ";
#define HTML_FONT_CLOSE			"</font>";

#define HTML_TAG_CLOSE '>'
#define HTML_TAG_OPEN '<'

#define HTML_QUOTE	'"'
#define HTML_EQUALS '='

#define HTML_BORDER_1			" BORDER=\"1\" "
#define HTML_ALIGN_CENTER		" align=\"center\" "
#define HTML_ALIGN_LEFT			" align=\"left\" "

//parameters names
#define PARAM_NAME_COMMAND		"cmd"
#define PARAM_COMMAND_VALUE_GLOBAL_HISTORY		1
#define PARAM_COMMAND_VALUE_FUNCTION_HISTORY	2
#define PARAM_COMMAND_VALUE_SEND_COMMAND		3
#define PARAM_COMMAND_VALUE_SCHEDULE_COMMAND	4

#define PARAM_NAME_FUNCTION					"function"
#define PARAM_NAME_FUNCTION_DEST_ADDRESS	"dst_address"
#define PARAM_NAME_FUNCTION_APCI			"apci"
#define PARAM_NAME_FUNCTION_DATETIME		"datetime"

#endif

