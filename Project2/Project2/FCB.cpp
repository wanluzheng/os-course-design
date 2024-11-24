#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <ctime>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "FCB.h"
using namespace std;

FCB::FCB()
{
	isDirectory = 0;
	fileName = "";
	fileData = "";
	isLocked = false;
	unlockCode = "";
	updateModifyTime();
}

FCB::~FCB()
{

}

bool FCB::getIsDir()
{
	return isDirectory;
}

string FCB::getFileName()
{
	return fileName;
}

string FCB::getFileData()
{
	return fileData;
}

bool FCB::getIsLocked()
{
	return isLocked;
}

string FCB::getUnlockCode()
{
	return unlockCode;
}

string FCB::getModifyTime()
{
	return modifyTime;
}

void FCB::setIsDir(const bool _isDir)
{
	isDirectory = _isDir;
}

void FCB::setFileName(const string _fileName)
{
	fileName = _fileName;
}

void FCB::setFileData(const string _fileData)
{
	fileData = _fileData;
}

void FCB::setIsLocked(const bool _isLocked)
{
	isLocked = _isLocked;
}

void FCB::setUnlockCode(const string _unlockCode)
{
	unlockCode = _unlockCode;
}

void FCB::setModifyTime(const string _mT)
{
	modifyTime = _mT;
}

void FCB::updateModifyTime()
{
	// 获取当前时间
	time_t now = time(0);
	tm* ltm = localtime(&now);

	// 格式化时间为"年+月份+日+时间"的形式
	std::ostringstream oss;
	oss << 1900 + ltm->tm_year;  // 年

	oss << std::setfill('0') << std::setw(2) << 1 + ltm->tm_mon;  // 月份
	oss << std::setfill('0') << std::setw(2) << ltm->tm_mday;  // 日
	oss << std::setfill('0') << std::setw(2) << ltm->tm_hour;  // 时间（小时）
	oss << std::setfill('0') << std::setw(2) << ltm->tm_min;  // 时间（分钟）

	modifyTime = oss.str();
}

FCBTree::FCBTree()
{

	file = FCB();
	isLocked = false;
}

//FCBTree::FCBTree(const FCBTree& demo)
//{
//	file = demo.file;
//	fNode=new FCBTree();
//	*fNode = *demo.fNode;
//	 cNodes=demo.cNodes;
//	isLocked=demo.isLocked;
//}

FCBTree::~FCBTree()
{

}




void FCBTree::insertNode(FCBTree* _fNode, FCBTree* cNode)
{
	_fNode->cNodes.push_back(cNode);
	cNode->fNode = _fNode;
	cNode->file.updateModifyTime();
}

bool FCBTree::deleteNode(FCBTree* _fNode, FCBTree* cNode)
{
	auto it = _fNode->cNodes.begin();
	while (it != _fNode->cNodes.end())
	{
		if ((*it)->file.getFileName() == cNode->file.getFileName())
			break;
		it++;
	}
	if (it != _fNode->cNodes.end())
	{
		_fNode->cNodes.erase(it);

	}
	//cNode->file.updateModifyTime();
	return true;
}

FCBTree* FCBTree::searchNode(FCBTree* _root, const string& _fileName)
{
	if (_root == nullptr || _root->file.getFileName() == _fileName) {
		return _root;
	}

	for (FCBTree* child : _root->cNodes)
	{
		FCBTree* res = searchNode(child, _fileName);
		if (res != nullptr)
			return res;
	}
	return nullptr;
}

FCBTree* FCBTree::searchChild(FCBTree* _fNode, const string& _fileName, bool _isDir)
{
	for (auto i = 0; i < _fNode->cNodes.size(); i++)
	{
		if (_fNode->cNodes[i]->file.getFileName() == _fileName && _fNode->cNodes[i]->file.getIsDir() == _isDir)
			return _fNode->cNodes[i];
	}
	return nullptr;
}