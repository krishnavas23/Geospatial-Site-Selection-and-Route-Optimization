#ifndef STEP1_CSV_HPP
#define STEP1_CSV_HPP

#include <bits/stdc++.h>
using namespace std;

inline vector<vector<float>> load_csv(const string &path) {
    ifstream in(path);//if no file then error
    if (!in) { cerr << "[ERROR] Cannot open file: " << path << "\n"; exit(1); }
    vector<vector<float>> data; string line; size_t expected_cols = 0;//for storing rows,same no. of columns

    auto trim = [](string &s){//trim for better spaces
        auto notspace = [](int ch){ return !isspace(ch); };
        s.erase(s.begin(), find_if(s.begin(), s.end(), notspace));
        s.erase(find_if(s.rbegin(), s.rend(), notspace).base(), s.end());
    };

    size_t line_no = 0;
    while (getline(in, line)) {//line by line reading
        line_no++; if (line.empty()) continue;
        vector<float> row; string cell; stringstream ss(line);
        while (getline(ss, cell, ',')) {
            trim(cell);
            if (cell.empty()) { row.push_back(0.0f); continue; }//if empty cell then put 0.0 value
            try { row.push_back(stof(cell)); }//otherwise covert to float
            catch (...) {//if non numeric value - error
                cerr << "[ERROR] Bad numeric value in " << path
                     << " at line " << line_no << ": '" << cell << "'\n";
                exit(1);
            }
        }
        //code to check rectangular csv file 
        if (row.empty()) continue;
        if (expected_cols == 0) expected_cols = row.size();
        if (row.size() != expected_cols) {
            cerr << "[ERROR] Non-rectangular CSV in " << path << " at line " << line_no << "\n";
            exit(1);
        }
        data.push_back(move(row));
    }
    if (data.empty()) { cerr << "[ERROR] Empty CSV: " << path << "\n"; exit(1); }
    return data;
}

inline void print_sample(const vector<vector<float>>& A, const string &name, int k=5){
    int R=A.size(), C=A[0].size();
    cout << name << " => " << R << "x" << C << "\n";
    int r=min(R,k), c=min(C,k);
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            cout << fixed << setprecision(2) << A[i][j];
            if(j+1<c) cout << ", ";
        }
        cout << "\n";
    }
    cout << "---\n";
}

#endif
