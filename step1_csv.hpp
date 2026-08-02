#ifndef STEP1_CSV_HPP
#define STEP1_CSV_HPP

#include <bits/stdc++.h>
using namespace std;

struct RasterMeta {
    string name;
    int rows = 0;
    int cols = 0;
    int missing_cells = 0;
    int invalid_range_cells = 0;
    float min_val = 0.0f;
    float max_val = 0.0f;
    double mean_val = 0.0;
};

struct RasterBundle {
    vector<vector<float>> elevation;
    vector<vector<float>> illumination;
    vector<vector<float>> water_ice;
    vector<vector<float>> signal_occultation;
    int R = 0;
    int C = 0;
    vector<RasterMeta> meta;
};

inline void trim_whitespace(string &s) {
    auto notspace = [](int ch) { return !isspace(ch); };
    s.erase(s.begin(), find_if(s.begin(), s.end(), notspace));
    s.erase(find_if(s.rbegin(), s.rend(), notspace).base(), s.end());
}

// Load one CSV raster. Empty cells are rejected (fail-fast) — never silently imputed as 0.
inline vector<vector<float>> load_csv(const string &path, RasterMeta &meta) {
    ifstream in(path);
    if (!in) {
        cerr << "[ERROR] Cannot open file: " << path << "\n";
        exit(1);
    }

    meta.name = path;
    meta.missing_cells = 0;
    meta.invalid_range_cells = 0;

    vector<vector<float>> data;
    string line;
    size_t expected_cols = 0;
    size_t line_no = 0;
    double sum = 0.0;
    long long count = 0;
    bool first_val = true;

    while (getline(in, line)) {
        line_no++;
        if (line.empty()) continue;

        vector<float> row;
        string cell;
        stringstream ss(line);
        size_t col_no = 0;

        while (getline(ss, cell, ',')) {
            col_no++;
            trim_whitespace(cell);

            if (cell.empty()) {
                meta.missing_cells++;
                cerr << "[ERROR] Missing value in " << path
                     << " at line " << line_no << ", column " << col_no
                     << " — empty cells are not imputed (0 would bias analytics)\n";
                exit(1);
            }

            float v;
            try {
                v = stof(cell);
            } catch (...) {
                cerr << "[ERROR] Non-numeric value in " << path
                     << " at line " << line_no << ", column " << col_no
                     << ": '" << cell << "'\n";
                exit(1);
            }

            if (!isfinite(v)) {
                cerr << "[ERROR] Non-finite value in " << path
                     << " at line " << line_no << ", column " << col_no << "\n";
                exit(1);
            }

            row.push_back(v);
            sum += v;
            count++;
            if (first_val) {
                meta.min_val = meta.max_val = v;
                first_val = false;
            } else {
                meta.min_val = min(meta.min_val, v);
                meta.max_val = max(meta.max_val, v);
            }
        }

        if (row.empty()) continue;

        if (expected_cols == 0) expected_cols = row.size();
        if (row.size() != expected_cols) {
            cerr << "[ERROR] Non-rectangular CSV in " << path
                 << " at line " << line_no
                 << " (expected " << expected_cols << " cols, got " << row.size() << ")\n";
            exit(1);
        }
        data.push_back(move(row));
    }

    if (data.empty()) {
        cerr << "[ERROR] Empty CSV: " << path << "\n";
        exit(1);
    }

    meta.rows = (int)data.size();
    meta.cols = (int)data[0].size();
    meta.mean_val = count ? sum / count : 0.0;

    return data;
}

inline void check_prob_range(RasterMeta &meta, float lo, float hi, const string &label) {
    if (meta.min_val < lo - 1e-4f || meta.max_val > hi + 1e-4f) {
        meta.invalid_range_cells++; // flag file-level range violation
        cerr << "[WARN] " << label << " (" << meta.name << ") values outside ["
             << lo << ", " << hi << "]: min=" << meta.min_val
             << " max=" << meta.max_val << "\n";
    }
}

