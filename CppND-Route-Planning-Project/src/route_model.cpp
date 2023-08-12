#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    // create the routemodel nodes
    int counter = 0;
    //  Here we are iterating through the nodes in the OSM data and creating new\
        nodes for each one, linking them to this routemodel object, and adding\
        them to m_Nodes and giving them an index like 0, 1, 2, 3, 4, 5, 6 etc.\
        These indices are not having any meaning in the OSM data, but they are\
        useful for us to be able to access the nodes in the m_Nodes vector.
    for (auto node : this->Nodes()) {
        RouteModel::Node NewNode {Node(counter, this, node)};
        counter++;
        m_Nodes.emplace_back(NewNode);
    }
    // create the node to road hashmap
    CreateNodeToRoadHashmap();
}

// RouteModel methods
void RouteModel::CreateNodeToRoadHashmap(){
    for(auto &road: Roads()){
        if(road.type != Model::Road::Type::Footway){
            for(auto node_idx: Ways()[road.way].nodes){
                if(node_to_road.find(node_idx) == node_to_road.end())
                    node_to_road[node_idx] = std::vector<const Model::Road *>();
                node_to_road[node_idx].push_back(&road);
            }
        }
    }
}

RouteModel::Node& RouteModel::FindClosestNode(float x, float y){
    Node tmp = Node();
    tmp.x = x; tmp.y = y;
    
    auto min_dist = std::numeric_limits<float>::max();
    float dist;
    int closest_idx;

    for(auto& road: Roads()){
        if(road.type != Model::Road::Type::Footway){
            for(auto node_idx : Ways()[road.way].nodes){
                dist = tmp.distance(SNodes()[node_idx]);
                if(dist < min_dist){
                    min_dist = dist;
                    closest_idx = node_idx;
                }
            }
        }
    }
    return SNodes()[closest_idx];
}


// Node methods
RouteModel::Node *RouteModel::Node::FindNeighbor(std::vector<int> node_indices) {
    Node *closest_node = nullptr;
    Node node;
    for (int node_index : node_indices){
        node = parent_model->SNodes()[node_index];
        if (this->distance(node) != 0 && !node.visited){
            if ( (closest_node == nullptr) || (this->distance(node) < this->distance(*closest_node)) )
                closest_node = &parent_model->SNodes()[node_index];
        }
    }
    return closest_node;
}

void RouteModel::Node::FindNeighbors(){
    for(auto &road: parent_model->node_to_road[this->index]){
        auto nearest = this->FindNeighbor(parent_model->Ways()[road->way].nodes);
        if(nearest != nullptr) 
            this->neighbors.emplace_back(nearest);
    }
}