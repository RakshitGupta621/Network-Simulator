#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);    
    }
};


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

class Router{
  public: 
    ll global_index;
    ll no_of_interfaces;
    vector< pair<pair<string,string>,string> > IPtoMAC;
};

class Host{
  public:
    ll global_index;
    string mac;
    string ipv4;
    string subnet;
    string nid;
    string gateway_mac;
    unordered_map<string,string> arp_table;
};

vector<string> macaddrlist;
ll mac_index;
unordered_map<ll, pair<string, ll>> Device_Identity;

vector<Hub> hub_arr;
vector<Device> device_arr;
vector<Switch> switch_arr;
vector<Bridge> bridge_arr;
vector<Router> router_arr;
vector<Host> host_arr;


ll device_index;
ll hub_index;
ll bridge_index;
ll switch_index;
ll router_index;
ll host_index;

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

vector<int> ip_bits(string ip) {

  vector<int> bit(32,0);

  ll i = 31;

  ll start = 0;
  string res = "";
  while(start < ip.length()) {
    if(ip[start] == '.') {
      ll val = stoi(res);
      vector<int> bitset2(8,0);
      for(int i=0;i<8;i++){
        if(val & (1<<i))
          bitset2[i]=1;
      }
      ll s = 7;
      while(s >= 0) {
        bit[i] = bitset2[s];
        i--;
        s--;
      }
      res = "";
    } else {
      res.push_back(ip[start]);
    }
    start++;
  }
  ll val = stoi(res);
  vector<int> bitset2(8,0);
  for(int i=0;i<8;i++){
    if(val & (1<<i))
      bitset2[i]=1;
  }
  ll s = 7;
  while(s >= 0) {
    bit[i] = bitset2[s];
    i--;
    s--;
  }

  return bit;
}


string NID (string ip,string subnet) {


vector<int> bitsip(ip_bits(ip));
vector<int> subnet(ip_bits(subnet));
  vector<int> nid(32,0);

  for(int i=0;i<32;i++)
    nid[i]=bitsip[i] & subnet[i];
   string res;

  ll ind = 7;

  vector<int>  nidbits(8,0);

  for(ll i = 31;i >= 0; i--) {
    if(ind == -1) {
      ind = 7;
      ll val =0;
      for(int i=0;i<8;i++){
        if(nidbits[i])
        val = val | (1<<i);
      }

      nids += to_string(val);
      nids.pb('.');
      nidbits.resize(8,0);
    }
    nidbits[ind] = nid[i];
    ind--;
  }
  ll val = 0;
  for(int i=0;i<8;i++){
    if(nidbits[i])
      val |=(1<<i);
  }

  res += to_string(val);
  return resize;
}


vector<ll> adjConnectionsList[1001];

void addEdge(ll u, ll v)
{
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
        cout << "Index in network:" << d.index << "\n";
        cout << "Mac address: " << d.mac_addr << "\n";
    }
    else if (type == "switch")
    {
        Switch s = switch_arr[Device_Identity[curr_device].second];
        cout << "Index in network: " << s.ports << "\n";
        cout << "Mac address:" << s.mac_addr << "\n";
    }
    else if (type == "bridge")
    {
        Bridge b = bridge_arr[Device_Identity[curr_device].second];
        cout << "Index in network:" << b.ports << "\n";
        cout << "Mac address:" << b.mac_addr << "\n";
    }

    ll sz = adjConnectionsList[curr_device].size();

    for (ll i = 0; i < sz; i++)
    {
        if (!vis[adjConnectionsList[curr_device][i]])
            DFS(adjConnectionsList[curr_device][i], vis);
    }
}

ll collision_occurance_prob()
{
    if (rand() < (.5))
        return 0;
    return 1;
}

