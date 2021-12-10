
#include "SQLite.h"
#include <iostream>

//#include <comdef.h>

#ifdef DEBUG //检测是否为调试模式
#define DBGprint(...) printf(__VA_ARGS__)
#else
#define DBGprint(...)
#endif
SQLite::SQLite()
{
	biao[0] = _T("cxMingDan");  //程序记录表
	biao[1] = _T("dqjincheng");  //当前表
	biao[2] = _T("csJinCheng");  //程序启动初始表
	biao[3] = _T("id");  //id

	bChengYuan[0] = _T("xh");  //序号;除id表外所有表第1列
	bChengYuan[1] = _T("th32ProcessID");  //pid;2
	bChengYuan[2] = _T("szExeFile");  //exe名字;3

	bChengYuan[3] = _T("szModule");  //模块名字;2
	bChengYuan[4] = _T("szExePath");  //模块路径;3
	bChengYuan[5] = _T("szTzm");  //程序特征码;4
	bChengYuan[6] = _T("szFenLei");  //程序类型,1为白名单,2为黑名单,3为未知,4为游戏;5

	bChengYuan[7] = _T("name");  //用户名;1
	bChengYuan[8] = _T("pass");  //密码;2
	bChengYuan[9] = _T("dqShiJian");  //到期时间;3
	bChengYuan[10] = _T("dengLu");  //记住密码自动登陆;4
	bChengYuan[11] = _T("dingShi");  //定时保护;5
	bChengYuan[12] = _T("YXshiJian");  //程序运行时间;6

// 	bMing[0] = _T("cxMingDan");  //程序记录表
// 	bMing[1] = _T("dqjincheng");  //当前表
// 	bMing[2] = _T("csJinCheng");  //程序启动初始表
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
	* 将字符串映射到 UTF-16（宽字符）字符串
	*/
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	/*
	* 将 UTF-16（宽字符）字符串映射到新字符串
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
	int ilen;//字节个数
	int nk = sj.GetLength();//调用此方法可返回对象中的字符数。 计数不包括 null 终止符。
	/*
	* 函数: WideCharToMultiByte
	* 作用: 将 UTF-16（宽字符）字符串映射到新字符串。新字符串不一定来自多字节字符集。此函数容易造成内存泄漏和转换后的数据丢失
	* 参数1: 用于执行转换的代码页。转换后的格式,CP_ACP=系统默认的 Windows ANSI 代码页
	* 参数2: 指示转换类型的标志。当没有设置这些标志时，该函数执行得更快。
	* 参数3: 指向要转换的 Unicode 字符串的指针。
	* 参数4: 要转换的源字符串的大小（以字符为单位）。或者，如果字符串以空字符结尾，则可以将此参数设置为 -1,为-1时返回的长度包括结束的空字符
	* 参数5: 转换后保存数据的缓冲区(指针)
	* 参数6: 由 参数4 指示的缓冲区的大小（以字节为单位）。如果此参数设置为 0，则该函数返回 参数4 所需的缓冲区大小,此参数为正数时才能转换成功
	* 参数7: 一般设置为NULL
	* 参数8: 可以设置NULL
	* 返回值: 如果成功，则返回写入 参数5 指向的缓冲区的字节数。
	*        如果函数成功并且 参数6 为 0，则返回值是 参数5 指示的缓冲区所需的大小（以字节为单位）
	*/
	ilen = WideCharToMultiByte(CP_ACP, 0, sj, nk, NULL, 0, NULL, NULL); //获取宽字节字符的大小，大小是按字节计算的.此步是为了避免内存泄漏
	if (len < ilen)//如果接受数组长度小于要转换的字符串长度,返回FALSE
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_ACP, 0, sj, nk+1, shuzu, ilen+1, NULL, NULL);//宽字节编码转换成多字节编码
	/*捕获错误*/
	shuzu[ilen + 1] = '\0';
	if (ERROR_INSUFFICIENT_BUFFER == GetLastError() || ERROR_INVALID_FLAGS == GetLastError() ||
		ERROR_INVALID_PARAMETER == GetLastError() || ERROR_NO_UNICODE_TRANSLATION == GetLastError())
	{
		printf("JSON char* 转换 errorNum = %d\n", GetLastError());//获取错误代码
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
		printf("cstring 转换 UTF8 = %d\n", GetLastError());//获取错误代码
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
		printf("cstring 转换 UTF8 = %d\n", GetLastError());//获取错误代码
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
	int i = WideCharToMultiByte(CP_UTF8, 0, pText, -1, NULL, 0, NULL, NULL); //输入缓冲区大小是宽字符数         
	char* strUTF8 = new   char[i];
	WideCharToMultiByte(CP_UTF8, 0, pText, -1, strUTF8, i, NULL, NULL);
	return strUTF8;
	//delete[]strUTF8;
}

