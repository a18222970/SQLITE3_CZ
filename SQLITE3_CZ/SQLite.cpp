
#include "SQLite.h"
#include <iostream>

//#include <comdef.h>

#ifdef DEBUG //����Ƿ�Ϊ����ģʽ
#define DBGprint(...) printf(__VA_ARGS__)
#else
#define DBGprint(...)
#endif
SQLite::SQLite()
{
	biao[0] = _T("cxMingDan");  //�����¼��
	biao[1] = _T("dqjincheng");  //��ǰ��
	biao[2] = _T("csJinCheng");  //����������ʼ��
	biao[3] = _T("id");  //id

	bChengYuan[0] = _T("xh");  //���;��id�������б��1��
	bChengYuan[1] = _T("th32ProcessID");  //pid;2
	bChengYuan[2] = _T("szExeFile");  //exe����;3

	bChengYuan[3] = _T("szModule");  //ģ������;2
	bChengYuan[4] = _T("szExePath");  //ģ��·��;3
	bChengYuan[5] = _T("szTzm");  //����������;4
	bChengYuan[6] = _T("szFenLei");  //��������,1Ϊ������,2Ϊ������,3Ϊδ֪,4Ϊ��Ϸ;5

	bChengYuan[7] = _T("name");  //�û���;1
	bChengYuan[8] = _T("pass");  //����;2
	bChengYuan[9] = _T("dqShiJian");  //����ʱ��;3
	bChengYuan[10] = _T("dengLu");  //��ס�����Զ���½;4
	bChengYuan[11] = _T("dingShi");  //��ʱ����;5
	bChengYuan[12] = _T("YXshiJian");  //��������ʱ��;6

// 	bMing[0] = _T("cxMingDan");  //�����¼��
// 	bMing[1] = _T("dqjincheng");  //��ǰ��
// 	bMing[2] = _T("csJinCheng");  //����������ʼ��
// 	bMing[3] = _T("id");  //id
}

SQLite::~SQLite()
{
}



void SQLite::NCshiFang(char* a, int b)
{
	switch (b)
	{
	case 1:
		delete[] a;
		a = NULL;
		break;
	case 2:
		delete a;
		a = NULL;
		break;
	default:
		break;
	}
}

void SQLite::NCshiFang(char** a, int b)
{
	for (int i=0;i<b;i++)
	{
		delete[] a[i];
		a[i] = NULL;
	}
	delete[] a;
	a = NULL;
}

char* SQLite::UTF8ZhaunUTF16(const char* utf8)
{
	/*
	* ���ַ���ӳ�䵽 UTF-16�����ַ����ַ���
	*/
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	/*
	* �� UTF-16�����ַ����ַ���ӳ�䵽���ַ���
	*/
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr)
	{
		delete[] wstr;
		wstr = NULL;
	}
	return str;
}

BOOL SQLite::zhuanChar(CString sj, char* shuzu, int len)
{
	int ilen;//�ֽڸ���
	int nk = sj.GetLength();//���ô˷����ɷ��ض����е��ַ����� ���������� null ��ֹ����
	/*
	* ����: WideCharToMultiByte
	* ����: �� UTF-16�����ַ����ַ���ӳ�䵽���ַ��������ַ�����һ�����Զ��ֽ��ַ������˺�����������ڴ�й©��ת��������ݶ�ʧ
	* ����1: ����ִ��ת���Ĵ���ҳ��ת����ĸ�ʽ,CP_ACP=ϵͳĬ�ϵ� Windows ANSI ����ҳ
	* ����2: ָʾת�����͵ı�־����û��������Щ��־ʱ���ú���ִ�еø��졣
	* ����3: ָ��Ҫת���� Unicode �ַ�����ָ�롣
	* ����4: Ҫת����Դ�ַ����Ĵ�С�����ַ�Ϊ��λ�������ߣ�����ַ����Կ��ַ���β������Խ��˲�������Ϊ -1,Ϊ-1ʱ���صĳ��Ȱ��������Ŀ��ַ�
	* ����5: ת���󱣴����ݵĻ�����(ָ��)
	* ����6: �� ����4 ָʾ�Ļ������Ĵ�С�����ֽ�Ϊ��λ��������˲�������Ϊ 0����ú������� ����4 ����Ļ�������С,�˲���Ϊ����ʱ����ת���ɹ�
	* ����7: һ������ΪNULL
	* ����8: ��������NULL
	* ����ֵ: ����ɹ����򷵻�д�� ����5 ָ��Ļ��������ֽ�����
	*        ��������ɹ����� ����6 Ϊ 0���򷵻�ֵ�� ����5 ָʾ�Ļ���������Ĵ�С�����ֽ�Ϊ��λ��
	*/
	ilen = WideCharToMultiByte(CP_ACP, 0, sj, nk, NULL, 0, NULL, NULL); //��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����.�˲���Ϊ�˱����ڴ�й©
	if (len < ilen)//����������鳤��С��Ҫת�����ַ�������,����FALSE
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_ACP, 0, sj, nk+1, shuzu, ilen+1, NULL, NULL);//���ֽڱ���ת���ɶ��ֽڱ���
	/*�������*/
	shuzu[ilen + 1] = '\0';
	if (ERROR_INSUFFICIENT_BUFFER == GetLastError() || ERROR_INVALID_FLAGS == GetLastError() ||
		ERROR_INVALID_PARAMETER == GetLastError() || ERROR_NO_UNICODE_TRANSLATION == GetLastError())
	{
		printf("JSON char* ת�� errorNum = %d\n", GetLastError());//��ȡ�������
		return FALSE;
	}
	return TRUE;
}

