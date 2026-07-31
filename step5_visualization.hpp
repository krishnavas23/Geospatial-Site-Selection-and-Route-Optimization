#ifndef STEP5_VISUALIZATION_HPP
#define STEP5_VISUALIZATION_HPP

#include <bits/stdc++.h>
using namespace std;

#include "step2_stats.hpp"

// Pure C++ BMP visualization (no external libraries).
// Generates terrain heatmaps + optimal habitat/mining site map, then opens them.

namespace viz_detail {

struct RGB { unsigned char r, g, b; };

inline RGB heatmap_color(float t) {
    // t in [0,1]: blue -> cyan -> green -> yellow -> red
    t = max(0.0f, min(1.0f, t));
    float r = 0, g = 0, b = 0;
    if (t < 0.25f) {
        float u = t / 0.25f;
        r = 0; g = u; b = 1;
    } else if (t < 0.5f) {
        float u = (t - 0.25f) / 0.25f;
        r = 0; g = 1; b = 1 - u;
    } else if (t < 0.75f) {
        float u = (t - 0.5f) / 0.25f;
        r = u; g = 1; b = 0;
    } else {
        float u = (t - 0.75f) / 0.25f;
        r = 1; g = 1 - u; b = 0;
    }
    return {
        (unsigned char)(r * 255),
        (unsigned char)(g * 255),
        (unsigned char)(b * 255)
    };
}

inline void find_minmax(const vector<vector<float>>& A, float& mn, float& mx) {
    mn = 1e30f; mx = -1e30f;
    for (const auto& row : A) {
        for (float v : row) {
            mn = min(mn, v);
            mx = max(mx, v);
        }
    }
    if (mx <= mn) { mn = 0; mx = 1; }
}

inline bool write_bmp(const string& path, int W, int H, const vector<RGB>& pixels) {
    // pixels are row-major top-to-bottom; BMP stores bottom-to-top
    ofstream out(path, ios::binary);
    if (!out) return false;

    int row_stride = (W * 3 + 3) & ~3; // pad to 4 bytes
    int pixel_bytes = row_stride * H;
    int file_size = 14 + 40 + pixel_bytes;

    auto put16 = [&](uint16_t v) {
        out.put((char)(v & 0xFF));
        out.put((char)((v >> 8) & 0xFF));
    };
    auto put32 = [&](uint32_t v) {
        out.put((char)(v & 0xFF));
        out.put((char)((v >> 8) & 0xFF));
        out.put((char)((v >> 16) & 0xFF));
        out.put((char)((v >> 24) & 0xFF));
    };

    // BITMAPFILEHEADER
    out.put('B'); out.put('M');
    put32(file_size);
    put16(0); put16(0);
    put32(54);

    // BITMAPINFOHEADER
    put32(40);
    put32(W);
    put32(H);
    put16(1);
    put16(24);
    put32(0);
    put32(pixel_bytes);
    put32(2835); put32(2835);
    put32(0); put32(0);

    vector<char> pad(row_stride - W * 3, 0);
    for (int y = H - 1; y >= 0; --y) {
        for (int x = 0; x < W; ++x) {
            const RGB& c = pixels[y * W + x];
            out.put((char)c.b);
            out.put((char)c.g);
            out.put((char)c.r);
        }
        if (!pad.empty()) out.write(pad.data(), (streamsize)pad.size());
    }
    return out.good();
}

inline vector<RGB> grid_to_heatmap(const vector<vector<float>>& A, int scale = 1) {
    int R = (int)A.size(), C = (int)A[0].size();
    int W = C * scale, H = R * scale;
    float mn, mx;
    find_minmax(A, mn, mx);
    float span = mx - mn;

    vector<RGB> pix(W * H);
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            float t = (A[r][c] - mn) / span;
            RGB col = heatmap_color(t);
            for (int dy = 0; dy < scale; ++dy) {
                for (int dx = 0; dx < scale; ++dx) {
                    int x = c * scale + dx;
                    int y = r * scale + dy;
                    pix[y * W + x] = col;
                }
            }
        }
    }
    return pix;
}

inline void set_pixel(vector<RGB>& pix, int W, int H, int x, int y, RGB col) {
    if (x < 0 || y < 0 || x >= W || y >= H) return;
    pix[y * W + x] = col;
}

inline void draw_filled_circle(vector<RGB>& pix, int W, int H, int cx, int cy, int radius, RGB col) {
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            if (dx * dx + dy * dy <= radius * radius)
                set_pixel(pix, W, H, cx + dx, cy + dy, col);
        }
    }
}

