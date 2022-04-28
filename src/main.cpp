#include <bits/stdc++.h>
using namespace std;
#define ll long long
// #define N 1001;
void cnproject()
{
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}

class Device
{
public:
    string mac_addr;
    ll index;
};

class Switch
{
public:
    ll ports;
    string mac_addr;
    // hashmap for mapping between port & mac address
    unordered_map<string, ll> mp;
};

class Bridge
{
public:
    ll ports;
    string mac_addr;
    unordered_map<string, ll> mp;
};

class Hub
{
public:
    ll ports;
    string mac_addr;
};

vector<string> macaddrlist;
ll mac_index;
unordered_map<ll, pair<string, ll>> Device_Identity;

vector<Hub> hub_arr;
vector<Device> device_arr;
vector<Switch> switch_arr;
vector<Bridge> bridge_arr;
ll device_index;
ll hub_index;
ll bridge_index;
ll switch_index;

string convertllToHex(ll x)
{
    unordered_map<int, string> mpp;
    mpp[10] = "A";
    mpp[11] = "B";
    mpp[12] = "C";
    mpp[13] = "D";
    mpp[14] = "E";
    mpp[15] = "F";
    if (x <= 9)
        return to_string(x);
    return mpp[x];
}

string generatingAddress(string add, ll i, ll j, ll k, ll l)
{
    add += convertllToHex(i);
    add += convertllToHex(j);
    add.push_back(':');
    add += convertllToHex(k);
    add += convertllToHex(l);
    return add;
}

void settingMacAddress()
{
    ll numFreq = 0;
    string generatedAddress = "00:AA:BB:";
    for (ll x = 0; x < 16; x++)
    {
        for (ll y = 0; y < 16; y++)
        {
            for (ll z = 0; z < 16; z++)
            {
                for (ll w = 0; w < 16; w++)
                {
                    string full_add = generatingAddress(generatedAddress, x, y, z, w);
                    macaddrlist.push_back(full_add);
                }
            }
        }
    }
}

vector<ll> adjConnectionsList[1001]; // adjacency List of connnected Networks

void addEdge(ll u, ll v)
{
    // Bidirectional edge connection
    adjConnectionsList[u].push_back(v);
    adjConnectionsList[v].push_back(u);
}

void DFS(ll curr_device, vector<bool> &vis)
{

    if (vis[curr_device])
        return;

    vis[curr_device] = 1;
    string type = Device_Identity[curr_device].first;
    if (type == "device")
    {
        Device d = device_arr[Device_Identity[curr_device].second];
        cout << "index in network:- " << d.index << "\n";
        cout << "mac addr" << d.mac_addr << "\n";
    }
    else if (type == "switch")
    {
        Switch s = switch_arr[Device_Identity[curr_device].second];
        cout << "index in network:- " << s.ports << "\n";
        cout << "mac addr" << s.mac_addr << "\n";
    }
    else if (type == "bridge")
    {
        Bridge b = bridge_arr[Device_Identity[curr_device].second];
        cout << "index in network:- " << b.ports << "\n";
        cout << "mac addr" << b.mac_addr << "\n";
    }

    ll sz = adjConnectionsList[curr_device].size();

    for (ll i = 0; i < sz; i++)
    {
        if (!vis[adjConnectionsList[curr_device][i]])
            DFS(adjConnectionsList[curr_device][i], vis);
    }
}

