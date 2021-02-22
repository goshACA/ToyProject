#include <iostream>
#include <cstdlib>
#include <vector>
#include "glutWindow.h"

#include "TinyToys.h"
using namespace std;
#define INF  99999

int main(int argc,char **argv) {
    
    vector<Room> rooms;
    rooms.push_back(Room("Alpha",  110, 450, 0xff0000));
    rooms.push_back(Room("Bravo", 400, 700, 0xff7700));
    rooms.push_back(Room("Charlie", 1100, 720, 0xffff00));
    rooms.push_back(Room("Delta", 510, 470, 0x00ff00));
    rooms.push_back(Room("Echo", 370, 120, 0x00ffff));
    rooms.push_back(Room("Foxrot", 680, 40, 0x0000ff));
    rooms.push_back(Room("Golf", 670, 230, 0x7700ff));
    rooms.push_back(Room("Hotel", 900, 280, 0x915136));
    rooms.push_back(Room("India", 1140, 350, 0x777777));
    rooms.push_back(Room("Juliett", 970, 80, 0xffffff));
    for(auto &p: rooms){
        std::cout << p.name + ": " << p.point.toString();
    }
    
    int V = 10;
    vector<vector<int> > graph
            = {         {INF, INF, 1, 1, INF, INF, INF, INF, INF, INF},
                        {INF, INF, INF, INF, INF, 1, INF, INF, INF, INF},
                        {1, INF, INF, INF, INF, INF, INF, 1, INF, INF},
                        {1, INF, INF, INF, 1, INF, INF, INF, INF, INF},
                        {INF, INF, INF, 1, INF, INF, INF, INF, INF, 1},
                        {INF, 1, INF, INF, INF, INF, INF, INF, INF, 1},
                        {INF, INF, 1, INF, INF, INF, INF, INF, INF, INF},
                        {INF, INF, INF, INF, 1, INF, INF, INF, 1, INF},
                        {INF, INF, INF, INF, INF, INF, 1, 1, INF, INF},
                        {INF, INF, INF, INF, INF, 1, INF, INF, 1, INF} };
    
    TinyToys toys(rooms, graph, argc, argv);
    toys.start();
    return 0;
}
