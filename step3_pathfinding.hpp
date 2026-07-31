#ifndef STEP3_PATHFINDING_HPP
#define STEP3_PATHFINDING_HPP

#include <bits/stdc++.h>
using namespace std;

struct AStarNode {
    int r, c;
    float f; // f = g + h
    float g;
    bool operator>(const AStarNode& o) const { return f > o.f; }
};

inline bool inb(int r,int c,int R,int C){ return (r>=0 && r<R && c>=0 && c<C); }//checks if given r,c are in the grid

inline bool slope_ok(float h1, float h2, float max_slope=22.0f){
    return fabsf(h1 - h2) <= max_slope;
}

//returns path length in cells or INF if none
inline float astar_path(
    const vector<vector<float>>& elev,
    pair<int,int> start, pair<int,int> goal
){
    const int R = elev.size(), C = elev[0].size();
    auto idx = [C](int r,int c){ return r*C + c; };//lambda fucntion that converts r,c into 1D index 

    const float INF = 1e9f;
    vector<float> g(R*C, INF);
    vector<char>  seen(R*C, 0);

    auto H = [&](int r,int c){
        // Manhattan distance is admissible for 4-neighbour unit steps
        return float(abs(r - goal.first) + abs(c - goal.second));//heuristic function (distance from current to goal)
    };

    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> pq;
    int sr=start.first, sc=start.second, tr=goal.first, tc=goal.second;
    g[idx(sr,sc)] = 0.0f;
    pq.push({sr,sc, H(sr,sc), 0.0f});

    const int dr[4]={1,-1,0,0};
    const int dc[4]={0,0,1,-1};

    while(!pq.empty()){
        AStarNode cur = pq.top(); pq.pop();
        int r=cur.r, c=cur.c; int k = idx(r,c);
        if(seen[k]) continue;
        seen[k]=1;

        if(r==tr && c==tc) return cur.g;

        for(int dir=0; dir<4; ++dir){
            int nr=r+dr[dir], nc=c+dc[dir];
            if(!inb(nr,nc,R,C)) continue;
            if(!slope_ok(elev[r][c], elev[nr][nc])) continue;
            float ng = cur.g + 1.0f;
            int nk = idx(nr,nc);
            if(ng < g[nk]){
                g[nk] = ng;
                float f = ng + H(nr,nc);
                pq.push({nr,nc,f,ng});
            }
        }
    }
    return INF;
}

#endif 
