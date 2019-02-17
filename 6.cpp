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

void producerF(vector<priority_queue<pair<int, int>>>& q, int c, bool& finished, vector<mutex>& queueM, mutex& printM)
{
	bool f = 0;
	while (!finished)
	{
		int i = ran(0, q.size());
		queueM[i].lock();
		if (q[i].size() < c)
		{
			q[i].push(mp(ran(1, 3), ran(3000, 5000)));
			f = 1;
		}
		queueM[i].unlock();
		if (f)
		{
			this_thread::sleep_for(chrono::milliseconds((int)ran(300, 500)));
			f = 0;
		}
	}

	printM.lock();
	cout << "Producer finished" << endl;
	printM.unlock();
}
void printInd(int i, int j, priority_queue<pair<int, int>>& q, mutex& queueM, mutex& printM, bool& finished, vector<chrono::high_resolution_clock::time_point>& times, vector<bool>& isFree, vector<int>& priorities)
{
	bool f = 0;
	while (!finished)
	{
		pair<int, int> p;
		queueM.lock();
		if (!q.empty())
		{
			p = q.top();
			q.pop();
			f = 1;
		}
		queueM.unlock();
		if (f)
		{
			chrono::milliseconds t = chrono::milliseconds(p.second);
			times[j] = chrono::high_resolution_clock::time_point(chrono::high_resolution_clock::now() + t);
			priorities[j] = p.first;
			isFree[j] = 0;
			this_thread::sleep_for(t);
			isFree[j] = 1;
			f = 0;
		}
	}

	printM.lock();
	cout << "Group " << i << ": Thread" << j << " finished" << endl;
	printM.unlock();
}

int main()
{
	srand(time(NULL));
	bool finished = 0;
	bool notified = 0;
	UserInterruptHandler h;
	int n, m, c;
	cin >> n >> m >> c;
	vector<mutex> queueM(n);
	mutex printM;
	vector<priority_queue<pair<int, int>>> q(n);
	vector<vector<chrono::high_resolution_clock::time_point>> times(n, vector<chrono::high_resolution_clock::time_point>(m));
	vector<vector<int>> priorities(n, vector<int>(m));
	vector<vector<bool>> isFree(n, vector <bool>(m, 1));
	vector<vector<thread>> t;
	for (int i = 0; i < n; i++)
		t.push_back(vector<thread>(m));
	thread producerT = thread(producerF, ref(q), c, ref(finished), ref(queueM), ref(printM));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			t[i][j] = thread(printInd, i, j, ref(q[i]), ref(queueM[i]), ref(printM), ref(finished), ref(times[i]), ref(isFree[i]), ref(priorities[i]));
	while (!h.checkInterruption())
	{

		string s;
		for (int i = 0; i < n; i++)
		{
			s += "Group " + to_string(i) + ": ";
			for (int j = 0; j < m; j++)
			{
				if (isFree[i][j])
				{
					s += "Thread " + to_string(j) + ": FREE      ";
				}
				else
				{
					int a = chrono::duration_cast<chrono::duration<int>>(times[i][j] - chrono::high_resolution_clock::now()).count();
					s += "Thread " + to_string(j) + ": P: " + to_string(priorities[i][j]) + " S: " + to_string(a) + " ";
				}
			}
			queueM[i].lock();
			s += "Q" + to_string(i) + ": " + to_string(q[i].size()) + "\n\r";
			queueM[i].unlock();
		}
		printM.lock();
		system("cls");
		cout << s << endl;
		printM.unlock();
		this_thread::sleep_for(chrono::milliseconds(500));
	}

	finished = 1;
	producerT.join();
	forall(i, t)
		forall(j, i)
		j.join();
	printM.lock();
	cout << "Main thread finished" << endl;
	printM.unlock();
	system("PAUSE");
}