std::wstring SQLite::ASCIIToUNICODE(char cArry[])
{
	/*使用CP_ACP代码页就实现了ANSI与Unicode之间的转换*/
	int nLen = MultiByteToWideChar(CP_ACP, 0, cArry, -1, NULL, NULL);   //将MultiByteToWideChar()的第四个形参设为-1,即可返回长度

	wchar_t* pTemp = new wchar_t[nLen];       //new一个wchar_t空间，保存Unicode串
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
	char* pTemp = new char[nLen];                //new 一个char数组，保存ANSI串
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
		str = (LPCSTR)cstr.GetBuffer(0);//未知
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
	unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr = *str;
	bool bAllAscii = true;
	for (unsigned int i = 0; str[i] != '\0'; ++i) 
	{
		chr = *(str + i);
		//判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
		if (nBytes == 0 && (chr & 0x80) != 0) 
		{
			bAllAscii = false;
		}
		if (nBytes == 0) 
		{
			//如果不是ASCII码,应该是多字节符,计算字节数
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
			//多字节符的非首字节,应为 10xxxxxx
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			//减到为零为止
			nBytes--;
		}
	}
	//违返UTF8编码规则
	if (nBytes != 0) 
	{
		return false;
	}
	if (bAllAscii) 
	{ //如果全部都是ASCII, 也是UTF8
		return true;
	}
	return true;
}