inline void draw_ring(vector<RGB>& pix, int W, int H, int cx, int cy, int radius, RGB col) {
    for (int a = 0; a < 360; ++a) {
        float rad = a * 3.14159265f / 180.0f;
        int x = cx + (int)(radius * cos(rad));
        int y = cy + (int)(radius * sin(rad));
        set_pixel(pix, W, H, x, y, col);
        set_pixel(pix, W, H, x + 1, y, col);
        set_pixel(pix, W, H, x, y + 1, col);
    }
}

inline void draw_line(vector<RGB>& pix, int W, int H, int x0, int y0, int x1, int y1, RGB col) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (true) {
        set_pixel(pix, W, H, x0, y0, col);
        set_pixel(pix, W, H, x0 + 1, y0, col);
        set_pixel(pix, W, H, x0, y0 + 1, col);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

inline void blit(vector<RGB>& dst, int dW, int dH, int ox, int oy,
                 const vector<RGB>& src, int sW, int sH) {
    for (int y = 0; y < sH; ++y) {
        for (int x = 0; x < sW; ++x) {
            int dx = ox + x, dy = oy + y;
            if (dx < 0 || dy < 0 || dx >= dW || dy >= dH) continue;
            dst[dy * dW + dx] = src[y * sW + x];
        }
    }
}

inline void downsample(const vector<vector<float>>& A, int factor, vector<vector<float>>& out) {
    int R = (int)A.size(), C = (int)A[0].size();
    int nR = (R + factor - 1) / factor;
    int nC = (C + factor - 1) / factor;
    out.assign(nR, vector<float>(nC, 0.0f));
    for (int i = 0; i < nR; ++i) {
        for (int j = 0; j < nC; ++j) {
            double sum = 0; int cnt = 0;
            for (int di = 0; di < factor; ++di) {
                for (int dj = 0; dj < factor; ++dj) {
                    int r = i * factor + di, c = j * factor + dj;
                    if (r < R && c < C) { sum += A[r][c]; cnt++; }
                }
            }
            out[i][j] = (float)(sum / max(1, cnt));
        }
    }
}

inline void ensure_dir(const string& path) {
#ifdef _WIN32
    system(("mkdir \"" + path + "\" 2>nul").c_str());
#else
    system(("mkdir -p \"" + path + "\" 2>/dev/null").c_str());
#endif
}

inline void open_file(const string& path) {
#ifdef _WIN32
    string cmd = "start \"\" \"" + path + "\"";
    system(cmd.c_str());
#else
    string cmd = "xdg-open \"" + path + "\" 2>/dev/null || open \"" + path + "\"";
    system(cmd.c_str());
#endif
}

} // namespace viz_detail

