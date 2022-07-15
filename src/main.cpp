#include<bits/stdc++.h>
using namespace std;
#define ll long long
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

class Router
{
    vector<vector<string>> routing_table;
    string ip_address;
    string mac_address;
    int num_interfaces;
    int router_id;
    int num_entries;           
    vector<string> interfaces; 
    static int rk ;

    Router(string mac, vector<string> input, int num_interfaces, int choice, int addn)
    {
        mac_address = mac;
        router_id = rk++;
        cout << "ROUTER CONFIGURATION";
        cout << " ENTER NUMBER OF INTERFACES OF THIS ROUTER : ";
        this->num_interfaces = num_interfaces;
        interfaces.resize(num_interfaces);

        num_entries = num_interfaces + addn;
        routing_table.resize(num_entries);

        for (int i = 0; i < num_interfaces; i++)
        {
            cout << "KINDLY ENTER THE I.P. ADDRESS OF INTERFACE - "<<i <<" AND THE N.I.D.: " << endl;
            string ip, nid;
            cin >> ip >> nid;
            interfaces[i] = ip;
            routing_table[i].resize(4);
            string subnet_mask = get_subnet_mask(ip);
            string nexthop = "NIL";
            routing_table[i][0] = subnet_mask;         
            routing_table[i][1] = nid;                
            routing_table[i][2] = nexthop;             
            routing_table[i][3] =to_string(i); 
        }
        if (choice == 0) 
        {
            static_routing(input);
        }
        else 
        {
            cout << "RIP TO BE CARRIED OUT LATER." << endl;
        }

        cout << "ROUTING-TABLE AS OF NOW:" << endl;
        show_ip_route(); 
    }

    void static_routing(vector<string> input) 
    {
        for (int i = num_interfaces; i < num_entries; i++)
        {
            string nid;
            cout<<"ENTER THE N.I.D.:"<<endl;
            cin>>nid;
            cout<<"ENTER THE SUBNET MASK : "<<endl;
            string subnet_mask;
            cin>>subnet_mask;

           cout<<"ENTER THE NEXT-HOP IP : ";
            string next_hop;
            cin>>next_hop;
            cout<<"ENTER THE INTERFACE ID : ";
            string interface_id;
            cin>>interface_id;
            routing_table[i][0] = subnet_mask;
            routing_table[i][1] = nid;
            routing_table[i][2] = next_hop;
            routing_table[i][3] = interface_id;
        }
    }

    void acceptRIPinfo(vector<vector<string>> neighbor_routing_table, Router *neighbor, string next_hop, string interface)
    {
        int k = 0;
        for (int i = 0; i < neighbor->num_entries; i++)
        {
            int found = 0;
            for (int j = 0; j < this->num_entries; j++)
            {
                if (this->routing_table[j][1]==neighbor->routing_table[i][1])
                {
                    found = 1;
                    break;
                }
            }
            if (found != 1)
            {
                cout << "RIP : UPDATING ROUTING TABLE OF ROUTER : " << this->router_id << endl;
                this->routing_table[this->num_entries + k][0] = neighbor->routing_table[i][0]; 
                this->routing_table[this->num_entries + k][1] = neighbor->routing_table[i][1]; 
                this->routing_table[this->num_entries + k][1] = next_hop;
                this->routing_table[this->num_entries + k][2] = interface;
                k++;
                cout << "UPDATED ROUTING TABLE : ";
                this->show_ip_route();
            }
        }
    }

    void rip(vector<Router> r) 
    {
        int isRouterNext = 0;
        for (int k = 0; k < r.size(); k++)
        {

            for (int j = 0; j < r[k].interfaces.size(); j++)
            {
                for (int i = 0; i < this->num_interfaces; i++) 
                {
                    if (routing_table[i][2] == r[k].interfaces[j])
                        {
                            isRouterNext = 1;
                            cout<<"ROUTER : "<< this->router_id <<" REPORT : ROUTING INFORMATION PROTOCOL : SENDING ROUTING TABLE TO ROUTER : "<< r[k].router_id;
                            r[k].acceptRIPinfo(this->routing_table, this, routing_table[i][2], routing_table[i][3]);
                            break;
                        }
                }
            }
        }
    }

    void show_ip_route() 
    {
        cout << "%20s\t\t|%20s\t\t|%20s\t\t|%20s\n SUBNET MASK NETWORK-ADDRESS NEXT HOP INTERFACE ID" << endl;
        for (int i = 0; i < num_entries; i++)
        {
            cout << "%20s\t\t|%20s\t\t|%20s\t\t|%20s\n "<<routing_table[i][0]<< routing_table[i][1]<< routing_table[i][2]<< routing_table[i][3] << endl;
        }
    }

