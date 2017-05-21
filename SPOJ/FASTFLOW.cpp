#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int const M = 1000 * 1000 * 1000 + 7;

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  int n, m;
  cin >> n >> m;
  vector<int> head(n+1, -1);  
  vector<int> to, nxt, cost;
  to.reserve(m+m);
  nxt.reserve(m+m);
  cost.reserve(m+m);

  for(int i = 0; i < m; ++i) {
    int u, v, c;
    cin >> u >> v >> c;
    if(u == v)
      continue;

    to.emplace_back(v);
    cost.emplace_back(c);
    nxt.emplace_back(head[u]);
    head[u] = nxt.size() - 1;

    to.emplace_back(u);
    cost.emplace_back(c);
    nxt.emplace_back(head[v]);
    head[v] = nxt.size() - 1;
  }
  
  // Dinic FastFlow O(V^2 * E)

  // build level graph
  vector<int> idx(n+1), dist(n+1);
  auto bfs = [&] () {
    fill(dist.begin(), dist.end(), n+n+n);
    queue<int> q;
    dist[1] = 0;
    for(q.push(1); q.size(); q.pop()) {
      int u = q.front();
      for(auto edge = head[u]; edge >= 0; edge = nxt[edge]) {
        int v = to[edge];
        int c = cost[edge];
        if(dist[v] == n + n + n && c) {
          dist[v] = dist[u] + 1;
          q.push(v);
        }
      }
    }

    return (dist[n] != n + n + n);
  };

  // send flow
  function<int(int, int)> dfs = [&] (int u, int f) {
    if(f == 0 || u == n)
      return f;

    for(int& edge = idx[u]; edge >= 0; edge = nxt[edge]) {
      int v = to[edge];
      int c = cost[edge];
      if(dist[v] != dist[u] + 1)
        continue;
      if(int x = dfs(v, min(f, c))) {
        cost[edge] -= x;
        cost[edge^1] += x;
        return x;
      }
    }
    return 0;
  };

  ll flow = 0;
  while(bfs()) {
    for(int i = 1; i <= n; ++i)
      idx[i] = head[i];

    while(int x = dfs(1, M))
      flow += x;
  }

  cout << flow << flush;
}
