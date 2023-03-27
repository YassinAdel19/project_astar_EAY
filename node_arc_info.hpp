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


class plan{
private :
    double lon_min, lon_max, lat_min, lat_max;  //lon = x, lat = y
    double middle_lon, middle_lat;
public:
    plan(double lo1, double lo2, double la1, double la2) : 
        lon_min(lo1), lon_max(lo2), lat_min(la1), lat_max(la2) {
            middle_lon=(lon_max + lon_min)/2;
            middle_lat=(lat_max + lat_min)/2;
        }
    
    double getX(map<int, node_info> nodes, int id){
        double R0 = 6378137.0; //earth radius  
        node_info info = nodes[id];
        return R0*cos(middle_lat)*(info.getLon()-middle_lon);
    }

    double getY(map<int, node_info> nodes, int id){
        double R0 = 6378137.0; //earth radius  
        node_info info = nodes[id];
        return R0*log2(tan(((info.getLat()-middle_lat)/2)*(M_PI_4)));
    }

    double getMiddle_lon(){
        return middle_lon;
    }

    double getMiddle_lat(){
        return middle_lat;
    }

};