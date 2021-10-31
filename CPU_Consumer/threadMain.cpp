#include <iostream>
#include <thread>
#include <mutex>
#include <windows.h>

using namespace std;
mutex g_mtx;   // Is this an appropriate place to define a mutex?  Is my usage below appropriate?
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

/* Why does this have to be here?  Why do we have to initialze, and why does it have to be outside the class def and outside main?*/
int MyClass::totalCount = 0;

void printStopMessage(void)
{
    cout << "Press Enter to end multithread application." << endl;
}
void detectStopSignal(bool *stopFlag)
{
    while (getchar() != '\n') Sleep(1000);
    *stopFlag = true;
}

void startupMessage(unsigned int seconds)
{
    printStopMessage();
    cout << "Spawing threads in";
    for (int countdown = seconds; countdown > 0; --countdown)
    {
        cout << " " << countdown;
        Sleep(1000);
    };
    cout << " Go!"<<endl<<endl;
}

void main(void)
{
    
    thread stopTriggerDetector(detectStopSignal,&g_stopFlag);
    startupMessage(3);

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

    MyClass obj10, obj11, obj12;
    thread t10(&MyClass::foo, &obj10);
    thread t11(&MyClass::foo, &obj11);
    thread t12(&MyClass::foo, &obj12);

    MyClass obj13, obj14, obj15;
    thread t13(&MyClass::foo, &obj13);
    thread t14(&MyClass::foo, &obj14);
    thread t15(&MyClass::foo, &obj15);

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
    t10.join();
    t11.join();
    t12.join();
    t13.join();
    t14.join();
    t15.join();

}