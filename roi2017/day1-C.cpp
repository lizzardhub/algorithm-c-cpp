// exa.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <queue>
#include <set>

using namespace std;

// Define
#define ll long long int
const ll INF = 1000000000000000;
#define foreach(i_, b_, e_) for (int i_ = b_; i_ < e_; i_++)
#define EPS 0.000001

template <class T>
T abs(T a) {
	if (a < 0) {
		return -a;
	}
	return a;
}

struct Vec {
	double x, y;

	Vec() {
		;
	}

	Vec(double x, double y) {
		this->x = x;
		this->y = y;
	}
};

Vec opp(Vec A) {
	return Vec(-A.x, -A.y);
}

Vec sum(Vec A, Vec B) {
	return Vec(A.x + B.x, A.y + B.y);
}

double dpr(Vec A, Vec B) {
	return A.x * B.y - A.y * B.x;
}

// Variables
int n, q;
vector<Vec> rx;

// Functions

double sq(Vec A, Vec B, Vec C) {
	Vec X = sum(opp(A), B);
	Vec Y = sum(opp(A), C);
	/*cout << X.x << " " << X.y << "\n";
	cout << Y.x << " " << Y.y << "\n";*/
	return abs(dpr(X, Y) / 2.0);
}

int contains(Vec A, Vec B, Vec C, Vec X) {
	double s = sq(A, X, B) + sq(B, X, C) + sq(C, X, A);
	if (abs(s - sq(A, B, C)) < EPS) {
		return 1;
	}
	else {
		return 0;
	}
}

void input() {
	//freopen("in.txt", "r", stdin);

	cin >> n;
	for (int i = 0; i < n; i++) {
		Vec A;
		cin >> A.x >> A.y;
		rx.push_back(A);
	}
	/*cout << contains(rx[0], rx[1], rx[2], rx[3]) << "\n";
	system("pause");*/
	cin >> q;
}


vector<vector<int> > good_tri;
set<vector<int> > good_tri_uni;

void calculate() {
	foreach(v1, 0, n) {
		foreach(v2, v1 + 1, n) {
			foreach(v3, v2 + 1, n) {
				int success = 1;
				foreach(x, 0, n) {
					if (x != v1 && x != v2 && x != v3 && contains(rx[v1], rx[v2], rx[v3], rx[x])) {
						success = 0;
					}
				}
				if (success) {
					vector<int> l({ v1, v2, v3 });
					//cout << v1 << " " << v2 << " " << v3 << "\n";
					if (good_tri_uni.find(l) == good_tri_uni.end()) {
						good_tri.push_back(l);
						good_tri_uni.insert(l);
					}
				}
				//cout << "\n";
			}
		}
	}
	for (int i = 0; i < q; i++) {
		for (int j = 0; j < good_tri.size(); j++) {
			cout << "? 3 ";
			for (int x : good_tri[j]) {
				cout << x + 1 << " ";
			}
			cout << "\n";
			fflush(stdout);

			string ans;
			cin >> ans;
			if (ans == "Yes") {
				cout << "! 3 ";
				for (int x : good_tri[j]) {
					cout << x + 1 << " ";
				}
				cout << "\n";
				fflush(stdout);
				break;
			}
		}
	}
}

void output() {
	/*while (true) {
		;
	}*/
	/*cout << "Hey!\n";
	system("pause");*/
}

int main()
{
	input();
	calculate();
	output();
	return 0;
}

