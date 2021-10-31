#include <iostream>
#include <thread>
#include <mutex>

//#include <stdio.h>
//#include <chrono>
//#include <windows.h>
//#include <time.h>
//#include <stdlib.h>
//#include <dos.h>
//#include <cstdlib>

using namespace std;
mutex g_mtx;
bool g_stopFlag = false;

class MyClass
{
public:
    static int totalCount;
    int thisObjNum;
    unsigned long long int loopCounter = 0;

    MyClass()
    {
        thisObjNum = ++totalCount;
    }

    void foo()
    {
        g_mtx.lock();
        cout << "Starting foo # " << thisObjNum << endl;
        g_mtx.unlock();
        while (g_stopFlag != true)
        {
            if (++loopCounter % 10000000ULL == 0ULL)
            {
                g_mtx.lock();
                cout << "foo #" << thisObjNum << " count = " << loopCounter << endl;
                g_mtx.unlock();
            }
        }
        g_mtx.lock();
        cout << "foo # " << thisObjNum << " stopFlag detected" << endl;
        g_mtx.unlock();
    }
};

int MyClass::totalCount = 0;

void detectStopSignal(bool *stopFlag)
{
    while (toupper(getchar()) != 'Q') continue;
    *stopFlag = true;

}

void main(void)
{

    cout << "Enter (q or Q) to end multithread application." << endl;
    thread stopTriggerDetector(detectStopSignal,&g_stopFlag);

    MyClass obj1, obj2, obj3;
    thread t1(&MyClass::foo, &obj1);
    thread t2(&MyClass::foo, &obj2);
    thread t3(&MyClass::foo, &obj3);

    MyClass obj4, obj5, obj6;
    thread t4(&MyClass::foo, &obj4);
    thread t5(&MyClass::foo, &obj5);
    thread t6(&MyClass::foo, &obj6);


    MyClass obj7, obj8, obj9;
    thread t7(&MyClass::foo, &obj7);
    thread t8(&MyClass::foo, &obj8);
    thread t9(&MyClass::foo, &obj9);

    stopTriggerDetector.join();
    t1.join(); 
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();

}