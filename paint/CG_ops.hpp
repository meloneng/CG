#include <bits/stdc++.h>
using namespace std;

#define PI 3.14159265

// Definicao de vertice
struct vertice{
    int x;
    int y;
};


// Predefinitions
vector<vertice> bresenham(vertice a, vertice b);
vector<vertice> lineBresenham(vertice a, vertice b);
vector<vertice> bresenhamCirc(vertice a, int R);
//-----------------------------------------------------------------------------------


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

    vector<vertice> line = lineBresenham(a,b);
    
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
// Line definition
vector<vertice> lineBresenham(vertice a, vertice b){
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

vector<vertice> bresenhamCirc(vertice a, int r){
    vector<vertice> vertices;
    int d = 1-r;
    int delE = 3;
    int delSE = (-2*r)+5;
    int x=0, y=r;

    vertice auxv = {0+a.x,r+a.y};
    vertices.push_back(auxv);
    auxv = {r+a.x,0+a.y};
    vertices.push_back(auxv);
    auxv = {0+a.x,-r+a.y};
    vertices.push_back(auxv);
    auxv = {-r+a.x,0+a.y};
    vertices.push_back(auxv);

    while(y > x){
        if(d<0){
            d+=delE;
            delE+=2;
            delSE+=2;
        }else{
            d+=delSE;
            delE+=2;
            delSE+=4;
            y--;
        }
        x++;

        auxv = {x+a.x,y+a.y};
        vertices.push_back(auxv);
        auxv = {x+a.x,-y+a.y};
        vertices.push_back(auxv);
        auxv = {y+a.x,x+a.y};
        vertices.push_back(auxv);
        auxv = {y+a.x,-x+a.y};
        vertices.push_back(auxv);
        auxv = {-x+a.x,y+a.y};
        vertices.push_back(auxv);
        auxv = {-x+a.x,-y+a.y};
        vertices.push_back(auxv);
        auxv = {-y+a.x,x+a.y};
        vertices.push_back(auxv);
        auxv = {-y+a.x,-x+a.y};
        vertices.push_back(auxv);

    }

    return vertices;
}