char* SQLite::zhuanUTF8(CString a)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, a, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, a, -1, str, len, NULL, NULL);
	if (ERROR_INSUFFICIENT_BUFFER == GetLastError() || ERROR_INVALID_FLAGS == GetLastError() ||
		ERROR_INVALID_PARAMETER == GetLastError() || ERROR_NO_UNICODE_TRANSLATION == GetLastError())
	{
		printf("cstring ת�� UTF8 = %d\n", GetLastError());//��ȡ�������
		return FALSE;
	}
	return str;
}

BOOL SQLite::zhuanUTF8(CString a, char* shuzu, int lLen)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, a, -1, NULL, 0, NULL, NULL);
	if (lLen<len+1)
	{
		return FALSE;
	}
	//memset(shuzu, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, a, -1, shuzu, len, NULL, NULL);
	shuzu[lLen + 1] = '\0';
	if (ERROR_INSUFFICIENT_BUFFER == GetLastError() || ERROR_INVALID_FLAGS == GetLastError() ||
		ERROR_INVALID_PARAMETER == GetLastError() || ERROR_NO_UNICODE_TRANSLATION == GetLastError())
	{
		printf("cstring ת�� UTF8 = %d\n", GetLastError());//��ȡ�������
		return FALSE;
	}
	return TRUE;
}

WCHAR* SQLite::UTF8ToUnicode(const char* str)
{
	
	int i = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, -1, NULL, 0);
	WCHAR* strUnicode = new   WCHAR[i];
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, -1, strUnicode, i);
	return strUnicode;
	//delete[]strUnicode;
}

char* SQLite::UnicodeToUTF8(const WCHAR* pText)
{
	int i = WideCharToMultiByte(CP_UTF8, 0, pText, -1, NULL, 0, NULL, NULL); //���뻺������С�ǿ��ַ���         
	char* strUTF8 = new   char[i];
	WideCharToMultiByte(CP_UTF8, 0, pText, -1, strUTF8, i, NULL, NULL);
	return strUTF8;
	//delete[]strUTF8;
}

std::wstring SQLite::ASCIIToUNICODE(char cArry[])
{
	/*ʹ��CP_ACP����ҳ��ʵ����ANSI��Unicode֮���ת��*/
	int nLen = MultiByteToWideChar(CP_ACP, 0, cArry, -1, NULL, NULL);   //��MultiByteToWideChar()�ĵ��ĸ��β���Ϊ-1,���ɷ��س���

	wchar_t* pTemp = new wchar_t[nLen];       //newһ��wchar_t�ռ䣬����Unicode��
	memset(pTemp, 0, nLen * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, cArry, -1, (LPWSTR)pTemp, nLen);
	std::wstring str = pTemp;
	if (pTemp)
	{
		
		delete[] pTemp;
		pTemp = NULL;
	}
	return str;
}

std::string SQLite::UNICODEToASCII(wchar_t cArry[])
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, cArry, -1, NULL, 0, NULL, NULL);
	char* pTemp = new char[nLen];                //new һ��char���飬����ANSI��
	memset(pTemp, 0, nLen);
	WideCharToMultiByte(CP_ACP, 0, cArry, -1, pTemp, nLen, NULL, NULL);
	std::string str = pTemp;
	if (pTemp)
	{
		delete[] pTemp;
		pTemp = NULL;
	}
	return str;
}

