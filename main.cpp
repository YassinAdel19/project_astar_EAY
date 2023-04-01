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
#include <lemon/dijkstra.h>
#include <deque>
#include <set>





using namespace lemon;
using namespace std;


// Get the bfs shotest path between two nodes
int nbNode(const ListDigraph &g, map<int, node_info> nodes, int id1, int id2){
    Bfs<ListDigraph> bfs(g);
    node_info s_info = nodes[id1];
    node_info t_info = nodes[id2];
    ListDigraph::Node s = s_info.getNode();
    ListDigraph::Node t = t_info.getNode();
    bfs.run(s);
    return bfs.dist(t);
}

double dijkstraLength(const ListDigraph &g,const ListDigraph::ArcMap<double> &length, map<int, node_info> nodes, int id1, int id2){
    ListDigraph::NodeMap<double> dist(g);
    node_info s_info = nodes[id1];
    node_info t_info = nodes[id2];
    ListDigraph::Node s = s_info.getNode();
    ListDigraph::Node t = t_info.getNode();   
    dijkstra(g, length).distMap(dist).run(s);
    return dist[t];
}




//astar

//Sortir class, faire une boucle qui parcourt les get estimate
//att creation vnext, getid1 ou get id2



//Verifie si id appartient déjà à v
template <typename T>
bool isIn(T &v, int id){
    for(auto c : v){
        if(c->get_id()==id)
            return 1;
    }
    return 0;
}




double heuristic_distance_estimator(map<int, node_info> nodes, plan &p, int id1, int id2){
    double x1, x2, y1, y2;
    x1 = p.getX(nodes, id1);
    x2 = p.getX(nodes, id2);
    y1 = p.getY(nodes, id1);
    y2 = p.getY(nodes, id2);
    return sqrt(((x1 - x2)*(x1 - x2)) + ((y1 - y2)*(y1-y2)));
}


//template<typename T>
void partial_sort_on_estimate(deque<node_astar*> &q){
    
    //std::sort(q.begin(), q.end());

    deque<node_astar*>::iterator min, jt;

    for( deque<node_astar*>::iterator it = q.begin(); it < q.end()-1; ++it )
    {
        min = it;
        for( jt = it+1; jt < q.end(); ++jt )
        {
            if( (*jt)->get_estimate() <(*min)->get_estimate() )
            {
                min = jt;
            }
        }
        if( min != it )
        {
            std::swap( it,min );
        }
    }
}


node_astar* astar(const ListDigraph &g, map<int, node_info> &nodes,map<ListDigraph::Arc, arc_info> &arcs, plan &p, int start, int end) {
    
    //double weightClos = 0;
    

    

    node_astar *vstart = new node_astar(start);
    node_astar *vend = new node_astar(end);

    deque<node_astar*> active_queue;
    set<node_astar*> closed_set;
    
    //set_all_vertex_weight_to_max_value();
    // ID of the start vertex
    
    active_queue.push_back(vstart);

    do {
        
        auto vcurrent = active_queue.front();   //On récupère le premier élement de la queue
        active_queue.pop_front();
        if (vcurrent->get_id() == vend->get_id()) break;                //On se situe à la fin 
        closed_set.insert(vcurrent);                   //Point faisant parti du chemin le plus court
        //double estimate = 0;
        double d = 0;
        double f = 0;
        //double weight=0;

        for(ListDigraph::OutArcIt arc(g,nodes[vcurrent->get_id()].getNode()); arc!= INVALID; ++arc) {                        //adjacency_list of vcurrent
            
            
            //On recup les infos de la node et de l'arc
            ListDigraph::Node nvnext = g.target(arc);                                              //On récupère le node à l'indice Arc
            arc_info ar = arcs[findArc(g,nodes[vcurrent->get_id()].getNode(), nvnext)];
            node_astar *vnext= new node_astar(ar.getid2());                                                                //On récupère l'id du node

            if (isIn(closed_set, vnext->get_id())) {       //losed_set.count(vcurrent)==0
                continue;
            }


            d = vcurrent->get_weight() + ar.getDistance(); 
            f = d + heuristic_distance_estimator(nodes,p,ar.getid2(), vend->get_id());

            if (isIn(active_queue,vnext->get_id())) {                     //Première fois que le point apparait, find(active_queue.begin(),active_queue.end(),vnext) == active_queue.end()
                vnext->set_weight(d);
                vnext->set_estimate(f);
                active_queue.push_back(vnext);

            } else if (f < vnext->get_estimate()){
                vnext->set_weight(d);
                vnext->set_estimate(f);
            }
        }
        partial_sort_on_estimate(active_queue);
        cout << "Ok" << endl;
    }while(active_queue.size() != 0);

    return active_queue.front();
}




int 
main(int argc, char **argv) {

  ListDigraph g;
  ListDigraph::ArcMap<double> length(g);
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
            length[arc] = distance;
        }
    }
    
    file.close(); // close the input file  

    cout<< "Latitude max = "<< lat_max<< " latitude min = "<<lat_min<<endl;
    cout<< "Longitude max = "<< lon_max<< " longitude min = "<<lon_min<<endl;    

    plan p(lon_min,lon_max,lat_min,lat_max);

    cout << "The middle point of the map is:  "<<endl;
    cout<< "LAT: "<< p.getMiddle_lat()<< endl << "LON: "<< p.getMiddle_lon() << endl;
    


    cout << "The number of nodes between the nodes 19791 and 50179 is "<<nbNode(g,nodes,19791, 50179)<< " nodes" << endl;
    cout << "The number of nodes between the nodes 73964 and 272851 is " <<nbNode(g,nodes,73964, 272851)<< " nodes" << endl;
    
    cout << "dist[t] = "<< dijkstraLength(g, length,nodes, 73964, 272851)<<endl;


    // Calculate weight of the shortest path between two Nodes  
    /*double length;
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
*/
    auto e =astar(g,nodes,arcs,p,73964,272851)->get_estimate();
    cout << e << endl;
}