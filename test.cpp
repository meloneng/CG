#include <bits/stdc++.h>
#include "./CG_ops.hpp"
using namespace std;

int main(){
    /*
    D2point xy;
    xy.x = 1;
    xy.y = 2;

    D2point tr;
    tr.x = 5;
    tr.y = 5;

    D2point newxy;

    newxy = D2translation(xy, tr);

    cout << "novo x: " << newxy.x << "\nnovo y: "
        << newxy.y << "\n\n" << endl;

    tr.x = 2;
    tr.y = 2;

    newxy = D2Scale(newxy, tr);

    cout << "novo x: " << newxy.x << "\nnovo y: "
        << newxy.y << "\n\n" << endl;

    cout << cg_sin(90) << endl;
    cout << cg_cos(60) << endl;
    cout << cg_tan(45) << endl;
    */

   vertice z = {4,1}, v = {1,5};
   vector<vertice>test = bresenham(z,v);

    for(auto it : test){
        cout << "\nX = " << it.x << "\nY = " << it.y << "\n------" << endl;
    }

}