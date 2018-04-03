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
		int from, to, capacity;
		cin >> from >> to >> capacity;
		g[from].push_back(edge(to, capacity, g[to].size()));
		g[to].push_back(edge(from, 0, g[from].size()-1));
	}
	
	ford_fulkerson ff(g, 1, n);
	cout << ff.evaluate() << endl;
	
	edmonds_carp ec(g, 1, n);
	cout << ec.evaluate() << endl;
	
	dinic d(g, 1, n);
	cout << d.evaluate() << endl;
	
	return 0;
}