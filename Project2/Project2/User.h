#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <vector>
#include "FCB.h"
using namespace std;

class User
{
private:
	string userName;	// 用户名
	string userPasswd;	// 密码
	string fileNum = "0";
private:
	int totalNum;		// 总文件数量：随着用户上传、创建、删除文件而动态变化
protected:
	vector<FCB> files;	// 存储所有文件信息
public:
	User();
	User(string _userName, string _userPasswd);
	~User();

	// function
	void clearData();

	// 接口
	string getUserName();
	string getUserPasswd();
	int getFileNum();
	FCB getFileMessage(int no);

	void setUserName(const string _userName);
	void setUserPasswd(const string _userPasswd);
	void setFileNum(const int _modifyNum);
	void addFile(const FCB _file);

	void updateFile(FCB _file);
	// 没用了，要删掉
	string getUserFileName(FCB _file);
	int getUserFileType(FCB _file);
};