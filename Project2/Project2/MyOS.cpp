#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <map>
#include <regex>
#include <thread>
#include "User.h"
#include "FCB.h"
#include "MyOS.h"
#include <vector>
#include <sstream>

#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

#define VERSION "0.0.0.1"

string PATH = "F:/";        // 默认路径
int curUser;        // 当前用户号

// 线程 1：命令行线程
thread::id _cmd_;
// 线程 2：内核线程
thread::id kernels;

// 两个互斥量mutex对象，模拟同步原语
mutex fileMutex;
mutex m;

string CMD;
string SUFFIX;

int modifedTimes = 0;      // 修改次数
int curTime = 0;

// 模拟多线程: 失败！
//bool MyOS::Multithreading()
//{
//    string name;
//    stringstream ss;
//    ss << root->file.getFileName();
//    getline(ss, name, ':');
//
//    ifstream file(PATH + "/data.bin", ios::binary | ios::in);
//    if (file.is_open())
//    {
//        string _name, _passwd;
//        int no;
//        while (getline(file, _name, '~'))
//        {
//            string cName;
//            if (cName == name)
//            {
//                getline(file, _passwd, '~');
//                string temp;
//                getline(file, temp, '~');
//                FCB _fcb;
//                int t;
//                stringstream _ss;
//                _ss << temp;
//                _ss >> t;
//                for (int i = 0; i < t; i++)
//                {
//                    string tType;
//                    getline(file, tType, '~');
//                    _fcb.setIsDir(stringToInt(tType));
//
//                    string tName;
//                    getline(file, tName, '~');
//                    _fcb.setFileName(tName);
//
//                    string tData;
//                    getline(file, tData, '~');
//                    _fcb.setFileData(tData);
//
//                    string tLock;
//                    getline(file, tLock, '~');
//                    _fcb.setIsLocked(stringToInt(tLock));
//
//                    string mT;
//                    getline(file, mT, '~');
//                    _fcb.setModifyTime(mT);
//
//                    if (_fcb.getIsLocked() == 1)
//                    {
//                        string tDecode;
//                        getline(file, tDecode, '~');
//                        _fcb.setUnlockCode(tDecode);
//                    }
//                }
//                continue;
//            }
//            stringstream _ss;
//            _ss << _name;
//            getline(_ss, cName, ':');
//            bool flag = false;
//            for (int i = 0; i < users.size(); i++)
//            {
//                if (users[i].getUserName() == cName)
//                {
//                    flag = true;
//                    no = i;
//                }
//            }
//            if (flag)
//            {
//                getline(file, _passwd, '~');
//                string _num;
//                getline(file, _num, '~');
//                users[no].setFileNum(stringToInt(_num));
//                users[no].clearData();
//
//                FCB _fcb;
//                stringstream _ss;
//                int total;
//                _ss << users[no].getFileNum();
//                _ss >> total;
//
//                for (int i = 0; i < total; i++)
//                {
//                    string tType;
//                    getline(file, tType, '~');
//                    _fcb.setIsDir(stringToInt(tType));
//
//                    string tName;
//                    getline(file, tName, '~');
//                    _fcb.setFileName(tName);
//
//                    string tData;
//                    getline(file, tData, '~');
//                    _fcb.setFileData(tData);
//
//                    string tLock;
//                    getline(file, tLock, '~');
//                    _fcb.setIsLocked(stringToInt(tLock));
//
//                    string mT;
//                    getline(file, mT, '~');
//                    _fcb.setModifyTime(mT);
//
//                    if (_fcb.getIsLocked() == 1)
//                    {
//                        string tDecode;
//                        getline(file, tDecode, '~');
//                        _fcb.setUnlockCode(tDecode);
//                    }
//                    users[no].addFile(_fcb);
//                }
//            }
//            else
//            {
//                getline(file, _passwd, '~');
//                users.push_back({ _name,_passwd });
//                no = users.size();
//                string _num;
//                getline(file, _num, '~');
//                users[no].setFileNum(stringToInt(_num));
//
//                FCB _fcb;
//                stringstream _ss;
//                int total;
//                _ss << users[no].getFileNum();
//                _ss >> total;
//
//                for (int i = 0; i < total; i++)
//                {
//                    string tType;
//                    getline(file, tType, '~');
//                    _fcb.setIsDir(stringToInt(tType));
//
//                    string tName;
//                    getline(file, tName, '~');
//                    _fcb.setFileName(tName);
//
//                    string tData;
//                    getline(file, tData, '~');
//                    _fcb.setFileData(tData);
//
//                    string tLock;
//                    getline(file, tLock, '~');
//                    _fcb.setIsLocked(stringToInt(tLock));
//
//                    string mT;
//                    getline(file, mT, '~');
//                    _fcb.setModifyTime(mT);
//
//                    if (_fcb.getIsLocked() == 1)
//                    {
//                        string tDecode;
//                        getline(file, tDecode, '~');
//                        _fcb.setUnlockCode(tDecode);
//                    }
//                    users[no].addFile(_fcb);
//                }
//            }
//        }
//        file.close();
//        return true;
//    }
//    else
//        return false;    
//}


