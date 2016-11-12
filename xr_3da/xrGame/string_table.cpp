#include "stdafx.h"
#include "string_table.h"

#include "ui/xrUIXmlParser.h"
#include "xr_level_controller.h"
#include "../xrCore/OPFuncs/utils.h"

#include <vector>
#include <sstream>
#include <iterator>
#include <string>
#include <algorithm>

STRING_TABLE_DATA* CStringTable::pData = NULL;
BOOL CStringTable::m_bWriteErrorsToLog = FALSE;

//winsor
const std::string CStringTable::idDelimiter="#|";
const size_t CStringTable::fixedSize=4000;

CStringTable::CStringTable	()
{
	Init();
}

void CStringTable::Destroy	()
{
	xr_delete(pData);
}

void CStringTable::Init		()
{
	if(NULL != pData) return;
	
	pData				= xr_new<STRING_TABLE_DATA>();
	
	//��� �����, ���� �� ������ (NULL), �� ������ <text> � <string> � XML
	pData->m_sLanguage	= pSettings->r_string("string_table", "language");

	LPCSTR S			= pSettings->r_string("string_table", "files");
	if (S && S[0]) 
	{
		string128	xml_file;
		int			count = _GetItemCount	(S);
		for (int it=0; it<count; ++it)	
		{
			_GetItem	(S,it, xml_file);
			Load		(xml_file);
		}
	}
}

#ifndef TS_ENABLE
//#define TS_ENABLE
#endif

#ifndef ETS_DECLARE
	#ifdef TS_ENABLE
		#define ETS_DECLARE(x) extern CTimerStat x
		#define ETSS_DECLARE(x,y) CTimerStatScoped x(y)
		#define ETS_BEGIN(x) x.Begin()
		#define ETS_END(x) x.End()
	#else
		#define ETS_DECLARE(x)
		#define ETSS_DECLARE(x,y)
		#define ETS_BEGIN(x)
		#define ETS_END(x)
	#endif
#endif //ETS_DECLARE

ETS_DECLARE(g_mmLoad);
ETS_DECLARE(g_mmForOuter);
ETS_DECLARE(g_mmFOFind);
ETS_DECLARE(g_mmFORead);
ETS_DECLARE(g_mmFOIf);
ETS_DECLARE(g_mmFOElse);

void CStringTable::Load	(LPCSTR xml_file)
{
	ETSS_DECLARE(mmLoad, g_mmLoad);

	CUIXml						uiXml;
	string_path					xml_file_full;
	strconcat					(sizeof(xml_file_full),xml_file_full, xml_file, ".xml");
	string_path					_s;
	strconcat					(sizeof(_s),_s, STRING_TABLE_PATH, "\\", *(pData->m_sLanguage) );

	bool xml_result				= uiXml.Init(CONFIG_PATH, _s, xml_file_full);
	if(!xml_result)
		Debug.fatal(DEBUG_INFO,"string table xml file not found %s, for language %s", xml_file_full, *(pData->m_sLanguage));

	//����� ������ ���� ������� ������� � �����
	int string_num = uiXml.GetNodesNum		(uiXml.GetRoot(), "string");
	ETS_BEGIN(g_mmForOuter);
	for(int i=0; i<string_num; ++i)
	{
		LPCSTR string_name = uiXml.ReadAttrib(uiXml.GetRoot(), "string", i, "id", NULL);

		//VERIFY3					(pData->m_StringTable.find(string_name) == pData->m_StringTable.end(), "duplicate string table id", string_name);
		ETS_BEGIN(g_mmFOFind);
		if (!(pData->m_StringTable.find(string_name) == pData->m_StringTable.end()))
		{
			Msg("! WARNING: duplicate string table id %s. Ignoring.", string_name);
		};
		ETS_END(g_mmFOFind);
		ETS_BEGIN(g_mmFORead);
		LPCSTR string_text		= uiXml.Read(uiXml.GetRoot(), "string:text", i,  NULL);
		ETS_END(g_mmFORead);
		if (lstrlen(string_text)>fixedSize) //winsor
		{
			ETSS_DECLARE(mmFOIf, g_mmFOIf);
			//split long text into more lines	
			//Msg("Text in '%s' too long,splitted.",string_name);
			std::string string_value(string_text);
			std::vector<std::string> newIds;
			while(string_value.length()>fixedSize || string_value.length()!=0)
			{
				std::string newId(string_name);	newId+="_"+std::to_string(newIds.size());
				std::string part=string_value.substr(0,fixedSize);
				pData->m_StringTable[newId.c_str()] = part.c_str();
				string_value.erase(0,fixedSize);
				newIds.push_back(newId);
			}
			std::stringstream joinedStr;
			std::copy(newIds.begin(), newIds.end(), std::ostream_iterator<std::string>(joinedStr, idDelimiter.c_str()));
			pData->m_StringTable[string_name] = joinedStr.str().c_str();
		}
		else
		{
			ETSS_DECLARE(mmFOElse, g_mmFOElse);
			if(m_bWriteErrorsToLog && string_text)
				Msg("[string table] '%s' no translation in '%s'", string_name, *(pData->m_sLanguage));
			if (!string_text)
			{
				FATAL2("string table entry does not has a text",string_name);
			}

			STRING_VALUE str_val		= ParseLine(string_text, string_name, true);

			pData->m_StringTable[string_name] = str_val;
		}
	}
	ETS_END(g_mmForOuter);
}
void CStringTable::ReparseKeyBindings()
{
	if(!pData)					return;
	STRING_TABLE_MAP_IT it		= pData->m_string_key_binding.begin();
	STRING_TABLE_MAP_IT it_e	= pData->m_string_key_binding.end();

	for(;it!=it_e;++it)
	{
		pData->m_StringTable[it->first]			= ParseLine(*it->second, *it->first, false);
	}
}


