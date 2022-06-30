#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>
#include <chrono>
#include <atomic>

using namespace std;

//Allows for safe access to shared resource
mutex m;
condition_variable tLock;
atomic <int> shared(22);
bool ready =false;
bool processed = false;
bool complete = false;

void IncrementCount(int id, int Loops, int Threads)
{
	unique_lock<mutex> lk(m);
	tLock.wait(lk, [] {return ready; });
	cout << "Thread " << id << " Value: " << ++shared << endl;
	processed = true;
	lk.unlock();
	processed = false;
	if (shared == shared + Loops)
	{
		complete = true;
	}
	tLock.notify_all();
}
int main(int argc, char** argv)
{
	int ThreadCount = atoi(argv[1]);
	int LoopCount = atoi(argv[2]);
	cout <<  "ArgCount: " << argc <<"\nThreadCount: " << ThreadCount << endl << "LoopCount: " << LoopCount << endl;
	vector<thread> ThreadVec;
	int j = 0;
	for (int i = 0; i < LoopCount; i++)
	{
		if (j == ThreadCount)
		{
			j = 0;
		}
		ThreadVec.push_back(thread(IncrementCount, j,LoopCount,ThreadCount));
		j++;
	}
	ready = true;
	cout << "Waiting 10 seconds" <<endl;
	//this_thread::sleep_for(chrono::milliseconds(10000));
	tLock.notify_all();

	unique_lock<mutex> lk(m);
	tLock.wait(lk, [] {return complete;});

	for (int i = 0; i < ThreadVec.size(); i++)
	{
		ThreadVec[i].join();
	}

	system("Pause");


}