void transmit_data(ll present_instru, vector<bool> &vis, ll ind_prev, string senders_mac, string destination_mac, bool isAck, bool &isAckRecieved)
{
    if (!vis[present_instru])
    {
        vis[present_instru] = true;

        string type = Device_Identity[present_instru].first;
        if (type == "switch" || type == "bridge")
        {
            ll port = -1;
            for (ll i = 0; i < adjConnectionsList[present_instru].size(); i++)
            {
                if (adjConnectionsList[present_instru][i] == ind_prev)
                {
                    port = i + 1;
                    break;
                }
            }

            if (type == "switch")
            {
                Switch &s = switch_arr[Device_Identity[present_instru].second];
                if (port != -1)
                {
                    s.mp[senders_mac] = port;
                }
                ll destPort = 0;
                destPort = s.mp[destination_mac];

                if (destPort != 0)
                {

                    cout << "Device Type: " << type << "\n";
                    cout << "Global Index: " << s.ports << "\n";
                    cout << "Mac address: " << s.mac_addr << "\n";
                    if (isAck)
                        cout << "isACK \n";
                    cout << "Sending to: " << adjConnectionsList[present_instru][destPort - 1] << "\n\n";

                    if (collision_occurance_prob() == 1)
                    {
                        transmit_data(adjConnectionsList[present_instru][destPort - 1], vis, present_instru, senders_mac, destination_mac, isAck, isAckRecieved);
                    }
                    else
                    {
                        cout << "\nCollision happened while sending! \n";
                    }
                }
                else
                {
                    cout << "Switch Broadcasts \n";
                    for (ll i = 0; i < adjConnectionsList[present_instru].size(); i++)
                    {
                        if (!vis[adjConnectionsList[present_instru][i]])
                        {
                            cout << "Device Type : " << type << "\n";
                            cout << "Global Index : " << s.ports << "\n";
                            cout << "Mac address : " << s.mac_addr << "\n";
                            if (isAck)
                                cout << "isACK \n";
                            cout << "Sending to: " << adjConnectionsList[present_instru][i] << "\n\n";
                            if (collision_occurance_prob() == 1)
                            {
                                transmit_data(adjConnectionsList[present_instru][i], vis, present_instru, senders_mac, destination_mac, isAck, isAckRecieved);
                            }
                            else
                            {
                                cout << "\nCollision happened while sending! \n";
                            }
                        }
                    }
                }
            }

            if (type == "bridge")
            {
                Bridge &b = bridge_arr[Device_Identity[present_instru].second];
                if (port != -1)
                    b.mp[senders_mac] = port;
                ll destPort = 0;
                destPort = b.mp[destination_mac];
                if (destPort != 0)
                {
                    cout << "Device Type: " << type << "\n";
                    cout << "Global Index: " << b.ports << "\n";
                    cout << "Mac address: " << b.mac_addr << "\n";
                    if (isAck)
                        cout << "isACK \n";
                    cout << "Sending to: " << adjConnectionsList[present_instru][destPort - 1] << "\n\n";
                    if (collision_occurance_prob() == 1)
                    {
                        transmit_data(adjConnectionsList[present_instru][destPort - 1], vis, present_instru, senders_mac, destination_mac, isAck, isAckRecieved);
                    }
                    else
                    {
                        cout << "\nCollision happened while sending! \n";
                    }
                }
                else
                {
                    for (ll i = 0; i < adjConnectionsList[present_instru].size(); i++)
                    {
                        if (!vis[adjConnectionsList[present_instru][i]])
                        {
                            cout << "Device Type: " << type << "\n";
                            cout << "Global Index: " << b.ports << "\n";
                            cout << "Mac address: " << b.mac_addr << "\n";
                            if (isAck)
                                cout << "isACK \n";
                            cout << "Sending to: " << adjConnectionsList[present_instru][i] << "\n\n";
                            if (collision_occurance_prob() == 1)
                            {
                                transmit_data(adjConnectionsList[present_instru][i], vis, present_instru, senders_mac, destination_mac, isAck, isAckRecieved);
                            }
                            else
                            {
                                cout << "\nCollision happened while sending! \n";
                            }
                        }
                    }
                }
            }
            return;
        }

        Hub h;
        Device d;
        d = device_arr[Device_Identity[present_instru].second];
        if (type == "device" && d.mac_addr == destination_mac && !isAck)
        {
            cout << "Data packet recieved sucessfully sending back ACK! \n";
            vector<bool> vis(10001, false);
            cout << "\nSending ACK from" << destination_mac << "  to  " << senders_mac << "\n\n";
            transmit_data(present_instru, vis, -1, destination_mac, senders_mac, true, isAckRecieved);
            return;
        }
        else if (type == "device" && d.mac_addr == destination_mac)
        {
            isAckRecieved = true;
            cout << "ACK recieved sucessfully \n\n";
            return;
        }
        for (ll i = 0; i < adjConnectionsList[present_instru].size(); i++)
        {
            if (!vis[adjConnectionsList[present_instru][i]])
            {
                if (type == "hub")
                {
                    h = hub_arr[Device_Identity[present_instru].second];
                    cout << "Device Type: " << type << "\n";
                    cout << "Global Index: " << h.ports << "\n";
                    cout << "Mac address: " << h.mac_addr << "\n";
                    if (isAck)
                        cout << "isACK \n";
                    cout << "Sending to: " << adjConnectionsList[present_instru][i] << "\n\n";
                }
                if (type == "device")
                {
                    d = device_arr[Device_Identity[present_instru].second];
                    cout << "Device Type: " << type << "\n";
                    cout << "Global Index: " << d.index << "\n";
                    cout << "Mac address: " << d.mac_addr << "\n";
                    if (isAck)
                        cout << "isACK \n";
                    cout << "Sending to: " << adjConnectionsList[present_instru][i] << "\n\n";

                    if (d.mac_addr == destination_mac && !isAck)
                    {
                        cout << "Data packet recieved sucessfully sending back ACK";
                        vector<bool> vis(10001, false);
                        cout << "\nSENDING ACK FROM " << destination_mac << "  to  " << senders_mac << "\n\n";
                        if (collision_occurance_prob() == 1)
                        {
                            transmit_data(present_instru, vis, -1, destination_mac, senders_mac, true, isAckRecieved);
                        }
                        else
                        {
                            cout << "\nCollision happened while sending! \n";
                        }
                        return;
                    }
                    else if (d.mac_addr == destination_mac)
                    {
                        isAckRecieved = true;
                        cout << "ACK recieved sucessfully! \n\n";
                    }
                }
                if (collision_occurance_prob() == 1)
                {
                    transmit_data(adjConnectionsList[present_instru][i], vis, present_instru, senders_mac, destination_mac, isAck, isAckRecieved);
                }
                else
                {
                    cout << "\nCollision happened while sending! \n";
                }
            }
        }
    }
}

