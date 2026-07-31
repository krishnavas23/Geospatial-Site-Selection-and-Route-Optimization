#ifndef STEP2_STATS_HPP
#define STEP2_STATS_HPP

#include <bits/stdc++.h>
using namespace std;

struct Candidate {
    int r, c;
    float avg;     // illumination or water (mean)
    float rough;   // std dev (terrain roughness)
};

struct Prefix2D {
    int R=0, C=0;
    vector<vector<double>> S, SS; // sum and sum of squares

    Prefix2D() = default;

    explicit Prefix2D(const vector<vector<float>>& M){
        R = (int)M.size(); 
        C = (int)M[0].size();
        S.assign(R+1, vector<double>(C+1,0.0));
        SS.assign(R+1, vector<double>(C+1,0.0));
        for(int i=1;i<=R;i++){
            double rows=0, rowss=0;
            for(int j=1;j<=C;j++){
                double v = M[i-1][j-1];
                rows  += v;
                rowss += v*v;
                S[i][j]  = S[i-1][j]  + rows;
                SS[i][j] = SS[i-1][j] + rowss;
            }
        }
    }

    inline double rect_sum(const vector<vector<double>>& P, int r1,int c1,int r2,int c2) const {
        r1++; c1++; r2++; c2++; // 1-based indexing
        return P[r2][c2] - P[r1-1][c2] - P[r2][c1-1] + P[r1-1][c1-1];
    }

    inline pair<float,float> block_stats5x5(int r,int c) const {
        int r2=r+4, c2=c+4;
        double n = 25.0; // 25 elements
        double sum  = rect_sum(S , r,c,r2,c2);
        double sum2 = rect_sum(SS, r,c,r2,c2);
        double mean = sum / n;
        double var  = max(0.0, sum2/n - mean*mean);
        return {(float)mean,(float)sqrt(var)};
    }
};

inline vector<Candidate> select_candidates(
    const Prefix2D& P_metric,   // illumination or water
    const Prefix2D& P_elev,     // elevation for roughness
    int R, int C,
    int K_keep,
    int clusterSize
){
    vector<Candidate> all; 
    all.reserve(max(0,(R-4)*(C-4)));

    for(int i=0;i<=R-5;i++){
        for(int j=0;j<=C-5;j++){
            auto [avg, dummy]   = P_metric.block_stats5x5(i,j);
            auto [_, roughElev] = P_elev.block_stats5x5(i,j);
            // store center coordinates of the 5x5 block
            all.push_back({i+2, j+2, (float)avg, (float)roughElev});
        }
    }

    sort(all.begin(), all.end(), [](const Candidate& a, const Candidate& b){
        if (a.avg==b.avg) return a.rough<b.rough;
        return a.avg>b.avg;
    });
    if((int)all.size() > K_keep) all.resize(K_keep);
    

    
    int nClusterR = (R + clusterSize - 1)/clusterSize;
    int nClusterC = (C + clusterSize - 1)/clusterSize;
    vector<vector<bool>> used(nClusterR, vector<bool>(nClusterC,false));
    vector<Candidate> picked; 
    picked.reserve(nClusterR*nClusterC);

    for(const auto& c : all){
        int cr = c.r / clusterSize;
        int cc = c.c / clusterSize;
        if(!used[cr][cc]){
            used[cr][cc] = true;
            picked.push_back(c);
        }
    }
    return picked;
}

inline void write_candidates_to_file(const vector<Candidate>& hab, const vector<Candidate>& mining, int topN=20){
    ofstream out("candidates.txt", ios::trunc);
    if(!out.is_open()){
        cerr << "[ERROR] Could not create candidates.txt\n";
        return;
    }

    out << "=== Top Habitat Candidates ===\n";
    for(int i=0; i<min(topN,(int)hab.size()); i++){
        out << "(" << hab[i].r << "," << hab[i].c << ")  "
            << "AvgIllum=" << hab[i].avg << "  Rough=" << hab[i].rough << "\n";
    }

    out << "\n=== Top Mining Candidates ===\n";
    for(int i=0; i<min(topN,(int)mining.size()); i++){
        out << "(" << mining[i].r << "," << mining[i].c << ")  "
            << "AvgWater=" << mining[i].avg << "  Rough=" << mining[i].rough << "\n";
    }

    out.close();
    cout << "Top candidates written to candidates.txt\n";
}

#endif 