inline void create_visualizations(
    const vector<vector<float>>& elevation,
    const vector<vector<float>>& illumination,
    const vector<vector<float>>& water_ice,
    const vector<vector<float>>& signal_occultation,
    const Candidate& habitat,
    const Candidate& mining,
    float score,
    float path_len
){
    using namespace viz_detail;

    const string out_dir = "visualization";
    ensure_dir(out_dir);

    cout << "\nGenerating visualizations (BMP heatmaps)...\n";

    // Downsample 500x500 -> 250x250 for faster file size / display
    const int DS = 2;
    vector<vector<float>> e2, i2, w2, s2;
    downsample(elevation, DS, e2);
    downsample(illumination, DS, i2);
    downsample(water_ice, DS, w2);
    downsample(signal_occultation, DS, s2);

    int tileH = (int)e2.size(), tileW = (int)e2[0].size();

    auto elev_hm  = grid_to_heatmap(e2);
    auto illum_hm = grid_to_heatmap(i2);
    auto water_hm = grid_to_heatmap(w2);
    auto sig_hm   = grid_to_heatmap(s2);

    write_bmp(out_dir + "/elevation_heatmap.bmp", tileW, tileH, elev_hm);
    write_bmp(out_dir + "/illumination_heatmap.bmp", tileW, tileH, illum_hm);
    write_bmp(out_dir + "/water_ice_heatmap.bmp", tileW, tileH, water_hm);
    write_bmp(out_dir + "/signal_occultation_heatmap.bmp", tileW, tileH, sig_hm);
    cout << "  Saved 4 terrain heatmaps in " << out_dir << "/\n";

    // Optimal sites map: illumination background + markers + connecting line
    auto sites = illum_hm;
    int hx = habitat.c / DS, hy = habitat.r / DS;
    int mx = mining.c / DS,  my = mining.r / DS;

    RGB white{255, 255, 255};
    RGB black{0, 0, 0};
    RGB green{0, 220, 60};   // habitat
    RGB cyan{0, 200, 255};   // mining
    RGB yellow{255, 255, 0}; // path line

    draw_line(sites, tileW, tileH, hx, hy, mx, my, yellow);
    draw_ring(sites, tileW, tileH, hx, hy, 10, white);
    draw_filled_circle(sites, tileW, tileH, hx, hy, 6, green);
    draw_ring(sites, tileW, tileH, mx, my, 10, white);
    draw_filled_circle(sites, tileW, tileH, mx, my, 6, cyan);

    string sites_path = out_dir + "/optimal_sites_map.bmp";
    write_bmp(sites_path, tileW, tileH, sites);
    cout << "  Saved habitat/mining site map: " << sites_path << "\n";
    cout << "    Habitat (green): (" << habitat.r << ", " << habitat.c << ")\n";
    cout << "    Mining  (cyan):  (" << mining.r << ", " << mining.c << ")\n";

    // Comparative dashboard: 2x2 heatmaps + sites map below + metric bars
    int gap = 8;
    int label_h = 24;
    int bar_panel_h = 80;
    int dashW = tileW * 2 + gap * 3;
    int dashH = label_h + tileH * 2 + gap * 3 + tileH + gap + bar_panel_h;
    vector<RGB> dash(dashW * dashH, RGB{20, 20, 28});

    auto place_labeled = [&](const vector<RGB>& tile, int ox, int oy) {
        blit(dash, dashW, dashH, ox, oy, tile, tileW, tileH);
    };

    place_labeled(elev_hm,  gap, label_h);
    place_labeled(illum_hm, gap * 2 + tileW, label_h);
    place_labeled(water_hm, gap, label_h + tileH + gap);
    place_labeled(sig_hm,   gap * 2 + tileW, label_h + tileH + gap);

    int sites_x = (dashW - tileW) / 2;
    int sites_y = label_h + tileH * 2 + gap * 2;
    place_labeled(sites, sites_x, sites_y);

    // Comparative metric bars (illumination %, water %, path normalized)
    float illum_pct = habitat.avg;          // 0..1
    float water_pct = mining.avg;           // 0..1
    float path_norm = min(1.0f, path_len / 500.0f);
    float score_norm = max(0.0f, min(1.0f, score));

    int bar_y = sites_y + tileH + gap + 20;
    int bar_x = gap * 2;
    int bar_max_w = dashW - gap * 4;
    int bar_h = 10;
    auto draw_bar = [&](int y, float val, RGB col, const string&) {
        for (int x = bar_x; x < bar_x + bar_max_w; ++x)
            for (int dy = 0; dy < bar_h; ++dy)
                set_pixel(dash, dashW, dashH, x, y + dy, RGB{50, 50, 60});
        int fill = (int)(val * bar_max_w);
        for (int x = bar_x; x < bar_x + fill; ++x)
            for (int dy = 0; dy < bar_h; ++dy)
                set_pixel(dash, dashW, dashH, x, y + dy, col);
    };

    draw_bar(bar_y,              illum_pct,  green,  "Illum");
    draw_bar(bar_y + 18,         water_pct,  cyan,   "Water");
    draw_bar(bar_y + 36,         1.0f - path_norm, yellow, "Path"); // shorter path = longer bar
    draw_bar(bar_y + 54,         score_norm, RGB{255, 120, 40}, "Score");

    string dash_path = out_dir + "/comparative_dashboard.bmp";
    write_bmp(dash_path, dashW, dashH, dash);
    cout << "  Saved comparative dashboard: " << dash_path << "\n";

    // Legend text file (coordinates + color meanings) for interview defense
    ofstream legend(out_dir + "/legend.txt");
    if (legend) {
        legend << "Optimal Moon Landing — Visualization Legend\n";
        legend << "===========================================\n\n";
        legend << "Terrain heatmaps (blue=low, red=high):\n";
        legend << "  elevation_heatmap.bmp\n";
        legend << "  illumination_heatmap.bmp\n";
        legend << "  water_ice_heatmap.bmp\n";
        legend << "  signal_occultation_heatmap.bmp\n\n";
        legend << "optimal_sites_map.bmp:\n";
        legend << "  Green circle = Optimal Habitat  (" << habitat.r << ", " << habitat.c << ")\n";
        legend << "  Cyan circle  = Optimal Mining   (" << mining.r << ", " << mining.c << ")\n";
        legend << "  Yellow line  = Habitat–Mining connection\n";
        legend << "  Background   = Illumination heatmap\n\n";
        legend << "comparative_dashboard.bmp:\n";
        legend << "  2x2 terrain heatmaps + sites map + metric bars\n";
        legend << "  Bars: Illumination, Water-Ice, Path (short=better), Combined Score\n\n";
        legend << fixed << setprecision(4);
        legend << "Result metrics:\n";
        legend << "  Score = " << score << "\n";
        legend << "  Habitat Illumination = " << habitat.avg * 100 << "%\n";
        legend << "  Mining Water-Ice = " << mining.avg << "\n";
        legend << "  Path Length = " << int(path_len) << " cells\n";
        legend.close();
    }

    cout << "Opening visualization dashboard...\n";
    open_file(dash_path);
    open_file(sites_path);
}

#endif
