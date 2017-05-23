#include<algorithm>
#include<iostream>
#include<iomanip>
#include<functional>
#include<numeric>
#include<iterator>
#include<queue>
#include<deque>
#include<vector>
#include<cstring>
#include<cmath>
#include<utility>
#include<set>
#include<map>
#include<random>

using namespace std;
using ll = long long;

int const M = 1000 * 1000 * 1000 + 7;

struct node {
  int x = 0;
  int y = 0;
  int rev = 0;
  
  node* l = 0;
  node* r = 0;
  node* p = 0;
  node* pp = 0;
  
  void upd() {
    y = x;
    if(l) y = max(y, l->y);
    if(r) y = max(y, r->y);
  }
};


struct linkcut {
  vector<node> a;
  
  linkcut(int n) {
    a.resize(n+1);
  }
  
  void push(auto p) {
    if(not p || not p->rev)
      return;
    p->rev ^= 1;
    swap(p->l, p->r);
    if(p->l) p->l->rev ^= 1;
    if(p->r) p->r->rev ^= 1;
  }
  
  node* rtl(auto p) {
    auto g = p->p;    
    auto c = p->r;
    p->r = c->l;
    c->l = p;
    c->p = p->p;
    p->p = c;
    
    if(p->r)
      p->r->p = p;
      
    if(g && g->l == p) g->l = c;
    else if(g && g->r == p) g->r = c;
    
    c->pp = p->pp;
    p->upd();
    c->upd();
    return c;
  }
  node* rtr(auto p){
    auto g = p->p;
    auto c = p->l;
    p->l = c->r;
    c->r = p;
    c->p = p->p;
    p->p = c;
    
    if(p->l)
      p->l->p = p;
    
    if(g && g->l == p) g->l = c;
    else if(g && g->r == p) g->r = c;
    
    c->pp = p->pp;
    p->pp = 0;
    p->upd();
    c->upd();
    return c;
  }
  
  void splay(auto c) {
    while(c->p) {
      auto p = c->p;
      auto g = p->p;
 
      push(g);
      push(p);
      push(c);
 
      if(c == p->l) {
        if(not g)               rtr(p);
        else if(g->l == p) rtr(g), rtr(p);
        else                      rtr(p), rtl(g);
      } else {
        if(not g)               rtl(p);
        else if(g->r == p) rtl(g), rtl(p);
        else                      rtl(p), rtr(g);
      }
    }
    push(c);
  }
  
  node* change(node* p, node* c) {
    if(p->r) {
      p->r->p = 0;
      p->r->pp = p;
    }
    if(c) {
      p->r = c;
      c->p = p;
      c->pp = 0;
    }
    p->r = c;
    p->upd();
    return p;
  }
  
  node* access(int u) {
    auto p = &a[u];
    splay(p);
    while(p->pp) {
      splay(p->pp);
      p = change(p->pp, p);
    }
    splay(&a[u]);
    return p;
  }
  
  void link(int u, int v) {
    access(v);
    set_root(u);
    auto U = &a[u];
    auto V = &a[v];
    U->pp = V;
  }
  
  void set_root(int u) {
    access(u);
    auto p = &a[u];
    if(p->l) {
      p->l->rev ^= 1;
      p->l->pp = p;
      p->l->p = 0;
      p->l = 0;
      p->upd();
    }
  }
  
  void update(int u, int v) {
    access(u);
    auto p = &a[u];
    p->x += v;
    p->upd();
  }
  
  int query(int u, int v) {
    set_root(u);
    access(v);
    return change(&a[v], 0)->y;
  }
};

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  int n;
  cin >> n;
  
  linkcut t(n);
  
  for(int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    t.link(u, v);
  }
  
  int m;
  cin >> m;
  for(int i = 0; i < m; ++i) {
    char c;
    int u, v;
    cin >> c >> u >> v;
    if(c == 'I') {
      t.update(u, v);
    } else {
      cout << t.query(u, v) << '\n';
    }
  }
}
