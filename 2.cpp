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

void genMatrix(vector <vector <int>> &m, int l, int r, promise<void>&& p)
{
	for (int i = l; i < r; i++)
	{
		m[i / m[0].size()][i % m[0].size()] = rand() % 101 - 50;
	}
	p.set_value();
}

void genThreads(vector <vector<int>> &m, int c)
{
	vector <promise<void>> p(c);
	vector <future<void>> f(c);
	for (int i = 0; i < c; i++)
	{
		f[i] = p[i].get_future();
		thread t(genMatrix, ref(m), m.size() * m[0].size() * i / c, m.size() * m[0].size()*(i + 1) / c, move(p[i]));
		t.detach();
	}

	for (int i = 0; i < c; i++)
	{
		f[i].get();
	}
}

void multiply(vector <vector <int>> &res, vector <vector <int>> &m1, vector <vector<int>> &m2, int l, int r, promise<void>&& p)
{
	for (int i = l; i < r; i++)
	{
		for (int j = 0; j < m2.size(); j++)
			res[i / res[0].size()][i % res[0].size()] += m1[i / res[0].size()][j] * m2[j][i % res[0].size()];
	}
	p.set_value();
}

int main()
{
	srand(time(NULL));
	fre;
	int n, k1, k2, m, c;
	cin >> n >> k1 >> k2 >> m >> c;
	if (k1 != k2)
	{
		cout << "Error";
		return 0;
	}
	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
	vector <vector <int>> m1(n, vector <int>(k1)), m2(k2, vector <int>(m));
	genThreads(m1, c);
	genThreads(m2, c);
	vector <vector <int>> res(n, vector <int>(m, 0));
	vector <promise<void>> p(c);
	vector <future<void>> f(c);
	for (int i = 0; i < c; i++)
	{
		f[i] = p[i].get_future();
		thread t(multiply, ref(res), ref(m1), ref(m2), n * m * i / c, n * m * (i + 1) / c, move(p[i]));
		t.detach();
	}
	for (int i = 0; i < c; i++)
	{
		f[i].get();
	}
	chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> dur = t2 - t1;
	cout << dur.count();
}