void send_packet(ll present_instru, vector<bool> & vis, ll ind_prev , string senders_mac,string destination_mac,bool isAck, bool &isAckRecieved) {
    if(!vis[present_instru]) {
        vis[present_instru] = true;
        
        string type = Device_Identity[present_instru].first;
        // If the device is switch or bridge we will cache port and source in the caching table first
        if(type == "switch" || type == "bridge") {
            // First lets indetify the port on which our source is 
            ll port = -1;
            for(ll i = 0;i < adjConnectionsList[present_instru].size(); i++) {
                if (adjConnectionsList[present_instru][i] == ind_prev) {
                    port = i+1;
                    break;
                }
            }

            if(type == "switch") {
                Switch &s = switch_arr[Device_Identity[present_instru].s];
                if(port != -1) { 
                    s.mp[senders_mac] = port;
                    // cout<<s.ports<<"\n";
                    // cout<<s.mac_addr<<"\n";
                    // cout<<"SWITCHING TABLE \n";
                    // for(auto it = s.mp.begin();it!= s.mp.end();it++) {
                    //     cout<<it->first<<" "<<it->second<<"\n"; 
                    // }
                }
                ll destPort = 0;
                destPort = s.mp[destination_mac];

                if(destPort != 0) {

                    cout<<"Device Type : "<< type<<"\n";
                    cout<<"Global Index : "<<s.ports<<"\n";
                    cout<<"Mac address : "<<s.mac_addr<<"\n";
                    if(isAck) cout<<"isACK \n";
                    cout<<"Sending to : "<<adjConnectionsList[present_instru][destPort-1]<<"\n\n";

                    if(collision_occurance_prob() == 1) {
                        send_packet(adjConnectionsList[present_instru][destPort-1],vis,present_instru,senders_mac,destination_mac,isAck,isAckRecieved);
                    } else {
                        cout<<"\n !!!!! COLLISION HAPPENED WHILE TRANSMISSION !!!!! \n";
                    }
                } else {
                    //Broadcast
                    cout<<"SWITCH IS BROADCASTING : \n\n";
                    for(ll i = 0;i < adjConnectionsList[present_instru].size(); i++) {
                        if(!vis[adjConnectionsList[present_instru][i]]) {
                            cout<<"Device Type : "<< type<<"\n";
                            cout<<"Global Index : "<<s.ports<<"\n";
                            cout<<"Mac address : "<<s.mac_addr<<"\n";
                            if(isAck) cout<<"isACK \n";
                            cout<<"Sending to : "<<adjConnectionsList[present_instru][i]<<"\n\n";
                            if(collision_occurance_prob() == 1) {
                                send_packet(adjConnectionsList[present_instru][i],vis,present_instru,senders_mac,destination_mac,isAck,isAckRecieved);
                            } else {
                                cout<<"\n !!!!! COLLISION HAPPENED WHILE TRANSMISSION !!!!! \n";
                            }
                        }
                    }
                }

            }

            if(type == "bridge") {
                Bridge &b = bridge_arr[Device_Identity[present_instru].s];
                if(port != -1) b.mp[senders_mac] = port;
                ll destPort = 0;
                destPort = b.mp[destination_mac];
                if(destPort != 0) {
                    cout<<"Device Type : "<< type<<"\n";
                    cout<<"Global Index : "<<b.ports<<"\n";
                    cout<<"Mac address : "<<b.mac_addr<<"\n";
                    if(isAck) cout<<"isACK \n";
                    cout<<"Sending to : "<<adjConnectionsList[present_instru][destPort-1]<<"\n\n";
                    if(collision_occurance_prob() == 1) {
                        send_packet(adjConnectionsList[present_instru][destPort-1],vis,present_instru,senders_mac,destination_mac,isAck,isAckRecieved);
                    } else {
                        cout<<"\n !!!!! COLLISION HAPPENED WHILE TRANSMISSION !!!!! \n";
                    }
                } else {
                    //Broadcast
                    for(ll i = 0;i < adjConnectionsList[present_instru].size(); i++) {
                        if(!vis[adjConnectionsList[present_instru][i]]) {
                            cout<<"Device Type : "<< type<<"\n";
                            cout<<"Global Index : "<<b.ports<<"\n";
                            cout<<"Mac address : "<<b.mac_addr<<"\n";
                            if(isAck) cout<<"isACK \n";
                            cout<<"Sending to : "<<adjConnectionsList[present_instru][i]<<"\n\n";
                            if(collision_occurance_prob() == 1) {    
                                send_packet(adjConnectionsList[present_instru][i],vis,present_instru,senders_mac,destination_mac,isAck,isAckRecieved);
                            } else {
                                cout<<" \n !!!!! COLLISION HAPPENED WHILE TRANSMISSION !!!!! \n";
                            }
                        }
                    }
                }
            }

            return;

        }      

        // if the device is Hub or a dedicated connection directly send
        Hub h;
        Device d;
        // CHECK IF WE ARE DESTINATION OR NOT
        d = device_arr[Device_Identity[present_instru].s];
        if(type == "device" && d.mac_addr == destination_mac && !isAck) {
            cout<<"Data packet recieved sucessfully sending back ACK";
            vector<bool> vis(10001,false);
            cout<<"\nSENDING ACK FROM "<< destination_mac<<"  to  "<<senders_mac<<"\n\n";
            // if(collision_occurance_prob() == 1) {
                send_packet(present_instru,vis,-1,destination_mac,senders_mac,true,isAckRecieved);
            // } else {
                // cout<<" \n !!!!! COLLISION HAPPENED WHILE TRANSMISSION !!!!! \n";
            // }
            return;
        } else if(type == "device" && d.mac_addr == destination_mac) {
            isAckRecieved = true;
            cout<<"ACK recieved sucessfully \n\n";
            return;
        }
        for(ll i = 0;i < adjConnectionsList[present_instru].size(); i++) {
            if(!vis[adjConnectionsList[present_instru][i]]) {
                if(type == "hub") {
                    h = hub_arr[Device_Identity[present_instru].s];
                    cout<<"Device Type : "<< type<<"\n";
                    cout<<"Global Index : "<<h.ports<<"\n";
                    cout<<"Mac address : "<<h.mac_addr<<"\n";
                    if(isAck) cout<<"isACK \n";
                    cout<<"Sending to : "<<adjConnectionsList[present_instru][i]<<"\n\n";
                } 
                if(type == "device") {
                    d = device_arr[Device_Identity[present_instru].s];
                    cout<<"Device Type : "<< type<<"\n";
                    cout<<"Global Index : "<<d.index<<"\n";
                    cout<<"Mac address : "<<d.mac_addr<<"\n";
                    if(isAck) cout<<"isACK \n";
                    cout<<"Sending to : "<<adjConnectionsList[present_instru][i]<<"\n\n";

                    if(d.mac_addr == destination_mac && !isAck) {
                        cout<<"Data packet recieved sucessfully sending back ACK";
                        vector<bool> vis(10001,false);
                        cout<<"\nSENDING ACK FROM "<< destination_mac<<"  to  "<<senders_mac<<"\n\n";
                        if(collision_occurance_prob() == 1) {
                            send_packet(present_instru,vis,-1,destination_mac,senders_mac,true,isAckRecieved);
                        } else {
                            cout<<"\n !!!!! COLLISION HAPPENED WHILE TRANSMISSION !!!!! \n";
                        }
                        return;
                    } else if(d.mac_addr == destination_mac) {
                        isAckRecieved = true;
                        cout<<"ACK recieved sucessfully";
                    }
                }
                if(collision_occurance_prob() == 1) {
                    send_packet(adjConnectionsList[present_instru][i],vis,present_instru,senders_mac,destination_mac,isAck,isAckRecieved);
                } else {
                    cout<<"\n !!!!! COLLISION HAPPENED WHILE TRANSMISSION !!!!! \n";
                }
            }
        }
    }
}