void initialization()
{
    ll n, m, ind;
    cout << "Enter the number of devices(nodes) :\n";
    cin >> n;
    cout << "Enter the number of connection between two devices(edges) :\n";
    cin >> m;
    for (ll i = 1; i <= n; i++)
    {
        string type;
        cout << "Enter the index and type of the device:\n";
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
        }
    }

    cout << "Enter " << m << " connections between two devices(edges):\n";
    for (ll i = 0; i < m; i++)
    {
        ll u, v;
        cin >> u >> v;
        addEdge(u, v);
    }
    vector<bool> vis(n + 1, false);
    DFS(1, vis);
}


void arp_request(ll current_device, vector<bool> & visited,string ip_dest,string &mac_dest) {
  if(!visited[current_device]) {
        visited[current_device] = true;
        string type = Device_Identity[current_device].first;
        if(type == "Host") {
          int h_index = Device_Identity[current_device].second;
          Host h = host_arr[h_index];
          if(h.ipv4 == ip_dest) {
            vector<bool> vis(visited.size(),false);
            cout<<"Host found !!!!!!";
            mac_dest = h.mac;
            return;
          }
        }


        if(type != "Router") {
          for(ll i = 0;i < adjConnectionsList[current_device].size(); i++) {
              if(!visited[adjConnectionsList[current_device][i]]) {
                arp_request(adjConnectionsList[current_device][i],visited,ip_dest,mac_dest);
              }
          }
        }


        if(type == "Router") {
          Router r = router_arr[Device_Identity[current_device].second];
          for(ll i = 0;i < r.IPtoMAC.size();i++) {
            if(r.IPtoMAC[i].first.first == ip_dest) {
              vector<bool> vis(visited.size(),false);
              cout<<"Gateway Found !!!!";
              mac_dest =  r.IPtoMAC[i].second;
              return;
            }
          }
        }
        return;
    }
}

