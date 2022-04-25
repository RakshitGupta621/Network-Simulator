#include<bits/stdc++.h>
using namespace std;
#define ll long long

void cnproject(){
    #ifndef ONLINE_JUDGE
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    #endif
}

class Device{
public:
string mac_addr;
string ip_addr;
int switch_port;  //stores which port of the switch is this end device is connected 
};

class Switch{
public:
 ll ports;
string mac_addr;
//hashmap for mapping between port & mac address
unordered_map<ll,string> mp;
};



class Bridge{
public:
	ll ports;
	string mac_addr;
};

class Hub{
public:
   ll ports;
   string mac_addr;
};