std::wstring SQLite::UTF8ToUnicode(const std::string& str)
{
	int  unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pUnicode = new  wchar_t[unicodeLen];

	memset(pUnicode, 0, unicodeLen * sizeof(wchar_t));

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);

	std::wstring  rt = pUnicode;

	if (pUnicode)

	{

		delete[] pUnicode;
		pUnicode = NULL;

	}

	return  rt;
}

std::string SQLite::UnicodeToUTF8(const std::wstring& str)
{
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);

	char* pElementText = new char[iTextLen];

	memset(pElementText, 0, iTextLen);

	WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);

	std::string strText;

	strText = pElementText;

	if (pElementText)

	{

		delete[] pElementText;

		pElementText = NULL;

	}

	return strText;
}

std::string SQLite::cstrZhuanStr(CString cstr ,int a)
{
	std::string str;
	switch (a)
	{
	case 1:
		str = CW2A(cstr.GetString());
		return str;
		break;
	case 2:
		str = (LPCSTR)cstr.GetBuffer(0);//δ֪
		return str;
	case 3:
		str = (CStringA)cstr;
		return str;
	default:
		return str = "";
		break;
	}
	
}

CString SQLite::strZhuanCstr(std::string str, int a)
{
	CString cstr;
	switch (a)
	{
	case 1:
		cstr.Format(L"%s", str.c_str());
		return cstr;
	case 2:
		cstr = str.c_str();
		return cstr;
	case 3:
		cstr = CString(str.c_str());
		return cstr;
	default:
		return cstr = "";
		break;
	}
	
}

BOOL SQLite::is_str_utf8(const char* str)
{
	unsigned int nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
	unsigned char chr = *str;
	bool bAllAscii = true;
	for (unsigned int i = 0; str[i] != '\0'; ++i) 
	{
		chr = *(str + i);
		//�ж��Ƿ�ASCII����,�������,˵���п�����UTF8,ASCII��7λ����,���λ���Ϊ0,0xxxxxxx
		if (nBytes == 0 && (chr & 0x80) != 0) 
		{
			bAllAscii = false;
		}
		if (nBytes == 0) 
		{
			//�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
			if (chr >= 0x80) 
			{
				if (chr >= 0xFC && chr <= 0xFD) 
				{
					nBytes = 6;
				}
				else if (chr >= 0xF8) 
				{
					nBytes = 5;
				}
				else if (chr >= 0xF0) 
				{
					nBytes = 4;
				}
				else if (chr >= 0xE0) 
				{
					nBytes = 3;
				}
				else if (chr >= 0xC0) 
				{
					nBytes = 2;
				}
				else 
				{
					return false;
				}
				nBytes--;
			}
		}
		else 
		{
			//���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			//����Ϊ��Ϊֹ
			nBytes--;
		}
	}
	//Υ��UTF8�������
	if (nBytes != 0) 
	{
		return false;
	}
	if (bAllAscii) 
	{ //���ȫ������ASCII, Ҳ��UTF8
		return true;
	}
	return true;
}

BOOL SQLite::is_str_gbk(const char* str)
{
	unsigned int nBytes = 0;//GBK����1-2���ֽڱ���,�������� ,Ӣ��һ��
	unsigned char chr = *str;
	bool bAllAscii = true; //���ȫ������ASCII,
	for (unsigned int i = 0; str[i] != '\0'; ++i) {
		chr = *(str + i);
		if ((chr & 0x80) != 0 && nBytes == 0) {// �ж��Ƿ�ASCII����,�������,˵���п�����GBK
			bAllAscii = false;
		}
		if (nBytes == 0) {
			if (chr >= 0x80) 
			{
				if (chr >= 0x81 && chr <= 0xFE) 
				{
					nBytes = +2;
				}
				else {
					return false;
				}
				nBytes--;
			}
		}
		else 
		{
			if (chr < 0x40 || chr>0xFE) 
			{
				return false;
			}
			nBytes--;
		}//else end
	}
	if (nBytes != 0) 
	{   //Υ������
		return false;
	}
	if (bAllAscii) 
	{ //���ȫ������ASCII, Ҳ��GBK
		return true;
	}
	return true;
}

