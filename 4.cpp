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


void printInd(condition_variable& cv, int i, mutex& m, shared_ptr<bool>& notified, shared_ptr<bool>& finished)
{
	while (!(*finished))
	{
		unique_lock<mutex> lock(m);
		cv.wait(lock);
		if (*notified)
		{
			*notified = 0;
			cout << i << endl;
		}
	}

	m.lock();
	cout << "Thread" << i << " finished" << endl;
	m.unlock();

}

int main()
{
	srand(time(NULL));
	shared_ptr<bool> finished = make_shared<bool>(0);
	shared_ptr<bool> notified = make_shared<bool>(0);
	UserInterruptHandler h;
	condition_variable cv;
	mutex m;
	int n;
	cin >> n;
	vector <thread> t(n);
	for (int i = 0; i < n; i++)
		t[i] = thread(printInd, ref(cv), i + 1, ref(m), ref(notified), ref(finished));
	while (!h.checkInterruption())
	{
		*notified = 1;
		cv.notify_one();
		this_thread::sleep_for(chrono::milliseconds(1000));
	}

	*finished = 1;
	cv.notify_all();
	forall(i, t)
		i.join();
	m.lock();
	cout << "Main thread finished" << endl;
	m.unlock();
	system("PAUSE");
}