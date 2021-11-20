#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
//#include <windows.h>


using namespace std;
mutex g_mtx;
bool g_stopFlag = false;

void foo(unsigned int msDelay)
{
    static unsigned int thisCallNumberStatic = 0;
    unsigned int thisCallNumber;
    unsigned long long int loopCounter = 0ULL;

    // Try to prove that calls to stand alone foo retain their own local data within each thread.
    g_mtx.lock();
    cout << "Foo Call Number: " << ++thisCallNumberStatic << endl;
    thisCallNumber = thisCallNumberStatic;
    g_mtx.unlock();

    while (g_stopFlag != true)
    {
            this_thread::sleep_for(chrono::milliseconds(msDelay));
            g_mtx.lock();
            cout << "Foo #"<<thisCallNumber<<": Loop Count = " << ++loopCounter << endl;
            g_mtx.unlock();
    }
    g_mtx.lock();
    cout <<"Foo # "<<thisCallNumber<<" stopFlag detected" << endl;
    g_mtx.unlock();
}

void printStopMessage(void)
{
    cout << "Press Enter to end multithread application." << endl;
}
void detectStopSignal(bool *stopFlag)
{
    while (getchar() != '\n') this_thread::sleep_for(chrono::milliseconds(200));
    *stopFlag = true;
}

void startupMessage()
{
    printStopMessage();
    cout << "Spawing threads in";
    for (int countdown = 3; countdown > 0; --countdown)
    {
        cout << " " << countdown;
        this_thread::sleep_for(1000ms);
    };
    cout << " Go!"<<endl<<endl;
}

int main(void)
{
    thread stopTriggerDetector(detectStopSignal,&g_stopFlag);
    startupMessage();

    thread t1(foo,1000);
    thread t2(foo,1000);
    thread t3(foo,1000);

    stopTriggerDetector.join();
    t1.join(); 
    t2.join();
    t3.join();

    return 0; 
}