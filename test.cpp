#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <tuple>
#include <vector>
#include <map>
#include <cmath>
#include "node_arc_info.hpp"
#include <lemon/list_graph.h>
#include <lemon/bfs.h>
#include <lemon/path.h>


using namespace lemon;
using namespace std;

double getX(map<int, node_info> nodes, int id, double middle_lat, double middle_lon){
    double R0 = 6378137.0; //earth radius  
    node_info info = nodes[id];
    return R0*cos(middle_lat)*(info.getLon()-middle_lon);
}

double getY(map<int, node_info> nodes, int id, double middle_lat, double middle_lon){
    double R0 = 6378137.0; //earth radius  
    node_info info = nodes[id];
    return R0*log2(tan(((info.getLat()-middle_lat)/2)*(M_PI_4)));
}

double

int 
main(int argc, char **argv) {

  ListDigraph g;
  Bfs<ListDigraph> bfs(g);
  map<int, node_info> nodes;
  map<ListDigraph::Arc, arc_info> arcs;
  double lon_max = -100;
  double lon_min = 0;
  double lat_max = 0;
  double lat_min = 100;
  

  // Read the CVS file
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
            sscanf(line.c_str(), "V,%d,%lf,%lf", &ID, &LON, &LAT); // parse the line
            ListDigraph::Node node = g.addNode();  // adds a new node
            node_info info = node_info(node, LAT, LON);
            nodes[ID] = info;
            cout<< LON << "   "<< LAT<<endl;
            if (LAT>lat_max){
                lat_max = LAT;
            }
            if(LAT<lat_min){
                lat_min = LAT;
            }
            if(LON>lon_max){
                lon_max = LON;
            }
            if(LON<lon_min){
                lon_min = LON;
            }
            
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

    cout<< "Latitude max = "<< lat_max<< " latitude min = "<<lat_min<<endl;
    cout<< "Longitude max = "<< lon_max<< " longitude min = "<<lon_min<<endl;    

    double middle_lat = (lat_max + lat_min)/2;
    double middle_lon = (lon_max + lon_min)/2;

    cout << "The middle point of the map is:  "<<endl;
    cout<< "LAT: "<< middle_lat<< endl << "LON: "<< middle_lon << endl;



    // Get the bfs shotest path between two nodes
    node_info s_info = nodes[19791];
    node_info t_info = nodes[50179];  
    ListDigraph::Node s = s_info.getNode();
    ListDigraph::Node t = t_info.getNode();
    bfs.run(s);
    int dist = bfs.dist(t);
    cout << "The number of nodes between the nodes 19791 and 50179 is "<<dist<< " nodes" << endl;
    

    // Calculate weight of the shortest path between two Nodes  
    double length;
    int id2_length;
    ListDigraph::Arc arc_new = bfs.predArc(t); 
    for(int i=0; i<bfs.path(t).length(); i++){
        id2_length = arcs[arc_new].getid1(); 
        arc_info arcDist_info = arcs[bfs.predArc(nodes[id2_length].getNode())];
        arc_new = bfs.predArc(nodes[id2_length].getNode());
        //cout<< arcDist_info.getDistance()<<endl;
        length+= arcDist_info.getDistance();
    }
    cout<< "The weight of the shortest path between 19791 and 50179 is "<< length<< endl;   


    // Get the bfs shotest path between two nodes
    node_info p_info = nodes[73964];
    node_info z_info = nodes[272851];  
    ListDigraph::Node p = p_info.getNode();
    ListDigraph::Node z = z_info.getNode();
    bfs.run(p);
    int dist2 = bfs.dist(z);
    cout << "The number of nodes between the nodes 73964 and 272851 is " <<dist2<< " nodes" << endl;

}



  
