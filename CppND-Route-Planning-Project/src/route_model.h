#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

class RouteModel : public Model {

  public:
    class Node : public Model::Node {
      public:
        // Add public Node variables and methods here.
        Node * parent = nullptr;
        float h_value = std::numeric_limits<float>::max();
        float g_value = 0.0;
        bool visited = false;
        std::vector<Node *> neighbors;
        // methods 
        float distance(Node other){return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));}
        void FindNeighbors();
        Node(){}
        Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}
      
      private:
        // Private Node variables and methods
        int index;
        RouteModel* parent_model = nullptr;
        // methods
        RouteModel::Node* FindNeighbor(std::vector<int> node_idx);
    };

    // Public RouteModel variables and methods
    RouteModel(const std::vector<std::byte> &xml);  
    std::vector<Node> path; // This variable will eventually store the path that is found by the A* search.
    // methods
    auto GetNodeToRoadMap() -> std::unordered_map<int, std::vector<const Model::Road*>> & {return node_to_road;}
    std::vector<Node> &SNodes(){ return m_Nodes; }
    RouteModel::Node& FindClosestNode(float x, float y);

  private:
    // Private RouteModel variables and methods
    std::vector<Node> m_Nodes;
    std::unordered_map<int, std::vector<const Model::Road *>> node_to_road;
    //methods
    void CreateNodeToRoadHashmap();
};

#endif