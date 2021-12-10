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
#pragma comment( lib, "ite86.lib" )//����dll
#endif


class SQLite
{

protected:
public:
	CString biao[4];//����
	CString bChengYuan[13];//���б��Ա
// 	const wchar_t* bMing[4];
// 	const wchar_t* bCY[13];
public:
	SQLite();
	~SQLite();
	sqlite3* sqla = NULL;
	sqlite3* sqlxie = NULL;
	sqlite3* sqldu = NULL;
public:
	/*�ͷ��ڴ�
	* ����1: һ��ָ��
	* ����2:Ĭ��1Ϊһ������ָ��[];2Ϊ��������ָ��
	*/
	void NCshiFang(char* a, int b=1);
	/*�ͷ��ڴ�
	* ����1: ����ָ��
	* ����2:Ĭ��1Ϊ��������ָ��[];2Ϊ���������
	*/
	void NCshiFang(char** a, int b);

	// ת�� utf-8 �� CString ����
	char* UTF8ZhaunUTF16(const char* utf8);
	// ת�� CString �� ANSI����,���ַ�תխ�ַ�
	BOOL zhuanChar(CString sj, char* shuzu, int len);
	// ���� ת�� CString �� utf-8����
	char* zhuanUTF8(CString a);//ת�����ݿ�·��ר��
	BOOL zhuanUTF8(CString sj, char* shuzu, int len);

	/*UTF8ת��ΪUnicode*/
	WCHAR* UTF8ToUnicode(const char* str);

	/*Unicodeת��ΪUTF8*/
	char* UnicodeToUTF8(const WCHAR* pText);

	/* ʹ��CP_UTF8����ҳ��ʵ����UTF-8��Unicode֮���ת��
	* ʹ��CP_ACP����ҳ��ʵ����ANSI��Unicode֮���ת��
	*/
	/*ANSI(CP_ACP)תUnicode*/
	std::wstring ASCIIToUNICODE(char cArry[]);//�������ΪANSI��������char�������string��ʾ�Ĵ�

	/*Unicode ת ASCII(CP_ACP)*/
	std::string UNICODEToASCII(wchar_t cArry[]);// //�������ΪUnicode��,wchar_t cArry[] = {L"���Ǹ�����"};

	/*UTF-8 ת Unicode(CP_UTF8)*/
	std::wstring UTF8ToUnicode(const std::string& str);

	/*Unicode ת UTF-8(CP_UTF8)*/
	std::string UnicodeToUTF8(const std::wstring& str);

	/*cstringתstring*/
	std::string cstrZhuanStr(CString cstr, int a = 1);
	/*stringתcstring*/
	CString strZhuanCstr(std::string str, int a = 1);

	/*�ж��ַ����Ƿ�Ϊutf8  �������ַ�Ϊ�������ж���ȷ��Ϊ��ż�����޷�����*/
	BOOL is_str_utf8(const char* str);

	/*�ж��ַ����Ƿ�ΪGBK*/
	BOOL is_str_gbk(const char* str);

public:	//�������ݿ�,�ر����ݿ�
	/*
	 SQLITE_OPEN_NOMUTEX: �������ݿ����������ڶ��߳�ģʽ(û��ָ�����߳�ģʽ�������)
	SQLITE_OPEN_FULLMUTEX�������µ����ݿ����ӽ�ʹ�á����л��� �߳�ģʽ
	SQLITE_OPEN_NOMUTEX    �µ����ݿ����ӽ�ʹ�á����̡߳� �߳�ģʽ
	SQLITE_OPEN_SHAREDCACHE�����������ڹ�����ģʽ��
	SQLITE_OPEN_PRIVATECACHE�����������ڷǹ�����ģʽ��
	SQLITE_OPEN_READWRITE��ָ�����ݿ����ӿ��Զ�д��
	SQLITE_OPEN_READONLY  ���ݿ���ֻ��ģʽ��
	SQLITE_OPEN_CREATE��������ݿⲻ���ڣ��򴴽���
	*/

	/*��д��ʽ�������ݿ�
	* paht:���ݿ��ļ�·��
	* duoXianCheng:�Ƿ������߳�,Ĭ�� TRUE �������߳�
	* ����ֵ: sqlite3* ����ĵ�ַ
	*/
	BOOL LJsjk(const char* paht, BOOL duoXianCheng = TRUE);

	/*��д��ʽ�������ݿ�
	* sql: һ���򿪵����ݿ�ʵ��;��ʽ: sqlite3 *sql = NULL;
	* paht:���ݿ��ļ�·��
	* duoXianCheng:�Ƿ������߳�,Ĭ�� TRUE �������߳�
	* ����ֵ: sqlite3* ����ĵ�ַ
	*/
	sqlite3* LJsjk(sqlite3* sql, const char* paht, BOOL duoXianCheng = TRUE);

	/*ֻ����ʽ�������ݿ�
	* sql: һ���򿪵����ݿ�ʵ��;��ʽ: sqlite3 *sql = NULL;
	* paht:���ݿ��ļ�·��
	* duoXianCheng:�Ƿ������߳�,Ĭ�� FALSE ������ȫģʽ���߳�
	* ����ֵ: sqlite3* ����ĵ�ַ
	*/
	sqlite3* LJsjk_zhiDu(sqlite3* sql, const char* paht, BOOL duoXianCheng = FALSE);
	
	
	/*�ر����ݿ�*/
	BOOL GBsjk();
	BOOL GBsjk(sqlite3* sql);
public://����
	BOOL ZJshuJu();
	BOOL ZJshuJu(sqlite3* sql);
	BOOL ZJshuJu(sqlite3* sql, CString shuJu);

	/*����id*/
	BOOL ZJid(sqlite3* sql, CString id);
	/*��������*/
	BOOL ZJshuJu(sqlite3* sql, CString biao, CString zD, CString sJ);
	BOOL ZJshuJu(sqlite3* sql, CString biao, CString zD, long sJ);
public://��ѯ
	BOOL CXshuJU();
	BOOL CXshuJU(sqlite3* sql);
	/*
	*/

	unsigned long cXid_int(sqlite3* sql, CString zD);
	CString cXid_txt(sqlite3* sql, CString zD);
	unsigned short getLie(sqlite3_stmt* stmt, CString lMing);
/*
*
* ����: ��ѯָ�������Ƿ��ڱ���ָ���ֶδ���
* ����1: ��
* ����2: ���ҵ��ֶ�
* ����3: ���ҵ�����
* ����ֵ:�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
	BOOL chaXunBMD(CString biao, CString ziDuan, CString shuJu);
public://ɾ��
	BOOL SCshuJU(CString biao, CString ziDuan, CString shuJu);
	BOOL SCshuJU(CString biao);
	BOOL SCshuJU(sqlite3* sql, CString biao);
public://�޸�

	/*�޸�ָ������*/
	BOOL xGsJ(sqlite3* sql, CString biao, CString dingWeiZiDuan, CString dingWeiZhi, CString muBiaoZiDuan, CString muBiaoZhi);
	BOOL xGsJ(sqlite3* sql, CString biao, CString dingWeiZiDuan, CString dingWeiZhi, CString muBiaoZiDuan, long muBiaoZhi);



};

