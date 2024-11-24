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
	string userName;	// �û���
	string userPasswd;	// ����
	string fileNum = "0";
private:
	int totalNum;		// ���ļ������������û��ϴ���������ɾ���ļ�����̬�仯
protected:
	vector<FCB> files;	// �洢�����ļ���Ϣ
public:
	User();
	User(string _userName, string _userPasswd);
	~User();

	// function
	void clearData();

	// �ӿ�
	string getUserName();
	string getUserPasswd();
	int getFileNum();
	FCB getFileMessage(int no);

	void setUserName(const string _userName);
	void setUserPasswd(const string _userPasswd);
	void setFileNum(const int _modifyNum);
	void addFile(const FCB _file);

	void updateFile(FCB _file);
	// û���ˣ�Ҫɾ��
	string getUserFileName(FCB _file);
	int getUserFileType(FCB _file);
};