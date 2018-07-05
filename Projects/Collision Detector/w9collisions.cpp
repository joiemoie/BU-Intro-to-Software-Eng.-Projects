// Copyright 2017 Joseph Liba joiemoie@bu.edu

#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
#include <unordered_set>

using std::cout;
using std::endl;
using std::string;
using std::vector;

struct vehicle {
  double startX, startY, velX, velY;
  string name;
  vehicle& operator=(const vehicle& a) {
    startX = a.startX;
    startY = a.startY;
    velX = a.velX;
    velY = a.velY;
    name = a.name;
    return *this;
  }
};

struct collision {
  string veh1;
  string veh2;
  double time;
  collision& operator=(const collision& a) {
    veh1 = a.veh1;
    veh2 = a.veh2;
    time = a.time;
    return *this;
  }
  bool operator<(collision& a) {
    return time < a.time;
  }
};

vector<collision> ValidCollisions(vector<collision>& collisions, vector<vehicle>& vehicles) {
  vector<collision> validCollisions;
  std::unordered_set<string> usedVehicles;
  vector<vehicle> unusedVehicles;
  for (collision& col : collisions) {
    bool isValid = true;
    if (!(usedVehicles.find(col.veh1) == usedVehicles.end())) isValid = false;
    if (!(usedVehicles.find(col.veh2) == usedVehicles.end())) isValid = false;
    if (isValid) {
      validCollisions.push_back(col);
      usedVehicles.insert(col.veh1);
      usedVehicles.insert(col.veh2);
    }
  }
  for (vehicle& veh : vehicles) {
    if (usedVehicles.find(veh.name) == usedVehicles.end())
      unusedVehicles.push_back(veh);
  }
  vehicles = unusedVehicles;
  return validCollisions;
}

int main() {
  string obj;
  double px;
  double py;
  double vx;
  double vy;
  vector< vehicle > vehicles;
  vector <collision> collisions;
  collision newCollision;
  vehicle newVehicle;
  int numVehicles = 0;
  while (std::cin>> obj >> px >> py >> vx >> vy) {
    newVehicle.name = obj;
    newVehicle.velX = vx;
    newVehicle.startX = px;
    newVehicle.velY = vy;
    newVehicle.startY = py;
    vehicles.push_back(newVehicle);
  }
  for (int i = 0; i < vehicles.size(); i++) {
    numVehicles++;
    double time;
    double a1 = vehicles[i].velX, b1 = vehicles[i].startX,
           c1 = vehicles[i].velY, d1 = vehicles[i].startY;
    for (int j = i+1; j < vehicles.size(); j++) {
      vehicle& veh2 = vehicles[j];
      double da = a1-veh2.velX, db = b1 - veh2.startX,
      dc = c1 - veh2.velY, dd = d1 - veh2.startY;
      double a = da*da + dc*dc;
      double b = 2*(da*db+dc*dd);
      double c = db*db+dd*dd-100;
      double root = b*b-4*a*c;
      if (root >= 0) {
        time = (-b - sqrt(root))/(2*a);
        if (time > 0) {
          newCollision.veh1 = vehicles[i].name;
          newCollision.veh2 = vehicles[j].name;
          newCollision.time = time;
          collisions.push_back(newCollision);
        }
      }
    }
  }
  std::sort(collisions.begin(), collisions.end());
  vector<collision> validCollisions = ValidCollisions(collisions, vehicles);
  std::cout << "there are " << numVehicles << " vehicles" << '\n';
  std::cout << "collision report" << '\n';
  if (validCollisions.size() == 0) { std::cout << "none" << '\n';
  } else {
    for (collision col : validCollisions) {
      std::cout << "at " << col.time << " " << col.veh1;
      std::cout << " collided with " << col.veh2 << '\n';
    }
  }
  std::cout << "the remaining vehicles are" << '\n';
  if (vehicles.size() == 0) { std::cout << "none" << '\n';
  } else {
    for (vehicle veh : vehicles) {
      std::cout << veh.name << " " << veh.startX << " " << veh.startY;
      std::cout << " " << veh.velX << " " << veh.velY << '\n';
    }
  }
  return 0;
}