string search_mac(ll current_device, string ip_Address) {
  unordered_map<string,string> arp_table = host_arr[Device_Identity[current_device].second].arp_table;
  string mac_dest = "";
  for(auto it = arp_table.begin(); it != arp_table.end(); it++) {
    if(it->first == ip_Address) {
      cout<<"found in arp table";
      return it->second;
    }
  }
  vector<bool> visited(1000,false);
  arp_request(current_device, visited ,ip_Address, mac_dest);
  return mac_dest;
}



void castNetwork()
{
    cout << "Enter 0 for reservation frame else 1: \n";
    ll res;
    cin >> res;

    if (res != 0)
    {
        ll q;
        cout << "Enter number of queries :\n";
        cin >> q;
        cout << "Enter sender, receiver and number of packets :\n";
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
                    cout << "\nTranferring packet from" << curr_send.mac_addr << "  to  " << curr_rec.mac_addr << "\n\n";
                    transmit_data(sender, vis, -1, curr_send.mac_addr, curr_rec.mac_addr, false, ackflag);
                    if (!ackflag)
                        cout << "\nAcknowledgment has not received! \n";
                }

                cout << "Switch Table contents " << endl;
                for (ll i = 0; i < switch_arr.size(); i++)
                {
                    Switch s = switch_arr[i];
                    cout << "Switch's global address: " << s.ports << "\n";
                    cout << "Switch's mac address: " << s.mac_addr << "\n";

                    for (auto it : s.mp)
                    {
                        cout << it.first << " " << it.second << "\n";
                    }
                    cout << "\n";
                }

                cout << "Bridge Table contents: " << endl;

                for (ll i = 0; i < bridge_arr.size(); i++)
                {
                    Bridge b = bridge_arr[i];
                    cout << "Bridge's global address : " << b.ports << "\n";
                    cout << "Bridge's mac address : " << b.mac_addr << "\n";

                    for (auto it : b.mp)
                    {
                        cout << it.first << " " << it.second << "\n";
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
        cout << "Enter number of queries :\n";
        cin >> q;

        cout << "Enter sender, receiver and number of packets :\n";
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
                        vector<bool> vis(1001, 0);
                        cout << "\nTransferring data from " << sender.mac_addr << "  to  " << reservation_frame[i].front().first << "\n\n";
                        transmit_data(sender.index, vis, -1, sender.mac_addr, reservation_frame[i].front().first, false, ackflag);
                        if (!ackflag)
                            cout << "\nAcknowledgment has not received! \n";
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

                        for (auto it : s.mp)
                        {
                            cout << it.first << " " << it.second << "\n";
                        }
                        cout << "\n";
                    }

                    cout << "Bridge Table contents " << endl;

                    for (ll i = 0; i < bridge_arr.size(); i++)
                    {
                        Bridge b = bridge_arr[i];
                        cout << "Bridge's global address : " << b.ports << "\n";
                        cout << "Bridge's mac address : " << b.mac_addr << "\n";

                        for (auto it : b.mp)
                        {
                            cout << it.first << " " << it.second << "\n";
                        }

                        cout << "\n";
                    }
                }
            }
            remaining = sent;
        }
    }
}

