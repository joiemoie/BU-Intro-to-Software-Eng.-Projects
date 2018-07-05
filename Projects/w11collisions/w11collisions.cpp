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

// Defines lower left and upper right coordinates
struct boundaries {
  double LLx, LLy, URx, URy;
};

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

// Adds a collision between two vehicles
void AddCollision(vehicle& veh1, vehicle& veh2,
                  vector<collision>& collisions, double currentTime) {
  collision newCollision;
  double time;
  double da = veh1.velX - veh2.velX, db = veh1.startX - veh2.startX,
         dc = veh1.velY - veh2.velY, dd = veh1.startY - veh2.startY;
  double a = da*da + dc*dc;
  double b = 2*(da*db+dc*dd);
  double c = db*db+dd*dd-100;
  double root = b*b-4*a*c;
  if (root >= 0) {
    time = (-b - sqrt(root))/(2*a);
    if (time > 0) {
      time += currentTime;
      newCollision.veh1 = veh1.name;
      newCollision.veh2 = veh2.name;
      newCollision.time = time;
      collisions.push_back(newCollision);
    }
  }
}

// Adds a collision between a vehicle and a wall
void AddCollisionWall(vehicle& veh, vector<collision>& collisions,
                      boundaries& bound, double currentTime) {
  double time = (bound.LLx + 5 - veh.startX) / veh.velX;
  collision newCollision;
  if (time * veh.velY + veh.startY > bound.LLy + 5 &&
      time * veh.velY + veh.startY < bound.URy - 5) {
    if (time > 0) {
      time += currentTime;
      newCollision.veh1 = veh.name;
      newCollision.veh2 = "Lwall";
      newCollision.time = time;
      collisions.push_back(newCollision);
    }
  }
  time = (bound.URx - 5 - veh.startX) / veh.velX;
  if (time * veh.velY + veh.startY > bound.LLy + 5 &&
      time * veh.velY + veh.startY < bound.URy - 5) {
    if (time > 0) {
      time += currentTime;
      newCollision.veh1 = veh.name;
      newCollision.veh2 = "Rwall";
      newCollision.time = time;
      collisions.push_back(newCollision);
    }
  }
  time = (bound.URy - 5 - veh.startY) / veh.velY;
  if (time * veh.velX + veh.startX > bound.LLx + 5 &&
      time * veh.velX + veh.startX < bound.URx - 5) {
    if (time > 0) {
      time += currentTime;
      newCollision.veh1 = veh.name;
      newCollision.veh2 = "Uwall";
      newCollision.time = time;
      collisions.push_back(newCollision);
    }
  }
  time = (bound.LLy + 5 - veh.startY) / veh.velY;
  if (time * veh.velX + veh.startX > bound.LLx + 5 &&
      time * veh.velX + veh.startX < bound.URx - 5) {
    if (time > 0) {
      time += currentTime;
      newCollision.veh1 = veh.name;
      newCollision.veh2 = "Dwall";
      newCollision.time = time;
      collisions.push_back(newCollision);
    }
  }
}

// Updates the velocities of vehicles that collide
void UpdateBothVehicles(vehicle& veh1, vehicle& veh2) {
  double velx1 = veh1.velX;
  double vely1 = veh1.velY;
  double velx2 = veh2.velX;
  double vely2 = veh2.velY;
  double x1 = veh1.startX;
  double y1 = veh1.startY;
  double x2 = veh2.startX;
  double y2 = veh2.startY;
  double magnitude = pow(x1-x2, 2)+pow(y1-y2, 2);
  double dotProduct = (velx1-velx2)*(x2-x1)
                      + (vely1-vely2)*(y2-y1);
  double collisionVelX = (x2-x1)*dotProduct/magnitude;
  double collisionVelY = (y2-y1)*dotProduct/magnitude;
  veh1.velX -= collisionVelX;
  veh1.velY -= collisionVelY;
  veh2.velX += collisionVelX;
  veh2.velY += collisionVelY;
}

// Updates the velocity of a vehicle that collides with a wall
void UpdateVehicle(vehicle& veh1, const string& wall) {
  if (wall == "Lwall" || wall == "Rwall")
    veh1.velX *= -1;
  else
    veh1.velY *= -1;
}

