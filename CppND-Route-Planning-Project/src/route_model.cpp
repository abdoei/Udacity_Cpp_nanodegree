#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int counter = 0;
    for (auto &node : this->Nodes()) {
        RouteModel::Node NewNode {Node(counter, this, node)};
        counter++;
        m_Nodes.emplace_back(NewNode);
    }
    
}