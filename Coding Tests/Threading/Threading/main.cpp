#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>
#include <chrono>
#include <atomic>

using namespace std;

//Allows for safe access to shared resources
mutex m;
condition_variable tLock;
atomic <int> shared(0);

//Bools to signal thread begin and completion of execution
bool ready =false;
bool complete = false;

//Increment the shared value and output your unique identifier
void IncrementCount(int id, int Loops, int Threads)
{
	//Lock the thread until signaled by main thread with the "ready" bool.
	unique_lock<mutex> lk(m);
	tLock.wait(lk, [] {return ready; });

	cout << "Thread " << id << " Value: " << ++shared << endl;
	
	lk.unlock();
	
	//Signal that the threads have run for the amount of time desired.
	if (shared == ((Loops * Threads)))
	{
		complete = true;
	}

	//Notify any waiting thread that they can begin execution
	tLock.notify_all();
}
int main(int argc, char** argv)
{
	//Grab information from the command line and parse to interger
	int ThreadCount = atoi(argv[1]);
	int LoopCount = atoi(argv[2]);

	//loop through Z amount of times for X amount of threads
	vector<thread> ThreadVec;
	for (int i = 0; i < LoopCount; i++)
	{
		for (int j = 0; j < ThreadCount; j++)
		{
			ThreadVec.push_back(thread(IncrementCount, j, LoopCount, ThreadCount));
		}
	}
	//Set parameter to release threads for execution
	ready = true;
	cout << "Waiting 10 seconds" <<endl;
	this_thread::sleep_for(chrono::milliseconds(10000));

	//notify all threads that they are waiting for execution, and wait until complete signal is given
	tLock.notify_all();
	unique_lock<mutex> lk(m);
	tLock.wait(lk, [] {return complete;});

	//join the threads
	for (int i = 0; i < ThreadVec.size(); i++)
	{
		ThreadVec[i].join();
	}
	return 0;
}