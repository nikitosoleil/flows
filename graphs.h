#pragma once

#include <vector>

using namespace std;

namespace graphs
{
	struct edge
	{
		int to, capacity, reverse, flow;
		edge(int a=0, int b=0, int c=0): to(a), capacity(b), reverse(c), flow(0) {}
	};
	typedef vector < vector < edge > > graph;
}