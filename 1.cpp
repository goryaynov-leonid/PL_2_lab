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

void gen(vector<int>::iterator l, vector<int>::iterator r)
{
	for (vector<int>::iterator i = l; i != r; i++)
		*i = rand();
}

pair <int, int> foo(vector <int> &v, int l, int r)
{
	int mi = INT_MAX, ma = INT_MIN;
	int imi = l, ima = l;
	for (int i = l; i < r; i++)
	{
		if (v[i] < mi)
		{
			imi = i;
			mi = v[i];
		}
		if (v[i] > ma)
		{
			ima = i;
			ma = v[i];
		}
	}
	return mp(imi, ima);
}

int main()
{
	srand(time(NULL));
	fre;
	int n;
	cin >> n;
	vector <int> c = { 1,2,4,8,16 };
	vector <int> v(n);
	gen(all(v));
	for (int j = 0; j < c.size(); j++)
	{
		vector <future<pair<int, int>>> p(c[j]);
		vector <future<void>> p1(c[j]);
		chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
		int imi = 0, ima = 0;
		for (int i = 0; i < c[j]; i++)
			p[i] = async(launch::async, foo, ref(v), i * n / c[j], (i + 1) * n / c[j]);
		for (int i = 0; i < c[j]; i++)
		{
			pair<int, int> pa = p[i].get();
			if (v[pa.first] < v[imi])
				imi = pa.first;
			if (v[pa.second] > v[ima])
				ima = pa.second;
		}
		chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
		chrono::duration<double> dur = t2 - t1;
		cout << c[j] << " " << imi << " " << ima << " " << dur.count() << endl;
	}
}