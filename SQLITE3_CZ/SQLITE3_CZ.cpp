// SQLITE3_CZ.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "SQLite.h"
#include <iostream>
//#include <string> //String
using namespace std;
using  std::endl;
SQLite sql;

int main()
{
	/************************   获取程序自身根文件夹  开始  **********************************/
	TCHAR  FileModule[0x100]{};//保存程序自身路径缓冲区

	/*函数:GetModuleFileName
	* 作用: 检索包含指定模块的文件的完全限定路径。该模块必须已被当前进程加载。
	* 参数1:正在请求其路径的已加载模块的句柄。如果此参数为NULL，则 GetModuleFileName检索当前进程的可执行文件的路径。
	* 参数2:指向接收模块完全限定路径的缓冲区的指针,用来保存获取到的路径
	* 参数3:缓冲区(保存路径)的大小,以TCHARs 为单位。
	*/
	GetModuleFileName(NULL, FileModule, 0x100);

	/*计算字符串长度*/
	int ii = 0;
	for (int i = 0x100 - 1; FileModule[i] != L'\\'; i--)//字符串从后到前比较,如果不为'\\'就将FileModule[i]赋值为0,为'\\'时循环终止
	{
		FileModule[i] = 0;
		ii++;
	}
	//FileModule[0x100 - 1 - ii] = 0;//消除斜杠'\'
	/************************   获取程序自身根文件夹  结束  **********************************/
	CString kuP;
	kuP.Format(L"%s同步\\hubao.db", FileModule);
    system("cls");//清空屏幕
    int a = 0;
    
    printf("1 为查询数据\n");
    printf("2 为增加数据\n");
    printf("4 为删除数据\n");
	char* diZhi;
	diZhi = sql.zhuanUTF8(kuP);

	sqlite3* sql3 = NULL;
	//sql.LJsjk(sql.sqlxie, diZhi);
	//sql.LJsjk(kuP);
	while (true)
	{
		printf("请输入要执行的项\n");
		std::cin >> a;
		string szf = "快点回家as";
		wstring kszf = L"快点回家as";
		CString kcszf = L"快点回家as";
		CString cszf = "快点回家as";
		const char* czf = "快点回家as";
		const wchar_t* kczf = L"快点回家as";
		unsigned long shi = 0;
		switch (a)
		{
		case 1:
			
			sql3 = sql.LJsjk(sql3, diZhi,FALSE);
			//sql.ZJshuJu(sql3,L"as好好的好的很axc");
			cszf=sql.cXid_txt(sql3, sql.bChengYuan[7]);
			MessageBox(NULL, cszf, L"ss", NULL);
			shi = sql.cXid_int(sql3, sql.bChengYuan[9]);
			std::cout << shi << "\n";
			break;
		case 2:
			sql3 = sql.LJsjk(sql3, diZhi, FALSE);
			sql.xGsJ(sql3, sql.biao[3], sql.bChengYuan[7], L"as好好的好的很axc", sql.bChengYuan[9], 1254896587);
			break;
		case 3:
			//sql.LJsjk(sql3, kuP);
			sql.ZJshuJu(sql3);
			break;
		case 4:
			sql.LJsjk(diZhi);
			sql.SCshuJU(L"id");
			sql.CXshuJU();
			break;
		case 5:
			printf("%S\n",kszf);
			printf("%s\n",szf);
			printf("%S  1\n", kcszf.GetBuffer());
			printf("%s  2\n", cszf.GetBuffer());
			printf("%S\n", kczf);
			printf("%s\n", czf);
			cout << szf << "   0\n";
			cout << cszf.GetBuffer() << "  3\n";
			cout << czf << "   0\n";
			MessageBox(NULL, kcszf, L"ok", NULL);
		default:
			break;
		}
		printf("%d", a);

	}

	

}

