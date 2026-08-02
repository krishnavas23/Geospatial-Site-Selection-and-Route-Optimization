# 🛰️ Geospatial Site Selection & Route Optimization

> **A high-performance C++ geospatial analytics pipeline that processes one million lunar terrain data points to identify the optimal habitat site, mining site, and shortest feasible power cable route using ETL, statistical analysis, spatial clustering, and A* pathfinding.**

<p align="center">

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-success)
![Algorithms](https://img.shields.io/badge/Algorithms-A*%20%7C%202D%20Prefix%20Sums-orange)
![Analytics](https://img.shields.io/badge/Data-Analytics-purple)
![Visualization](https://img.shields.io/badge/Visualization-BMP%20Heatmaps-red)

</p>

<p align="center">
<img src="screenshots/comparative_dashboard.png" width="900">
</p>

---

## 🌍 Overview

Planning infrastructure on the Moon is a complex geospatial optimization problem involving multiple environmental constraints rather than a single objective.

This project processes **four 500 × 500 lunar terrain datasets (1,000,000 total data points)** to recommend:

- 🏠 Optimal Habitat Site
- 🧊 Optimal Mining Site
- ⚡ Shortest Feasible Power Cable Route
- 📊 Interactive Analytical Dashboard
- 🎨 Terrain Heatmaps

The project follows a complete **ETL-style analytics workflow**, combining large-scale CSV processing, statistical analysis, optimization algorithms, and automated visualization.

```text
Extract
   │
   ▼
Clean & Validate
   │
   ▼
Transform
   │
   ▼
Analyze
   │
   ▼
Optimize
   │
   ▼
Visualize
```

---

# ✨ Why This Project?

Most pathfinding projects focus on finding a route.

This project goes much further.

It demonstrates how **large-scale geospatial datasets** can be transformed into actionable decisions through an end-to-end analytics pipeline.

The implementation combines concepts from:

- Data Analytics
- Data Engineering
- ETL Pipelines
- Statistical Analysis
- Geospatial Computing
- Optimization Algorithms
- Scientific Visualization

Instead of simply running A*, the system evaluates thousands of candidate locations, ranks them using statistical metrics, optimizes the final solution, and generates visual reports automatically.

---

# 🚀 Project Highlights

✅ Processed **1,000,000 terrain data points**

✅ Built entirely in **Modern C++17**

✅ No external libraries

✅ ETL-style data processing pipeline

✅ Automated data validation

✅ Statistical terrain analysis

✅ Spatial candidate clustering

✅ Multi-criteria optimization

✅ A* pathfinding with slope constraints

✅ Automatic heatmap generation

✅ Comparative analytical dashboard

---

# ✨ Key Features

### 📂 Data Processing
- Ingests four aligned **500 × 500 lunar terrain CSV datasets**
- Processes over **1,000,000 data points**
- Performs strict data validation with fail-fast error handling
- Ensures cross-layer dimensional consistency

### 🧹 Data Cleaning & Validation
- Whitespace trimming
- Numeric integrity verification
- Empty-cell detection
- Automated data quality reporting

### 📊 Statistical Analytics
- 5 × 5 sliding-window analysis
- Terrain roughness calculation using standard deviation
- Efficient mean computation using **2D Prefix Sums**
- Analysis of **246,016 spatial windows**

### 📍 Candidate Selection
- Top-K ranking based on resource quality
- Spatial clustering for geographic diversity
- Multi-criteria decision scoring

### 🛣 Route Optimization
- A* pathfinding algorithm
- Elevation-aware navigation
- Maximum elevation step constraint (22 m)
- Shortest feasible power cable routing

### 🎨 Visualization
- Pure C++ BMP image generation
- Terrain heatmaps
- Optimal site map
- Comparative analytical dashboard

---

# 📊 Project Workflow

```text
              Lunar Terrain CSV Files
                        │
        ┌───────────────┼───────────────┐
        │               │               │
        ▼               ▼               ▼
  Data Cleaning   Data Validation   Quality Checks
        │
        ▼
  Statistical Analysis
 (Mean + Standard Deviation)
        │
        ▼
 2D Prefix Sum Optimization
        │
        ▼
 Candidate Ranking (Top-K)
        │
        ▼
 Spatial Candidate Clustering
        │
        ▼
 Multi-Criteria Decision Scoring
        │
        ▼
 A* Route Optimization
        │
        ▼
 Report Generation
        │
        ▼
 Heatmaps + Dashboard
```

---

# 📈 Dataset Statistics

| Metric | Value |
|---------|------:|
| Input CSV Layers | 4 |
| Grid Size | 500 × 500 |
| Cells Per Layer | 250,000 |
| Total Data Points Processed | **1,000,000** |
| Sliding Window Size | 5 × 5 |
| Spatial Windows Evaluated | **246,016** |
| Candidate Pool | 3,000 |
| Pathfinding Algorithm | A* |
| Maximum Elevation Step | 22 m |

---

# 📋 Sample Output

```text
Optimal Pair Found with Combined Score: 0.6863

🏠 Habitat Site
Coordinates : (303,264)
Average Illumination : 56.56%
Terrain Roughness : 2.5955 m

🧊 Mining Site
Coordinates : (311,200)
Average Water-Ice Probability : 0.9510
Terrain Roughness : 2.0769 m

⚡ Power Cable Route
Length : 72 cells (7200 m)
```

### Final Optimization Score

```text
score =
0.5 × Habitat Illumination
+ 0.5 × Water-Ice Probability
− 0.001 × Path Length
```

The scoring function balances resource availability with construction cost, producing the optimal habitat–mining pair connected by the shortest feasible power route.

---

# 🧠 Skills Demonstrated

This project demonstrates practical experience with:

- Large-scale CSV Processing
- ETL Pipeline Design
- Data Cleaning & Validation
- Descriptive Statistics
- Geospatial Data Analytics
- Multi-Criteria Decision Analysis
- Performance Optimization
- Graph Algorithms (A*)
- Scientific Visualization
- Modern C++17 Development

- ---

# 🧠 Algorithms Used

| Algorithm | Purpose |
|-----------|---------|
| **2D Prefix Sums** | O(1) sliding-window statistical queries |
| **Mean & Standard Deviation** | Terrain roughness analysis |
| **Top-K Ranking** | Shortlist high-quality candidate locations |
| **Spatial Clustering** | Ensure geographically diverse candidate selection |
| **Weighted Multi-Criteria Scoring** | Evaluate habitat–mining site pairs |
| **A* Search Algorithm** | Compute the shortest feasible power cable route |

---

# 📸 Project Outputs

## 📊 Comparative Dashboard

<p align="center">
<img src="screenshots/comparative_dashboard.png" width="900">
</p>

The dashboard consolidates the complete analysis into a single view, including terrain statistics, selected sites, and generated visualizations.

---

## 🛰️ Optimal Habitat & Mining Sites

<p align="center">
<img src="screenshots/optimal_sites_map.png" width="750">
</p>

The generated map highlights:

- 🟢 Optimal Habitat Site
- 🔷 Optimal Mining Site
- 🟡 Shortest Feasible Power Cable Route
- 🌞 Illumination Heatmap Background

---

## 🌍 Terrain Heatmaps

| Elevation | Illumination |
|:---------:|:------------:|
| ![](screenshots/elevation_heatmap.png) | ![](screenshots/illumination_heatmap.png) |

| Water-Ice | Signal Occultation |
|:---------:|:------------------:|
| ![](screenshots/water_ice_heatmap.png) | ![](screenshots/signal_occultation_heatmap.png) |

The heatmaps are generated automatically using a custom **BMP renderer written entirely in C++**, without relying on external visualization libraries.

---

# 📂 Repository Structure

```text
Geospatial-Site-Selection-and-Route-Optimization/
│
├── src/
│   ├── main.cpp
│   ├── step1_csv.hpp
│   ├── step2_stats.hpp
│   ├── step3_pathfinding.hpp
│   ├── step4_result.hpp
│   └── step5_visualization.hpp
│
├── data/
│   ├── elevation.csv
│   ├── illumination.csv
│   ├── water_ice.csv
│   └── signal_occultation.csv
│
├── screenshots/
│
├── outputs/
│
├── README.md
└── LICENSE
```

---

# 📥 Input Datasets

| Dataset | Description |
|---------|-------------|
| `elevation.csv` | Terrain elevation values (meters) |
| `illumination.csv` | Solar illumination ratio |
| `water_ice.csv` | Water-ice probability |
| `signal_occultation.csv` | Communication obstruction values |

Each dataset represents a **500 × 500** spatial raster covering the same region of the lunar surface.

---

# 💻 Tech Stack

| Category | Technology |
|----------|------------|
| **Language** | Modern C++17 |
| **Data Processing** | CSV Parsing |
| **Data Engineering** | ETL Pipeline |
| **Data Validation** | Custom Validation Engine |
| **Analytics** | Mean, Standard Deviation |
| **Optimization** | 2D Prefix Sums, Spatial Clustering |
| **Pathfinding** | A* Search |
| **Visualization** | Pure C++ BMP Rendering |

---

# 🎯 Skills Demonstrated

### Data Analytics
- ETL Pipeline Development
- CSV Processing
- Data Cleaning
- Data Validation
- Descriptive Statistics
- Data Quality Assessment

### Algorithms
- Prefix Sum Optimization
- Graph Search (A*)
- Spatial Clustering
- Multi-Criteria Decision Analysis

### Software Engineering
- Modular C++ Design
- Performance Optimization
- File Handling
- Automated Report Generation
- Visualization Pipeline

---

# 🚀 Build & Run

### Requirements

- C++17 compatible compiler
- No external libraries required

### Compile

```bash
g++ -std=c++17 -O2 src/main.cpp -o moon_landing
```

### Run

#### Windows

```bash
moon_landing.exe
```

#### Linux / macOS

```bash
./moon_landing
```

---

# 🎓 Learning Outcomes

This project demonstrates practical implementation of:

- Large-scale data processing
- ETL pipeline architecture
- Data validation strategies
- Statistical analytics
- Geospatial optimization
- Graph algorithms
- Scientific visualization
- Performance-focused C++ programming

---

# 📜 License

This project is released for **educational, research, and portfolio purposes**.

If you found this project useful, consider giving it a ⭐ on GitHub!