void initialization()
{
    ll n, m, ind;
    cin >> n >> m;
    for (ll i = 1; i <= n; i++)
    {
        string type;
        cin >> ind >> type;
        if (type == "device")
        {
            Device d;
            d.index = ind;
            d.mac_addr = macaddrlist[mac_index];
            device_arr.push_back(d);
            Device_Identity[ind] = make_pair(type, device_index);
            device_index++;
            mac_index++;
            // cout<< " mac assigned to device "<<ind<<" is : "<<d.mac_addr<<"\n";
        }
        else if (type == "hub")
        {
            Hub h;
            h.ports = ind;
            h.mac_addr = macaddrlist[mac_index];
            hub_arr.push_back(h);
            Device_Identity[ind] = make_pair(type, hub_index);
            hub_index++;
            mac_index++;
            // cout<< " mac assigned to device "<<ind<<" is : "<<h.mac_addr<<"\n";
        }
        else if (type == "bridge")
        {
            Bridge b;
            b.ports = ind;
            b.mac_addr = macaddrlist[mac_index];
            bridge_arr.push_back(b);
            Device_Identity[ind] = make_pair(type, bridge_index);
            bridge_index++;
            mac_index++;
            // cout<< " mac assigned to device "<<ind<<" is : "<<b.mac_addr<<"\n";
        }
        else if (type == "switch")
        {
            Switch s;
            s.ports = ind;
            s.mac_addr = macaddrlist[mac_index];
            switch_arr.push_back(s);
            Device_Identity[ind] = make_pair(type, switch_index);
            switch_index++;
            mac_index++;
            // cout<< " mac assigned to device "<<ind<<" is : "<<s.mac_addr<<"\n";
        }
    }

    cout << "\n";

    for (ll i = 0; i < m; i++)
    {
        ll u, v;
        cin >> u >> v;
        addEdge(u, v);
    }
    vector<bool> vis(n + 1, false);
    DFS(1, vis);
}

