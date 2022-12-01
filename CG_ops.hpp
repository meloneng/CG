#include <bits/stdc++.h>
using namespace std;

typedef struct D2point{
    int x;
    int y;
} 2Dpoint;

typedef struct D3point{
    int x;
    int y;
    int z;
} 3Dpoint;

D2point D2translation(D2point xy, D3point tr){

    D2point newxy;
    newxy.x = xy.x + tr.x;
    newxy.y = xy.y + tr.y;

    return newxy;
}


D3point D3translation(D3point xyz, D3point tr){

    D3point newxyz;
    newxyz.x = xyz.x + tr.x;
    newxyz.y = xyz.y + tr.y;
    newxyz.z = xyz.z + tr.z;

    return newxyz;
}

D2point D2Scaling(D2point xy, D2point tr){

    D2point newxy;
    newxy.x = xy.x + tr.x;
    newxy.y = xy.y + tr.y;

    return newxy
}

D3point D3Scaling(D3point xyz, D3point tr){

    D3point newxyz;
    newxyz.x = xyz.x + tr.x;
    newxyz.y = xyz.y + tr.y;
    newxyz.z = xyz.z + tr.z;

    return newxyz;
}