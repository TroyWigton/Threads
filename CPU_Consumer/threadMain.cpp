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

class MyClass
{
public:
    static int totalCount;
    int thisObjNum;
    bool* g_stopFlag;
    unsigned long long int loopCounter = 0;

    MyClass(bool* stopFlag)
    {
        thisObjNum = ++totalCount;
        g_stopFlag = stopFlag;
    }

    void foo()
    {
        g_mtx.lock();
        cout << "Starting foo # " << thisObjNum << endl;
        g_mtx.unlock();
        while (*g_stopFlag != true)
        {
            if (++loopCounter % 100000000ULL == 0ULL)
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

void detectStopSignal(bool *g_stopFlag)
{
    while (toupper(getchar()) != 'Q') continue;
    *g_stopFlag = true;

}

void main(void)
{

    bool g_stopFlag = false;
    cout << "Enter (q or Q) to end multithread application." << endl;
    thread stopTriggerDetector(detectStopSignal, &g_stopFlag);

    MyClass obj1(&g_stopFlag), obj2(&g_stopFlag), obj3(&g_stopFlag);
    thread t1(&MyClass::foo, &obj1);
    thread t2(&MyClass::foo, &obj2);
    thread t3(&MyClass::foo, &obj3);

    MyClass obj4(&g_stopFlag), obj5(&g_stopFlag), obj6(&g_stopFlag);
    thread t4(&MyClass::foo, &obj4);
    thread t5(&MyClass::foo, &obj5);
    thread t6(&MyClass::foo, &obj6);


    MyClass obj7(&g_stopFlag), obj8(&g_stopFlag), obj9(&g_stopFlag);
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