BOOL SQLite::is_str_gbk(const char* str)
{
	unsigned int nBytes = 0;//GBK可用1-2个字节编码,中文两个 ,英文一个
	unsigned char chr = *str;
	bool bAllAscii = true; //如果全部都是ASCII,
	for (unsigned int i = 0; str[i] != '\0'; ++i) {
		chr = *(str + i);
		if ((chr & 0x80) != 0 && nBytes == 0) {// 判断是否ASCII编码,如果不是,说明有可能是GBK
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
	{   //违返规则
		return false;
	}
	if (bAllAscii) 
	{ //如果全部都是ASCII, 也是GBK
		return true;
	}
	return true;
}

BOOL SQLite::LJsjk(const char* paht, BOOL duoXianCheng)
{
	if (duoXianCheng)
	{
		int pd = sqlite3_open_v2(
			paht,//sql库文件的路径
			&sqla,// 一个打开的数据库实例
			/*
			* SQLITE_OPEN_NOMUTEX: 设置数据库连接运行在多线程模式(没有指定单线程模式的情况下)
			  SQLITE_OPEN_FULLMUTEX：设置数据库连接运行在串行模式。
			  SQLITE_OPEN_NOMUTEX    新的数据库连接将使用“多线程” 线程模式
			  SQLITE_OPEN_SHAREDCACHE：设置运行在共享缓存模式。
			  SQLITE_OPEN_PRIVATECACHE：设置运行在非共享缓存模式。
			  SQLITE_OPEN_READWRITE：指定数据库连接可以读写。
			  SQLITE_OPEN_CREATE：如果数据库不存在，则创建。
			*/
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE,
			NULL
		);
		if (pd == SQLITE_OK)
		{
			printf("ite数据库连接成功\n");
			return TRUE;
		}
		else
		{
			printf("ite数据库连接失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
			return FALSE;
		}
	} 
	else
	{
		int pd = sqlite3_open_v2(
			paht,//sql库文件的路径
			&sqla,// 一个打开的数据库实例
			/*
			* SQLITE_OPEN_NOMUTEX: 设置数据库连接运行在多线程模式(没有指定单线程模式的情况下)
			  SQLITE_OPEN_FULLMUTEX：设置数据库连接运行在串行模式。
			  SQLITE_OPEN_NOMUTEX    新的数据库连接将使用“多线程” 线程模式
			  SQLITE_OPEN_SHAREDCACHE：设置运行在共享缓存模式。
			  SQLITE_OPEN_PRIVATECACHE：设置运行在非共享缓存模式。
			  SQLITE_OPEN_READWRITE：指定数据库连接可以读写。
			  SQLITE_OPEN_CREATE：如果数据库不存在，则创建。
			*/
			SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_SHAREDCACHE,
			NULL
		);
		if (pd == SQLITE_OK)
		{
			printf("ite数据库连接成功\n");
			return TRUE;
		}
		else
		{
			printf("ite数据库连接失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
			return FALSE;
		}
	}
	
}


sqlite3* SQLite::LJsjk(sqlite3* sql, const char* paht, BOOL duoXianCheng)
{
	int sd = SQLITE_OPEN_NOMUTEX;//多线程
	if (!duoXianCheng)
	{
		sd = SQLITE_OPEN_FULLMUTEX;//序列化
		
	}
	int pd = sqlite3_open_v2(
		paht,//sql库文件的路径
		&sql,// 一个打开的数据库实例
		/*
		* SQLITE_OPEN_NOMUTEX: 设置数据库连接运行在多线程模式(没有指定单线程模式的情况下)
		  SQLITE_OPEN_FULLMUTEX：设置新的数据库连接将使用“序列化” 线程模式
		  SQLITE_OPEN_NOMUTEX    新的数据库连接将使用“多线程” 线程模式。
		  SQLITE_OPEN_SHAREDCACHE：设置运行在共享缓存模式。
		  SQLITE_OPEN_PRIVATECACHE：设置运行在非共享缓存模式。
		  SQLITE_OPEN_READWRITE：指定数据库连接可以读写。
		  SQLITE_OPEN_CREATE：如果数据库不存在，则创建。
		*/
		SQLITE_OPEN_READWRITE | sd | SQLITE_OPEN_SHAREDCACHE,
		NULL
	);
	if (pd == SQLITE_OK)
	{
		if (!duoXianCheng)
		{
			printf("ite数据库 序列化 连接成功\n");
			return sql;
		}
		printf("ite数据库 多线程 连接成功\n");
		return sql;
	}
	else
	{
		printf("ite数据库连接失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		return sql;
	}
	
}


sqlite3* SQLite::LJsjk_zhiDu(sqlite3* sql, const char* paht, BOOL duoXianCheng)
{
	if (duoXianCheng)
	{
		int pd = sqlite3_open_v2(
			paht,//sql库文件的路径
			&sql,// 一个打开的数据库实例
			/*
			* SQLITE_OPEN_NOMUTEX: 设置数据库连接运行在多线程模式(没有指定单线程模式的情况下)
			  SQLITE_OPEN_FULLMUTEX：设置数据库连接运行在串行模式
			  SQLITE_OPEN_NOMUTEX    新的数据库连接将使用“多线程” 线程模式。
			  SQLITE_OPEN_SHAREDCACHE：设置运行在共享缓存模式。
			  SQLITE_OPEN_PRIVATECACHE：设置运行在非共享缓存模式。
			  SQLITE_OPEN_READWRITE：指定数据库连接可以读写。
			  SQLITE_OPEN_READONLY  数据库以只读模式打开
			  SQLITE_OPEN_CREATE：如果数据库不存在，则创建。
			*/
			SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE,
			NULL
		);
		if (pd == SQLITE_OK)
		{
			printf("ite 只读数据库 多线程 连接成功\n");
			return sql;
		}
		else
		{
			printf("ite数据库连接失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
			return sql;
		}
	}
	else
	{
		int pd = sqlite3_open_v2(
			paht,//sql库文件的路径
			&sql,// 一个打开的数据库实例
			/*
			* SQLITE_OPEN_NOMUTEX: 设置数据库连接运行在多线程模式(没有指定单线程模式的情况下)
			  SQLITE_OPEN_FULLMUTEX：设置新的数据库连接将使用“序列化” 线程模式
			  SQLITE_OPEN_NOMUTEX    新的数据库连接将使用“多线程” 线程模式
			  SQLITE_OPEN_SHAREDCACHE：设置运行在共享缓存模式。
			  SQLITE_OPEN_PRIVATECACHE：设置运行在非共享缓存模式。
			  SQLITE_OPEN_READWRITE：指定数据库连接可以读写。
			  SQLITE_OPEN_READONLY  数据库以只读模式打开
			  SQLITE_OPEN_CREATE：如果数据库不存在，则创建。
			*/
			SQLITE_OPEN_READONLY | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_SHAREDCACHE,
			NULL
		);
		if (pd == SQLITE_OK)
		{
			printf("ite数据库连接成功\n");
			return sql;
		}
		else
		{
			printf("ite 只读数据库 序列化 连接失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
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
		printf("ite 关闭失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
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
		printf("ite 关闭失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		return FALSE;
	}
}

BOOL SQLite::ZJshuJu()
{
	CString s= L"INSERT INTO id(name, pass, dqShiJian, dengLu) VALUES('咯忽高忽低', '22',1524544545,1);";
	
	char ss[160] = { 0 };
	zhuanChar(s, ss, 160);
	const char* str = "INSERT INTO id(name, pass) VALUES('咯忽高忽低', '22');";
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare(sqla, ss, -1, &stmt, NULL);
	if (pd==SQLITE_OK)
	{
		sqlite3_step(stmt);//成功返回 SQLITE_DONE
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		printf("ite 增加成功\n");
		return TRUE;
	}
	else
	{
		printf("ite 增加失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		return FALSE;
	}
}

BOOL SQLite::ZJshuJu(sqlite3* sql)
{
	CString s = L"INSERT INTO id(name, pass, dqShiJian, dengLu) VALUES('风光风光风光高忽低abc', '22',1524544545,1);";
	char* sq = NULL;
	char ss[260] = { 0 };
	zhuanChar(s, ss, 160);
	//zhuanUTF8(s, ss, 260);
	//sq = zhuanUTF8(s);
	sqlite3_stmt* stmt = NULL;
	int pd = sqlite3_prepare_v2(sql, ss, -1, &stmt, NULL);
	if (pd == SQLITE_OK)
	{
		sqlite3_step(stmt);//成功返回 SQLITE_DONE
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		printf("ite 增加成功\n");
		return TRUE;
	}
	else
	{
		printf("ite 增加失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
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
		sqlite3_step(stmt);//成功返回 SQLITE_DONE
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		printf("ite ID增加成功\n");
		return TRUE;
	}
	else
	{
		printf("ite ID增加失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
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
		sqlite3_step(stmt);//成功返回 SQLITE_DONE
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		printf("ite数据库增加成功\n");
		return TRUE;
	}
	else
	{
		printf("ite 增加失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
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
		sqlite3_step(stmt);//成功返回 SQLITE_DONE
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		printf("ite数据库增加成功\n");
		return TRUE;
	}
	else
	{
		printf("ite 增加失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
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
		sqlite3_step(stmt);//成功返回 SQLITE_DONE
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		printf("ite数据库增加成功\n");
		return TRUE;
	}
	else
	{
		printf("ite 增加失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
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
		printf("查询成功\n");
		while (sqlite3_step(stmt)==SQLITE_ROW)// 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
		{
			lx = sqlite3_column_type(stmt, 1);//返回结果列的初始数据类型的 数据类型代码
			switch (lx)
			{
			case SQLITE_INTEGER://64位有符号整数
				printf("整数 1\n");
				sqlite3_column_int64(stmt, 1);//64 位整数结果
				break;
			case SQLITE_FLOAT://64有符号位浮点数
				printf("浮点数 2\n");
				break;
			case SQLITE_TEXT://字符串
				printf("字符串 3\n");
				/*const void *  */
				szfc = (wchar_t)sqlite3_column_text16(stmt, 1);
				/*确定该 BLOB 或字符串的大小*/
				/*如果结果是 UTF-16 字符串，则 sqlite3_column_bytes() 将字符串转换为 UTF-8, 然后返回字节数*/
				//int daXiao=sqlite3_column_bytes(stmt, 1);
				daXiao = sqlite3_column_bytes16(stmt, 1);
				
				break;
			case SQLITE_BLOB://布尔值
				printf("布尔值 4\n");
				/*const void *  */
				sqlite3_column_text16(stmt, 1);
				/*确定该 BLOB 或字符串的大小*/
				/*如果结果是 UTF-8 字符串，则 sqlite3_column_bytes16() 将字符串转换为 UTF-16，然后返回字节数*/
				//int daXiao=sqlite3_column_bytes(stmt, 1);
				daXiao = sqlite3_column_bytes16(stmt, 1);
				break;
			case SQLITE_NULL://空值
				printf("空值 5\n");
				break;
			default:
				printf("程序返回类型错误\n");
				return FALSE;
				break;
			}
			const char* s1 = (char* )sqlite3_column_text16(stmt, 1);// 取出第1列字段的字符串
			//s = sqlite3_column_text16(stmt, 1);
			s.Format(L"%s", sqlite3_column_text16(stmt, 1)); 
			int ss = sqlite3_column_bytes16(stmt, 1);
			//int ss = sqlite3_column_int(stmt, 2);// 取出第2列字段的整数
			printf("mingz:%S;daima%d\n", s, ss);
		}
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		return TRUE;
	} 
	else
	{
		printf("ite 查询失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		sqlite3_finalize(stmt);
		return FALSE;
	}
}

BOOL SQLite::CXshuJU(sqlite3* sql)
{

	CString s = L"SELECT * FROM id;";
	std::wcout.imbue(std::locale("chs")); //控制台的编码是跟随系统，所以wcout要求给出编码
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
		USES_CONVERSION;//const WCHAR *转换为const char *
		printf("查询成功\n");
		while (sqlite3_step(stmt) == SQLITE_ROW)// 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
		{
			lx = sqlite3_column_type(stmt, 1);//返回结果列的初始数据类型的 数据类型代码
			switch (lx)
			{
			case SQLITE_INTEGER://64位有符号整数
				printf("整数\n");
				sqlite3_column_int64(stmt, 1);//64 位整数结果
				break;
			case SQLITE_FLOAT://64有符号位浮点数
				printf("浮点数\n");
				break;
			case SQLITE_TEXT://字符串
				printf("字符串\n");
				/*const void *  */
				ssa = sqlite3_column_text16(stmt, 1);
				ssaa= (wchar_t*)sqlite3_column_text16(stmt, 1);
				cszfc.Format(L"%s", sqlite3_column_text16(stmt, 1));

				/*确定该 BLOB 或字符串的大小*/
				/*如果结果是 UTF-16 字符串，则 sqlite3_column_bytes() 将字符串转换为 UTF-8, 然后返回字节数*/
				//int daXiao=sqlite3_column_bytes(stmt, 1);
				daXiao = sqlite3_column_bytes16(stmt, 1);
				
				std::wcout << ssaa << "\n";
				
				csa = W2A(ssaa);//const WCHAR *转换为const char *
				printf("%s", csa);
				MessageBox(NULL, cszfc, L"ok", NULL);
				break;
			case SQLITE_BLOB://布尔值
				printf("布尔值\n");
				/*const void *  */
				sqlite3_column_text16(stmt, 1);
				/*确定该 BLOB 或字符串的大小*/
				/*如果结果是 UTF-8 字符串，则 sqlite3_column_bytes16() 将字符串转换为 UTF-16，然后返回字节数*/
				//int daXiao=sqlite3_column_bytes(stmt, 1);
				daXiao = sqlite3_column_bytes16(stmt, 1);
				break;
			case SQLITE_NULL://空值
				printf("空值\n");
				break;
			default:
				printf("程序返回类型错误\n");
				return FALSE;
				break;
			}
		}
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		return TRUE;
	}
	else
	{
		printf("ite 查询失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
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
// 		int lie = sqlite3_column_count(stmt);//获取返回的列数
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
		unsigned long sj32 = 0;//32位有符号整数
		while (sqlite3_step(stmt) == SQLITE_ROW)// 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
		{
			//sj64 = sqlite3_column_int64(stmt, 1);//64 位整数结果
			sj32 = sqlite3_column_int(stmt, lie);//64 位整数结果
		}
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		return sj32;
	}
	else
	{
		printf("ite 查询失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
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
		// 		int lie = sqlite3_column_count(stmt);//获取返回的列数
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
		while (sqlite3_step(stmt) == SQLITE_ROW)// 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
		{
			s.Format(L"%s", sqlite3_column_text16(stmt, lie));
		}
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		return s;
	}
	else
	{
		printf("ite 查询失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt);
		s = "";
		return s;
	}
}

unsigned short SQLite::getLie(sqlite3_stmt* stmt, CString lMing)
{
	int lie = sqlite3_column_count(stmt);//获取返回的列数
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
		int lie = sqlite3_column_count(stmt);//获取返回的列数
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
		while (sqlite3_step(stmt) == SQLITE_ROW)// 每调一次sqlite3_step()函数，stmt语句句柄就会指向下一条记录
		{
			const unsigned char* s = sqlite3_column_text(stmt, lieSL);// 取出第1列字段的字符串
			int ss = sqlite3_column_int(stmt, 2);// 取出第2列字段的整数
			printf("mingz:%s;daima%d", s, ss);
		}
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		return TRUE;
	}
	else
	{
		printf("查询失败\n");
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
		int pd=sqlite3_step(stmt);//成功返回 SQLITE_DONE
		if (pd== SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			return TRUE;
		}
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		return FALSE;
	}
	else
	{
		printf("ite 数据删除失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
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
		int pd = sqlite3_step(stmt);//成功返回 SQLITE_DONE
		if (pd == SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			return TRUE;
		}
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		return FALSE;
	}
	else
	{
		printf("ite 清空表 失败,错误代码为,%d | %s\n", sqlite3_errcode(sqla), sqlite3_errmsg(sqla));
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
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
		int pd = sqlite3_step(stmt);//成功返回 SQLITE_DONE
		if (pd == SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			return TRUE;
		}
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
		return FALSE;
	}
	else
	{
		printf("ite 清空表 失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt); // 清理语句句柄，准备执行下一个语句
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
		int pd = sqlite3_step(stmt);//成功返回 SQLITE_DONE
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
		printf("ite 修改失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
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
		int pd = sqlite3_step(stmt);//成功返回 SQLITE_DONE
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
		printf("ite 修改失败,错误代码为,%d | %s\n", sqlite3_errcode(sql), sqlite3_errmsg(sql));
		sqlite3_finalize(stmt);
		return FALSE;
	}
}
