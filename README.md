<div align="center">

# 🛰️ Optimal Moon Landing

### Geospatial Site Selection & Route Optimization

*A geospatial data analytics pipeline built in C++ that processes high-resolution lunar terrain datasets to identify the optimal habitat site, mining site, and shortest feasible power cable route.*

![Comparative Dashboard](screenshots/comparative_dashboard.png)

![C++](https://img.shields.io/badge/C++-17-blue?style=for-the-badge&logo=c%2B%2B)
![CSV](https://img.shields.io/badge/Data-CSV-success?style=for-the-badge)
![Algorithms](https://img.shields.io/badge/A*-Pathfinding-orange?style=for-the-badge)
![Analytics](https://img.shields.io/badge/Data-Analytics-purple?style=for-the-badge)

</div>

---

# 📖 Overview

This project simulates a **lunar mission planning system** by analyzing four large geospatial datasets representing terrain elevation, illumination, water-ice probability, and communication visibility.

The pipeline performs:

- 📂 CSV Processing & Validation
- 🧹 Data Cleaning
- 📊 Statistical Analysis
- 📍 Candidate Site Selection
- 🛰️ Multi-Criteria Optimization
- 🛣️ A* Route Planning
- 🎨 Heatmap & Dashboard Generation

The complete solution is implemented entirely in **modern C++17** without using external GIS or visualization libraries.

---

# ✨ Key Features

- ✅ Processes **1,000,000+ terrain values**
- ✅ CSV Parsing & Data Validation
- ✅ ETL-style Data Processing Pipeline
- ✅ Sliding Window Statistics using **2D Prefix Sums**
- ✅ Terrain Roughness Analysis (Standard Deviation)
- ✅ Spatial Clustering for Candidate Diversity
- ✅ Multi-Criteria Site Scoring
- ✅ A* Pathfinding with 22 m Elevation Constraint
- ✅ Automatic BMP Heatmap Generation
- ✅ Interactive Comparative Dashboard

---

# 📊 Demo Results

| Metric | Value |
|---------|-------|
| 🏠 Habitat Site | **(303, 264)** |
| ⛏️ Mining Site | **(311, 200)** |
| ☀️ Habitat Illumination | **56.56%** |
| 💧 Water-Ice Probability | **0.9510** |
| 🛣️ Route Length | **72 Cells (~7200 m)** |
| ⭐ Combined Score | **0.6863** |

### Scoring Formula

```text
score = 0.5 × illumination
      + 0.5 × water_ice
      - 0.001 × path_length
```

---

# 🖼️ Visual Results

## Comparative Dashboard

![Comparative Dashboard](screenshots/comparative_dashboard.png)

---

## Optimal Habitat & Mining Sites

Green = Habitat

Cyan = Mining

Yellow = Optimal Route

![Optimal Sites](screenshots/optimal_sites_map.png)

---

## Terrain Heatmaps

| Elevation | Illumination |
|:---------:|:------------:|
| ![](screenshots/elevation_heatmap.png) | ![](screenshots/illumination_heatmap.png) |

| Water-Ice | Signal Occultation |
|:---------:|:------------------:|
| ![](screenshots/water_ice_heatmap.png) | ![](screenshots/signal_occultation_heatmap.png) |

---

# ⚙️ Pipeline Overview

```text
CSV Files
     │
     ▼
Data Cleaning & Validation
     │
     ▼
2D Prefix Sum Statistics
     │
     ▼
Candidate Ranking
     │
     ▼
Spatial Clustering
     │
     ▼
Multi-Criteria Optimization
     │
     ▼
A* Pathfinding
     │
     ▼
Result Generation
     │
     ├── result.txt
     ├── candidates.txt
     └── visualization/
```

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
├── elevation.csv
├── illumination.csv
├── water_ice.csv
├── signal_occultation.csv
│
├── screenshots/
├── README.md
├── .gitignore
└── LICENSE
```

---

# 🚀 Build & Run

### Requirements

- C++17 Compiler
- No External Libraries

### Compile

```bash
g++ -std=c++17 -O2 main.cpp -o moon_landing
```

### Run

**Linux / macOS**

```bash
./moon_landing
```

**Windows**

```bash
moon_landing.exe
```

---

# 📁 Generated Outputs

| Output | Description |
|----------|------------|
| `result.txt` | Final optimal habitat & mining report |
| `candidates.txt` | Ranked candidate locations |
| `visualization/*.bmp` | Generated terrain heatmaps |
| Dashboard | Automatically generated comparison dashboard |

---

# 📊 Input Data

| Dataset | Description |
|----------|------------|
| elevation.csv | Terrain Elevation |
| illumination.csv | Solar Illumination |
| water_ice.csv | Water-Ice Probability |
| signal_occultation.csv | Communication Visibility |

Each dataset contains a **500 × 500** raster grid representing approximately **250,000 spatial cells**.

---

# 🧠 Algorithms Used

- CSV Parsing
- Data Cleaning & Validation
- ETL Processing
- Sliding Window Statistics
- 2D Prefix Sums
- Standard Deviation
- Spatial Clustering
- Multi-Criteria Optimization
- A* Search Algorithm

---

# 💻 Tech Stack

| Category | Technology |
|-----------|------------|
| Language | C++17 |
| Data Processing | CSV |
| Analytics | Statistics |
| Algorithms | A* Search, Prefix Sums |
| Visualization | Pure C++ BMP Generator |

---

# 📄 Sample Output

```text
Optimal Pair Found with Combined Score: 0.6863

Optimal Habitat Site
Coordinates : (303,264)

Average Illumination : 56.56%

Optimal Mining Site
Coordinates : (311,200)

Average Water-Ice Probability : 0.9510

Path Length : 72 Cells (7200 m)
```

---

# 🌟 Highlights

- 📊 Geospatial Data Analytics
- 🛰️ Spatial Site Selection
- 📈 Statistical Analysis
- 🧮 Algorithm Design
- 🗺️ Route Optimization
- 🎨 Data Visualization
- ⚡ Modern C++17

---

# 📜 License

This project is intended for **educational, research, and portfolio purposes**.

If you find this project useful, consider giving it a ⭐ on GitHub.