int main() {
  // Reads in the interval length and total runtime
  double interval;
  double endTime;
  std::cin >> interval >> endTime;
  std::cout << interval << " " << endTime << '\n';

  // Reads in the Bounds
  double LLx;
  double LLy;
  double URx;
  double URy;
  boundaries bound;
  std::cin >> LLx >> LLy >> URx >> URy;
  std::cout << LLx << " " << LLy << " " << URx << " " << URy << '\n';
  bound.LLx = LLx, bound.LLy = LLy, bound.URx = URx, bound.URy = URy;

  // Reads in the vehicles
  string obj;
  double px;
  double py;
  double vx;
  double vy;
  vector< vehicle > vehicles;
  vehicle newVehicle;
  while (std::cin>> obj >> px >> py >> vx >> vy) {
    newVehicle.name = obj;
    newVehicle.velX = vx;
    newVehicle.startX = px;
    newVehicle.velY = vy;
    newVehicle.startY = py;
    vehicles.push_back(newVehicle);
  }

  // The current time of the simulation
  double currentTime = 0;
  // The current interval number
  int intervalNum = 0;
  
  // Collides all the vehicles
  vector <collision> collisions;
  collision newCollision;
  for (int i = 0; i < vehicles.size(); i++) {
    for (int j = i+1; j < vehicles.size(); j++) {
      AddCollision(vehicles[i], vehicles[j], collisions, currentTime);
    }
    AddCollisionWall(vehicles[i], collisions, bound, currentTime);
  }
  
  // The time of the earliest collision
  double minColTime = 0;
  // The vehicles in the earliest collision
  string veh1, veh2;
  // The velocities of the vehicles before they collided
  double oldVel1X, oldVel1Y, oldVel2X, oldVel2Y;

  // Loops through the collisions one at a time going to the next earliest
  // until the end of the simultion
  while (interval*intervalNum <= endTime) {
    while (currentTime < interval*intervalNum) {
      // Finds the earliest collision
      collision * minCol = &collisions[0];
      for (int i = 0; i < collisions.size(); i++) {
        if (collisions[i] < (*minCol)) minCol = &(collisions[i]);
      }
      minColTime = (*minCol).time;
      veh1 = (*minCol).veh1;
      veh2 = (*minCol).veh2;

      // The addresses of the vehicles in the earliest collision
      vehicle* vehicle1 = 0;
      vehicle* vehicle2 = 0;
      for (vehicle& veh : vehicles) {
        if (veh.name == veh1) vehicle1 = &veh;
        if (veh.name == veh2) vehicle2 = &veh;
      }

      // Updates all the vehicles to the current time
      for (vehicle& veh : vehicles) {
        veh.startX = veh.startX+veh.velX*(minColTime-currentTime);
        veh.startY = veh.startY+veh.velY*(minColTime-currentTime);
      }

      // Updates the velocities of the collided vehicles and
      // remembers the old velocities
      oldVel1X = (*vehicle1).velX;
      oldVel1Y = (*vehicle1).velY;
      if (vehicle2 != NULL) {
        oldVel2X = (*vehicle2).velX;
        oldVel2Y = (*vehicle2).velY;
        UpdateBothVehicles((*vehicle1), (*vehicle2));
      } else {
        UpdateVehicle((*vehicle1), veh2);
      }

      // Updates the current time
      currentTime = (*minCol).time;

      // Copies over all the collisions that did not include
      // those two vehicles that collided
      vector<collision> newCollisions;
      for (collision& col : collisions) {
        if (col.veh1 != veh1 && col.veh2 != veh1) {
          if ((col.veh1 != veh2 && col.veh2 != veh2) ||
              veh2.substr(1) == "wall") {
            newCollisions.push_back(col);
          }
        }
      }
      collisions = newCollisions;

      // Recalculates the collisions for the vehicles that collided
      for (int i = 0; i < vehicles.size(); i++) {
        if (vehicles[i].name == veh1 || vehicles[i].name == veh2) {
          for (int j = 0; j < vehicles.size(); j++) {
            if (!(vehicles[i].name == veh1 && vehicles[j].name == veh2) &&
                !(vehicles[i].name == veh2 && vehicles[j].name == veh1))
              AddCollision(vehicles[i], vehicles[j], collisions, currentTime);
          }
          AddCollisionWall(vehicles[i], collisions, bound, currentTime);
        }
      }
    }
    // Remembers the time of the next collision
    double oldCurrentTime = currentTime;
    // Sets the current time to the time at the current interval
    currentTime = interval * intervalNum;

    // Shifts all the vehicles back to the time at the current interval
    for (vehicle& veh : vehicles) {
      if (veh.name != veh1 && veh.name != veh2) {
        veh.startX = veh.startX-veh.velX*(minColTime-currentTime);
        veh.startY = veh.startY-veh.velY*(minColTime-currentTime);
      } else {
        if (veh.name == veh1) {
          veh.startX = veh.startX-oldVel1X*(minColTime-currentTime);
          veh.startY = veh.startY-oldVel1Y*(minColTime-currentTime);
        }
        if (veh.name == veh2) {
          veh.startX = veh.startX-oldVel2X*(minColTime-currentTime);
          veh.startY = veh.startY-oldVel2Y*(minColTime-currentTime);
        }
      }
    }

    // Outputs all the vehicle positions
    std::cout << "at " << interval*intervalNum << '\n';
    for (vehicle& veh : vehicles) {
      std::cout << veh.name << " " << veh.startX << " " << veh.startY << '\n';
    }
    std::cout << '\n';

    // Advances all the vehicles to the next collision time
    for (vehicle& veh : vehicles) {
      if (veh.name != veh1 && veh.name != veh2) {
        veh.startX = veh.startX+veh.velX*(minColTime-currentTime);
        veh.startY = veh.startY+veh.velY*(minColTime-currentTime);
      } else {
        if (veh.name == veh1) {
          veh.startX = veh.startX+oldVel1X*(minColTime-currentTime);
          veh.startY = veh.startY+oldVel1Y*(minColTime-currentTime);
        }
        if (veh.name == veh2) {
          veh.startX = veh.startX+oldVel2X*(minColTime-currentTime);
          veh.startY = veh.startY+oldVel2Y*(minColTime-currentTime);
        }
      }
    }
    currentTime = oldCurrentTime;
    intervalNum++;
  }

  return 0;
}
