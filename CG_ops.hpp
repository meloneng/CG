#include <bits/stdc++.h>
using namespace std;

typedef struct D2point{
    int x;
    int y;
}D2point;

typedef struct D3point{
    int x;
    int y;
    int z;
}D3point;

D2point D2translation(D2point xy, D2point tr){

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

D2point D2Scale(D2point xy, D2point sc){
    D2point newxy;
    newxy.x = xy.x * sc.x;
    newxy.y = xy.y * sc.y;

    return newxy;   
}

D3point D3Scale(D3point xyz, D3point sc){
    
    D3point newxyz;
    newxyz.x = xyz.x * sc.x;
    newxyz.y = xyz.y * sc.y;
    newxyz.z = xyz.z * sc.z;

    return newxyz;
}

D2point D2Rotation(D2point xy, D2point ro){
    
    return newxy;
}

