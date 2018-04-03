#pragma once

#include <bitset>
#include <queue>
#include "graphs.h"

using namespace graphs;

namespace flows
{
	const int INF = 1e9;
	class ford_fulkerson
	{
	private:
		static const int N = 1e3;
		bitset <N> vis;
		graph g;
		int source, sink, ans = 0;
		int dfs (int v, int mn)
		{
			if (v == sink)
				return mn;
			vis[v] = true;
			for (auto &it: g[v])
				if (!vis[it.to] && it.flow < it.capacity)
				{
					int delta = dfs(it.to, min(mn, it.capacity-it.flow));
					if (delta > 0)
					{
						it.flow += delta;
						g[it.to][it.reverse].flow -= delta;
						return delta;
					}
				}
			return 0;
		}
	public:
		ford_fulkerson (const graph &g, int source, int sink): g(g), source(source), sink(sink) {}
		int evaluate ()
		{
			if (ans == 0)
			{
				int tmp = 0;
				do
				{
					vis.reset();
					tmp = dfs(source, INF);
					ans += tmp;
					
				} while (tmp != 0);
			}
			return ans;
		}
	};
	
	class edmonds_carp
	{
	private:
		vector < int > mn;
		graph g;
		int source, sink, ans = 0;
	public:
		edmonds_carp (const graph &g, int source, int sink): g(g), source(source), sink(sink) {}
		int evaluate ()
		{
			if (ans == 0)
			{
				int tmp;
				do
				{
					vector < int > mn(g.size(), 0), p(g.size(), 0);
					mn[source] = INF;
					queue < int > que;
					que.push(source);
					while (!que.empty())
					{
						int v = que.front();
						que.pop();
						for (auto &it: g[v])
						{
							if (mn[it.to] == 0 && it.flow < it.capacity)
							{
								mn[it.to] = min(mn[v], it.capacity-it.flow);
								p[it.to] = it.reverse;
								que.push(it.to);
							}
						}
					}
					tmp = mn[sink];
					int v = sink;
					while (v != source)
					{
						g[v][p[v]].flow -= tmp;
						auto edge = g[v][p[v]];
						g[edge.to][edge.reverse].flow += tmp;
						v = edge.to;
					}
					ans += tmp;
				} while (tmp != 0);
			}
			return ans;
		}
	};
	
	class dinic
	{
	private:
		graph g;
		vector < int > p, d;
		int source, sink, ans = 0;
		bool bfs ()
		{
			fill(d.begin(), d.end(), INF);
			d[source] = 0;
			queue < int > que;
			que.push(source);
			while (!que.empty())
			{
				int v = que.front();
				que.pop();
				for (auto it: g[v])
				{
					if (d[it.to] == INF && it.flow < it.capacity)
					{
						d[it.to] = d[v]+1;
						que.push(it.to);
					}
				}
			}
			return d[sink] != INF;
		}
		int dfs (int v, int mn)
		{
			if (v == sink)
				return mn;
			while (p[v] != g[v].size())
			{
				auto it = g[v][p[v]];
				if (d[it.to] == d[v]+1 && it.flow < it.capacity)
				{
					int delta = dfs(it.to, min(mn, it.capacity-it.flow));
					if (delta > 0)
					{
						g[v][p[v]].flow += delta;
						g[it.to][it.reverse].flow -= delta;
						return delta;
					}
				}
				++p[v];
			}
			return 0;
		}
	public:
		dinic (const graph &g, int source, int sink): g(g), source(source), sink(sink)
		{
			p.resize(g.size(), 0);
			d.resize(g.size(), INF);
		}
		int evaluate ()
		{
			if (ans == 0)
			{
				while (bfs())
				{
					fill(p.begin(), p.end(), 0);
					int tmp;
					do
					{
						tmp = dfs(source, INF);
						ans += tmp;
					} while (tmp);
				}
			}
			return ans;
		}
	};
}