    string returnBinaryIP(string ip) 
    {
        string binary_ip;
        istringstream iss(ip);
        vector<string> ip_part;
        string token;
        while (getline(iss, token, '.'))
        {
            if (!token.empty())
                ip_part.push_back(token);
        }
        for (int i = 0; i < 4; i++)
        {
            string str=(ip_part[i]);
            int strlen = str.length();
            string fr = "";
            if (strlen != 8)
            {
                int numzeros = 8 - strlen;
                int j = 0;
                while (j != numzeros)
                {
                    fr = fr + "0";
                    j++;
                }
                fr = fr + str;
                binary_ip = binary_ip + fr;
            }
            else
            {
                binary_ip = binary_ip + str;
            }
        }
        return binary_ip;
    }

    string returnBitwiseAnd(string s1, string s2)
    {
        string ip = s1;
        string ip2 = s2;
        cout << "IN BITWISE AND : ip= " << ip << " + AND ip2= " + << ip2;
        string and_ip = "";
        for (int i = 0; i < ip.length(); i++)
        {
            char ch1 = ip[i];
            char ch2 = ip2[i];
            int val1 = ch1 - '0';
            int val2 = ch2 - '0';
            and_ip = and_ip + to_string(val1 & val2);
        }
        return and_ip;
    }

    string get_subnet_mask(string ip)
    {
        string sentence = "@event.hello.dc1";
        istringstream iss(ip);
        vector<std::string> tokens;
        string token;
        while (getline(iss, token, '.'))
        {
            if (!token.empty())
                tokens.push_back(token);
        }
        if (stoi(tokens[0]) >= 0 && stoi(tokens[0]) <= 127)
        {
            return "255.0.0.0";
        }
        else if (stoi(tokens[0]) > 127 && stoi(tokens[0]) <= 191) 
        {
            return "255.255.0.0";
        }
        else if (stoi(tokens[0]) > 191 && stoi(tokens[0]) <= 223) 
        {
            return "255.255.255.0";
        }
        else
        {
            return "INVALID ADDRESS";
        }
    }

    int transfer_data(Device sender, Device receiver, string packet, vector<Device> earr, vector<Router> r, vector<Switch> s)
    {
        cout << "ROUTER : " + this->router_id + " RECEIVED THE PACKET : " + packet << endl;
        cout << "ROUTER : " + this->router_id + " REPORT : " + "LENGTH OF THE PACKET IS : " + packet.length() << endl;
        string packet_dest_nid = returnBitwiseAnd(packet.substr(217, 249), returnBinaryIP(get_subnet_mask(packet.substr(217, 249))));
        cout << "ROUTER : " + this->router_id + " REPORT : " + "ARRIVED PACKET'S DESTINATION N.I.D. IS : " + packet_dest_nid << endl;

        int forwardingInterface = -1;
        int matchedEntryIndex = -1;
        int isRouterNext = 0;
        int ackToReturn = -5;

        for (int i = 0; i < num_entries; i++)
        {
            if (packet_dest_nid == returnBinaryIP(routing_table[i][1]))
            {
                matchedEntryIndex = i;
                cout << "ROUTER : " + this->router_id + " REPORT : " + "MATCHED ENTRY INDEX = " + matchedEntryIndex << endl;
                forwardingInterface = stoi(routing_table[i][3]);
                cout << "ROUTER : " + this->router_id + " REPORT : " + "PACKET DESTINATION FOUND : FORWARDING PACKET TO INTERFACE : " + forwardingInterface << endl;
                break;
            }
        }

        for (int k = 0; k < r.size(); k++)
        {
            if (r[k] != this)
            {
                for (int j = 0; j < r[k].interfaces.size(); j++)
                {
                    string s1 = routing_table[matchedEntryIndex][2];
                    string s2 = r[k].interfaces[j];
                    if (routing_table[matchedEntryIndex][2] == r[k].interfaces[j])
                    {
                        isRouterNext = 1;
                        cout << "ROUTER : " + this->router_id << " REPORT : FORWARDING PACKET TO ROUTER : " << r[k].router_id << endl;
                        ackToReturn = r[k].transfer_data(sender, receiver, packet, earr, r, s);
                        cout << "ROUTER : " + this->router_id << " REPORT :  ACK : " << ackToReturn + " RECEIVED VIA ROUTER - " + r[k].router_id << endl;
                        break;
                    }
                }
            }
        }

        cout << "ROUTER : " + this->router_id + " REPORT : " + "1 : isRouterNext = " + isRouterNext << endl;
        if (isRouterNext != 1)
        {
            cout << "ROUTER : " + this->router_id + " REPORT : " + "2 : isRouterNext = " + isRouterNext << endl;
            for (int g = 0; g < s.size(); g++)
            {
                if (s[g].connectedRouterID == this->router_id && s[g].connectedRouterInterface == forwardingInterface)
                {
                    cout << "ROUTER : " + this->router_id + " REPORT : " + "FORWARDING PACKET TO SWITCH : " + s[g].switch_id << endl;
                    ackToReturn = s[g].transfer_data(sender, receiver, packet, earr, r, s); // COMPLETE THIS
                    cout << "ROUTER : " + this->router_id + " REPORT : " + " ACK : " + ackToReturn + " RECEIVED VIA SWITCH." << endl;
                }
            }
        }
        return ackToReturn;
    }
};
int Router::rk=1;