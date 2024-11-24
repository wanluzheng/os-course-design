#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <fstream>
#include "User.h"
using namespace std;

//#define PATH "F:/"

User::User()
{
	userName = "";
	userPasswd = "";
	totalNum = 0;
}

User::User(string _userName, string _userPasswd)
{
	userName = _userName;
	userPasswd = _userPasswd;
}

User::~User()
{

}

string User::getUserName()
{
	return userName;
}

string User::getUserPasswd()
{
	return userPasswd;
}

int User::getFileNum()
{
	return totalNum;
}

FCB User::getFileMessage(int no)
{
	return files[no];
}

void User::setUserName(const string _userName)
{
	userName = _userName;
}

void User::setUserPasswd(const string _userPasswd)
{
	userPasswd = _userPasswd;
}

void User::setFileNum(const int _modifyNum)
{
	// ¿É+¿É-
	totalNum = _modifyNum;
}

void User::clearData()
{
	files.clear();
}

void User::addFile(const FCB _file)
{
	files.push_back(_file);
}

void User::updateFile(FCB _file)
{
	_file.updateModifyTime();
}

string User::getUserFileName(FCB _file)
{
	return _file.getFileName();
}

int User::getUserFileType(FCB _file)
{
	return _file.getIsDir();
}