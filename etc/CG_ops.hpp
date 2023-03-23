#include <bits/stdc++.h>
using namespace std;

#define PI 3.14159265


// Definicao de vertice
struct vertice{
    int x;
    int y;
};


/*
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


D2point D2reflectionOrigin(D2point xy){
    D2point newxy;

    newxy.x = xy.x * -1;
    newxy.y = xy.y * -1;

    return newxy;
}

D2point D2reflectionX(D2point xy){
    D2point newxy;

    newxy.y = xy.y * -1;

    return newxy;
}


D2point D2reflectionY(D2point xy){
    D2point newxy;

    newxy.x = xy.x * -1;

    return newxy;
}



D2point D2Rotation(D2point xy, D2point ro){

    D2point newxy;

    newxy.x = (xy.x * cg_cos(ro)) - (xy.y * cg_sin(ro));
    newxy.y = (xy.x * cg_sin(ro)) + (xy.y * cg_cos(ro));

    return newxy;
}

double cg_sin(double dg){
    
    double dgSin;
    dgSin = sin((dg * PI)/180);
    
    return dgSin;
}

double cg_cos(double dg){
    
    double dgCos;
    dgCos = cos((dg * PI)/180);
    
    return dgCos;
}

double cg_tan(double dg){
    double dgTan;
    dgTan = tan((dg * PI)/180);

    return dgTan;
}

*/

// Line Rasterization
vector<vertice> rasterization(vertice a, vertice b){
    vector<vertice> line;
    int delx = b.x - a.x;
    int dely = b.y - a.y;
    int dis = 2*dely - delx;
    int ince = 2*dely;
    int incne = 2*(dely - delx);

    int x = a.x, y = a.y;
    vertice w = {x,y};
    line.push_back(w);
    while(x < b.x){
        if(dis <= 0){
            dis += ince;
        }else{
            dis += incne;
            y++;
        }
        x++;
        w = {x,y};
        line.push_back(w);
    }
    return line;
}
// Bresenham algorithm
vector<vertice> bresenham(vertice a, vertice b){
    bool slope = false;
    bool symmetry = false;
    int delx = b.x - a.x;
    int dely = b.y - a.y;
    if(delx*dely < 0){
        a.y *= -1;
        b.y *= -1;
        dely *= -1;
        symmetry = true;
    }
    if(abs(delx) < abs(dely)){
        swap(a.x,a.y);
        swap(b.x,b.y);
        swap(delx,dely);
        slope = true;
    }
    if(a.x > b.x){
        swap(a.x,b.x);
        swap(a.y,b.y);
        dely *= -1;
        delx *= -1;
    }

    cout << "A = (" << a.x << a.y << ")\nB = (" << b.x << b.y << ")\n" << endl;

    vector<vertice> line = rasterization(a,b);
    
    if(slope && symmetry){
        for(auto &i : line){
            swap(i.x,i.y);
            i.y *= -1;
        }
    }else if(slope && !symmetry){
        for(auto &i : line){
            swap(i.x,i.y);
        }        
    }else if(!slope && symmetry){
        for(auto &i : line){
            i.y *= -1;
        }
    }
    return line;
}
