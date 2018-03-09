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

struct arc {
	ll b, t, id;
};

// Variables
int n, m;
vector< vector<arc> > arcs;
vector< vector<arc> > rev_arcs;

// Functions
vector<int> visited;
vector<ll> D;
vector<int> last_upd;

void djkstra(int v) {
	visited.assign(n, 0);
	D.assign(n, INF);
	last_upd.assign(n, 0);

	D[v] = 0;

	while (true) {
		ll mn_dst = INF;
		v = -1;
		for (int i = 0; i < n; i++) {
			if (!visited[i] && D[i] < mn_dst) {
				mn_dst = D[i];
				v = i;
			}
		}

		if (mn_dst >= INF) {
			return;
			// Algorithm complete
		}
		else {
			visited[v] = 1;
			for (arc e : arcs[v]) {
				if (D[v] + e.t < D[e.b]) {
					last_upd[e.b] = e.t;
					D[e.b] = min(D[e.b], D[v] + e.t);
				}
			}
		}
	}
}

// visited
vector<int> gray;
vector<ll> quadro;
//vector<int> ids;
queue<int> bfs_q;
void quadratic_reverse() {
	visited.assign(n, 0);
	gray.assign(n, 0);
	quadro.assign(n, 0);
	int v = n - 1;
	// quadro[v] = 0;
	bfs_q.push(v);
	gray[v] = 1;

	while (!bfs_q.empty()) {
		v = bfs_q.front();
		bfs_q.pop();
		visited[v] = 1;
		for (arc e : rev_arcs[v]) {
			if (!visited[e.b] && D[v] == D[e.b] + e.t) {
				if (!gray[e.b]) {
					bfs_q.push(e.b);
					gray[e.b] = 1;
				}

				quadro[e.b] = max(quadro[e.b], quadro[v] + e.t * e.t);
			}
		}
	}
}

void input() {
	freopen("trains.in", "r", stdin);

	cin >> n >> m;
	arcs.assign(n, vector<arc>());
	rev_arcs.assign(n, vector<arc>());
	for (int i = 0; i < m; i++) {
		int s;
		cin >> s;
		int v1, t, v2;
		cin >> v1;
		v1--;
		for (int j = 0; j < s; j++) {
			cin >> t >> v2;
			v2--;

			arc e; e.b = v2; e.t = t; e.id = i;
			arc e1; e1.b = v1; e1.t = t; e1.id = i;
			arcs[v1].push_back(e);
			rev_arcs[v2].push_back(e1);

			v1 = v2;
		}
	}
}

void calculate() {
	djkstra(0);
	quadratic_reverse();
	/*for (int i = 0; i < n; i++) {
		cout << D[i] << " ";
	}
	cout << "\n\n";*/
	/*for (int i = 0; i < n; i++) {
		cout << quadro[i] << " ";
	}
	cout << "\n\n";*/
}

void output() {
	cout << D[n - 1] << " " << quadro[0] << "\n";
	fflush(stdin);
	/*while (true) {
		;
	}*/
}

int main()
{
	input();
	calculate();
	output();
	return 0;
}

