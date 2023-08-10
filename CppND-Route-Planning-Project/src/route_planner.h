#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"

class RoutePlanner
{
public: // constructor
  RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
  // Public variables and method declarations

  // methods
  float GetDistance() const noexcept { return distance;}

private:
  // Private variables and method declarations
  RouteModel &m_Model;
  RouteModel::Node *start_node, *end_node;
  float distance; 

  //methods
  
};

#endif