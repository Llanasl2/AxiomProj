#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

//Allows for safe access to shared resource
mutex IncLock;

void IncrementCount(int id, int &shared)
{
	lock_guard<mutex> lock(IncLock);
	cout << "Thread " << id << " Value: " << ++shared << endl;

}
int main(int argc, char** argv)
{
	int ThreadCount = atoi(argv[1]);
	int LoopCount = atoi(argv[2]);
	cout <<  "ArgCount: " << argc <<"\nThreadCount: " << ThreadCount << endl << "LoopCount: " << LoopCount << endl;

	int shared = 22;
	vector<thread> ThreadVec;
	int j = 0;
	for (int i = 0; i < LoopCount; i++)
	{
		if (j == ThreadCount)
		{
			j = 0;
		}
		ThreadVec.push_back(thread(IncrementCount, j, ref(shared)));
		j++;
	}

	for (int i = 0; i < ThreadVec.size(); i++)
	{
		ThreadVec[i].join();
	}

	system("Pause");


}