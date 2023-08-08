#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    // create the routemodel nodes
    int counter = 0;
    for (auto &node : this->Nodes()) {
        RouteModel::Node NewNode {Node(counter, this, node)};
        counter++;
        m_Nodes.emplace_back(NewNode);
    }
    // create the node to road hashmap
    CreateNodeToRoadHashmap();
}

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