BOOL SQLite::LJsjk(const char* paht, BOOL duoXianCheng)
{
	if (duoXianCheng)
	{
		int pd = sqlite3_open_v2(
			paht,//sql���ļ���·��
			&sqla,// һ���򿪵����ݿ�ʵ��
			/*
			* SQLITE_OPEN_NOMUTEX: �������ݿ����������ڶ��߳�ģʽ(û��ָ�����߳�ģʽ�������)
			  SQLITE_OPEN_FULLMUTEX���������ݿ����������ڴ���ģʽ��
			  SQLITE_OPEN_NOMUTEX    �µ����ݿ����ӽ�ʹ�á����̡߳� �߳�ģʽ
			  SQLITE_OPEN_SHAREDCACHE�����������ڹ�����ģʽ��
			  SQLITE_OPEN_PRIVATECACHE�����������ڷǹ�����ģʽ��
			  SQLITE_OPEN_READWRITE��ָ�����ݿ����ӿ��Զ�д��
			  SQLITE_OPEN_CREATE��������ݿⲻ���ڣ��򴴽���
			*/
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE,
			NULL
		);
		if (pd == SQLITE_OK)
		{
			printf("ite���ݿ����ӳɹ�\n");
			return TRUE;
		}
		else
		{
			printf("ite���ݿ�����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
			return FALSE;
		}
	} 
	else
	{
		int pd = sqlite3_open_v2(
			paht,//sql���ļ���·��
			&sqla,// һ���򿪵����ݿ�ʵ��
			/*
			* SQLITE_OPEN_NOMUTEX: �������ݿ����������ڶ��߳�ģʽ(û��ָ�����߳�ģʽ�������)
			  SQLITE_OPEN_FULLMUTEX���������ݿ����������ڴ���ģʽ��
			  SQLITE_OPEN_NOMUTEX    �µ����ݿ����ӽ�ʹ�á����̡߳� �߳�ģʽ
			  SQLITE_OPEN_SHAREDCACHE�����������ڹ�����ģʽ��
			  SQLITE_OPEN_PRIVATECACHE�����������ڷǹ�����ģʽ��
			  SQLITE_OPEN_READWRITE��ָ�����ݿ����ӿ��Զ�д��
			  SQLITE_OPEN_CREATE��������ݿⲻ���ڣ��򴴽���
			*/
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_SHAREDCACHE,
			NULL
		);
		if (pd == SQLITE_OK)
		{
			printf("ite���ݿ����ӳɹ�\n");
			return TRUE;
		}
		else
		{
			printf("ite���ݿ�����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
			return FALSE;
		}
	}
	
}


sqlite3* SQLite::LJsjk(sqlite3* sql, const char* paht, BOOL duoXianCheng)
{
	int sd = SQLITE_OPEN_NOMUTEX;//���߳�
	if (!duoXianCheng)
	{
		sd = SQLITE_OPEN_FULLMUTEX;//���л�
		
	}
	int pd = sqlite3_open_v2(
		paht,//sql���ļ���·��
		&sql,// һ���򿪵����ݿ�ʵ��
		/*
		* SQLITE_OPEN_NOMUTEX: �������ݿ����������ڶ��߳�ģʽ(û��ָ�����߳�ģʽ�������)
		  SQLITE_OPEN_FULLMUTEX�������µ����ݿ����ӽ�ʹ�á����л��� �߳�ģʽ
		  SQLITE_OPEN_NOMUTEX    �µ����ݿ����ӽ�ʹ�á����̡߳� �߳�ģʽ��
		  SQLITE_OPEN_SHAREDCACHE�����������ڹ�����ģʽ��
		  SQLITE_OPEN_PRIVATECACHE�����������ڷǹ�����ģʽ��
		  SQLITE_OPEN_READWRITE��ָ�����ݿ����ӿ��Զ�д��
		  SQLITE_OPEN_CREATE��������ݿⲻ���ڣ��򴴽���
		*/
		SQLITE_OPEN_READWRITE | sd | SQLITE_OPEN_SHAREDCACHE,
		NULL
	);
	if (pd == SQLITE_OK)
	{
		if (!duoXianCheng)
		{
			printf("ite���ݿ� ���л� ���ӳɹ�\n");
			return sql;
		}
		printf("ite���ݿ� ���߳� ���ӳɹ�\n");
		return sql;
	}
	else
	{
		printf("ite���ݿ�����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		return sql;
	}
	
}


sqlite3* SQLite::LJsjk_zhiDu(sqlite3* sql, const char* paht, BOOL duoXianCheng)
{
	if (duoXianCheng)
	{
		int pd = sqlite3_open_v2(
			paht,//sql���ļ���·��
			&sql,// һ���򿪵����ݿ�ʵ��
			/*
			* SQLITE_OPEN_NOMUTEX: �������ݿ����������ڶ��߳�ģʽ(û��ָ�����߳�ģʽ�������)
			  SQLITE_OPEN_FULLMUTEX���������ݿ����������ڴ���ģʽ
			  SQLITE_OPEN_NOMUTEX    �µ����ݿ����ӽ�ʹ�á����̡߳� �߳�ģʽ��
			  SQLITE_OPEN_SHAREDCACHE�����������ڹ�����ģʽ��
			  SQLITE_OPEN_PRIVATECACHE�����������ڷǹ�����ģʽ��
			  SQLITE_OPEN_READWRITE��ָ�����ݿ����ӿ��Զ�д��
			  SQLITE_OPEN_READONLY  ���ݿ���ֻ��ģʽ��
			  SQLITE_OPEN_CREATE��������ݿⲻ���ڣ��򴴽���
			*/
			SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE,
			NULL
		);
		if (pd == SQLITE_OK)
		{
			printf("ite ֻ�����ݿ� ���߳� ���ӳɹ�\n");
			return sql;
		}
		else
		{
			printf("ite���ݿ�����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
			return sql;
		}
	}
	else
	{
		int pd = sqlite3_open_v2(
			paht,//sql���ļ���·��
			&sql,// һ���򿪵����ݿ�ʵ��
			/*
			* SQLITE_OPEN_NOMUTEX: �������ݿ����������ڶ��߳�ģʽ(û��ָ�����߳�ģʽ�������)
			  SQLITE_OPEN_FULLMUTEX�������µ����ݿ����ӽ�ʹ�á����л��� �߳�ģʽ
			  SQLITE_OPEN_NOMUTEX    �µ����ݿ����ӽ�ʹ�á����̡߳� �߳�ģʽ
			  SQLITE_OPEN_SHAREDCACHE�����������ڹ�����ģʽ��
			  SQLITE_OPEN_PRIVATECACHE�����������ڷǹ�����ģʽ��
			  SQLITE_OPEN_READWRITE��ָ�����ݿ����ӿ��Զ�д��
			  SQLITE_OPEN_READONLY  ���ݿ���ֻ��ģʽ��
			  SQLITE_OPEN_CREATE��������ݿⲻ���ڣ��򴴽���
			*/
			SQLITE_OPEN_READONLY | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_SHAREDCACHE,
			NULL
		);
		if (pd == SQLITE_OK)
		{
			printf("ite���ݿ����ӳɹ�\n");
			return sql;
		}
		else
		{
			printf("ite ֻ�����ݿ� ���л� ����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
			return sql;
		}
	}
}

BOOL SQLite::GBsjk()
{
	if (sqlite3_close(sqla) == SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		printf("ite �ر�ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		return FALSE;
	}
}

BOOL SQLite::GBsjk(sqlite3* sql)
{
	if (sqlite3_close(sql) == SQLITE_OK)
	{
		return TRUE;
	}
	else
	{
		printf("ite �ر�ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		return FALSE;
	}
}

BOOL SQLite::ZJshuJu()
{
	CString s= L"INSERT INTO id(name, pass, dqShiJian, dengLu) VALUES('�����ߺ���', '22',1524544545,1);";
	
	char ss[160] = { 0 };
	zhuanChar(s, ss, 160);
	const char* str = "INSERT INTO id(name, pass) VALUES('�����ߺ���', '22');";
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare(sqla, ss, -1, &stmt, NULL);
	if (pd==SQLITE_OK)
	{
		sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		printf("ite ���ӳɹ�\n");
		return TRUE;
	}
	else
	{
		printf("ite ����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
}

BOOL SQLite::ZJshuJu(sqlite3* sql)
{
	CString s = L"INSERT INTO id(name, pass, dqShiJian, dengLu) VALUES('�������ߺ���abc', '22',1524544545,1);";
	char* sq = NULL;
	char ss[260] = { 0 };
	zhuanChar(s, ss, 160);
	//zhuanUTF8(s, ss, 260);
	//sq = zhuanUTF8(s);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare_v2(sql, ss, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		printf("ite ���ӳɹ�\n");
		return TRUE;
	}
	else
	{
		printf("ite ����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
}

BOOL SQLite::ZJid(sqlite3* sql, CString id)
{
	CString s;
	s.Format(_T("INSERT INTO id(name, pass, dqShiJian, dengLu) VALUES('%s', '',0,0);"), id);
	char* sq = NULL;
	char ss[260] = { 0 };
	zhuanChar(s, ss, 160);
	//zhuanUTF8(s, ss, 260);
	//sq = zhuanUTF8(s);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare_v2(sql, ss, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		printf("ite ID���ӳɹ�\n");
		return TRUE;
	}
	else
	{
		printf("ite ID����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
}

BOOL SQLite::ZJshuJu(sqlite3* sql, CString shuJu)
{
	CString s;
	s.Format(_T("INSERT INTO id(name, pass, dqShiJian, dengLu) VALUES('%s', '22',1524544545,1);"), shuJu);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sql, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		printf("ite���ݿ����ӳɹ�\n");
		return TRUE;
	}
	else
	{
		printf("ite ����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
}

BOOL SQLite::ZJshuJu(sqlite3* sql, CString biao, CString zD, CString sJ)
{
	CString s;
	s.Format(_T("INSERT INTO %s(%s) VALUES('%s');"), biao,zD,sJ);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sql, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		printf("ite���ݿ����ӳɹ�\n");
		return TRUE;
	}
	else
	{
		printf("ite ����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
}

BOOL SQLite::ZJshuJu(sqlite3* sql, CString biao, CString zD, long sJ)
{
	CString s;
	s.Format(_T("INSERT INTO %s(%s) VALUES(%d);"), biao, zD, sJ);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sql, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		printf("ite���ݿ����ӳɹ�\n");
		return TRUE;
	}
	else
	{
		printf("ite ����ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
}

BOOL SQLite::CXshuJU()
{
	CString s = L"SELECT * FROM id;";
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sqla, s, -1, &stmt, NULL);
	if (pd==SQLITE_OK)
	{
		int lx = 0;
		int daXiao = 0;
		std::string szfc;
		CString cszfc;
		printf("��ѯ�ɹ�\n");
		while (sqlite3_step(stmt)==SQLITE_ROW)// ÿ��һ��sqlite3_step()������stmt������ͻ�ָ����һ����¼
		{
			lx = sqlite3_column_type(stmt, 1);//���ؽ���еĳ�ʼ�������͵� �������ʹ���
			switch (lx)
			{
			case SQLITE_INTEGER://64λ�з�������
				printf("���� 1\n");
				sqlite3_column_int64(stmt, 1);//64 λ�������
				break;
			case SQLITE_FLOAT://64�з���λ������
				printf("������ 2\n");
				break;
			case SQLITE_TEXT://�ַ���
				printf("�ַ��� 3\n");
				/*const void *  */
				szfc = (wchar_t)sqlite3_column_text16(stmt, 1);
				/*ȷ���� BLOB ���ַ����Ĵ�С*/
				/*�������� UTF-16 �ַ������� sqlite3_column_bytes() ���ַ���ת��Ϊ UTF-8, Ȼ�󷵻��ֽ���*/
				//int daXiao=sqlite3_column_bytes(stmt, 1);
				daXiao = sqlite3_column_bytes16(stmt, 1);
				
				break;
			case SQLITE_BLOB://����ֵ
				printf("����ֵ 4\n");
				/*const void *  */
				sqlite3_column_text16(stmt, 1);
				/*ȷ���� BLOB ���ַ����Ĵ�С*/
				/*�������� UTF-8 �ַ������� sqlite3_column_bytes16() ���ַ���ת��Ϊ UTF-16��Ȼ�󷵻��ֽ���*/
				//int daXiao=sqlite3_column_bytes(stmt, 1);
				daXiao = sqlite3_column_bytes16(stmt, 1);
				break;
			case SQLITE_NULL://��ֵ
				printf("��ֵ 5\n");
				break;
			default:
				printf("���򷵻����ʹ���\n");
				return FALSE;
				break;
			}
			const char* s1 = (char* )sqlite3_column_text16(stmt, 1);// ȡ����1���ֶε��ַ���
			//s = sqlite3_column_text16(stmt, 1);
			s.Format(L"%s", sqlite3_column_text16(stmt, 1)); 
			int ss = sqlite3_column_bytes16(stmt, 1);
			//int ss = sqlite3_column_int(stmt, 2);// ȡ����2���ֶε�����
			printf("mingz:%S;daima%d\n", s, ss);
		}
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return TRUE;
	} 
	else
	{
		printf("ite ��ѯʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		sqlite3_finalize(stmt);
		return FALSE;
	}
}

BOOL SQLite::CXshuJU(sqlite3* sql)
{

	CString s = L"SELECT * FROM id;";
	std::wcout.imbue(std::locale("chs")); //����̨�ı����Ǹ���ϵͳ������wcoutҪ���������
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sql, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		int lx = 0;
		int daXiao = 0;
		std::string szfc;
		CString cszfc;
		const void* ssa;
		const wchar_t* ssaa;
		const char* csa;
		USES_CONVERSION;//const WCHAR *ת��Ϊconst char *
		printf("��ѯ�ɹ�\n");
		while (sqlite3_step(stmt) == SQLITE_ROW)// ÿ��һ��sqlite3_step()������stmt������ͻ�ָ����һ����¼
		{
			lx = sqlite3_column_type(stmt, 1);//���ؽ���еĳ�ʼ�������͵� �������ʹ���
			switch (lx)
			{
			case SQLITE_INTEGER://64λ�з�������
				printf("����\n");
				sqlite3_column_int64(stmt, 1);//64 λ�������
				break;
			case SQLITE_FLOAT://64�з���λ������
				printf("������\n");
				break;
			case SQLITE_TEXT://�ַ���
				printf("�ַ���\n");
				/*const void *  */
				ssa = sqlite3_column_text16(stmt, 1);
				ssaa= (wchar_t*)sqlite3_column_text16(stmt, 1);
				cszfc.Format(L"%s", sqlite3_column_text16(stmt, 1));

				/*ȷ���� BLOB ���ַ����Ĵ�С*/
				/*�������� UTF-16 �ַ������� sqlite3_column_bytes() ���ַ���ת��Ϊ UTF-8, Ȼ�󷵻��ֽ���*/
				//int daXiao=sqlite3_column_bytes(stmt, 1);
				daXiao = sqlite3_column_bytes16(stmt, 1);
				
				std::wcout << ssaa << "\n";
				
				csa = W2A(ssaa);//const WCHAR *ת��Ϊconst char *
				printf("%s", csa);
				MessageBox(NULL, cszfc, L"ok", NULL);
				break;
			case SQLITE_BLOB://����ֵ
				printf("����ֵ\n");
				/*const void *  */
				sqlite3_column_text16(stmt, 1);
				/*ȷ���� BLOB ���ַ����Ĵ�С*/
				/*�������� UTF-8 �ַ������� sqlite3_column_bytes16() ���ַ���ת��Ϊ UTF-16��Ȼ�󷵻��ֽ���*/
				//int daXiao=sqlite3_column_bytes(stmt, 1);
				daXiao = sqlite3_column_bytes16(stmt, 1);
				break;
			case SQLITE_NULL://��ֵ
				printf("��ֵ\n");
				break;
			default:
				printf("���򷵻����ʹ���\n");
				return FALSE;
				break;
			}
		}
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return TRUE;
	}
	else
	{
		printf("ite ��ѯʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt);
		return FALSE;
	}
}

unsigned long SQLite::cXid_int(sqlite3* sql, CString zD)
{
	CString s;
	s.Format(_T("SELECT %s FROM id;"),zD);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sql, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
// 		int lie = sqlite3_column_count(stmt);//��ȡ���ص�����
// 		CString lieMZ;
// 		char lieSL = 0;
// 		for (int i = 0; i < lie; i++)
// 		{
// 			std::string lieM = sqlite3_column_name(stmt, i);
// 			lieMZ = CString(lieM.c_str());
// 			if (lieMZ == L"dqShiJian")
// 			{
// 				lieSL = i;
// 				break;
// 			}
// 		}

		unsigned short lie = getLie(stmt, zD);
		unsigned long sj32 = 0;//32λ�з�������
		while (sqlite3_step(stmt) == SQLITE_ROW)// ÿ��һ��sqlite3_step()������stmt������ͻ�ָ����һ����¼
		{
			//sj64 = sqlite3_column_int64(stmt, 1);//64 λ�������
			sj32 = sqlite3_column_int(stmt, lie);//64 λ�������
		}
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return sj32;
	}
	else
	{
		printf("ite ��ѯʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt);
		return 0;
	}
}

CString SQLite::cXid_txt(sqlite3* sql, CString zD)
{
	CString s;
	s.Format(_T("SELECT %s FROM id;"), zD);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sql, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		// 		int lie = sqlite3_column_count(stmt);//��ȡ���ص�����
		// 		CString lieMZ;
		// 		char lieSL = 0;
		// 		for (int i = 0; i < lie; i++)
		// 		{
		// 			std::string lieM = sqlite3_column_name(stmt, i);
		// 			lieMZ = CString(lieM.c_str());
		// 			if (lieMZ == L"dqShiJian")
		// 			{
		// 				lieSL = i;
		// 				break;
		// 			}
		// 		}

		unsigned short lie = getLie(stmt, zD);
		s = "";
		while (sqlite3_step(stmt) == SQLITE_ROW)// ÿ��һ��sqlite3_step()������stmt������ͻ�ָ����һ����¼
		{
			s.Format(L"%s", sqlite3_column_text16(stmt, lie));
		}
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return s;
	}
	else
	{
		printf("ite ��ѯʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt);
		s = "";
		return s;
	}
}

unsigned short SQLite::getLie(sqlite3_stmt* stmt, CString lMing)
{
	int lie = sqlite3_column_count(stmt);//��ȡ���ص�����
	CString lieMZ;
	unsigned short lieSL = 0;
	for (int i = 0; i < lie; i++)
	{
// 		std::string lieM = sqlite3_column_name(stmt, i);
// 		lieMZ = CString(lieM.c_str());
		lieMZ.Format(_T("%s"), sqlite3_column_name16(stmt, i));
		if (lieMZ == lMing)
		{
			lieSL = i;
			return lieSL;
		}
	}
	return 0;
}

BOOL SQLite::chaXunBMD(CString biao, CString ziDuan, CString shuJu)
{
	CString s;
	s.Format(L"SELECT %s FROM %s", ziDuan, biao);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sqla, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		int lie = sqlite3_column_count(stmt);//��ȡ���ص�����
		CString lieMZ;
		char lieSL = 0;
		for (int i=0;i<lie;i++)
		{
			std::string lieM = sqlite3_column_name(stmt, i);
			lieMZ = CString(lieM.c_str());
			if (lieMZ==ziDuan)
			{
				lieSL = i;
				break;
			}
		}
		while (sqlite3_step(stmt) == SQLITE_ROW)// ÿ��һ��sqlite3_step()������stmt������ͻ�ָ����һ����¼
		{
			const unsigned char* s = sqlite3_column_text(stmt, lieSL);// ȡ����1���ֶε��ַ���
			int ss = sqlite3_column_int(stmt, 2);// ȡ����2���ֶε�����
			printf("mingz:%s;daima%d", s, ss);
		}
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return TRUE;
	}
	else
	{
		printf("��ѯʧ��\n");
		sqlite3_finalize(stmt);
		return FALSE;
	}
}

BOOL SQLite::SCshuJU(CString biao, CString ziDuan, CString shuJu)
{
	CString s;
	s.Format(_T("DELETE from %s where %s='%s'"), biao, ziDuan, shuJu);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sqla, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		int pd=sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		if (pd== SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			return TRUE;
		}
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
	else
	{
		printf("ite ����ɾ��ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
}

BOOL SQLite::SCshuJU(CString biao)
{
	CString s;
	s.Format(_T("DELETE from %s"), biao);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sqla, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		int pd = sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		if (pd == SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			return TRUE;
		}
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
	else
	{
		printf("ite ��ձ� ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
}

BOOL SQLite::SCshuJU(sqlite3* sql, CString biao)
{
	CString s;
	s.Format(_T("DELETE from %s"), biao);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sql, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		int pd = sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		if (pd == SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			return TRUE;
		}
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
	else
	{
		printf("ite ��ձ� ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // �����������׼��ִ����һ�����
		return FALSE;
	}
}


BOOL SQLite::xGsJ(sqlite3* sql, CString biao, CString dingWeiZiDuan, CString dingWeiZhi, CString muBiaoZiDuan, CString muBiaoZhi)
{
	CString s;
	s.Format(_T("UPDATE %s SET %s = '%s' WHERE %s = '%s' "), biao, muBiaoZiDuan, muBiaoZhi, dingWeiZiDuan, dingWeiZhi);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sql, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		int pd = sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		if (pd == SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			return TRUE;
		}
		sqlite3_finalize(stmt);
		return FALSE;
	}
	else
	{
		printf("ite �޸�ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt);
		return FALSE;
	}
}

BOOL SQLite::xGsJ(sqlite3* sql, CString biao, CString dingWeiZiDuan, CString dingWeiZhi, CString muBiaoZiDuan, long muBiaoZhi)
{
	CString s;
	s.Format(_T("UPDATE %s SET %s = %d WHERE %s = '%s' "), biao, muBiaoZiDuan, muBiaoZhi, dingWeiZiDuan, dingWeiZhi);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare16_v2(sql, s, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		int pd = sqlite3_step(stmt);//�ɹ����� SQLITE_DONE
		if (pd == SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			return TRUE;
		}
		sqlite3_finalize(stmt);
		return FALSE;
	}
	else
	{
		printf("ite �޸�ʧ��,�������Ϊ,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt);
		return FALSE;
	}
}
