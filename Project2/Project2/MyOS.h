#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iomanip>
#include <Windows.h>
#include "FCB.h"
#include "User.h"

#include <thread>
#include <condition_variable>
#include <mutex>
using namespace std;

#define RootName "ZWL"

enum class Command {
	//基础补充
	empty,	// 空白
	help,	// 帮助指令
	ver,	// 查看版本号

	// 文件类
	exit,	// 退出
	create,	// 创建文件
	_delete,// 删除文件
	open,	// 打开文件
	close,	// 关闭文件
	read,	// 读文件
	write,	// 写文件
	lseek,	// 文件指针移动
	head,	// 显示文件前num行
	tail,	// 显示文件后num行
	clear,	// 清屏
	flock,	// 文件加锁，解锁
	move,	// 移动文件
	copy,	// 拷贝文件

	// 目录类
	cd,		// 进入目录
	dir,	// 显示当前目录
	mkdir,	// 创建目录
	rmdir,	// 移除目录

	_import,// 导入
	_export	// 导出
};

class MyOS
{
private:
	FCBTree* root;
	FCBTree* cNode;
	vector<User> users;
public:
	condition_variable cv;
	bool ready = false;

	//bool Multithreading();
	[[noreturn]] void kernel();
public:
	MyOS() {};
	~MyOS() {};
	void initFCBTree();

	// 功能函数：打印
	static void printLogo();				// 显示logo
	static void printTime();				// 显示当前time
	static void printBar();					// 模拟进度条
	static void printVersion();				// 显示版本号
	static void printPath(FCBTree* _node);	// 显示文件路径
	static void printFileFunc();			// 文件操作信息
	void printUsers();						// 显示用户信息


	// 功能函数：写信息
	void writeFile(ofstream& _file, FCB _f);

	// 功能函数：格式重整
	string standardPath(FCBTree* _node);
	string standardString(const string& str);
	string standardString(string temp, int len);
	int stringToInt(const string& str);
	string intToString(const int& str, int len);
	string enumToString(const Command cmd);
	Command stringToEnum(const string str);


	void analysisPath();
	void analysisCmd(const std::string& input, string& cmd, string& suffix);// 解析指令

	// 功能函数：菜单
	int initMenu();															// 登入界面
	void funcMenu();														// 主功能界面
	void fileMenu(int num, int& pos, string cmd, FCBTree* node, bool& f);	// 文件操作界面

	// 用户操作
	void saveUsers();
	void loadMT();
	void loadUsers();
	// 注册+登录
	int createUser();
	int userLogin();
	int saveFCBTree(FCBTree* _node);
	// 文件权限
	bool fileRight(FCBTree* _node);
	bool verifyPasswd(const string _code, string _input);
	// 重名rename
	bool rename(FCBTree* _node, string _name);

	bool Update();
	bool saveModifyTimesToFile();
	void updateData();
public:
	/*
	* 文件类
		create创建文件
		delete删除文件
		open打开文件
		close关闭文件
		read读文件
		write写文件
		move        移动文件
		copy  拷贝文件
		flock         文件加锁，需要实现加锁和解锁功能
		head –num  显示文件的前num行
		tail  -num  显示文件尾巴上的num行
		lseek      文件读写指针的移动，从文件指针当前位置处向后移动 offset，负数时向前移动offset
	*/
	void helpMenu();							// 帮助界面
	void exitSystem();							// 退出系统
	void clearSys();							// 清屏


	bool createFile();							// 创建文件
	bool deleteFile(string path);				// 删除文件
	void openFile(string path);					// 打开文件
	void closeFile(FCBTree* _node);// 关闭文件
	bool _readFile(int& pos, FCBTree* _node, int num);	// 读文件
	bool _writeFile(int& pos, FCBTree* _node);	// 写文件
	void lseek(int& pos, FCBTree* _node);		// 文件读写指针的移动
	void headFile(int num, FCBTree* _node);		// 显示文件的前num行
	void tailFile(int num, FCBTree* _node);		// 显示文件的尾num行
	void lockFile();							// 文件加锁
	void moveFile(string str);					// 移动文件
	void copyFile();							// 拷贝文件

	/*
	* 目录类
		cd			进入目录
		dir         显示当前目录
		mkdir		创建目录
		rmdir		删除目录（空/不为空，子目录？）
	*/
	bool cd(string path);						// 进入目录
	void dir();									// 显示当前目录
	void mkdir();								// 创建目录
	void rmdir(string path);					// 删除目录

	/*
	“import c:\a.txt .”    将本地C盘下的a.txt导入到当前目录。
	“export a.txt c:\”   将当前目录下的a.txt导出到本地C盘。
	*/
	bool importFile(string str);				// 导入到当前目录
	bool exportFile(string str);				// 导出到本地C盘


};

