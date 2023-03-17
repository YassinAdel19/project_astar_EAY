#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <tuple>
#include <vector>
#include <map>
#include <lemon/list_graph.h>
#include <lemon/bfs.h>
#include <lemon/path.h>

using namespace lemon;
using namespace std;

class node_info{
    public:
        double lat, lon; // Latitude and longitude 
        ListDigraph::Node node;
    
        //Default constructor
        node_info() : lat(0.0), lon(0.0) {}
        // Constructor
        node_info(const ListDigraph::Node n,const double l=0.0, const double L=0.0): node(n),lat(l), lon(L){}
        // Copie constructor
        node_info(const node_info& other) : lat(other.lat), lon(other.lon), node(other.node) {}

        double getLat() const{ // return latitude
            return this->lat;
        }

        double getLon() const{ // return longitude
            return this->lon;
        }

        ListDigraph::Node getNode() const{
            return this->node;
        }
};

class arc_info{
    public:
        int id1, id2;
        double distance;
        ListDigraph::Arc arc;
        string street;
    
        //Default constructor
        arc_info(): id1(0), id2(0), distance(0.0), street("???"){}
        // Constructor
        arc_info(const ListDigraph::Arc a , const int ID1=0, const int ID2=0, const double D=0.0, const string st="???"): arc(a), id1(ID1), id2(ID2), distance(D), street(st){}
        // Copie constructor
        arc_info(const arc_info& other) :id1(other.id1), id2(other.id2), distance(other.distance), street(other.street), arc(other.arc){}

        int getid1() const{ // return latitude
            return this->id1;
        }

        int getid2() const{ // return longitude
            return this->id2;
        }

        double getDistance() const {
            return this->distance;
        }

        ListDigraph::Arc getArc() const{
            return this->arc;
        }

        string getStreet() const{
            return this->street;
        }

};

int 
main(int argc, char **argv) {

  ListDigraph g;
  Bfs<ListDigraph> bfs(g);
  map<int, node_info> nodes;
  map<ListDigraph::Arc, arc_info> arcs;


  ifstream file("graph_dc_area.2022-03-11.txt");
  string line;

    while (getline(file, line)) { // read each line from the input file
        if (line[0] == '#') { // if the line starts with #, ignore it
            continue;
        }
        else if (line[0] == 'V') { // if the line starts with V, store the first three values in nodes vector
            int ID;
            double LAT, LON;
            // Reads the CVS file
            sscanf(line.c_str(), "V,%d,%lf,%lf", &ID, &LAT, &LON); // parse the line
            ListDigraph::Node node = g.addNode();  // adds a new node
            node_info info = node_info(node, LAT, LON);
            nodes[ID] = info;
            
        }
        else if (line[0] == 'E') { // if the line starts with E, store the values in arc vector
            int id1, id2;
            double distance;
            //string street;
            // Reads the CVS file
            sscanf(line.c_str(), "E,%d,%d,%lf", &id1, &id2, &distance); // parse the line
            //string token;
            //To read the streat name
            /*for (int i = 0; i < 5; i++) {
                getline(file, token, ',');
                
            }*/
            //street = token;
            node_info node1 = nodes[id1];
            node_info node2 = nodes[id2];
            ListDigraph::Arc arc = g.addArc(node1.getNode(), node2.getNode());
            arc_info arc1 = arc_info(arc, id1, id2, distance);
            //cout<< " id1= " << arc1.getid1()<< " id2= " << arc1.getid2() << " Distance = "<< arc1.getDistance()<<" Street: "<<endl;
            arcs[arc] = arc1;
        }
    }
    
    file.close(); // close the input file  

  



    node_info node1 = nodes[193];
    node_info node2 = nodes[9999];
    cout<<"Lat= " <<node2.getLat() << "    Lon=" << node2.getLon() <<endl;

    node_info s_info = nodes[19791];
    node_info t_info = nodes[50179];  
    ListDigraph::Node s = s_info.getNode();
    ListDigraph::Node t = t_info.getNode();
    bfs.run(s);
    int dist = bfs.dist(t);
    cout << "Distance between 19791 and 50179 is "<<dist<< " arcs" << endl;
    //PredMapPath< ListDigraph,lemon::Bfs<>::PredMap > path = bfs.path(t);
    //cout<< "lenght of path is"<<path.length() <<endl;
    
    double length;
    ListDigraph::Arc fev=bfs.path(t).front();
    for(int i=0; i<bfs.path(t).length(); i++){
        arc_info arcDist_info = arcs[bfs.path(t).front()];
        length+= arcDist_info.getDistance();
    }
    cout<< "Total distance is "<< length<< endl;
    
    

    node_info p_info = nodes[73964];
    node_info z_info = nodes[272851];  
    ListDigraph::Node p = p_info.getNode();
    ListDigraph::Node z = z_info.getNode();
    bfs.run(p);
    int dist2 = bfs.dist(z);
    cout << "Distance between 73964 and 272851 is "<<dist2<< " arcs" << endl;


}

/*
    ListDigraph::Node x = g.addNode();
    ListDigraph::Node y = g.addNode();
    ListDigraph::Node z = g.addNode();
    ListDigraph::Node l = g.addNode();
    ListDigraph::Node m = g.addNode();
    g.addArc(x,y);
    g.addArc(y,z);
    g.addArc(z,x);
    g.addArc(y,l);
    g.addArc(l,m);
*/

  
