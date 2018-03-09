#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>

using namespace std;

#define ll long long int
ll n, a;

void input() {
	cin >> n >> a;
}

void calculate() {
	if (n == 1) {
		ll l, r, c;
		cin >> l >> r >> c;
		if (r > a) {
			cout << 0;
		}
		else {
			//ll min_sum = 1000000000000000;
			/*for (ll res = r * (a / r); res <= a; res++) {
				min_sum = min(min_sum, res * 1000 * 1000 * 1000 - c * (res1 / l + 1));

			}*/
			//cout << max_sum << " ";
			//cout << max((ll)0, min(l * (a / l) * 1000 * 1000 * 1000 - c * (a / l), r * (a / r) * 1000 * 1000 * 1000 - c * (a / r)));
			ll anti = r * (a / r);
			cout << anti * 1000000000 - (anti / l) * c;
			/*if (anti % l == 0) {
				cout << anti * 1000000000 - (anti / l) * c;
			}
			else {
				cout << anti * 1000000000 - (anti / l + 1) * c;
			}*/
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