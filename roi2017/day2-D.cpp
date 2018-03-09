#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <set>
#include <bitset>

using namespace std;

#define ll long long int

ll n, m;
vector<ll> una, unb;
vector<ll> ra, rb;
bitset<100000001> uniq;

void input() {
	//freopen("internship.in", "r", stdin);

	cin >> n >> m;
	una.assign(n, 0);
	ra.assign(n, 0);
	unb.assign(m, 0);
	rb.assign(m, 0);
	for (ll i = 0; i < n; i++) {
		cin >> una[i];
	}
	for (ll i = 0; i < n; i++) {
		cin >> ra[i];
	}
	for (ll i = 0; i < m; i++) {
		cin >> unb[i];
	}
	for (ll i = 0; i < m; i++) {
		cin >> rb[i];
	}
}


void calculate() {
	ll max_rate = 0;
	ll mil, mir, mjl, mjr;
	for (ll il = 0; il < n; il++) {
		ll irate = 0;
		uniq.reset();
		for (ll ir = il; ir < n; ir++) {
			uniq[una[ir]] = 1;
			irate += ra[ir];

			ll jl = 0;
			ll jr = 0;
			ll jrate = 0;
			while (jr < m) {
				if (!uniq[unb[jr]]) {
					jrate += rb[jr];
					if (irate + jrate > max_rate) {
						max_rate = max(max_rate, irate + jrate);
						mil = il + 1;
						mir = ir + 1;
						mjl = jl + 1;
						mjr = jr + 1;
					}

					jr++;
				}
				else {
					jrate = 0;
					jr++;
					jl = jr;
				}
			}
		}
	}

	// not go to 1-st
	ll sum = 0;
	for (ll i = 0; i < m; i++) {
		sum += rb[i];
	}
	if (sum > max_rate) {
		max_rate = sum;
		mil = 0;
		mir = 0;
		mjl = 1;
		mjr = m;
	}
	// not go to 2-st
	sum = 0;
	for (ll i = 0; i < n; i++) {
		sum += ra[i];
	}
	if (sum > max_rate) {
		max_rate = sum;
		mil = 1;
		mir = n;
		mjl = 0;
		mjr = 0;
	}

	cout << max_rate << endl << mil << " " << mir << endl << mjl << " " << mjr << endl;
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