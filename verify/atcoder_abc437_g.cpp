#include <bits/stdc++.h>
#include "flow/maxflow.hpp"
using namespace std;using namespace poe;
struct operation{int flow_id,u,cu,v,cv,edge_id;};
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int T;cin>>T;while(T--){
 int n;string s;cin>>n>>s;vector<int>c(n);for(int i=0;i<n;++i)c[i]=s[i]=='R'?0:s[i]=='G'?1:2;
 vector<pair<int,int>>edges(n-1);vector<vector<pair<int,int>>>g(n);for(int i=0;i<n-1;++i){auto&[u,v]=edges[i];cin>>u>>v;--u;--v;g[u].push_back({v,i});g[v].push_back({u,i});}
 vector<int>side(n,-1),st{0};side[0]=0;while(!st.empty()){int v=st.back();st.pop_back();for(auto[to,id]:g[v])if(side[to]<0)side[to]=side[v]^1,st.push_back(to);}
 int source=3*n,sink=source+1;maxflow<int>flow(3*n+2);for(int v=0;v<n;++v){array<int,3>cnt{};for(int i=0;i<(int)g[v].size();++i)++cnt[(c[v]+i)%3];for(int x=0;x<3;++x)if(side[v])flow.add_edge(3*v+x,sink,cnt[x]);else flow.add_edge(source,3*v+x,cnt[x]);}
 vector<operation>cand;for(int id=0;id<n-1;++id){auto[u,v]=edges[id];if(side[u])swap(u,v);for(int x=0;x<3;++x)for(int y=0;y<3;++y)if(x!=y){int f=flow.add_edge(3*u+x,3*v+y,1);cand.push_back({f,u,x,v,y,id});}}
 if(flow.flow(source,sink)!=n-1){cout<<"No\n";continue;}vector<operation>ops;for(auto x:cand)if(flow.get_edge(x.flow_id).flow)ops.push_back(x);vector<int>ans;
 while(!ops.empty()){auto it=find_if(ops.begin(),ops.end(),[&](auto x){return c[x.u]==x.cu&&c[x.v]==x.cv;});assert(it!=ops.end());ans.push_back(it->edge_id);c[it->u]=(c[it->u]+1)%3;c[it->v]=(c[it->v]+1)%3;ops.erase(it);}
 cout<<"Yes\n";for(int i=0;i<n-1;++i)cout<<ans[i]+1<<" \n"[i+1==n-1];}}
