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
							if (mn[it.to] == 0 && it.flow < it.capacity)
							{
								mn[it.to] = min(mn[v], it.capacity-it.flow);
								p[it.to] = it.reverse;
								que.push(it.to);
							}
					}
					tmp = mn[sink];
					if (tmp != 0)
					{
						int v = sink;
						while (v != source)
						{
							g[v][p[v]].flow -= tmp;
							auto edge = g[v][p[v]];
							g[edge.to][edge.reverse].flow += tmp;
							v = edge.to;
						}
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
					if (d[it.to] == INF && it.flow < it.capacity)
					{
						d[it.to] = d[v]+1;
						que.push(it.to);
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
			return ans;
		}
	};
	
	class min_cost_max_flow_increasing_paths
	{
	private:
		graph g;
		int source, sink = 0;
		pair < int, int > ans = {0, 0};
	public:
		min_cost_max_flow_increasing_paths (const graph &g, int source, int sink): g(g), source(source), sink(sink) {}
		pair < int, int > evaluate ()
		{
			if (ans == make_pair(0, 0))
			{
				int tmp;
				do
				{
					vector < int > dist(g.size(), INF), mn(g.size(), 0), p(g.size(), 0);
					dist[source] = 0;
					mn[source] = INF;
					bool changed;
					do
					{
						changed = false;
						for (int v = 1; v < g.size(); ++v)
							for (auto &it: g[v])
								if (it.flow < it.capacity && dist[v]+it.cost < dist[it.to])
								{
									dist[it.to] = dist[v]+it.cost;
									mn[it.to] = min(mn[v], it.capacity-it.flow);
									p[it.to] = it.reverse;
									changed = true;
								}
					} while (changed);
					tmp = mn[sink];
					if (tmp != 0)
					{
						int v = sink;
						while (v != source)
						{
							g[v][p[v]].flow -= tmp;
							auto edge = g[v][p[v]];
							g[edge.to][edge.reverse].flow += tmp;
							v = edge.to;
						}
						ans.first += tmp;
						ans.second += dist[sink]*tmp;
					}
				} while (tmp != 0);
			}
			return ans;
		}
	};
	
	class min_cost_max_flow_cycles_removal
	{
	private:
		static const int N = 1e3;
		graph g;
		int source, sink = 0;
		pair < int, int > ans = {0, 0};
	public:
		min_cost_max_flow_cycles_removal (const graph &g, int source, int sink): g(g), source(source), sink(sink) {}
		pair < int, int > evaluate ()
		{
			if (ans.first == 0 && ans.second == 0)
			{
				int tmp;
				do
				{
					vector < int > dist(g.size(), INF), mn(g.size(), 0), p(g.size(), 0);
					dist[source] = 0;
					mn[source] = INF;
					queue < int > que;
					que.push(source);
					while (!que.empty())
					{
						int v = que.front();
						que.pop();
						for (auto &it: g[v])
							if (mn[it.to] == 0 && it.flow < it.capacity)
							{
								dist[it.to] = dist[v]+it.cost;
								mn[it.to] = min(mn[v], it.capacity-it.flow);
								p[it.to] = it.reverse;
								que.push(it.to);
							}
					}
					tmp = mn[sink];
					if (tmp != 0)
					{
						int v = sink;
						while (v != source)
						{
							g[v][p[v]].flow -= tmp;
							auto edge = g[v][p[v]];
							g[edge.to][edge.reverse].flow += tmp;
							v = edge.to;
						}
					}
					ans.first += tmp;
					ans.second += dist[sink]*tmp;
				} while (tmp != 0);
				// cerr << ans.second << endl;
				bool changed = false;
				do
				{
					int last;
					vector < int > dist(g.size(), INF), mn(g.size(), 0), p(g.size(), 0);
					for (int i = 1; i < g.size(); ++i)
					{
						dist[source] = 0;
						mn[source] = INF;
						changed = false;
						for (int v = 1; v < g.size(); ++v)
							for (auto &it: g[v])
								if (it.flow < it.capacity && dist[v]+it.cost < dist[it.to])
								{
									dist[it.to] = dist[v]+it.cost;
									mn[it.to] = min(mn[v], it.capacity-it.flow);
									p[it.to] = it.reverse;
									changed = true;
									last = v;
								}
					}
					if (changed)
					{
						int flow = INF;
						int cost = 0;
						int v = last;
						bitset <N> vis;
						do
						{
							auto edge = g[v][p[v]];
							flow = min(flow, g[edge.to][edge.reverse].capacity-g[edge.to][edge.reverse].flow);
							vis[v] = true;
							v = edge.to;
						} while (!vis[v]);
						last = v;
						do
						{
							g[v][p[v]].flow -= flow;
							auto edge = g[v][p[v]];
							cost += g[edge.to][edge.reverse].cost;
							g[edge.to][edge.reverse].flow += flow;
							v = edge.to;
						} while (v != last);
						ans.second += cost*flow;
					}
				} while (changed);
			}
			return ans;
		}
	};
};