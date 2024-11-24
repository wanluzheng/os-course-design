#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "MyOS.h"

#include <thread>
#include <condition_variable>
#include <mutex>
using namespace std;

MyOS OS;

int main()
{
    //zwlOS.funcMenu();
    thread t1(&MyOS::funcMenu, ref(OS));
    thread t2(&MyOS::kernel, ref(OS));


    t1.join();
    t2.join();

    //t1.detach();
    //t2.detach();
    return 0;
}