#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>

using namespace std;

#define ll long long int
const int INF = 1000000000;

int n;
vector<int> t, l, r;

vector<int> froml1, froml2, fromr1, fromr2;


void input() {
	cin >> n;
	t.assign(n, 0);
	for (int i = 0; i < n; i++) {
		cin >> t[i];
	}
	for (int i = 0; i < n - 1; i++) {
		int l1, r1;
		cin >> l1 >> r1;
		l.push_back(l1);
		r.push_back(r1);
	}
}

void calculate() {
	if (n == 1) {
		cout << "0\n";
		return;
	}
	froml1.assign(n, -INF);
	froml2.assign(n, INF);
	fromr1.assign(n, -INF);
	fromr2.assign(n, INF);

	froml1[n - 2] = l[n - 2] - t[n - 2];
	froml2[n - 2] = r[n - 2];
	for (int v = n - 3; v >= 0; v--) {
		int a = froml1[v + 1];
		int b = froml2[v + 1];

		if (a == -1) {
			froml1[v] = -1;
		}
		if (l[v] < a) {
			if (r[v] + t[v] < a) {
				froml1[v] = -1;
			}
			else {
				froml1[v] = min(a - t[v], l[v]);
				froml2[v] = min(b, r[v]);
			}
		}
		else {
			if (l[v] <= b) {
				froml1[v] = l[v] - t[v];
				froml2[v] = min(b, r[v]);
			}
			else {
				froml1[v] = -1;
			}
		}
	}

	// IMPORTANT
	/*if (froml1[0] != -1 && n > 2) {
		froml1[0] = froml1[1];
	}*/

	fromr1[1] = l[0] - t[1];
	fromr2[1] = r[0];
	for (int v = 2; v < n; v++) {
		int a = fromr1[v - 1];
		int b = fromr2[v - 1];

		if (a == -1) {
			fromr1[v] = -1;
		}
		if (l[v - 1] < a) {
			if (r[v - 1] + t[v] < a) {
				fromr1[v] = -1;
			}
			else {
				fromr1[v] = min(a - t[v], l[v - 1]);
				fromr2[v] = min(b, r[v - 1]);
			}
		}
		else {
			if (l[v - 1] <= b) {
				fromr1[v] = l[v - 1] - t[v];
				fromr2[v] = min(b, r[v - 1]);
			}
			else {
				froml1[v] = -1;
			}
		}
	}

	// IMPORTANT
	/*if (fromr1[n - 1] != -1 && n > 2) {
		fromr1[n - 1] = fromr1[n - 2];
	}*/

	/*for (int i = 0; i < n; i++) {
		cout << fromr1[i] << " " << fromr2[i] << endl;
	}
	cout << endl;*/

	for (int v = 0; v < n; v++) {
		if (froml1[v] == -1 || fromr1[v] == -1) {
			cout << "-1\n";
		}
		else {
			int la, ra;
			la = max(froml1[v], fromr1[v]);
			ra = min(froml2[v], fromr2[v]);
			if (la > ra) {
				cout << "-1\n";
			}
			else {
				cout << max(0, la) << endl;
			}
		}
	}
}

void output() {
	/*cout << "Continue...\n";
	fflush(stdout);
	while (true) {
	;
	}*/
}

int main() {
	input();
	calculate();
	output();
	return 0;
}