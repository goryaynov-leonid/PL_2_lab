#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <algorithm>
#include <set>
#include <climits>
#include <queue>
#include <vector>
#include <time.h>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <bitset>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "UserInterruptHandler.h"
#include <climits>
#include <random>
#include <iomanip>
#define E 2.718281828459045
#define PI 3.141592653
#define mp make_pair
#define ull unsigned long long
#define ll long long
#define all(i) i.begin(), i.end()
#define forall(item, where) for (auto & item : where)
#define fre freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);
using namespace std;

double ran(double a, double b)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(a, b);
	return dist(mt);
}

void producerF(queue<int>& q, int c, bool& finished, mutex& queueM, mutex& printM)
{
	bool f = 0;
	while (!finished)
	{
		queueM.lock();
		if (q.size() < c)
		{
			q.push(1);
			f = 1;
			
		}
		queueM.unlock();
		if (f)
		{
			this_thread::sleep_for(chrono::milliseconds((int)ran(300, 1000)));
			f = 0;
		}
	}

	printM.lock();
	cout << "Producer finished" << endl;
	printM.unlock();
}
void printInd(int i, queue<int>& q, mutex& queueM, mutex& printM, bool& finished, vector<chrono::high_resolution_clock::time_point>& times, vector<bool>& isFree)
{
	bool f = 0;
	while (!finished)
	{
		queueM.lock();
		if (!q.empty())
		{
			q.pop();
			f = 1;
		}
		queueM.unlock();
		if (f)
		{
			chrono::milliseconds t = chrono::milliseconds((int)ran(3000, 5000));
			times[i] = chrono::high_resolution_clock::time_point(chrono::high_resolution_clock::now() + t);
			isFree[i] = 0;
			this_thread::sleep_for(t);
			isFree[i] = 1;
			f = 0;		
		}
	}

	printM.lock();
	cout << "Thread" << i << " finished" << endl;
	printM.unlock();
}

int main()
{
	srand(time(NULL));
	bool finished = 0;
	bool notified = 0;
	UserInterruptHandler h;
	mutex queueM, printM;
	int n, c;
	cin >> n >> c;
	queue<int> q;
	vector<chrono::high_resolution_clock::time_point> times(n);
	vector<bool> isFree(n, 1);
	vector <thread> t(n);
	thread producerT = thread(producerF, ref(q), c, ref(finished), ref(queueM), ref(printM));
	for (int i = 0; i < n; i++)
		t[i] = thread(printInd, i, ref(q), ref(queueM), ref(printM), ref(finished), ref(times), ref(isFree));
	while (!h.checkInterruption())
	{
		
		string s;
		for (int i = 0; i < n; i++)
		{
			if (isFree[i])
			{
				s += to_string(i) + ": F ";
			}
			else
			{
				int a = chrono::duration_cast<chrono::duration<int>>(times[i] - chrono::high_resolution_clock::now()).count();
				s += to_string(i) + ": " + to_string(a) + ' ';
			}
		}
		queueM.lock();
		s += "q: " + to_string(q.size());
		queueM.unlock();
		printM.lock();
		system("cls");
		cout << s << endl;
		printM.unlock();
		this_thread::sleep_for(chrono::milliseconds(500));
	}

	finished = 1;
	producerT.join();
	forall(i, t)
		i.join();
	printM.lock();
	cout << "Main thread finished" << endl;
	printM.unlock();
	system("PAUSE");
}