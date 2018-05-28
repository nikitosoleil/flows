#include <iostream>
#include "graphs.h"
#include "flows.h"

using namespace graphs;
using namespace flows;

int main ()
{
	int n, m;
	cin >> n >> m;
	graph g;
	g.resize(n+1);
	for (int i = 1; i <= m; ++i)
	{
		int from, to, capacity, cost;
		cin >> from >> to >> capacity >> cost;
		g[from].push_back(edge(to, capacity, g[to].size(), cost));
		g[to].push_back(edge(from, 0, g[from].size()-1, -cost));
	}
	
	ford_fulkerson ff(g, 1, n);
	cout << ff.evaluate() << endl;
	
	edmonds_carp ec(g, 1, n);
	cout << ec.evaluate() << endl;
	
	dinic d(g, 1, n);
	cout << d.evaluate() << endl;
	
	min_cost_max_flow mcmf(g, 1, n);
	cout << mcmf.evaluate().first << ' ' << mcmf.evaluate().second << endl;
	return 0;
}