STRING_VALUE CStringTable::ParseLine(LPCSTR str, LPCSTR skey, bool bFirst)
{
//	LPCSTR str = "1 $$action_left$$ 2 $$action_right$$ 3 $$action_left$$ 4";
	xr_string			res;
	int k = 0;
	const char*			b;
	#define ACTION_STR "$$ACTION_"

//.	int LEN				= (int)xr_strlen(ACTION_STR);
	#define LEN			9

	string256				buff;
	string256				srcbuff;
	bool	b_hit			= false;

	while( (b = strstr( str+k,ACTION_STR)) !=0 )
	{
		buff[0]				= 0;
		srcbuff[0]			= 0;
		res.append			(str+k, b-str-k);
		const char* e		= strstr( b+LEN,"$$" );

		int len				= (int)(e-b-LEN);

		strncpy				(srcbuff,b+LEN, len);
		srcbuff[len]		= 0;
		GetActionAllBinding	(srcbuff, buff, sizeof(buff) );
		res.append			(buff, xr_strlen(buff) );

		k					= (int)(b-str);
		k					+= len;
		k					+= LEN;
		k					+= 2;
		b_hit				= true;
	};

	if(k<(int)xr_strlen(str)){
		res.append(str+k);
	}

	if(b_hit&&bFirst) pData->m_string_key_binding[skey] = str;

	return STRING_VALUE(res.c_str());
}

STRING_VALUE CStringTable::translate (const STRING_ID& str_id) const
{
	VERIFY					(pData);

	STRING_VALUE res =  pData->m_StringTable[str_id];
	if(!res)
	{
		if(m_bWriteErrorsToLog && *str_id != NULL && xr_strlen(*str_id)>0)
			Msg("[string table] '%s' has no entry", *str_id);

		if (str_id==nullptr)
			return str_id;
		std::string resStr(str_id.c_str());
		if (resStr.front()=='"' && resStr.back()=='"') 
			OPFuncs::trimq(resStr);
		return resStr.c_str();
		//return str_id;
	}

	bool splited=false;
	std::string value(res.c_str());
	std::string unitedValue;
	size_t delimiterPos=0;
	while ((delimiterPos=value.find(idDelimiter))!=std::string::npos)
	{
		std::string parsedId=value.substr(0,delimiterPos);
		value.erase(0,delimiterPos+idDelimiter.length());
		splited=true;
		unitedValue+=translate(parsedId.c_str()).c_str();
	};
	if (splited)
	{
		//Msg("%s united from parts",str_id);
		return unitedValue.c_str();
	}
	else
		return pData->m_StringTable[str_id];
}