void setUpNetwork()
{
    ll n = 0;

    bool runner = true;

    while (runner)
    {
        ll category;
        cin >> category;
        if (category == 1)
        {
            cout << "Enter device and its global index : ";
            string device;
            ll index;
            cin >> device;
            cin >> index;
            n = max(n, index);
            cout << "\n";
            if (device == "Host")
            {
                Host h;
                h.global_index = index;
                h.mac = macaddrlist[mac_index];
                Device_Identity[index] = make_pair(device, host_index);
                mac_index++;
                cout << "IP Address : ";
                cin >> h.ipv4;
                cout << "\n";
                cout << "Subnet Mask : ";
                cin >> h.subnet;
                cout << "\n";
                host_arr.push_back(h);
                host_index++;
            }

            if (device == "Switch")
            {
                Switch s;
                s.ports = index;
                s.mac_addr = macaddrlist[mac_index];
                mac_index++;
                switch_arr.push_back(s);
                Device_Identity[index] = make_pair(device, switch_index);
                switch_index++;
            }

            if (device == "Hub")
            {
                Hub h;
                h.ports = index;
                h.mac_addr = macaddrlist[mac_index];
                mac_index++;
                Device_Identity[index] = make_pair(device, hub_index);
                hub_index++;
                hub_arr.push_back(h);
            }

            if (device == "Router")
            {
                cout << "Enter the maximum no of iterfaces need : ";
                ll no;
                cin >> no;
                cout << "\n";
                Router r;
                r.global_index = index;
                ll curr = 0;
                while (no > 0)
                {
                    cout << "ip for interface " << curr << " : ";
                    ll ip;
                    cin >> ip;
                    cout << "Subnet for interface " << curr << " : ";
                    ll subnet;
                    cin >> subnet;
                    cout << "\n";
                    r.IPtoMAC.push_back(make_pair(make_pair(ip, subnet), macaddrlist[mac_index]));
                    mac_index++;
                    curr++;
                    no--;
                }
                Device_Identity[index] = make_pair(device, router_index);
                router_arr.push_back(r);
                router_index++;
            }
        }

        if (category == 2)
        {
            cout << "Enter the connection (u,v) : ";
            cin >> (u);
            cin >> (v);
            cout << "\n";
            addEdge(u, v);
        }

        if (category == 3)
        {
            cout << "Enter source's global_index : ";
            ll ind;
            cin >> ind;
            cout << "Enter source's IP : ";
            string ips;
            cin >> ips;
            cout << "Enter source's subnet : ";
            string subnets;
            cin >> subnets;
            cout << "\n";

            cout << "Enter destination's IP : ";
            string ipd;
            cin >> ipd;
            cout << "Enter destination's subnet :";
            string subnetd;
            cin >> subnetd;

            if (find_nid(ips, subnets) == find_nid(ipd, subnetd))
            {
                cout << "Source and destination are in same subnet \n";
                string dest_mac = search_mac(ind, ipd);
                Host &sender = host_arr[Device_Identity[ind].s];
                sender.arp_table[ipd] = dest_mac;
                bool isAckRecieved = false;
                vector<bool> visited(10001, false);
                cout << "\nSENDING PACKET FROM " << sender.mac << "  to  " << dest_mac << "\n\n";
                transmit_data(ind, visited, -1, sender.mac, dest_mac, false, isAckRecieved);
                cout << "Is ack recieved : " << isAckRecieved << "\n";

                for (ll i = 0; i < switch_arr.size(); i++)
                {
                    Switch s = switch_arr[i];
                    cout << "Global Index " << s.ports << " \n ";
                    cout << "SWITCHING TABLE";
                    for (auto it = s.mp.begin(); it != s.mp.end(); it++)
                    {
                        cout << it->first << " " << it->second << "\n";
                    }
                    cout << "\n\n";
                }
            }
            else
            {
                cout << "Source and destination are in different subnet";
            }
        }

        if (category == 4)
        {
            runner = false;
        }
    }

    vector<bool> visited(n + 1, false);
    DFS(1, visited);
}

