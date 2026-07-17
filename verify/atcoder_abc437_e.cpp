#include <bits/stdc++.h>
#include "string/persistentsequence.hpp"
using namespace std;using namespace poe;
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);int n;cin>>n;persistentsequenceorder<long long>s;for(int i=1;i<=n;++i){int p;long long y;cin>>p>>y;s.add(p,y);}auto a=s.order();for(int i=0;i<n;++i)cout<<a[i]<<" \n"[i+1==n];}
