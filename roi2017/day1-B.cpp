// exa.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>

using namespace std;

// Define
#define ll long long int

// Variables
int n;
vector< vector<int> > list;
vector<int> magic(32, -2);

// Functions

vector<int> binary(ll n) {
	vector<int> res(32, 0);
	for (int i = 0; i < 32; i++) {
		res[31 - i] = n % 2;
		n = (n >> 1);
	}
	return res;
}

int iterate(int pos, int l, int r) {
	if (pos >= 32) {
		return 1;
	}

	//cout << "iterate " << pos << " " << l << " " << r << endl;
	int b = list[l][pos];
	int e = list[r - 1][pos];
	if (b == e) {
		for (int i = l; i < r; i++) {
			if (list[i][pos] != b) {
				// Cannot construct magic number
				//cout << pos << " 0 0 fault\n";
				return 0;
			}
		}

		magic[pos] = max(magic[pos], -1);
		return iterate(pos + 1, l, r);
	}
	else {
		if (b == 0) {
			// 00000...0011111...111
			int m;

			int i = l;
			while (i < r && list[i][pos] == 0) {
				i++;
			}
			m = i;
			while (i < r && list[i][pos] == 1) {
				i++;
			}
			if (i < r) {
				// This happened: 001110...
				//cout << pos << " 0 1 fault\n";
				return 0;
			}
			else {
				if (magic[pos] == 1) {
					return 0;
				}
				magic[pos] = 0;
				return min(iterate(pos + 1, l, m), iterate(pos + 1, m, r));
			}
		}
		else {
			// 111...110000...000
			int m;

			int i = l;
			while (i < r && list[i][pos] == 1) {
				i++;
			}
			m = i;
			while (i < r && list[i][pos] == 0) {
				i++;
			}
			if (i < r) {
				// This happened: 111001..
				//cout << pos << " 1 0 fault\n";
				return 0;
			}
			else {
				if (magic[pos] == 0) {
					return 0;
				}
				magic[pos] = 1;
				return min(iterate(pos + 1, l, m), iterate(pos + 1, m, r));
			}
		}
	}
	return 0;
}

void output_magic() {
	ll res = 0;
	ll mul = (1 << 30);
	for (int i = 1; i < 32; i++) {
		if (magic[i] < 0) {
			// 0
		}
		else {
			res += mul * magic[i];
		}
		mul = (mul >> 1);
	}
	cout << res << "\n";
}

void input() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		ll a;
		cin >> a;
		list.push_back(binary(a));

	}
}

void calculate() {
	magic.assign(32, -2);
	int res = iterate(0, 0, n);
	if (!res) {
		cout << "-1\n";
	}
	else {
		output_magic();
	}

	int q;
	cin >> q;
	for (int i = 0; i < q; i++) {
		int idx;
		ll b;
		cin >> idx >> b;
		list[idx - 1] = binary(b);

		magic.assign(32, -2);
		int res = iterate(0, 0, n);
		if (!res) {
			cout << "-1\n";
		}
		else {
			output_magic();
		}
	}
}

void output() {
	//system("pause");
}

int main()
{
	input();
	calculate();
	output();
	return 0;
}