// 多线程
[[noreturn]] void MyOS::kernel()
{
    kernels = this_thread::get_id();
    while (true)
    {
        unique_lock<mutex> lock(m); // 加锁，防止多个线程同时访问
        cv.wait(lock, [this] { return ready; });    // 等待 ready 变为 true

        Command _cmd = stringToEnum(CMD);
        if (!Update() || CMD == "open")
        {
            cout << 1 << endl;
            updateData();
        }

        if (CMD == "")
        {
            _cmd = Command::empty;
        }
        switch (_cmd)
        {
        case Command::help:
        {
            helpMenu();
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::ver:
        {
            printVersion();
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::exit:
        {
            exitSystem();
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::create:
        {
            if (createFile())
            {
                modifedTimes++;
                saveUsers();
            }
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::_delete:
        {
            if (deleteFile(SUFFIX))
            {
                modifedTimes++;
                saveUsers();
            }

            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::open:
        {
            openFile(SUFFIX);
            modifedTimes++;
            saveUsers();

            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::read:
        case Command::write:
        case Command::close:
        case Command::lseek:
        case Command::head:
        case Command::tail:
        {
            cout << "Error: Please open the file first!" << endl;
            break;
        }
        case Command::clear:
        {

            clearSys();
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::flock:
        {
            lockFile();
            modifedTimes++;
            saveUsers();
            //saveModifyTimesToFile();
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::move:
        {
            moveFile(SUFFIX);
            //saveUsers();
            //saveModifyTimesToFile();
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::copy:
        {
            copyFile();
            modifedTimes++;
            saveUsers();
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::cd:
        {
            cd(SUFFIX);
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::dir:
        {
            dir();
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::mkdir:
        {
            mkdir();
            modifedTimes++;
            saveUsers();
            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::rmdir:
        {
            rmdir(SUFFIX);
            modifedTimes++;
            saveUsers();

            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::_import:
        {
            importFile(SUFFIX);
            modifedTimes++;
            saveUsers();

            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        case Command::_export:
        {
            exportFile(SUFFIX);
            modifedTimes++;
            saveUsers();

            CMD = "";
            ready = false;
            cv.notify_all();    // 唤醒所有线程
            break;
        }
        default:
            break;
        }
    }


}


void MyOS::initFCBTree()
{
    root = new FCBTree();
    root->fNode = nullptr;
    root->file.setIsDir(1);
    root->file.setFileName(users[curUser].getUserName() + ":/");
    cNode = root;
}

// 功能函数：打印函数
// 打印函数
void MyOS::printLogo()
{
    //    cout << endl <<
    //        "\033[1;37m ███████  ██   ██    ██   ██        █     █████           ████        █████            \033[0m\n"
    //        "\033[1;37m     ██   ██   ██    ██   ██       █    ███             ██    ██    ███                \033[0m\n"
    //        "\033[1;37m    ██     ██ ██ ██ ██    ██      █       ████         ██      ██     ████            \033[0m\n"
    //        "\033[1;37m  ██       ██ ██ ██ ██    ██                 ███        ██    ██         ███          \033[0m\n"
    //        "\033[1;37m ███████    ██    ██      ███████        █████            ████       █████            \033[0m\n";
    //}
    std::cout << "  _____       _             _____ _           _   _             \n";
    std::cout << " |  __ \\     (_)           / ____| |         | | (_)            \n";
    std::cout << " | |  | | ___ _ _ __ ___  | (___ | |__   __ _| |_ _  ___  _ __  \n";
    std::cout << " | |  | |/ _ \\ | '_ ` _ \\  \\___ \\| '_ \\ / _` | __| |/ _ \\| '_ \\ \n";
    std::cout << " | |__| |  __/ | | | | | | ____) | | | | (_| | |_| | (_) | | | |\n";
    std::cout << " |_____/ \\___|_|_| |_| |_| _____/|_| |_|\\__,_|\\__|_|\\___/|_| |_|\n";
    std::cout << "                                                               \n";
}


// 模拟缓冲条
void MyOS::printBar()
{
    cout << "Wait[";
    for (int i = 0; i < 15; i++)
    {
        cout << "███";
        this_thread::sleep_for(std::chrono::milliseconds(100));
        cout.flush();
    }
    cout << "]" << endl;
}

// 打印时间
void MyOS::printTime()
{
    time_t currentTime = time(nullptr);

    // 将时间转换为本地时间
    tm* localTime = localtime(&currentTime);

    // 输出当前时间
    cout << localTime->tm_year + 1900 << "-" << localTime->tm_mon + 1 << "-" << localTime->tm_mday << " ";
    cout << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << std::endl;
}

// 打印版本号
void MyOS::printVersion()
{
    cout << "Current version: " << VERSION << endl;
}

// 显示用户信息
void MyOS::printUsers()
{
    cout << "All user information is as follows" << endl;
    for (auto i = 0; i < users.size(); i++)
    {
        cout << "User name: " << users[i].getUserName() << "\t User's password: " << users[i].getUserPasswd() << endl;
    }
    cout << endl;
}

// 显示文件路径
void MyOS::printPath(FCBTree* _node)
{
    if (_node->fNode != nullptr)
    {
        printPath(_node->fNode);
    }
    cout << _node->file.getFileName() << '/';
}

// 显示文件操作
void MyOS::printFileFunc()
{
    std::cout << "read\t\t读文件" << std::endl;
    std::cout << "write\t\t写文件" << std::endl;
    std::cout << "head -num\t显示文件的前num行" << std::endl;
    std::cout << "tail -num\t显示文件尾巴上的num行" << std::endl;
    std::cout << "lseek\t\t文件" << std::endl;
    std::cout << "exit\t\t退出" << std::endl;
}

// 功能函数：格式重整
// 递归获取路径
string MyOS::standardPath(FCBTree* _node)
{
    string sPath;
    if (_node->fNode != nullptr)
    {
        sPath += standardPath(_node->fNode);
        sPath += '/';
    }
    sPath += _node->file.getFileName();
    return sPath;
}

// 去除指令两端的空格
string MyOS::standardString(const string& str)
{
    size_t start = str.find_first_not_of(" \t"); // 找到第一个非空格字符的位置
    if (start == std::string::npos) {
        return ""; // 如果字符串全是空格，则返回空字符串
    }

    size_t end = str.find_last_not_of(" \t"); // 找到最后一个非空格字符的位置
    return str.substr(start, end - start + 1); // 返回去除两端空格后的子字符串
}

string MyOS::standardString(string str, int len)
{
    // 如果输入字符串长度已经达到或超过指定长度，则直接返回输入字符串
    if (str.length() >= len)
    {
        return str;
    }
    // 如果输入字符串长度小于指定长度，则在末尾添加特定字符 '~' 直到达到指定长度
    else
        return str + std::string(len - str.length(), '~');
}
// string转int
int MyOS::stringToInt(const string& str)
{
    return strtol(str.c_str(), nullptr, 10);
}

// int转string 直接用to_string
string MyOS::intToString(const int& str, int len)
{
    string temp = to_string(str);
    string res = standardString(temp, len);
    return temp;
}

// enum转换为string
string MyOS::enumToString(const Command cmd)
{
    static std::map<Command, string> cmdMap =
    {
        {Command::help,"help"},
        {Command::ver,"ver"},
        {Command::exit,"exit"},
        {Command::create,"create"},
        {Command::_delete,"delete"},
        {Command::open,"open"},
        {Command::close,"close"},
        {Command::read,"read"},
        {Command::write,"write"},
        {Command::lseek,"lseek"},
        {Command::head,"head"},
        {Command::tail,"tail"},
        {Command::clear,"clear"},
        {Command::flock,"flock"},
        {Command::move,"move"},
        {Command::copy,"copy"},
        {Command::cd,"cd"},
        {Command::dir,"dir"},
        {Command::mkdir,"mkdir"},
        {Command::rmdir,"rmdir"},
        {Command::_import,"import"},
        {Command::_export,"export"},
    };
    return cmdMap[cmd];
}

// string转换为enum
Command MyOS::stringToEnum(const string str)
{
    static std::map<string, Command> stringMap =
    {
        {"help",Command::help},
        {"ver",Command::ver},
        {"exit",Command::exit},
        {"create",Command::create},
        {"delete",Command::_delete},
        {"open",Command::open},
        {"close",Command::close},
        {"read",Command::read},
        {"write",Command::write},
        {"lseek",Command::lseek},
        {"head",Command::head},
        {"tail",Command::tail},
        {"clear",Command::clear},
        {"flock",Command::flock},
        {"move",Command::move},
        {"copy",Command::copy},
        {"cd",Command::cd},
        {"dir",Command::dir},
        {"mkdir",Command::mkdir},
        {"rmdir",Command::rmdir},
        {"import",Command::_import},
        {"export",Command::_export},

    };
    return stringMap[str];
}

// 解析path，好多bug，有待改进
void MyOS::analysisPath()
{
    stringstream ss;
    int temp;
    ss << users[curUser].getFileNum();
    ss >> temp;

    for (auto i = 0; i < temp; i++)
    {
        stringstream ss;
        string name = users[curUser].getFileMessage(i).getFileName();

        ss << name;
        string temp, next;
        FCBTree* node = root;
        getline(ss, temp, '/');
        getline(ss, temp, '/');

        // 非目录
        if (users[curUser].getFileMessage(i).getIsDir() == 0)
        {
            while (getline(ss, temp, '/'))
            {
                bool test = ss.eof();
                if (!ss.eof())
                {
                    bool exist = false;
                    FCBTree* ex = node->searchChild(node, temp, 1);
                    if (ex != nullptr)
                    {
                        exist = true;
                        node = ex;
                    }
                    if (!exist)
                    {
                        FCBTree* add = new FCBTree();
                        add->file.setIsDir(1);
                        add->file.setFileName(temp);

                        node->insertNode(node, add);
                        node = add;
                    }
                }
                else
                {
                    break;
                }
            }

            FCBTree* add = new FCBTree();
            add->file = users[curUser].getFileMessage(i);
            add->file.setFileName(temp);
            node->insertNode(node, add);
            continue;
        }

        while (getline(ss, temp, '/'))
        {
            bool exist = false;

            FCBTree* ex = node->searchChild(node, temp, 1);
            if (ex != nullptr) // 存在文件夹且目标文件夹加密
            {
                exist = true;
                if (users[curUser].getFileMessage(i).getIsLocked() == 1 && users[curUser].getFileMessage(i).getFileName() == temp)
                {
                    ex->file = users[curUser].getFileMessage(i);
                    ex->file.setFileName(temp);
                }
                node = ex;
            }
            if (!exist)//不存在文件
            {
                FCBTree* add = new FCBTree();
                add->file.setIsDir(1);
                add->file = users[curUser].getFileMessage(i);
                add->file.setFileName(temp);
                node->insertNode(node, add);
                node = add;
            }
        }
    }
}

// 解析指令
void MyOS::analysisCmd(const std::string& input, string& cmd, string& suffix)
{
    istringstream iss(input);
    string command;
    string path;
    iss >> command >> path;

    cmd = command;
    suffix = path;
    //std::cout << "Command: " << command << std::endl;
    //std::cout << "Path: " << path << std::endl;
}


// 功能函数：菜单
// 获取用户信息菜单
int MyOS::initMenu()
{
    bool flag = true;
    while (true)
    {
        printLogo();
        cout << "****************Welcome to ZWL's OS****************" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << "****************************************************" << endl << endl;
        cout << "	1、Login		2、Register		0、Exit	" << endl << endl;
        cout << "****************************************************" << endl;
        cout << "Please input...：";
        int choice;
        cin >> choice;
        curUser = -1;
        if (choice == 1)
        {
            curUser = userLogin();
            //cout << curUser << endl;
            return curUser;
        }
        else if (choice == 2)
        {
            curUser = createUser();
            //cout << curUser << endl;
            return curUser;
        }
        else if (choice == 0)
        {
            exitSystem();
        }
        else
        {
            cout << "Error: Please input again." << endl;
        }
    }
    cout << "\n\n\nEnter to continue......" << endl;
    system("cls");
    return curUser;

}

// 主功能界面
void MyOS::funcMenu()
{
    // 线程 1：命令行线程
    _cmd_ = this_thread::get_id();
    system("color 0E");

    // 载入用户信息
    loadUsers();
    initMenu();
    // 初始化
    initFCBTree();
    analysisPath();
    system("cls");

    cout << "****************Welcome to ZWL's OS****************" << endl;
    cout << "----------------------------------------------------" << endl;
    string input = "";
    string command = "";
    string path = "";
    Command _cmd;
    bool update = true;

    while (true)
    {
        if (update) //判断有无更新，根据修改次数来进行判断
        {
            printPath(cNode);
            cout << ">";
        }
        update = true;

        getline(cin, input);

        analysisCmd(input, command, path);
        //cout << "Cmd: " << command << " Path: " << path << endl;
        SUFFIX = path;
        CMD = command;

        if (command == "")
        {
            _cmd = Command::empty;
        }
        _cmd = stringToEnum(command);

        switch (_cmd)
        {
        case Command::help:
        {
            unique_lock<mutex> lock(m);
            //if (deleteFile(path))
            //    saveUsers();
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::ver:
        {
            unique_lock<mutex> lock(m);
            //if (deleteFile(path))
            //    saveUsers();
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::exit:
        {
            //unique_lock<mutex> lock(m);
            //if (deleteFile(path))
            //    saveUsers();
            ready = true;
            exitSystem();
            //cv.notify_all();
            //cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::create:
        {
            unique_lock<mutex> lock(m);
            //if (createFile())
            //    saveUsers();
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::_delete:
        {
            unique_lock<mutex> lock(m);
            //if (deleteFile(path))
            //    saveUsers();
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::open:
        {
            unique_lock<mutex> lock(m);
            //openFile(path);
            ready = true;
            cv.notify_all();    //用于唤醒所有正在等待与 cv 相关联的条件变量的线程
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::read:
        case Command::write:
        case Command::close:
        case Command::lseek:
        case Command::head:
        case Command::tail:
        {
            cout << "Error: Please open the file first!" << endl;
            break;
        }
        case Command::clear:
        {
            unique_lock<mutex> lock(m);

            //lockFile();
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
            break;
        }
        case Command::flock:
        {
            unique_lock<mutex> lock(m);

            //lockFile();
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::move:
        {
            unique_lock<mutex> lock(m);
            SUFFIX = input;
            //moveFile();
            //saveUsers();
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::copy:
        {
            unique_lock<mutex> lock(m);

            /*            copyFile();
                        saveUsers();*/
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::cd:
        {
            unique_lock<mutex> lock(m);
            //cd(path);
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::dir:
        {
            unique_lock<mutex> lock(m);
            //dir();
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::mkdir:
        {
            unique_lock<mutex> lock(m);
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::rmdir:
        {
            unique_lock<mutex> lock(m);
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::_import:
        {
            unique_lock<mutex> lock(m);
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        case Command::_export:
        {
            unique_lock<mutex> lock(m);
            SUFFIX = input;
            ready = true;
            cv.notify_all();
            cv.wait(lock, [this] { return !ready; });
            break;
        }
        default:
            break;
        }
        CMD = "";
        SUFFIX = "";
        input = "";
        command = "";
        path = "";
    }


}

// 文件操作界面
void MyOS::fileMenu(int num, int& pos, string cmd, FCBTree* node, bool& f)
{
    //unique_lock<mutex> fileLock(fileMutex);

    Command _cmd = stringToEnum(cmd);
    switch (_cmd)
    {
    case Command::help:
    {
        printFileFunc();
        break;
    }
    case Command::read:
    {
        _readFile(pos, node, num);
        break;
    }
    case Command::write:
    {
        _writeFile(pos, node);
        saveUsers();
        break;
    }
    case Command::lseek:
    {
        lseek(pos, node);
        break;
    }
    case Command::close:
    {
        closeFile(node);
        break;
    }
    case Command::head:
    {
        headFile(num, node);
        break;
    }
    case Command::tail:
    {
        tailFile(num, node);
        break;
    }
    case Command::exit:
    {
        closeFile(node);
        f = false;
        return;
    }
    default:
        break;
    }
    //fileLock.unlock();
    return;
}


// 获取权限
// 文件权限
bool MyOS::fileRight(FCBTree* node)
{
    // 如果文件已上锁
    if (node->file.getIsLocked() && node->file.getUnlockCode() != "$$$")
    {
        // 输入密码解锁
        cout << "Please enter the password to unlock the file:" << endl;
        string passwd;
        cin >> passwd;

        // 密码错误
        if (!verifyPasswd(node->file.getUnlockCode(), passwd))
        {
            cout << "Error: Password error!" << endl;
            return false;
        }
        else
        {
            cin.ignore();
            node->file.setUnlockCode("$$$");
            node->file.setIsLocked(true);
            return true;
        }
    }
    else if (node->file.getUnlockCode() == "$$$")
        return false;
    return true;
}

// 核对密码
bool MyOS::verifyPasswd(const string _code, string _input)
{
    if (_code == _input)
        return true;
    return false;
}

// 重名~重命名
bool MyOS::rename(FCBTree* _node, string _name)
{
    if (_name == _node->file.getFileName())
    {
        return false;
    }
    if (cNode->searchChild(cNode, _name, 0))
    {
        return false;
    }
    _node->file.setFileName(_name);
    return true;
}

bool MyOS::Update()
{
    //unique_lock<mutex> fileLock(fileMutex);
    fstream file(PATH + "\data.bin", ios::in | ios::out);   //输出流
    string ss;
    if (!file.is_open()) {
        cout << "File open failed!" << endl;
        return false;
    }
    file.seekg(0, ios::beg);//将文件指针定位到文件开头
    file >> ss;
    // 检查是否修改
    if (strtod(ss.c_str(), nullptr) == modifedTimes) {
        return true;
    }
    else {
        modifedTimes = strtod(ss.c_str(), nullptr);
        return false;
    }
    file.close();
    // 解锁
    //fileLock.unlock();
}

bool MyOS::saveModifyTimesToFile()
{
    //unique_lock<mutex> fileLock(fileMutex);
    fstream file;
    string ss;
    file.open(PATH + "/data.bin", ios::out | ios::in);
    if (!file.is_open()) {
        cout << "Error: Can't open file!1111111" << endl;
        return false;
    }
    std::string firstLine;
    if (std::getline(file, firstLine)) {
        // 将文件指针移动到文件的开头
        file.seekp(0, std::ios::beg);
        // 写入新的第一行内容
        file << to_string(modifedTimes) << std::endl;

        //// 清空之前的内容（如果新内容长度小于旧内容）
        //file.seekp(file.tellp(), std::ios::beg);
        //file << std::string(firstLine.size() - to_string(modifedTimes).size(), ' ');
        return true;
    }
    else
        return false;
    // 解锁
    //fileLock.unlock();
    return true;
}

// 更新数据
void MyOS::updateData()
{
    users.clear();
    loadUsers();
    initFCBTree();
    analysisPath();

}

// 保存操作
// 写文件信息
void MyOS::writeFile(ofstream& _file, FCB _f)
{
    _file << to_string(_f.getIsDir()) << '~';
    _file << _f.getFileName() << '~';
    _file << _f.getFileData() << '~';
    _file << to_string(_f.getIsLocked()) << '~';
    _file << _f.getModifyTime() << '~';

    if (_f.getIsLocked())
        _file << _f.getUnlockCode() << '~';


}

// 保存用户信息
void MyOS::saveUsers()
{
    //Multithreading();
    unique_lock<mutex> fileLock(fileMutex);

    ofstream file(PATH + "/data.bin", ios::binary | ios::trunc);
    if (!file.is_open())
    {
        cout << "Error: Other users are currently using it!" << endl;
        return;
    }
    if (!users.empty() && root != nullptr)
    {
        users[curUser].clearData();
        users[curUser].setFileNum(saveFCBTree(root));
    }
    cout << 1 << endl;
    //saveModifyTimesToFile();
    file << to_string(modifedTimes) << endl;
    for (User& u : users)
    {

        file << u.getUserName() << '~';
        file << u.getUserPasswd() << '~';
        file << to_string(u.getFileNum()) << '~';


        int temp;
        stringstream ss;
        ss << u.getFileNum();
        ss >> temp;
        for (int i = 0; i < temp; i++)
            writeFile(file, u.getFileMessage(i));
    }
    fileLock.unlock();

    file.close();
    //cout << "User information saved successfully!" << endl;
}

// 保存结构信息
int MyOS::saveFCBTree(FCBTree* _node)
{
    //unique_lock<mutex> fileLock(fileMutex);

    int num = 0;
    if (_node->fNode != nullptr)
    {
        num++;
        string temp = _node->file.getFileName();
        _node->file.setFileName(standardPath(_node));

        users[curUser].addFile(_node->file);
        _node->file.setFileName(temp);
    }
    for (auto i = 0; i < _node->cNodes.size(); i++)
    {
        num += saveFCBTree(_node->cNodes[i]);
    }
    //fileLock.unlock();
    return num;
}

void MyOS::loadMT()
{
    ifstream file(PATH + "/data.bin", ios::binary | ios::in);
    if (!file.is_open())
    {
        cout << "Error: Can't open file!2222" << endl;
        return;
    }

    string mT;
    getline(file, mT, '\n');
    modifedTimes = stringToInt(mT);
    cout << modifedTimes;
}

// 载入用户信息
void MyOS::loadUsers()
{
    ifstream file(PATH + "/data.bin", ios::binary | ios::in);
    if (!file.is_open())
    {
        cout << "Error: Can't open file!" << endl;
        return;
    }
    else
    {
        //cout << "Open successfully!" << endl;
        string _userName, _passwd;
        int no = 0;

        string mt;
        getline(file, mt, '\n');
        modifedTimes = stringToInt(mt);

        while (getline(file, _userName, '~'))
        {
            getline(file, _passwd, '~');
            users.push_back({ _userName, _passwd });
            string tempNum;
            getline(file, tempNum, '~');
            users[no].setFileNum(stringToInt(tempNum));

            FCB _fcb;
            stringstream ss;
            int temp;
            ss << users[no].getFileNum();
            ss >> temp;
            for (int i = 0; i < temp; i++)
            {
                string tType;
                getline(file, tType, '~');
                _fcb.setIsDir(stringToInt(tType));

                string tName;
                getline(file, tName, '~');
                _fcb.setFileName(tName);

                string tData;
                getline(file, tData, '~');
                _fcb.setFileData(tData);

                string tLock;
                getline(file, tLock, '~');
                _fcb.setIsLocked(stringToInt(tLock));

                string mT;
                getline(file, mT, '~');
                _fcb.setModifyTime(mT);

                if (_fcb.getIsLocked() == 1)
                {
                    string tDecode;
                    getline(file, tDecode, '~');
                    _fcb.setUnlockCode(tDecode);
                }

                users[no].addFile(_fcb);
            }
            no++;
        }

        file.close();
        //printUsers();
    }
}

// 创建新用户
int MyOS::createUser()
{
    cout << "Please input [-l] to login" << endl;

    // 输入用户名
    cout << "Please input your choice (Enter any key to continue):" << endl;
    string userName;
    cin >> userName;

    // 如果是登录，跳转~
    if (userName.find("-l") != string::npos)
    {
        userLogin();
        return 0;
    }

    cout << "Welcome to ZWL's OS!" << endl;

    // 用户名
    cout << "Please input your username :" << endl;
    while (cin >> userName)
    {
        //// 规范性判断
        //// 过长
        //if (userName.length() > 20)
        //{
        //    cout << "Error: the username must be less than 20 characters!" << endl;
        //    cout << "Please input your username (less than 20 characters, more than 6 characters):" << endl;
        //    continue;
        //}

        // 过短
        if (userName.length() < 3)
        {
            //cout << "Error: the username must be more than 6 characters!" << endl;
            cout << "Please input your username (less than 20 characters, more than 6 characters):" << endl;
            continue;
        }

        // 重名
        for (int i = 0; i < users.size(); i++)
        {
            if (users[i].getUserName() == userName)
            {
                cout << "Error: the username already exists!" << endl;
                cout << "Please input your username (less than 20 characters, more than 6 characters):" << endl;
                continue;
            }
        }

        //// 字符范围和格式要求
        //std::regex pattern("[a-zA-Z][a-zA-Z0-9_]+");
        //if (!std::regex_match(userName, pattern))
        //{
        //    cout << "Error: the username format is illegal!" << endl;
        //    cout << "Please input your username (less than 20 characters, more than 6 characters):" << endl;
        //    continue;
        //}
        break;
    }
    string userPasswd;
    cout << "Please input your password(less than 10 characters):" << endl;

    while (cin >> userPasswd)
    {
        // 规范性判断
        // 过长
        if (userPasswd.length() < 3)
        {
            cout << "Error: your password is too short!" << endl;
            cout << "Please input your password:" << endl;
            continue;
        }
        //// 字符范围和格式要求
        //std::regex pattern("[a-zA-Z][a-zA-Z0-9_]+");
        //if (!std::regex_match(userPasswd, pattern))
        //{
        //    cout << "Error: the password format is illegal!" << endl;
        //    cout << "Please input your password(less than 10 characters):" << endl;
        //    continue;
        //}
        break;
    }
    cout << "Welcome " << userName << endl;
    printBar();
    //// 确认密码
    //string verifyPasswd;
    //cout << "Please input your password again." << endl;
    //while (cin >> verifyPasswd)
    //{
    //    if (verifyPasswd != userPasswd)
    //    {
    //        cout << "The password is not as same as the first one, please input again" << endl;
    //        cout << "Please input your password(less than 10 characters):" << endl;
    //        continue;
    //    }
    //    else
    //    {

    //    }
    //}

    User temp(userName, userPasswd);
    users.push_back(temp);
    modifedTimes = 0;
    saveUsers();
    printUsers();
    return users.size() - 1;

}

// 用户登录
int MyOS::userLogin()
{
    // 输入用户名
    cout << "Please input your username:" << endl;
    string userName;
    cin >> userName;

    int exist = -1;
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i].getUserName() == userName)
        {
            exist = i;
        }
    }
    if (exist < 0)
    {
        cout << userName << " doesn't exist!" << endl;
        cout << "Please register a new account" << endl;
        return -1;
    }
    else
    {
        cout << "Please input your password:" << endl;
        string password;
        cin >> password;
        if (users[exist].getUserPasswd() != password)
        {
            cout << "The password doesn't match!" << endl;
            return -1;
        }
        else
        {
            printBar();
            cout << "Welcome " << userName << endl;
            return exist;
        }
    }
}

// 指令
// 退出系统
void MyOS::exitSystem()
{
    modifedTimes = 0;
    saveUsers();
    cout << "Looking forward to meeting you again." << endl;
    exit(0);
}

// 帮助（查看所有指令）
void MyOS::helpMenu()
{
    std::cout << "命令列表：" << std::endl;
    std::cout << "create\t\t创建文件" << std::endl;
    std::cout << "delete\t\t删除文件" << std::endl;
    std::cout << "open\t\t打开文件" << std::endl;
    std::cout << "close\t\t关闭文件" << std::endl;
    std::cout << "read\t\t读文件" << std::endl;
    std::cout << "write\t\t写文件" << std::endl;
    std::cout << "move\t\t移动文件" << std::endl;
    std::cout << "copy\t\t拷贝文件" << std::endl;
    std::cout << "flock\t\t文件加锁，需要实现加锁和解锁功能" << std::endl;
    std::cout << "head -num\t显示文件的前num行" << std::endl;
    std::cout << "tail -num\t显示文件尾巴上的num行" << std::endl;
    std::cout << "lseek\t\t文件" << std::endl;
    std::cout << "cd\t\t进入目录" << std::endl;
    std::cout << "dir\t\t显示目录" << std::endl;
    std::cout << "mkdir\t\t创建目录" << std::endl;
    std::cout << "rmdir\t\t删除目录" << std::endl;
    std::cout << "ver\t\t查看版本号" << std::endl;
    std::cout << "import\t\t导入文件" << std::endl;
    std::cout << "export\t\t导出文件" << std::endl;
}

// 清屏
void MyOS::clearSys()
{
    // 清屏
    system("cls");
}

// 创建文件
bool MyOS::createFile()
{
    //unique_lock<mutex> fileLock(fileMutex);
    FCBTree* addNode = new FCBTree();
    cout << cNode->file.getFileName() << ">";

    cout << "Please input the name of the file:" << endl;
    string _fileName;
    cin >> _fileName;
    cin.ignore();

    // 鲁棒性
    if (_fileName == "")
    {
        cout << "Error: The file name cannot be empty!" << endl;
        return false;
    }

    if (cNode->searchChild(cNode, addNode->file.getFileName(), 0) != nullptr)
    {
        cout << "Error: The file with this name already exists!" << endl;
        return false;
    }
    // 输入内容
    cout << "Please enter the file content (ctrl + z to finish):" << endl;
    string line;
    string content;

    while (getline(cin, line))
    {
        line += '\n';
        content += line;
    }

    addNode->file.setFileName(_fileName);
    addNode->file.setIsDir(0);
    addNode->file.setFileData(content);
    addNode->file.updateModifyTime();
    cin.clear();

    //cout << "Test content:" << endl;
    //cout << content << endl;
    cout << "File " << addNode->file.getFileName() << " created successfully!" << endl;
    cNode->insertNode(cNode, addNode);
    //fileLock.unlock();
    return true;
}

// 删除文件
bool MyOS::deleteFile(string path)
{

    //unique_lock<mutex> fileLock(fileMutex);

    FCBTree* dNode = cNode->searchChild(cNode, path, 0);
    //cout << dNode->file.getFileData();

    if (dNode != nullptr&&dNode->file.getUnlockCode()!="$$$")
    {
        cNode->deleteNode(cNode, dNode);
        cout << "Delete successful!" << endl;
        users[curUser].clearData();
        users[curUser].setFileNum(saveFCBTree(root));
        //cout << "1" << endl;
        return true;
    }
    else if (dNode->file.getUnlockCode() == "$$$")
    {
        cout << "Error: A user is currently modifying!" << endl;
        return false;
    }
    else
    {
        cout << path << " not found." << endl;
        return false;
    }
    //fileLock.unlock();
}

// 打开文件
void MyOS::openFile(string path)
{
    FCBTree* oNode;
    oNode = cNode->searchChild(cNode, path, 0);

    if (oNode == nullptr)
    {
        cout << "Error: Unable to find the file to open!" << endl;
        return;
    }

    // 如果文件未上锁
    if (fileRight(oNode))
    {
        // oNode正在编辑
        oNode->file.setIsLocked(true);
        oNode->file.setUnlockCode("$$$");
        saveUsers();

        cout << "Successfully opened file " << path << endl;

        int pos = 0;
        bool flag = true;
        while (flag)
        {
            string input;

            getline(cin, input);
            string path = "";
            string cmd = "";
            analysisCmd(input, cmd, path);
            int num = 0;
            if (path.size() != 0)
            {
                size_t position = path.find_first_not_of("-");
                path = path.substr(position);
                cout << path;

                num = stringToInt(path);
            }


            fileMenu(num, pos, cmd, oNode, flag);
        }
    }
    // 如果有用户正在编辑
    else if (oNode->file.getUnlockCode() == "$$$")
    {
        cout << "Error: A user is currently modifying!" << endl;
        return;
    }
    else
        return;
}

// 关闭文件
void MyOS::closeFile(FCBTree* _node)
{
    _node->file.setIsLocked(false);
    _node->file.setUnlockCode("");
    cout << "The file " << _node->file.getFileName() << " has been closed." << endl;

}

// 读文件
bool MyOS::_readFile(int& pos, FCBTree* _node, int num)
{
    //unique_lock<mutex> fileLock(fileMutex);
    int len = num;

    string data = _node->file.getFileData().substr(pos, len);
    pos += len;
    if (pos >= _node->file.getFileData().size())
    {
        pos = _node->file.getFileData().size();
    }
    stringstream ss;
    ss << data;
    string line;
    while (getline(ss, line))
    {
        cout << line << endl;
    }
    //fileLock.unlock();
    return true;
}

// 写文件
bool MyOS::_writeFile(int& pos, FCBTree* _node)
{
    //unique_lock<mutex> fileLock(fileMutex);

    string front = _node->file.getFileData().substr(0, pos);
    string data;

    // 输入内容
    cout << "Please enter the file content (ctrl + z to finish):" << endl;
    string line;

    while (getline(cin, line))
    {
        data += line;
        data += '\n';
    }
    front += data;
    _node->file.setFileData(front);
    _node->file.updateModifyTime();
    cout << "Write successful!" << endl;
    cin.clear();
    //fileLock.unlock();
    return true;

}

// 文件读写指针的移动
void MyOS::lseek(int& pos, FCBTree* _node)
{
    //unique_lock<mutex> fileLock(fileMutex);

    int offset;
    cout << "Please enter the amount of movement:" << endl;
    cin >> offset;
    cin.ignore();

    pos += offset;
    if (pos < 0)   pos = 0;
    else if (pos > _node->file.getFileData().size())
        pos = _node->file.getFileData().size();

    //fileLock.unlock();
}

// 显示文件的前num行
void MyOS::headFile(int num, FCBTree* _node)
{
    //unique_lock<mutex> fileLock(fileMutex);

    string data = _node->file.getFileData();
    stringstream ss;
    ss << data;
    string line;

    while (getline(ss, line))
    {
        if (num <= 0)
            return;
        num--;
        cout << line << endl;
    }
    //fileLock.unlock();
}

// 显示文件的尾num行
void MyOS::tailFile(int num, FCBTree* _node)
{
    //unique_lock<mutex> fileLock(fileMutex);

    string data = _node->file.getFileData();
    stringstream ss;
    ss << data;
    string line;
    int len = 0;
    while (getline(ss, line))
    {
        len++;
    }
    len -= num;
    stringstream _ss;
    _ss << _node->file.getFileData();
    while (getline(_ss, line))
    {
        len--;
        if (len < 0)
            cout << line << endl;
    }
    //fileLock.unlock();
}

// 文件加锁
void MyOS::lockFile()
{
    //unique_lock<mutex> fileLock(fileMutex);

    FCBTree* lNode = nullptr;
    cout << "Please select the file type to be locked (1.Dir or Folder 2.File):" << endl;
    int choice;
    cin >> choice;

    string filename;
    switch (choice)
    {
    case 1:
        cout << "Please input the name of the dir:" << endl;
        cin >> filename;
        lNode = cNode->searchChild(cNode, filename, 1);
        break;
    case 2:
        cout << "Please input the name of the dir:" << endl;
        cin >> filename;
        lNode = cNode->searchChild(cNode, filename, 0);
        break;
    default:
        cout << "Error: Invalid choice." << endl;
        break;
    }

    if (lNode != nullptr)
    {
        string passwd;
        // 文件未上锁，加锁
        if (lNode->file.getIsLocked() == 0)
        {
            lNode->file.setIsLocked(1);
            cout << "Please input the password of the file:" << endl;
            cin >> passwd;

            if (passwd == "$$$")
            {
                cout << "Error:Invalide password." << endl;
                return;
            }

            lNode->file.setUnlockCode(passwd);
            cout << "File encryption successful!" << endl;
        }
        // 文件已经上锁，解锁
        else
        {
            cout << "This file is encrypted." << endl;
            cout << "Please input the password of the file:" << endl;
            cin >> passwd;

            if (verifyPasswd(lNode->file.getUnlockCode(), passwd))
            {
                cout << "File unlocked successfully!" << endl;
                lNode->file.setIsLocked(0);
                lNode->file.setUnlockCode("");
            }
            else
            {
                cout << "Error: Password error." << endl;
            }
        }

    }
    else
    {
        cout << "Error: File information not found." << endl;
    }
    //fileLock.unlock();
    saveUsers();
}

// 移动文件
void MyOS::moveFile(string str)
{
    string name = "";
    string path = "";
    //cout << str << endl;
    size_t firstSpace = str.find(' ');

    // 使用 substr 函数提取所需的部分
    std::string extracted = str.substr(firstSpace + 1);
    analysisCmd(extracted, name, path);
    //cout << name << endl;
    //cout << path << endl;


    FCBTree* toMove = cNode->searchChild(cNode, name, 0);
    if (toMove != nullptr&&path!="")
    {
        //cout << "path:" << endl;
        //string path;
        //cin >> path;
        stringstream ss;
        ss << path;
        string line;
        getline(ss, line, '/');

        FCBTree* end = nullptr;
        if (line == users[curUser].getUserName())
        {
            end = root;
            while (getline(ss, line, '/'))
            {
                end = end->searchChild(end, line, 1);
                if (end == nullptr)
                {
                    cout << "Invalid path." << endl;
                    return;
                }
            }
        }
        else if (line == ".")
        {
            end = cNode;
            while (getline(ss, line, '/'))
            {
                end = end->searchChild(end, line, 1);
                if (end == nullptr)
                {
                    cout << "Invalid path." << endl;
                    return;
                }
            }
        }
        else
        {
            cout << "Invalid path." << endl;
            return;
        }
        if (end->searchChild(end, toMove->file.getFileName(), 0))
        {
            cout << "存在同名文件" << endl;
            return;
        }

        end->insertNode(end, toMove);
        cNode->deleteNode(cNode, toMove);
        modifedTimes++;

        saveUsers();
        return;
    }
    else
    {
        cout << "找不到操作的文件" << endl;
    }
}

// 拷贝文件
void MyOS::copyFile()
{
    //unique_lock<mutex> fileLock(fileMutex);

    string fileName;
    cout << "Please enter the name of the file to be copied:" << endl;
    cin >> fileName;

    FCBTree* coNode = cNode->searchChild(cNode, fileName, 0);

    if (coNode != nullptr)
    {
        FCBTree* copy = new FCBTree();
        copy->file = coNode->file;
        copy->isLocked = false;
        cout << "Please enter the copied file name:" << endl;
        cin >> fileName;

        if (rename(coNode, fileName))
            cNode->cNodes.push_back(copy);
        else
        {
            cout << "Error: The file with this name already exists." << endl;
            return;
        }
    }
    //fileLock.unlock();
}


// 进入目录
bool MyOS::cd(string path)
{
    //cout << "Please input the path:";
    //string path;
    //cin >> path;

    // 返回上一级or返回根目录orNULL?or ...???

    // 返回上一级
    if (path == "..")
    {
        // ok
        if (cNode->fNode != nullptr)
        {
            cNode = cNode->fNode;
            return true;
        }
        // not ok
        else
        {
            cout << "Error: Already in root directory." << endl;
            return false;
        }
    }

    FCBTree* _node = cNode->searchChild(cNode, path, 1);
    if (_node != nullptr)
    {
        if (fileRight(_node))
        {
            cNode = _node;
            return true;
        }
    }
    else
    {
        cout << "Error: Unable to find folder." << endl;
        return false;
    }
}

// 显示当前目录
void MyOS::dir()
{
    for (auto i = 0; i < cNode->cNodes.size(); i++)
    {
        cout << cNode->cNodes[i]->file.getModifyTime() << '\t';
        if (cNode->cNodes[i]->file.getIsDir() == 1)
            cout << "<DIR>\t";
        else
            cout << '\t';
        cout << cNode->cNodes[i]->file.getFileName() << endl;
    }
    cout << endl;
}

// 创建目录
void MyOS::mkdir()
{
    //unique_lock<mutex> fileLock(fileMutex);

    FCBTree* dNode = new FCBTree();
    cout << cNode->file.getFileName() << ">";

    dNode->file.setIsDir(1);
    cout << "Please input the name of the directory:" << endl;

    if (cNode->searchChild(cNode, dNode->file.getFileName(), 1) != nullptr)
    {
        cout << "Error: There is a folder with the same name in the current directory." << endl;
        return;
    }

    string name;
    cin >> name;
    dNode->file.setFileName(name);
    dNode->file.setIsDir(1);
    cNode->insertNode(cNode, dNode);
    //fileLock.unlock();
    return;
}

// 删除目录
void MyOS::rmdir(string path)
{
    //unique_lock<mutex> fileLock(fileMutex);

    cout << cNode->file.getFileName() << ">";
    //cout << "Please input the file you want to delete:" << endl;
    //string name;
    //cin >> name;

    FCBTree* dNode = cNode->searchChild(cNode, path, "1");
    if (dNode != nullptr)
    {
        cNode->deleteNode(cNode, dNode);
        cout << "File deletion successful!" << endl;

        users[curUser].clearData();
        users[curUser].setFileNum(saveFCBTree(root));
        return;

    }
    else
    {
        cout << "Error: Invalid filename." << endl;
        return;
    }
    //fileLock.unlock();

}

// 导入文件
bool MyOS::importFile(string str)
{
    //unique_lock<mutex> fileLock(fileMutex);

    string path = "";
    string name = "";
    size_t lastSlashPos = str.find_last_of("\\");
    if (lastSlashPos != std::string::npos)
    {
        path = str.substr(0, lastSlashPos);
        name = str.substr(lastSlashPos + 1);
    }
    else {
        path = "";
        name = str;
    }
    /*  string name, path;
      cout << "Please input the name of the file:" << endl;
      cin >> name;
      cin.ignore();
      cout << "Please input the path of the file" << endl;
      getline(cin, path);*/
    ifstream file(str, ios::in);

    if (file.is_open())
    {
        FCBTree* _node = new FCBTree();
        _node->file.setFileName(name);
        if (cNode->searchNode(cNode, name))
        {
            cout << "Error: A file with the same name exists." << endl;
            return false;
        }
        string line;
        string content;
        while (getline(file, line))
        {
            content += line;
            content += '\n';
        }
        _node->file.setFileData(content);
        file.close();
        _node->file.setIsDir(0);
        cNode->insertNode(cNode, _node);
        return true;

    }
    else
    {
        cout << "Error: File does not exist." << endl;
        return true;
    }
    //fileLock.unlock();
}

// 导出文件
bool MyOS::exportFile(string str)
{
    //unique_lock<mutex> fileLock(fileMutex);

    string name = "";
    string path = "";
    //cout << str << endl;
    size_t firstSpace = str.find(' ');

    // 使用 substr 函数提取所需的部分
    std::string extracted = str.substr(firstSpace + 1);
    analysisCmd(extracted, name, path);
    //cout << name << endl;
    //cout << path << endl;

    FCBTree* eNode = cNode->searchChild(cNode, name, 0);

    if (eNode->file.getUnlockCode() == "$$$")
    {
        cout << "Error: A user is currently modifying!" << endl;
        return false;
    }

    if (eNode == nullptr)
    {
        cout << "Error: File does not exist." << endl;
        return false;
    }

    ofstream file(path + name, ios::trunc);

    if (file.is_open())
    {
        cout << str << endl;
        file << eNode->file.getFileData();
        file.close();
        cout << "Successfully exported!" << endl;
        //fileLock.unlock();

        return true;
    }
    else
    {
        cout << "Error: Invalid path!" << endl;
        //fileLock.unlock();

        return false;
    }
}

