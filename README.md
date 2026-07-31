# Optimal Moon Landing

**Geospatial Site Selection & Route Optimization**

A C++ data analytics pipeline that processes lunar terrain CSV rasters to recommend the best **habitat site** (high illumination), **mining site** (high water-ice probability), and a **slope-constrained connecting path** between them — then generates **terrain heatmaps** and an interactive result dashboard.

![Comparative Dashboard](screenshots/comparative_dashboard.png)

---

## Features

- **CSV Processing & Validation** — Load and validate four 500×500 lunar raster datasets
- **Data Cleaning** — Whitespace trimming, empty-cell handling, numeric integrity checks
- **ETL Pipeline** — Extract → transform (spatial statistics) → load structured results
- **Statistics** — Sliding-window mean & standard deviation (terrain roughness) via 2D prefix sums
- **Candidate Ranking** — Top-K filtering + spatial clustering for geographic diversity
- **Multi-criteria Scoring** — Balances illumination, water-ice, and path length
- **A\* Route Optimization** — Shortest path with 22 m max elevation step constraint
- **Visualization** — Pure C++ BMP heatmaps for all four maps + optimal site markers (no external viz libraries)

---

## Demo Results

| Metric | Value |
|--------|-------|
| **Habitat** | `(303, 264)` — 56.56% illumination |
| **Mining** | `(311, 200)` — 0.9510 water-ice probability |
| **Path** | 72 cells (~7200 m) |
| **Combined Score** | **0.6863** |

**Score formula:**

```text
score = 0.5 × illumination + 0.5 × water_ice − 0.001 × path_length
```

---

## Screenshots

### Optimal Habitat & Mining Sites

Green = habitat · Cyan = mining · Yellow = connection · Background = illumination heatmap

![Optimal Sites Map](screenshots/optimal_sites_map.png)

### Terrain Heatmaps (4 CSV layers)

| Elevation | Illumination |
|:---------:|:------------:|
| ![Elevation](screenshots/elevation_heatmap.png) | ![Illumination](screenshots/illumination_heatmap.png) |

| Water Ice | Signal Occultation |
|:---------:|:------------------:|
| ![Water Ice](screenshots/water_ice_heatmap.png) | ![Signal](screenshots/signal_occultation_heatmap.png) |

---

## Pipeline Overview

```text
CSV Load (4 rasters)
        │
        ▼
Data Cleaning & Validation
        │
        ▼
2D Prefix Sums → 5×5 Window Mean / Std Dev
        │
        ▼
Top-K Ranking + Spatial Clustering
        │
        ▼
Pair Scoring + A* Pathfinding (slope ≤ 22 m)
        │
        ├──► result.txt
        ├──► candidates.txt
        └──► visualization/ (heatmaps + dashboard)
```

---

## Project Structure

```text
Optimal-Moon-Landing/
├── main.cpp                    # Pipeline orchestrator
├── step1_csv.hpp               # CSV load, cleaning, validation
├── step2_stats.hpp             # Prefix sums, candidates, clustering
├── step3_pathfinding.hpp       # A* with slope constraint
├── step4_result.hpp            # Scoring + result.txt writer
├── step5_visualization.hpp     # Pure C++ BMP heatmaps & site maps
├── elevation.csv               # Terrain height (500×500)
├── illumination.csv            # Solar illumination (500×500)
├── water_ice.csv               # Water-ice probability (500×500)
├── signal_occultation.csv      # Signal occultation (500×500)
├── result.txt                  # Optimal pair report (generated)
├── candidates.txt              # Top candidate shortlist (generated)
├── visualization/              # BMP outputs (generated on run)
├── screenshots/                # PNG previews for this README
└── helper.txt                  # Interview / deep-dive study guide
```

---

## Build & Run

**Requirements:** C++17 compiler (`g++` recommended). No external libraries.

```bash
# Compile
g++ -O2 -std=c++17 main.cpp -o moon_landing

# Run (from the project root — CSV files must be in the same folder)
./moon_landing          # Linux / macOS
.\moon_landing.exe      # Windows
```

**Outputs after a successful run:**

| Output | Description |
|--------|-------------|
| `result.txt` | Optimal habitat, mining site, path length, score |
| `candidates.txt` | Top 20 habitat & mining candidates |
| `visualization/*.bmp` | Heatmaps, sites map, comparative dashboard |
| *(auto-open)* | Dashboard & sites map open in the default image viewer |

---

## Input Data

| File | Meaning |
|------|---------|
| `elevation.csv` | Ground elevation (meters) |
| `illumination.csv` | Sunlight fraction (0–1) |
| `water_ice.csv` | Water-ice probability (0–1) |
| `signal_occultation.csv` | Communication occultation values |

Each file is a **500×500** comma-separated grid (~250,000 cells). Approximate cell size: **100 m** (from path reporting).

---

## How It Works (Short)

1. **Ingest** four CSV rasters with validation and basic cleaning  
2. **Transform** each 5×5 neighborhood into mean resource value + elevation roughness (std. dev.) using **2D prefix sums** (O(1) queries)  
3. **Select** diverse candidates via top-3000 ranking and 25×25 spatial buckets  
4. **Optimize** habitat–mining pairs with weighted scoring and **A\*** under a 22 m slope limit  
5. **Report** text results and **visualize** all four layers plus the winning coordinates  

---

## Tech Stack

| Area | Used |
|------|------|
| Language | C++17 |
| Data | CSV Processing |
| Analytics | Statistics, ETL-style pipeline |
| Algorithms | 2D Prefix Sums, A\*, Spatial Clustering |
| Visualization | Pure C++ BMP heatmaps (no external viz libraries) |

---

## Sample `result.txt`

```text
Optimal Pair Found with Combined Score: 0.6863

--- Optimal Habitat Site ---
> Coordinates (row, col): (303, 264)
> Avg Illumination: 56.56%
> Terrain Roughness (Std Dev): 2.5955 m

--- Optimal Mining Site ---
> Coordinates (row, col): (311, 200)
> Avg Water-Ice Probability: 0.9510
> Terrain Roughness (Std Dev): 2.0769 m

--- Power Cable Path ---
> Path Length: 72 cells (7200 m)
```

---

## License

This project is provided for educational and portfolio use.
