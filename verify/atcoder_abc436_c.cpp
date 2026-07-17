#include <bits/stdc++.h>
using namespace std;
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n,m;cin>>n>>m;set<pair<int,int>> placed;for(int q=0;q<m;++q){int r,c;cin>>r>>c;bool ok=true;for(int dr=-1;dr<=1;++dr)for(int dc=-1;dc<=1;++dc)ok&=!placed.contains({r+dr,c+dc});if(ok)placed.insert({r,c});}cout<<placed.size()<<'\n';}
