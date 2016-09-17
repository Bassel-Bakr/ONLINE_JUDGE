#include <bits/stdc++.h>

using namespace std;

struct node {
  int x;
  node *l, *r, *p, *pp;
  node(int x) : x(x) {}
};

struct linkcut_tree {
  int n;
  vector<node*> nodes;

  linkcut_tree(int n) {
    this->n = n;
    nodes.resize(n+1);
    for(int i = 1; i <= n; ++i)
      nodes[i] = new node(i);
  }

  ~linkcut_tree() {
    for(int i = 1; i <= n; ++i)
      delete nodes[i];
  }

  void set_parent(node* p, node* c) {
    if(c) c->p = p;
  }

  void rtr(node* c) {
    node* p = c->p;
    node* g = p->p;

    p->l = c->r;
    c->r = p;

    if(g) if (g->l == p) g->l = c;
          else           g->r = c;

    set_parent(g, c);
    set_parent(c, p);
    set_parent(p, p->l);

    c->pp = p->pp, p->pp = 0;
  }

  void rtl(node* c) {
    auto p = c->p;
    auto g = p->p;

    p->r = c->l;
    c->l = p;

    if(g) if (g->l == p) g->l = c;
          else           g->r = c;

    set_parent(g, c);
    set_parent(c, p);
    set_parent(p, p->r);

    c->pp = p->pp, p->pp = 0;
  }

  void splay(node* c) {
    while(c->p) {
      auto p = c->p;
      auto g = p->p;

      if(c == p->l) {
        if(not g)          rtr(c);
        else if(p == g->l) rtr(p), rtr(c);
        else               rtr(c), rtl(c);
      } else {
        if(not g)          rtl(c);
        else if(p == g->r) rtl(p), rtl(c);
        else               rtl(c), rtr(c);
      }
    }
  }

  void detach_right(node* p) {
    if(not p->r) return;
    p->r->pp = p;
    p->r->p = 0;
    p->r = 0;
  }

  void attach_right(node* p, node* c) {
    if(p->r) detach_right(p);
    c->pp = 0;
    c->p = p;
    p->r = c;
  }

  // make (u) the root and detach all nodes below it
  // from prefered path
  node* access(int u) {
    auto p = nodes[u];
    splay(p);
    detach_right(p);

    while(p->pp) {
      auto pp = p->pp;
      splay(pp);
      attach_right(pp, p);
      p = pp;
    }

    splay(nodes[u]);
    return p;
  }

  // (b)->(a), (a) is root
  void link(int a, int b) {
    access(b);
    access(a);
    auto A = nodes[a];
    auto B = nodes[b];
    A->l = B;
    B->p = A;
  }

  // (b)->(a), (a) is child
  void cut(int a) {
    access(a);
    auto A = nodes[a];
    A->l->p = 0;
    A->l = 0;    
  }

  int lca(int a, int b) {
    access(a);
    auto x = access(b);
    return x->x;
  }
};

char w[8];
int main() {
  int n, m;
  scanf("%d%d", &n, &m);
  linkcut_tree t(n);

  while(m--) {
    int a, b;
    scanf(" %s %d", w, &a);
    if(w[1] == 'u')
      t.cut(a);
    else {
      scanf("%d", &b);
      if(w[1] == 'i')
        t.link(a, b);
      else
        printf("%d\n", t.lca(a, b));
    }    
  }
}
