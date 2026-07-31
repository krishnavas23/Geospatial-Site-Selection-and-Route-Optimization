# 🛰️ Geospatial Site Selection & Route Optimization

> **A C++ geospatial analytics pipeline that identifies the optimal lunar habitat site, mining site, and shortest feasible power cable route using terrain analysis, statistical processing, and A* pathfinding.**

<p align="center">

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-success)
![Algorithms](https://img.shields.io/badge/Algorithms-A*%20%7C%20Prefix%20Sums-orange)
![Visualization](https://img.shields.io/badge/Visualization-BMP%20Heatmaps-purple)
![License](https://img.shields.io/badge/License-Educational-lightgrey)

</p>

---

## 🌙 Project Overview

Planning infrastructure on the Moon requires balancing multiple environmental factors rather than optimizing a single variable.

This project analyzes **500×500 lunar terrain datasets** to determine:

- 🏠 Best habitat location
- 🧊 Best mining location
- ⚡ Shortest feasible power cable route
- 📊 Interactive terrain visualizations

The complete workflow follows a **data analytics pipeline** consisting of:

> **Extract → Clean → Transform → Analyze → Optimize → Visualize**

---

## 📸 Dashboard Preview

<p align="center">

![Dashboard](screenshots/comparative_dashboard.png)

</p>

---

# ✨ Key Features

✅ CSV Processing & Validation

✅ Data Cleaning

✅ ETL-style Analytics Pipeline

✅ Statistical Analysis

✅ Sliding Window Analytics

✅ Spatial Clustering

✅ Multi-Criteria Decision Making

✅ A* Pathfinding

✅ Automatic Heatmap Generation

✅ Comparative Dashboard

---

# ⚙️ Pipeline

```text
CSV Files
    │
    ▼
Data Cleaning
    │
    ▼
Validation
    │
    ▼
Prefix Sum Statistics
    │
    ▼
Candidate Ranking
    │
    ▼
Spatial Clustering
    │
    ▼
Pair Evaluation
    │
    ▼
A* Route Optimization
    │
    ▼
Visualization
```

---

# 🧠 Algorithms Used

| Algorithm | Purpose |
|------------|---------|
| 2D Prefix Sums | Fast sliding-window statistics |
| Mean & Standard Deviation | Terrain roughness analysis |
| Spatial Clustering | Candidate diversification |
| Top-K Ranking | Best habitat/mining candidates |
| Multi-Criteria Scoring | Pair evaluation |
| A* Search | Shortest feasible route |

---

# 📊 Analytics Workflow

### Extract

- Read 4 CSV raster datasets

### Clean

- Missing values
- Empty cells
- Invalid numbers
- Whitespace trimming

### Transform

- 5×5 moving window
- Mean
- Standard deviation

### Analyze

- Rank candidates
- Filter unsuitable regions

### Optimize

- Habitat–Mining pair scoring
- A* shortest path

### Visualize

- Heatmaps
- Candidate map
- Dashboard

---

# 📈 Demo Results

| Metric | Result |
|---------|---------|
| Habitat Site | (303,264) |
| Mining Site | (311,200) |
| Illumination | 56.56% |
| Water Ice | 95.10% |
| Cable Length | 7200 m |
| Combined Score | **0.6863** |

---

# 🖼️ Heatmaps

## Habitat & Mining

![Optimal](screenshots/optimal_sites_map.png)

---

## Terrain Layers

| Elevation | Illumination |
|-----------|--------------|
| ![](screenshots/elevation_heatmap.png) | ![](screenshots/illumination_heatmap.png) |

| Water Ice | Signal |
|------------|---------|
| ![](screenshots/water_ice_heatmap.png) | ![](screenshots/signal_occultation_heatmap.png) |

---

# 📂 Project Structure

```text
Optimal-Moon-Landing
│
├── main.cpp
├── step1_csv.hpp
├── step2_stats.hpp
├── step3_pathfinding.hpp
├── step4_result.hpp
├── step5_visualization.hpp
│
├── visualization/
├── screenshots/
├── result.txt
├── candidates.txt
└── helper.txt
```

---

# 🚀 Build

```bash
g++ -O2 -std=c++17 main.cpp -o moon_landing
```

Run

```bash
./moon_landing
```

Windows

```bash
moon_landing.exe
```

---

# 📄 Sample Output

```text
Optimal Pair Found

Habitat
(303,264)

Mining
(311,200)

Path Length
72 cells

Combined Score
0.6863
```

---

# 🛠 Tech Stack

| Category | Technology |
|-----------|------------|
| Language | C++17 |
| Data Processing | CSV |
| Statistics | Prefix Sums |
| Algorithms | A*, Spatial Clustering |
| Analytics | ETL Pipeline |
| Visualization | BMP Generation |

---

# 🎯 Learning Outcomes

- Large-scale CSV processing
- Data cleaning & validation
- Statistical analysis
- ETL pipeline design
- Spatial analytics
- Graph search algorithms
- Data visualization
- Performance optimization

---

# 📜 License

This repository is intended for educational and portfolio purposes.
