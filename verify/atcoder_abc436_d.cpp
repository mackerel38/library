#include <bits/stdc++.h>
#include "graph/hyperedge.hpp"
using namespace std;using namespace poe;
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int h,w;cin>>h>>w;vector<string>s(h);vector<vector<int>>groups(26);for(int r=0;r<h;++r){cin>>s[r];for(int c=0;c<w;++c)if(islower(s[r][c]))groups[s[r][c]-'a'].push_back(r*w+c);}auto d=bfs_with_hyperedges(h*w,groups,0,[&](int v,auto visit){int r=v/w,c=v%w;for(auto[dr,dc]:array<pair<int,int>,4>{{{1,0},{-1,0},{0,1},{0,-1}}}){int nr=r+dr,nc=c+dc;if(0<=nr&&nr<h&&0<=nc&&nc<w&&s[nr][nc]!='#')visit(nr*w+nc);}});cout<<d.back()<<'\n';}
