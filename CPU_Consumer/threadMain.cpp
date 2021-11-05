#include <iostream>
#include <thread>
#include <mutex>
#include <windows.h>

using namespace std;
mutex g_mtx;   // Is this an appropriate place to define a mutex?  Is my usage below appropriate?
bool g_stopFlag = false;





    void foo(unsigned int msDelay)
    {
        bool fast = false;
        unsigned int sleep_ms;
        unsigned long long int countModulo;
        static unsigned int thisCallNumberStatic = 0;
        unsigned int thisCallNumberAuto;
        unsigned long long int loopCounter = 0ULL;

        Sleep(msDelay);

        // Try to prove that calls to stand alone foo retain their own local data within each thread.
        g_mtx.lock();
        cout << "Foo Call Number: " << ++thisCallNumberStatic << endl;
        thisCallNumberAuto = thisCallNumberStatic;
        g_mtx.unlock();

        if (fast)
        {
            sleep_ms = 0; //zero for max CPU usage
            countModulo = 100000000ULL;
        }
        else
        {
            sleep_ms = 1; // 1ms ootherwise, consumes less CPU
            countModulo = 100ULL;
        }

       
        while (g_stopFlag != true)
        {
            if (sleep_ms != 0) Sleep(sleep_ms); //Avoiding Sleep() overhead

            if (++loopCounter % countModulo == 0ULL)
            {
                g_mtx.lock();
                cout << "Foo #"<<thisCallNumberAuto<<": Loop Count = " << loopCounter << endl;
                g_mtx.unlock();
            }
        }
        g_mtx.lock();
        cout <<"Foo # "<<thisCallNumberAuto<<" stopFlag detected" << endl;
        g_mtx.unlock();
    }

void printStopMessage(void)
{
    cout << "Press Enter to end multithread application." << endl;
}
void detectStopSignal(bool *stopFlag)
{
    while (getchar() != '\n') Sleep(100);
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

int main(void)
{
    thread stopTriggerDetector(detectStopSignal,&g_stopFlag);
    startupMessage(3);

   
    thread t1(foo,0);
    thread t2(foo,2000);
    thread t3(foo,4000);

    stopTriggerDetector.join();
    t1.join(); 
    t2.join();
    t3.join();

    return 0; 
}