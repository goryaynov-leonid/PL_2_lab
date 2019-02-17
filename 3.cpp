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


void printInd(shared_ptr<bool> flag, int i, mutex& m)
{
	while (!(*flag))
	{
		m.lock();
		cout << "Thread" << i << endl;
		m.unlock();
		this_thread::sleep_for(chrono::milliseconds(200));
	}
	{
		m.lock();
		cout << "Thread" << i << " finished" << endl;
		m.unlock();
	}
}

int main()
{
	srand(time(NULL));
	//fre;
	UserInterruptHandler h;
	shared_ptr<bool> f = make_shared<bool>(false);
	mutex m;
	future<void> t1 = async(launch::async, printInd, ref(f), 1, ref(m));
	future<void> t2 = async(launch::async, printInd, ref(f), 2, ref(m));
	while (!h.checkInterruption())
	{
		m.lock();
		cout << "Main thread" << endl;
		m.unlock();
		this_thread::sleep_for(chrono::milliseconds(200));
	}
	*f = true;
	t1.get();
	t2.get();
	m.lock();
	cout << "Main thread finished" << endl;
	m.unlock();
	system("PAUSE");
}