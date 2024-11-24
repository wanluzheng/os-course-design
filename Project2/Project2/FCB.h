#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

class FCB
{
private:
	bool isDirectory;	// 文件类型（1-目录，0-文件）
	string fileName;	// 文件名称
	string fileData;	// 存储文件的内容
	bool isLocked;		// 加锁（1-锁定，0-未锁定）
	string unlockCode;	// unlock_file（文件加锁的时候用到）
	string modifyTime;	// 修改时间
public:

	FCB();
	~FCB();

	bool getIsDir();
	string getFileName();
	string getFileData();
	bool getIsLocked();
	string getUnlockCode();
	string getModifyTime();

	void setIsDir(const bool _isDir);
	void setFileName(const string _fileName);
	void setFileData(const string _fileData);
	void setIsLocked(const bool _isLocked);
	void setUnlockCode(const string _unlockCode);
	void setModifyTime(const string _mT);

	void updateModifyTime();
};

class FCBTree :private FCB
{
private:
public:
	FCB file;
	FCBTree* fNode;
	vector<FCBTree*> cNodes;
	bool isLocked;

	FCBTree();
	//FCBTree(const FCBTree* demo);
	~FCBTree();

	// function
	//string standardPath(FCBTree* _node);

	void insertNode(FCBTree* _fNode, FCBTree* cNode);
	bool deleteNode(FCBTree* _fNode, FCBTree* cNode);
	FCBTree* searchNode(FCBTree* _root, const string& fileName);
	FCBTree* searchChild(FCBTree* _fNode, const string& _fileName, bool _isDir);
};