#pragma once

#include "sqlite3.h"
//#include <winsqlite/winsqlite3.h>
#include <windows.h>
#include <string> //String
//using namespace std;
//#include <xstring>//String
#include <atlstr.h>//CString

#if defined _M_X64
#pragma comment( lib, "ite.lib" )
#elif defined _M_IX86
#pragma comment( lib, "ite86.lib" )//加载dll
#endif


class SQLite
{

protected:
public:
	CString biao[4];//表名
	CString bChengYuan[13];//所有表成员
// 	const wchar_t* bMing[4];
// 	const wchar_t* bCY[13];
public:
	SQLite();
	~SQLite();
	sqlite3* sqla = NULL;
	sqlite3* sqlxie = NULL;
	sqlite3* sqldu = NULL;
public:
	/*释放内存
	* 参数1: 一级指针
	* 参数2:默认1为一级数组指针[];2为不带数组指针
	*/
	void NCshiFang(char* a, int b=1);
	/*释放内存
	* 参数1: 二级指针
	* 参数2:默认1为二级数组指针[];2为分配的行数
	*/
	void NCshiFang(char** a, int b);

	// 转换 utf-8 到 CString 编码
	char* UTF8ZhaunUTF16(const char* utf8);
	// 转换 CString 到 ANSI编码,宽字符转窄字符
	BOOL zhuanChar(CString sj, char* shuzu, int len);
	// 汉字 转换 CString 到 utf-8编码
	char* zhuanUTF8(CString a);//转换数据库路径专用
	BOOL zhuanUTF8(CString sj, char* shuzu, int len);

	/*UTF8转化为Unicode*/
	WCHAR* UTF8ToUnicode(const char* str);

	/*Unicode转化为UTF8*/
	char* UnicodeToUTF8(const WCHAR* pText);

	/* 使用CP_UTF8代码页就实现了UTF-8与Unicode之间的转换
	* 使用CP_ACP代码页就实现了ANSI与Unicode之间的转换
	*/
	/*ANSI(CP_ACP)转Unicode*/
	std::wstring ASCIIToUNICODE(char cArry[]);//传入参数为ANSI串，即用char数组或者string表示的串

	/*Unicode 转 ASCII(CP_ACP)*/
	std::string UNICODEToASCII(wchar_t cArry[]);// //传入参数为Unicode串,wchar_t cArry[] = {L"这是个测试"};

	/*UTF-8 转 Unicode(CP_UTF8)*/
	std::wstring UTF8ToUnicode(const std::string& str);

	/*Unicode 转 UTF-8(CP_UTF8)*/
	std::string UnicodeToUTF8(const std::wstring& str);

	/*cstring转string*/
	std::string cstrZhuanStr(CString cstr, int a = 1);
	/*string转cstring*/
	CString strZhuanCstr(std::string str, int a = 1);

	/*判断字符串是否为utf8  当中文字符为奇数个判断正确，为则偶数个无法区别。*/
	BOOL is_str_utf8(const char* str);

	/*判断字符串是否为GBK*/
	BOOL is_str_gbk(const char* str);

public:	//连接数据库,关闭数据库
	/*
	 SQLITE_OPEN_NOMUTEX: 设置数据库连接运行在多线程模式(没有指定单线程模式的情况下)
	SQLITE_OPEN_FULLMUTEX：设置新的数据库连接将使用“序列化” 线程模式
	SQLITE_OPEN_NOMUTEX    新的数据库连接将使用“多线程” 线程模式
	SQLITE_OPEN_SHAREDCACHE：设置运行在共享缓存模式。
	SQLITE_OPEN_PRIVATECACHE：设置运行在非共享缓存模式。
	SQLITE_OPEN_READWRITE：指定数据库连接可以读写。
	SQLITE_OPEN_READONLY  数据库以只读模式打开
	SQLITE_OPEN_CREATE：如果数据库不存在，则创建。
	*/

	/*读写方式连接数据库
	* paht:数据库文件路径
	* duoXianCheng:是否开启多线程,默认 TRUE 开启多线程
	* 返回值: sqlite3* 分配的地址
	*/
	BOOL LJsjk(const char* paht, BOOL duoXianCheng = TRUE);

	/*读写方式连接数据库
	* sql: 一个打开的数据库实例;格式: sqlite3 *sql = NULL;
	* paht:数据库文件路径
	* duoXianCheng:是否开启多线程,默认 TRUE 开启多线程
	* 返回值: sqlite3* 分配的地址
	*/
	sqlite3* LJsjk(sqlite3* sql, const char* paht, BOOL duoXianCheng = TRUE);

	/*只读方式连接数据库
	* sql: 一个打开的数据库实例;格式: sqlite3 *sql = NULL;
	* paht:数据库文件路径
	* duoXianCheng:是否开启多线程,默认 FALSE 开启安全模式单线程
	* 返回值: sqlite3* 分配的地址
	*/
	sqlite3* LJsjk_zhiDu(sqlite3* sql, const char* paht, BOOL duoXianCheng = FALSE);
	
	
	/*关闭数据库*/
	BOOL GBsjk();
	BOOL GBsjk(sqlite3* sql);
public://增加
	BOOL ZJshuJu();
	BOOL ZJshuJu(sqlite3* sql);
	BOOL ZJshuJu(sqlite3* sql, CString shuJu);

	/*增加id*/
	BOOL ZJid(sqlite3* sql, CString id);
	/*增加数据*/
	BOOL ZJshuJu(sqlite3* sql, CString biao, CString zD, CString sJ);
	BOOL ZJshuJu(sqlite3* sql, CString biao, CString zD, long sJ);
public://查询
	BOOL CXshuJU();
	BOOL CXshuJU(sqlite3* sql);
	/*
	*/

	unsigned long cXid_int(sqlite3* sql, CString zD);
	CString cXid_txt(sqlite3* sql, CString zD);
	unsigned short getLie(sqlite3_stmt* stmt, CString lMing);
/*
*
* 作用: 查询指定数据是否在表内指定字段存在
* 参数1: 表
* 参数2: 查找的字段
* 参数3: 查找的数据
* 返回值:成功返回TRUE,失败返回FALSE
*/
	BOOL chaXunBMD(CString biao, CString ziDuan, CString shuJu);
public://删除
	BOOL SCshuJU(CString biao, CString ziDuan, CString shuJu);
	BOOL SCshuJU(CString biao);
	BOOL SCshuJU(sqlite3* sql, CString biao);
public://修改

	/*修改指定数据*/
	BOOL xGsJ(sqlite3* sql, CString biao, CString dingWeiZiDuan, CString dingWeiZhi, CString muBiaoZiDuan, CString muBiaoZhi);
	BOOL xGsJ(sqlite3* sql, CString biao, CString dingWeiZiDuan, CString dingWeiZhi, CString muBiaoZiDuan, long muBiaoZhi);



};