void castNetwork()
{
    ll res, prob;
    cin >> res >> prob;
    p = prob;

    if (res == 0)
    {
        ll q;
        cin >> q;
        for (ll i = 0; i < q; i++)
        {
            ll sender, rec, num_packets;
            cin >> sender >> rec >> num_packets;

            Device curr_send = device_arr[Device_Identity[sender].second];
            Device curr_rec = device_arr[Device_Identity[rec].second];

            for (ll i = 0; i < num_packets; i++)
            {
                bool ackflag = false;

                while (!ackflag)
                {
                    vector<bool> vis(10001, false);
                    cout << "\nSENDING PACKET FROM " << curr_send.mac_addr << "  to  " << curr_rec.mac_addr << "\n\n";
                    send_packet(sender, vis, -1, curr_send.mac_addr, curr_rec.mac_addr, false, ackflag);
                    if (!ackflag)
                        cout << "\n !!!!!! ACK NOT RECIEVED TIMED OUT RESEND ACK !!!!!! \n";
                }

                cout << "Switch Table contents " << endl;
                for (ll i = 0; i < switch_arr.size(); i++)
                {
                    Switch s = switch_arr[i];
                    cout << "Switch's global address : " << s.ports << "\n";
                    cout << "Switch's mac address : " << s.mac_addr << "\n";

                    for (auto it = s.mp.begin(); it != s.mp.end(); it++)
                    {
                        cout << it->first << " " << it->second << "\n";
                    }
                    cout << "\n";
                }

                cout << "Bridge Table contents " << endl;

                for (ll i = 0; i < bridge_arr.size(); i++)
                {
                    Bridge b = bridge_arr[i];
                    cout << "Bridge's global address : " << b.ports << "\n";
                    cout << "Bridge's mac address : " << b.mac_addr << "\n";

                    for (auto it = b.mp.begin(); it != b.mp.end(); it++)
                    {
                        cout << it->first << " " << it->second << "\n";
                    }

                    cout << "\n";
                }
            }
        }
    }
    else
    {

        queue<pair<string, ll>> reservation_frame[device_arr.size() + 1];
        ll q;
        cin >> q;
        while (q--)
        {
            ll sender, rec, num_packets;
            cin >> sender >> rec >> num_packets;

            ll a_device_index = Device_Identity[sender].second;
            ll b_device_index = Device_Identity[rec].second;

            Device curr_sender = device_arr[a_device_index];
            Device curr_rec = device_arr[b_device_index];

            reservation_frame[a_device_index].push(make_pair(curr_rec.mac_addr, num_packets));
        }

        // Sending packets according to reservation frames
        bool remaining = true;
        while (remaining)
        {
            bool sent = false;
            for (ll i = 0; i < device_arr.size(); i++)
            {
                if (reservation_frame[i].size() > 0)
                {

                    Device sender = device_arr[i];

                    bool ackflag = false;

                    while (!ackflag)
                    {
                        vector<int> vis(1001, 0);
                        cout << "\nSENDING PACKET FROM " << sender.mac_addr << "  to  " << reservation_frame[i].front().first << "\n\n";
                        send_packet(sender.index, vis, -1, sender.mac_addr, reservation_frame[i].front().first, false, ackflag);
                        if (!ackflag)
                            cout << "\n !!!!!! ACK NOT RECIEVED TIMED OUT RESENDING !!!!!! \n";
                    }
                    sent = true;
                    reservation_frame[i].front().second--;
                    if (reservation_frame[i].front().second == 0)
                    {
                        reservation_frame[i].pop();
                    }

                    cout << "Switch Table contents " << endl;
                    for (ll i = 0; i < switch_arr.size(); i++)
                    {
                        Switch s = switch_arr[i];
                        cout << "Switch's global address : " << s.ports << "\n";
                        cout << "Switch's mac address : " << s.mac_addr << "\n";

                        for (auto it = s.mp.begin(); it != s.mp.end(); it++)
                        {
                            cout << it->first << " " << it->second << "\n";
                        }
                        cout << "\n";
                    }

                    cout << "Bridge Table contents " << endl;

                    for (ll i = 0; i < bridge_arr.size(); i++)
                    {
                        Bridge b = bridge_arr[i];
                        cout << "Bridge's global address : " << b.ports << "\n";
                        cout << "Bridge's mac address : " << b.mac_addr << "\n";

                        for (auto it = b.mp.begin(); it != b.mp.end(); it++)
                        {
                            cout << it->first << " " << it->second << "\n";
                        }

                        cout << "\n";
                    }
                }
            }
            remaining = sent;
        }
    }
}

