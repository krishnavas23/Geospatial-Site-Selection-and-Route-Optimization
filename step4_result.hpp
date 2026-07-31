#ifndef STEP4_RESULT_HPP
#define STEP4_RESULT_HPP

#include <bits/stdc++.h>
using namespace std;

#include "step2_stats.hpp"

inline float score_pair(const Candidate& Hc, const Candidate& Mc, float path_len){
    return 0.5f*Hc.avg + 0.5f*Mc.avg - 0.001f*path_len;
}

inline void write_result(const string &filename, float score,
                         const Candidate& habitat, const Candidate& mining, float path_len){
    ofstream out(filename, ios::trunc);
    if(!out.is_open()){
        cerr << "[ERROR] Could not create result file!\n";
        return;
    }
    out << fixed << setprecision(4);
    out << "Optimal Pair Found with Combined Score: " << score << "\n\n";
    out << "--- Optimal Habitat Site ---\n";
    out << "> Coordinates (row, col): (" << habitat.r << ", " << habitat.c << ")\n";
    out << "> Avg Illumination: " << fixed << setprecision(2) << habitat.avg*100 << "%\n";
    out << "> Terrain Roughness (Std Dev): " << fixed << setprecision(4) << habitat.rough << " m\n\n";
    out << "--- Optimal Mining Site ---\n";
    out << "> Coordinates (row, col): (" << mining.r << ", " << mining.c << ")\n";
    out << "> Avg Water-Ice Probability: " << fixed << setprecision(4) << mining.avg << "\n";
    out << "> Terrain Roughness (Std Dev): " << fixed << setprecision(4) << mining.rough << " m\n\n";
    out << "--- Power Cable Path ---\n";
    out << "> Path Length: " << int(path_len) << " cells (" << int(path_len*100) << " m)\n";
    out.close();
}

#endif 
