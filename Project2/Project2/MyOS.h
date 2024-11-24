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
	//��������
	empty,	// �հ�
	help,	// ����ָ��
	ver,	// �鿴�汾��

	// �ļ���
	exit,	// �˳�
	create,	// �����ļ�
	_delete,// ɾ���ļ�
	open,	// ���ļ�
	close,	// �ر��ļ�
	read,	// ���ļ�
	write,	// д�ļ�
	lseek,	// �ļ�ָ���ƶ�
	head,	// ��ʾ�ļ�ǰnum��
	tail,	// ��ʾ�ļ���num��
	clear,	// ����
	flock,	// �ļ�����������
	move,	// �ƶ��ļ�
	copy,	// �����ļ�

	// Ŀ¼��
	cd,		// ����Ŀ¼
	dir,	// ��ʾ��ǰĿ¼
	mkdir,	// ����Ŀ¼
	rmdir,	// �Ƴ�Ŀ¼

	_import,// ����
	_export	// ����
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

	// ���ܺ�������ӡ
	static void printLogo();				// ��ʾlogo
	static void printTime();				// ��ʾ��ǰtime
	static void printBar();					// ģ�������
	static void printVersion();				// ��ʾ�汾��
	static void printPath(FCBTree* _node);	// ��ʾ�ļ�·��
	static void printFileFunc();			// �ļ�������Ϣ
	void printUsers();						// ��ʾ�û���Ϣ


	// ���ܺ�����д��Ϣ
	void writeFile(ofstream& _file, FCB _f);

	// ���ܺ�������ʽ����
	string standardPath(FCBTree* _node);
	string standardString(const string& str);
	string standardString(string temp, int len);
	int stringToInt(const string& str);
	string intToString(const int& str, int len);
	string enumToString(const Command cmd);
	Command stringToEnum(const string str);


	void analysisPath();
	void analysisCmd(const std::string& input, string& cmd, string& suffix);// ����ָ��

	// ���ܺ������˵�
	int initMenu();															// �������
	void funcMenu();														// �����ܽ���
	void fileMenu(int num, int& pos, string cmd, FCBTree* node, bool& f);	// �ļ���������

	// �û�����
	void saveUsers();
	void loadMT();
	void loadUsers();
	// ע��+��¼
	int createUser();
	int userLogin();
	int saveFCBTree(FCBTree* _node);
	// �ļ�Ȩ��
	bool fileRight(FCBTree* _node);
	bool verifyPasswd(const string _code, string _input);
	// ����rename
	bool rename(FCBTree* _node, string _name);

	bool Update();
	bool saveModifyTimesToFile();
	void updateData();
public:
	/*
	* �ļ���
		create�����ļ�
		deleteɾ���ļ�
		open���ļ�
		close�ر��ļ�
		read���ļ�
		writeд�ļ�
		move        �ƶ��ļ�
		copy  �����ļ�
		flock         �ļ���������Ҫʵ�ּ����ͽ�������
		head �Cnum  ��ʾ�ļ���ǰnum��
		tail  -num  ��ʾ�ļ�β���ϵ�num��
		lseek      �ļ���дָ����ƶ������ļ�ָ�뵱ǰλ�ô�����ƶ� offset������ʱ��ǰ�ƶ�offset
	*/
	void helpMenu();							// ��������
	void exitSystem();							// �˳�ϵͳ
	void clearSys();							// ����


	bool createFile();							// �����ļ�
	bool deleteFile(string path);				// ɾ���ļ�
	void openFile(string path);					// ���ļ�
	void closeFile(FCBTree* _node);// �ر��ļ�
	bool _readFile(int& pos, FCBTree* _node, int num);	// ���ļ�
	bool _writeFile(int& pos, FCBTree* _node);	// д�ļ�
	void lseek(int& pos, FCBTree* _node);		// �ļ���дָ����ƶ�
	void headFile(int num, FCBTree* _node);		// ��ʾ�ļ���ǰnum��
	void tailFile(int num, FCBTree* _node);		// ��ʾ�ļ���βnum��
	void lockFile();							// �ļ�����
	void moveFile(string str);					// �ƶ��ļ�
	void copyFile();							// �����ļ�

	/*
	* Ŀ¼��
		cd			����Ŀ¼
		dir         ��ʾ��ǰĿ¼
		mkdir		����Ŀ¼
		rmdir		ɾ��Ŀ¼����/��Ϊ�գ���Ŀ¼����
	*/
	bool cd(string path);						// ����Ŀ¼
	void dir();									// ��ʾ��ǰĿ¼
	void mkdir();								// ����Ŀ¼
	void rmdir(string path);					// ɾ��Ŀ¼

	/*
	��import c:\a.txt .��    ������C���µ�a.txt���뵽��ǰĿ¼��
	��export a.txt c:\��   ����ǰĿ¼�µ�a.txt����������C�̡�
	*/
	bool importFile(string str);				// ���뵽��ǰĿ¼
	bool exportFile(string str);				// ����������C��


};