map<pair<ll, ll>, ll> edge_Weight;

void count_impact_zone(ll present_instru, ll prev_instru, vector<ll> &vis, ll &count)
{
    if (!vis[present_instru])
    {
        vis[present_instru] = true;

        if (prev_instru != -1)
        {
            edge_Weight[(make_pair(present_instru, prev_instru))] = count;
            edge_Weight[make_pair(prev_instru, present_instru)] = count;
        }

        for (ll i = 0; i < adjConnectionsList[present_instru].size(); i++)
        {
            if (!vis[adjConnectionsList[present_instru][i]])
            {

                if ((Device_Identity[present_instru].first == "switch" || Device_Identity[present_instru].first == "bridge") && Device_Identity[adjConnectionsList[present_instru][i]].first == "device")
                {
                    count++;
                    count_impact_zone(adjConnectionsList[present_instru][i], present_instru, vis, count);
                }

                if (Device_Identity[present_instru].first == "device" && (Device_Identity[adjConnectionsList[present_instru][i]].first == "switch" || Device_Identity[adjConnectionsList[present_instru][i]].first == "bridge"))
                {
                    count++;
                    count_impact_zone(adjConnectionsList[present_instru][i], present_instru, vis, count);
                }

                if ((Device_Identity[present_instru].first == "switch" || Device_Identity[present_instru].first == "bridge") && (Device_Identity[adjConnectionsList[present_instru][i]].first == "switch" || Device_Identity[adjConnectionsList[present_instru][i]].first == "bridge"))
                {
                    count++;
                    count_impact_zone(adjConnectionsList[present_instru][i], present_instru, vis, count);
                }

                if ((Device_Identity[present_instru].first == "switch" || Device_Identity[present_instru].first == "bridge") && Device_Identity[adjConnectionsList[present_instru][i]].first == "hub")
                {
                    count++;
                    count_impact_zone(adjConnectionsList[present_instru][i], present_instru, vis, count);
                }

                if (Device_Identity[present_instru].first == "hub" && (Device_Identity[adjConnectionsList[present_instru][i]].first == "switch" || Device_Identity[adjConnectionsList[present_instru][i]].first == "bridge"))
                {
                    count_impact_zone(adjConnectionsList[present_instru][i], present_instru, vis, count);
                }

                if (Device_Identity[present_instru].first == "hub" && Device_Identity[adjConnectionsList[present_instru][i]].first == "device")
                {
                    count_impact_zone(adjConnectionsList[present_instru][i], present_instru, vis, count);
                }

                if (Device_Identity[present_instru].first == "device" && Device_Identity[adjConnectionsList[present_instru][i]].first == "hub")
                {
                    count_impact_zone(adjConnectionsList[present_instru][i], present_instru, vis, count);
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
    cout << "Number of BroadCast Domains : " << 1 << " \n";
}

int main()
{
    settingMacAddress();
    initialization();
    castNetwork();
    impact_zone();
    setUpNetwork();
    return 0;
}

/*
 TEST CASE 1 :
 10 9
 1 device
 2 switch
 10 device
 3 device
 4 device
 5 device
 6 device
 7 device
 8 bridge
 9 switch
 1 2
 2 7
 2 10
 2 8
 8 9
 3 9
 4 9
 5 9
 6 9
 0
 4
 1 10 3
 10 1 1
 7 4 1
 5 7 2

 TEST CASE 2:
 20 19
 1 device
 2 device
 3 device
 4 switch
 5 switch
 6 hub
 7 device
 8 device
 9 device
 10 hub
 11 switch
 12 device
 13 device
 14 device
 15 hub
 16 switch
 17 hub
 18 device
 19 device
 20 device
 1 4
 3 4
 2 4
 4 5
 6 5
 6 7
 6 8
 6 9
 5 15
 15 16
 16 17
 17 18
 17 19
 17 20
 5 10
 10 11
 11 12
 11 13
 11 14
 0
 1
 12 1 4
*/