inline void validate_raster_bundle(RasterBundle &bundle) {
    bundle.R = (int)bundle.elevation.size();
    bundle.C = (int)bundle.elevation[0].size();

    const int expected_cells = bundle.R * bundle.C;
    const int expected_windows = max(0, bundle.R - 4) * max(0, bundle.C - 4); // 246,016 at 500x500

    for (const auto &m : bundle.meta) {
        if (m.rows != bundle.R || m.cols != bundle.C) {
            cerr << "[ERROR] Dimension mismatch: " << m.name
                 << " is " << m.rows << "x" << m.cols
                 << " but elevation is " << bundle.R << "x" << bundle.C << "\n";
            exit(1);
        }
        if (m.rows * m.cols != expected_cells) {
            cerr << "[ERROR] Expected " << expected_cells << " cells in " << m.name
                 << ", got " << m.rows * m.cols << "\n";
            exit(1);
        }
    }

    check_prob_range(bundle.meta[1], 0.0f, 1.0f, "Illumination");
    check_prob_range(bundle.meta[2], 0.0f, 1.0f, "Water-ice");
    // signal_occultation is not a probability — report stats only, no [0,1] gate

    ofstream dq("data_quality.txt", ios::trunc);
    if (!dq.is_open()) {
        cerr << "[WARN] Could not write data_quality.txt\n";
        return;
    }

    dq << fixed << setprecision(4);
    dq << "Data Quality Report — Optimal Moon Landing Pipeline\n";
    dq << "===================================================\n\n";
    dq << "Grid dimensions: " << bundle.R << " x " << bundle.C
       << " (" << expected_cells << " cells per layer)\n";
    dq << "Valid 5x5 spatial windows: " << expected_windows << "\n\n";

    dq << "Ingestion & cleaning checks (all passed):\n";
    dq << "  [OK] Whitespace trimmed on every cell\n";
    dq << "  [OK] Empty cells rejected (fail-fast, no zero imputation)\n";
    dq << "  [OK] Numeric integrity validated (non-numeric / non-finite rejected)\n";
    dq << "  [OK] Rectangular structure enforced across all rows\n";
    dq << "  [OK] Cross-layer dimension alignment verified\n\n";

    dq << "Layer summary:\n";
    for (const auto &m : bundle.meta) {
        dq << "  " << m.name << "\n";
        dq << "    Shape: " << m.rows << " x " << m.cols << "\n";
        dq << "    Missing cells: " << m.missing_cells << "\n";
        dq << "    Min / Max / Mean: " << m.min_val << " / " << m.max_val
           << " / " << m.mean_val << "\n";
        if (m.invalid_range_cells > 0)
            dq << "    Range warning: values outside expected bounds\n";
        dq << "\n";
    }

    dq << "Expected probability layers (illumination, water_ice): [0, 1]\n";
    dq << "Signal occultation: continuous occultation metric (not bounded to [0,1])\n";
    dq.close();
    cout << "Data quality report written to data_quality.txt\n";
}

inline RasterBundle load_all_rasters() {
    RasterBundle bundle;
    bundle.meta.resize(4);

    bundle.elevation          = load_csv("elevation.csv",          bundle.meta[0]);
    bundle.illumination       = load_csv("illumination.csv",       bundle.meta[1]);
    bundle.water_ice          = load_csv("water_ice.csv",          bundle.meta[2]);
    bundle.signal_occultation = load_csv("signal_occultation.csv", bundle.meta[3]);

    validate_raster_bundle(bundle);
    return bundle;
}

inline void print_sample(const vector<vector<float>> &A, const string &name, int k = 5) {
    int R = (int)A.size(), C = (int)A[0].size();
    cout << name << " => " << R << "x" << C << "\n";
    int r = min(R, k), c = min(C, k);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cout << fixed << setprecision(2) << A[i][j];
            if (j + 1 < c) cout << ", ";
        }
        cout << "\n";
    }
    cout << "---\n";
}

#endif
