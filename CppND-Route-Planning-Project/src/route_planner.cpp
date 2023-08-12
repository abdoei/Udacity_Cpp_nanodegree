#include "route_planner.h"
#include <algorithm>
#include <vector>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    start_x *= 0.01;
    start_y *= 0.01;
    end_x   *= 0.01;
    end_y   *= 0.01;
    
    start_node= & m_Model.FindClosestNode(start_x, start_y);
    end_node  = & m_Model.FindClosestNode(end_x, end_y);
    
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node){
    std::vector<RouteModel::Node> path_found;
    distance = 0.0f;
    while(current_node->parent != nullptr){
        path_found.push_back(*current_node);
        distance += current_node->distance(*current_node->parent);
        current_node = current_node->parent;
    }
    path_found.push_back(*current_node);
    distance *= m_Model.MetricScale();
    return path_found;
}

// A* search function stub 
void RoutePlanner::AStarSearch(){
    start_node->visited = true;
    open_list.emplace_back(start_node);
    RouteModel::Node* current_node = nullptr;
    while(open_list.size() > 0){
        current_node = NextNode();
        if(current_node->distance(*end_node) == 0){
            m_Model.path = ConstructFinalPath(current_node);
            return;
        }
        AddNeighbors(current_node);
    }
}

float RoutePlanner::CalculateHValue(RouteModel::Node* nodePtr){
    return nodePtr->distance(*end_node);
}

RouteModel::Node* RoutePlanner::NextNode(){
    std::sort(open_list.begin(), open_list.end(), [](const RouteModel::Node* a, const RouteModel::Node* b){
        return (a->h_value + a->g_value) < (b->h_value + b->g_value);
    });
    auto lowest_node = *open_list.begin();
    open_list.erase(open_list.begin());
    return lowest_node;
}

void RoutePlanner::AddNeighbors(RouteModel::Node * current_node){
    current_node->FindNeighbors();
    auto neighbours = current_node->neighbors;
    for(auto neighbour: neighbours){
        // this line was fialing the test of the A* search and the distance and the numbere of nodes was worng and the A* search was not optimal due to this error in the heuristics function.
        // neighbour->g_value += current_node->distance(*neighbour);
        neighbour->g_value = current_node->g_value + current_node->distance(*neighbour);
        neighbour->h_value = CalculateHValue(neighbour);
        neighbour->parent = current_node;
        open_list.emplace_back(neighbour);
        neighbour->visited = true;
    }
}