map<pair<ll, ll>, ll> edge_Weight;


void count_impact_zone(ll present_instru,ll prev_instru, vector<ll> &vis,ll &count) {
    if(!vis[present_instru]) {
        vis[present_instru] = true;

        if(prev_instru != -1) {
            edge_Weight[(make_pair(present_instru,prev_instru))] = count;
            edge_Weight[make_pair(prev_instru,present_instru)] = count;
        }

        for(ll i=0;i < adjConnectionsList[present_instru].size();i++) {
            if(!vis[adjConnectionsList[present_instru][i]]) {

                if( (Device_Identity[present_instru].first == "switch" || Device_Identity[present_instru].first == "bridge") && Device_Identity[adjConnectionsList[present_instru][i]].first == "device") {
                    count++;
                    count_impact_zone(adjConnectionsList[present_instru][i],present_instru,vis,count);
                } 

                if(Device_Identity[present_instru].first == "device" && (Device_Identity[adjConnectionsList[present_instru][i]].first == "switch" || Device_Identity[adjConnectionsList[present_instru][i]].first == "bridge" ) ) {
                    count++;
                    count_impact_zone(adjConnectionsList[present_instru][i],present_instru,vis,count);
                } 

                if( (Device_Identity[present_instru].first == "switch" || Device_Identity[present_instru].first == "bridge") &&  (Device_Identity[adjConnectionsList[present_instru][i]].first == "switch" || Device_Identity[adjConnectionsList[present_instru][i]].first == "bridge" )) {
                    count++;
                    count_impact_zone(adjConnectionsList[present_instru][i],present_instru,vis,count);
                } 

                if( (Device_Identity[present_instru].first == "switch" || Device_Identity[present_instru].first == "bridge") && Device_Identity[adjConnectionsList[present_instru][i]].first == "hub") {
                    count++;
                    count_impact_zone(adjConnectionsList[present_instru][i],present_instru,vis,count);
                }             

                if(Device_Identity[present_instru].first == "hub" && (Device_Identity[adjConnectionsList[present_instru][i]].first == "switch" || Device_Identity[adjConnectionsList[present_instru][i]].first == "bridge" )) {
                    count_impact_zone(adjConnectionsList[present_instru][i],present_instru,vis,count);
                } 

                if(Device_Identity[present_instru].first == "hub" && Device_Identity[adjConnectionsList[present_instru][i]].first == "device") {
                    count_impact_zone(adjConnectionsList[present_instru][i],present_instru,vis,count);
                } 

                if(Device_Identity[present_instru].first == "device" && Device_Identity[adjConnectionsList[present_instru][i]].first == "hub") {
                    count_impact_zone(adjConnectionsList[present_instru][i],present_instru,vis,count);
                } 

            }   

        }

    }
}


void impact_zone()
{
    int N = 1001;
    vector<ll> vis(N, 0);

    ll count = 1;
    count_impact_zone(1, -1, vis, count);

    unordered_set<ll> result;

    for (auto it : edge_Weight)
    {
        result.insert(it.second);
    }

    cout << "Number of Collisions: -" << result.size() << "\n";
    cout << "Number of BroadCast Domains :- " << 1 << " \n";
}
ll p;

ll collision_occurance_prob()
{
    vector<ll> collision_prob(101, 0);
    for (ll i = 0; i < p; i++)
    {
        collision_prob[i] = 1;
    }
    ll idx = rand() % 100;
    return collision_prob[idx];
}

void CNproject() {
    settingMacAddress();
    initialization();
    castNetwork();
    impact_zone();
    return;
}

// Main function
int main()
{
    cnproject();
}