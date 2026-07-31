# Optimal Moon Landing — Complete Project Documentation

> **Purpose of this document:** A beginner-friendly, interview-ready guide to every part of this C++ project. Everything below is inferred directly from the source code and data files unless explicitly marked as uncertain.

> **📖 How to study:** Read [Section 22 — Complete Learning Guide](#22-complete-learning-guide-learn-the-project-from-scratch) for the recommended file-by-file learning order, self-test questions after every file, a final exam, and a full answer key. Combined with Sections 1–21, this README is designed so you can explain **every part** of the project in a technical interview without looking at the code.

---

## Table of Contents

1. [Project Overview](#1-project-overview)
2. [Architecture](#2-architecture)
3. [Project Flow](#3-project-flow)
4. [Detailed Code Explanation](#4-detailed-code-explanation)
5. [Algorithms Used](#5-algorithms-used)
6. [Data Structures Used](#6-data-structures-used)
7. [Mathematical Concepts](#7-mathematical-concepts)
8. [Input and Output](#8-input-and-output)
9. [Major Features](#9-major-features)
10. [Challenges Faced](#10-challenges-faced)
11. [Design Decisions](#11-design-decisions)
12. [Learning Outcomes](#12-learning-outcomes)
13. [Interview Preparation](#13-interview-preparation)
14. [Resume Explanation](#14-resume-explanation)
15. [Future Improvements](#15-future-improvements)
16. [Code Quality Review](#16-code-quality-review)
17. [File Dependency Map](#17-file-dependency-map)
18. [Execution Walkthrough](#18-execution-walkthrough)
19. [Key Takeaways](#19-key-takeaways)
20. [README Quality](#20-readme-quality)
21. [Interview Cheat Sheet](#interview-cheat-sheet)
22. [Complete Learning Guide (Learn From Scratch)](#22-complete-learning-guide-learn-the-project-from-scratch)
    - [Recommended README Reading Order](#recommended-readme-reading-order)
    - [Step 0: The Data Files](#step-0-the-data-files-before-any-code)
    - [File 1: step1_csv.hpp](#file-1-step1_csvhpp)
    - [File 2: step2_stats.hpp](#file-2-step2_statshpp)
    - [File 3: step3_pathfinding.hpp](#file-3-step3_pathfindinghpp)
    - [File 4: step4_result.hpp](#file-4-step4_resulthpp)
    - [File 5: main.cpp](#file-5-maincpp)
    - [Final Exam](#final-exam--tie-everything-together)
    - [Interview Script (Memorize)](#your-interview-script-memorize-this-flow)
    - [Answer Key](#answer-key-for-all-self-test-questions)

---

# 1. Project Overview

## What is this project?

This is a **C++ command-line application** that analyzes a **500×500 lunar terrain grid** and recommends:

1. An **optimal habitat site** (high solar illumination, smoother terrain)
2. An **optimal mining site** (high water-ice probability, smoother terrain)
3. A **feasible power-cable route** between them (shortest path respecting slope limits)

The program reads four CSV raster maps, scores thousands of candidate locations, runs pathfinding between promising pairs, and writes the best result to `result.txt`.

## What real-world problem does it solve?

Permanent lunar bases need **sunlight for power** and **water ice for life support and fuel**. Those resources are rarely co-located on the Moon. This tool models a practical planning question:

> *"Where should we build the habitat and the mining outpost, and how do we connect them with a cable across terrain that rovers can actually traverse?"*

## Why would someone build this?

| Motivation | Explanation |
|---|---|
| Mission planning | Automate site selection from satellite/sensor data |
| Multi-criteria optimization | Balance illumination, water, terrain roughness, and distance |
| Constraint-aware routing | Ensure paths respect maximum slope (rover/cable feasibility) |
| Scalability | Reduce a 250,000-cell search space to a tractable candidate set |

## Who are the intended users?

Based on the code (not documented elsewhere), likely users include:

- **Space mission planners** or researchers doing lunar site analysis
- **Students / engineers** learning grid algorithms (prefix sums, A*, clustering)
- **Hackathon or competition participants** solving an "optimal moon landing" problem

*Note: No explicit user documentation exists in the repository beyond a one-line README.*

## What is the main objective?

**Maximize a combined score** for a habitat–mining pair:

```
score = 0.5 × habitat_illumination + 0.5 × mining_water_ice − 0.001 × path_length
```

Subject to: a valid A* path exists between the two sites where adjacent cells differ in elevation by at most **22 meters**.

---

### Simple explanation

Imagine a Moon map split into 500×500 squares. Each square has elevation, sunlight level, and water-ice chance. The program:

1. Finds the best sunny spots for a base
2. Finds the best icy spots for mining
3. Checks whether you can drive a cable between each pair without hitting cliffs
4. Picks the pair with the best overall score

### Technical explanation

The pipeline is a **four-stage geospatial optimization**:

```
CSV Load → 2D Prefix Sums → Candidate Selection (top-K + spatial clustering)
         → Pairwise Scoring with A* pathfinding → Result output
```

- **Habitat metric:** mean illumination in a 5×5 window; **roughness** = elevation standard deviation in that window
- **Mining metric:** mean water-ice probability in a 5×5 window; same roughness from elevation
- **Pathfinding:** A* on a 4-connected grid with unit edge cost and a **22 m slope constraint**
- **Search pruning:** optimistic upper-bound scoring + cap of 2,000 A* evaluations

---

# 2. Architecture

## Folder structure

```
Optimal-Moon-Landing--main/
├── main.cpp                  # Entry point, orchestration, pair evaluation loop
├── step1_csv.hpp             # CSV loading and sample printing
├── step2_stats.hpp           # Prefix sums, candidate selection, clustering
├── step3_pathfinding.hpp     # A* pathfinding with slope constraint
├── step4_result.hpp          # Scoring function and result file writer
├── elevation.csv             # 500×500 elevation map (meters)
├── illumination.csv          # 500×500 illumination fractions (0–1)
├── water_ice.csv             # 500×500 water-ice probability (0–1)
├── signal_occultation.csv    # 500×500 signal data (loaded but unused)
├── candidates.txt            # Generated: top 20 habitat + mining candidates
├── result.txt                # Generated: optimal pair and path summary
└── README.md                 # This file
```

*There is no `Makefile`, `CMakeLists.txt`, or package manager configuration in the repository.*

## Purpose of every folder

This project is **flat** — all files live in the root directory. No subfolders exist.

## Purpose of every important file

| File | Purpose | Why it exists |
|---|---|---|
| `main.cpp` | Program entry, wires all modules, runs pair search | Central orchestrator |
| `step1_csv.hpp` | Parse rectangular CSV grids into `vector<vector<float>>` | Input layer |
| `step2_stats.hpp` | Fast 5×5 statistics, candidate ranking, spatial deduplication | Reduces search space |
| `step3_pathfinding.hpp` | A* shortest path with slope feasibility | Models cable/rover routing |
| `step4_result.hpp` | Score formula and formatted output | Output layer |
| `elevation.csv` | Terrain height per cell | Slope checks + roughness |
| `illumination.csv` | Solar exposure per cell | Habitat site quality |
| `water_ice.csv` | Ice deposit probability per cell | Mining site quality |
| `signal_occultation.csv` | Communication obstruction data | **Loaded only; not used in logic** |
| `candidates.txt` | Debug/inspection output | Human-readable shortlist |
| `result.txt` | Final answer | Primary deliverable |

## How files communicate

```
┌─────────────┐
│  main.cpp   │
└──────┬──────┘
       │ #includes
       ├──────────────────┬──────────────────┬─────────────────┐
       ▼                  ▼                  ▼                 ▼
 step1_csv.hpp      step2_stats.hpp    step3_pathfinding.hpp  step4_result.hpp
       │                  ▲                                      │
       │                  │                                      │
       │            (step4 includes step2)                       │
       ▼                  ▼                                      ▼
  *.csv files      Candidate structs                      result.txt
                   Prefix2D, clustering                   candidates.txt
```

**Data flow:**

```
CSV files ──load_csv()──► float grids ──Prefix2D──► prefix tables
                                              │
                                              ▼
                                    select_candidates() × 2
                                              │
                                              ▼
                                    hab_pool, min_pool (sorted)
                                              │
                         ┌────────────────────┴────────────────────┐
                         ▼                                         ▼
              write_candidates_to_file()                    nested pair loop
                                                                    │
                                                                    ▼
                                                          astar_path() per pair
                                                                    │
                                                                    ▼
                                                          score_pair() → write_result()
```

## Execution flow (ASCII diagram)

```
main()
   │
   ├─► load_csv × 4  ─────────────────────────► 4 × (500×500) grids
   │
   ├─► print_sample × 4  ───────────────────────► console preview
   │
   ├─► Prefix2D(illum), Prefix2D(water), Prefix2D(elev)
   │
   ├─► select_candidates(illum, elev) ──────────► hab_pool
   ├─► select_candidates(water, elev) ──────────► min_pool
   │
   ├─► sort both pools by (avg desc, rough asc)
   ├─► write_candidates_to_file()
   ├─► print top 5 of each pool
   │
   ├─► FOR hi in 0..49, mi in 0..49 (max 2000 A* runs):
   │       optimistic prune
   │       astar_path(habitat → mining)
   │       score_pair()
   │       track best
   │
   └─► write_result("result.txt") or fallback message
```

---

# 3. Project Flow

Read this as the story of one program run.

## Step 1 — Program starts

Execution begins at `main()` in `main.cpp`. I/O is optimized with `ios::sync_with_stdio(false)` and `cin.tie(nullptr)`.

## Step 2 — Load the lunar maps

Four hardcoded filenames are loaded:

```cpp
auto elevation = load_csv("elevation.csv");
auto illumination = load_csv("illumination.csv");
auto water_ice = load_csv("water_ice.csv");
auto signal_occultation = load_csv("signal_occultation.csv");
```

`load_csv` reads line-by-line, splits on commas, converts to `float`, validates rectangular shape, and **exits the program** on any error.

Grid dimensions are derived: `R = elevation.size()`, `C = elevation[0].size()` → **500 × 500**.

## Step 3 — Preview the data

`print_sample()` prints the first 5×5 corner of each map to stdout so you can sanity-check values.

## Step 4 — Build prefix-sum tables

Three `Prefix2D` objects are constructed:

- `P_illum` from illumination
- `P_water` from water_ice
- `P_elev` from elevation

Each stores two tables: `S` (sum) and `SS` (sum of squares) for O(1) rectangle queries.

## Step 5 — Select habitat candidates

`select_candidates(P_illum, P_elev, R, C, K_keep=3000, clusterSize=25)`:

1. Slides a 5×5 window over every valid position (246,016 windows)
2. Computes mean illumination and elevation roughness per window
3. Stores center coordinate `(i+2, j+2)` for each window
4. Sorts all candidates by highest `avg`, tie-break lower `rough`
5. Keeps top 3,000
6. Spatial clustering: one pick per 25×25 cell region

Result: `hab_pool` — diverse high-illumination sites.

## Step 6 — Select mining candidates

Same process with `P_water` instead of `P_illum`. Result: `min_pool`.

## Step 7 — Sort and export shortlists

Both pools are re-sorted (same comparator), written to `candidates.txt` (top 20 each), and top 5 printed to console.

## Step 8 — Evaluate habitat–mining pairs

Constants:

| Constant | Value | Meaning |
|---|---|---|
| `NH` | `min(50, hab_pool.size())` | Habitat candidates to try |
| `NM` | `min(50, min_pool.size())` | Mining candidates to try |
| `MAX_EVAL` | 2000 | Maximum A* runs |

Nested loop over `(hi, mi)`:

1. **Optimistic bound:** `0.5×Hc.avg + 0.5×Mc.avg − 0.001×manhattan_distance`
   - If bound ≤ current `bestScore`, skip (branch-and-bound prune)
2. **A* path:** `astar_path(elevation, habitat, mining)`
   - Returns path length in cells, or `1e9` (INF) if no path
3. **Score:** `score_pair(Hc, Mc, path_len)`
4. Update best if score improves

Loop stops early if `evals >= MAX_EVAL`.

## Step 9 — Write the answer

If no valid pair: write fallback message to `result.txt`.

Otherwise: print combined score, call `write_result()` with best habitat, mining site, path length.

**Typical runtime:** ~15–20 seconds on a modern machine (dominated by A* calls).

---

# 4. Detailed Code Explanation

## 4.1 `main.cpp`

### Purpose

Orchestrates the full pipeline: load → analyze → select → pathfind → output.

### Key variables

| Variable | Type | Purpose |
|---|---|---|
| `elevation`, `illumination`, `water_ice`, `signal_occultation` | `vector<vector<float>>` | Input grids |
| `R`, `C` | `int` | Grid rows and columns |
| `P_illum`, `P_water`, `P_elev` | `Prefix2D` | Fast statistics |
| `hab_pool`, `min_pool` | `vector<Candidate>` | Filtered site lists |
| `bestScore` | `float` | Running maximum score (init `-1e9`) |
| `bestPath` | `int` | Path length of best pair |
| `bestHab`, `bestMin` | `Candidate` | Best sites found |
| `evals` | `int` | A* call counter |

### Important logic

- `signal_occultation` is loaded and printed but **never passed to any algorithm**
- Pair loop uses Manhattan distance only for pruning, not for actual scoring
- `path_len >= 1e8f` treats unreachable pairs as invalid (compares float to near-INF)

### `main()` walkthrough (line by line)

| Lines | What happens |
|---|---|
| 9–11 | Speed up C++ I/O |
| 14–17 | Load four CSV files; exit on failure |
| 19 | Read grid dimensions from elevation |
| 20–23 | Print 5×5 samples of each map |
| 28 | Build three prefix-sum structures |
| 31–32 | `K_keep=3000`, `clusterSize=25` |
| 34–35 | Generate habitat and mining candidate pools |
| 37–42 | Sort both pools: higher avg first, lower rough on ties |
| 45 | Write top 20 of each to `candidates.txt` |
| 47–56 | Print top 5 habitat and mining candidates |
| 59–60 | Limit pair search to top 50 each |
| 63–65 | Manhattan distance lambda for pruning |
| 67–71 | Initialize best trackers; `MAX_EVAL=2000` |
| 76–100 | Nested pair evaluation with pruning and early exit |
| 103–112 | Handle no-solution case |
| 114–117 | Print score, write `result.txt`, return 0 |

---

## 4.2 `step1_csv.hpp`

### Purpose

CSV parsing and data preview utilities.

### Functions

#### `load_csv(const string &path) → vector<vector<float>>`

| Aspect | Detail |
|---|---|
| **Parameters** | `path` — filename to open |
| **Returns** | 2D float array of all CSV values |
| **Errors** | Missing file, bad number, non-rectangular rows, empty file → `cerr` + `exit(1)` |

**Line-by-line logic:**

1. Open file; fail fast if missing
2. For each non-empty line:
   - Split by `,`
   - Trim whitespace from each cell
   - Empty cell → `0.0f`
   - Non-numeric cell → error and exit
   - Enforce same column count on every row
3. Return accumulated rows

#### `print_sample(A, name, k=5)`

Prints map name, full dimensions, and top-left `k×k` submatrix with 2 decimal places.

---

## 4.3 `step2_stats.hpp`

### Purpose

Statistical analysis on grids, candidate generation, and spatial filtering.

### Struct: `Candidate`

```cpp
struct Candidate {
    int r, c;      // grid coordinates (row, col)
    float avg;     // mean illumination OR water-ice in 5×5 window
    float rough;   // elevation std dev in same window (terrain roughness)
};
```

### Struct: `Prefix2D`

| Member | Purpose |
|---|---|
| `R`, `C` | Grid dimensions |
| `S` | 2D prefix sum of values |
| `SS` | 2D prefix sum of squared values |

#### Constructor `Prefix2D(M)`

Builds `(R+1)×(C+1)` prefix tables using row-cumulative approach:

```
For each cell (i,j) in 1-based prefix space:
  S[i][j]  = S[i-1][j]  + sum of row i from col 1..j
  SS[i][j] = SS[i-1][j] + sum of squares of row i from col 1..j
```

#### `rect_sum(P, r1, c1, r2, c2)`

Standard **inclusion-exclusion** on prefix table (converts 0-based input to 1-based internally):

```
sum = P[r2][c2] - P[r1-1][c2] - P[r2][c1-1] + P[r1-1][c1-1]
```

#### `block_stats5x5(r, c) → pair<float,float>`

For 5×5 block starting at `(r,c)`:

1. `sum` = total of 25 cells
2. `sum2` = sum of squares
3. `mean = sum / 25`
4. `variance = max(0, sum2/25 - mean²)`
5. Return `(mean, sqrt(variance))`

The `max(0, ...)` guards against tiny negative variance from floating-point error.

#### `select_candidates(P_metric, P_elev, R, C, K_keep, clusterSize)`

**Parameters:**

| Param | Role |
|---|---|
| `P_metric` | Illumination or water prefix table |
| `P_elev` | Elevation prefix table (for roughness) |
| `K_keep` | Max candidates before clustering |
| `clusterSize` | Grid bucket size for spatial dedup |

**Returns:** `vector<Candidate>` after top-K + one-per-cluster filtering.

**Algorithm:**

```
1. For i in 0..R-5, j in 0..C-5:
     avg, _ = P_metric.block_stats5x5(i,j)
     _, rough = P_elev.block_stats5x5(i,j)
     push Candidate{i+2, j+2, avg, rough}

2. Sort by avg descending, rough ascending
3. Resize to K_keep
4. Cluster grid into ceil(R/25) × ceil(C/25) buckets
5. Walk sorted list; first candidate in each bucket wins
6. Return picked list
```

#### `write_candidates_to_file(hab, mining, topN=20)`

Writes formatted top-N lists to `candidates.txt`.

---

## 4.4 `step3_pathfinding.hpp`

### Purpose

Find shortest feasible path on elevation grid respecting slope limits.

### Struct: `AStarNode`

```cpp
struct AStarNode {
    int r, c;
    float f;   // f = g + h (total estimated cost)
    float g;   // cost from start
    bool operator>(const AStarNode& o) const { return f > o.f; }
};
```

`operator>` enables `priority_queue` as a **min-heap** via `greater<AStarNode>`.

### Helper functions

#### `inb(r, c, R, C)`

Returns true if `(r,c)` is inside grid bounds.

#### `slope_ok(h1, h2, max_slope=22.0f)`

Returns `|h1 - h2| <= 22`. Adjacent move allowed only if elevation change ≤ 22 meters.

### `astar_path(elev, start, goal) → float`

| Aspect | Detail |
|---|---|
| **Parameters** | `elev` grid, `start` and `goal` as `(row,col)` pairs |
| **Returns** | Path length in cells, or `1e9` if unreachable |
| **Movement** | 4-connected (up/down/left/right), cost 1 per step |
| **Heuristic** | Manhattan distance to goal (admissible for unit-cost 4-neighbor grid) |

**Line-by-line logic:**

1. Flatten grid index: `idx(r,c) = r*C + c`
2. Initialize `g[]` to INF, `seen[]` to false
3. Push start with `g=0`, `f=H(start)`
4. While PQ not empty:
   - Pop lowest `f` node; skip if already seen
   - Mark seen; if at goal, return `g`
   - For each of 4 neighbors:
     - Skip if out of bounds
     - Skip if `slope_ok` fails
     - If `new_g < g[neighbor]`, update and push
5. Return INF if queue exhausted

**Note:** This implementation does **not** reconstruct the actual path — only its length.

---

## 4.5 `step4_result.hpp`

### Purpose

Scoring and formatted output.

### `score_pair(Hc, Mc, path_len) → float`

```cpp
return 0.5f * Hc.avg + 0.5f * Mc.avg - 0.001f * float(path_len);
```

Equal weight to habitat illumination and mining water; path length penalized at 0.001 per cell.

### `write_result(filename, score, habitat, mining, path_len)`

Writes human-readable report:

- Combined score (4 decimal places)
- Habitat coordinates, illumination as **percentage** (`avg * 100`)
- Mining coordinates, water-ice probability
- Both roughness values in meters
- Path length in cells and meters (`cells × 100`)

*The ×100 meter conversion is hardcoded in output formatting — implying each cell represents 100 m × 100 m (inferred from code, not documented elsewhere).*

---

# 5. Algorithms Used

## 5.1 CSV Parsing (Sequential I/O)

| Property | Value |
|---|---|
| **Why chosen** | Simple, no external libraries |
| **Time** | O(R × C) |
| **Space** | O(R × C) |
| **Alternatives** | `pandas` (Python), `fast-cpp-csv-parser`, binary formats |
| **Pros** | Zero dependencies, human-readable data |
| **Cons** | Slow for huge maps; loads entire file into memory |

## 5.2 2D Prefix Sum (Summed-Area Table)

| Property | Value |
|---|---|
| **Why chosen** | Need mean/variance over many 5×5 windows (246K+ queries) |
| **How** | Precompute cumulative sums; rectangle query in O(1) |
| **Time** | Build: O(R×C); Query: O(1) |
| **Space** | O(R×C) per prefix table (2 tables × 3 maps) |
| **Alternatives** | Naive 5×5 loop per window: O(25 × R × C) — ~25× slower |
| **Best/Avg/Worst** | All O(R×C) build, O(1) query |

## 5.3 Sliding Window Mean & Standard Deviation

| Property | Value |
|---|---|
| **Why chosen** | Smooths noise; 5×5 = 25 cells = 2.5 km × 2.5 km if cell = 100 m |
| **Formula** | `σ = sqrt(E[X²] - E[X]²)` via prefix sums |
| **Time** | O(1) per window after preprocessing |
| **Alternatives** | Gaussian blur, larger windows, median filtering |
| **Pros** | Fast, statistically meaningful roughness proxy |
| **Cons** | Fixed window size; edges excluded (need 5×5 fit) |

## 5.4 Top-K Selection via Sort

| Property | Value |
|---|---|
| **Why chosen** | Simplicity; K=3000 is manageable |
| **Time** | O(N log N) where N ≈ 246,016 |
| **Space** | O(N) |
| **Alternatives** | `nth_element` + partial sort: O(N) average; heap for streaming top-K |
| **Pros** | Easy to implement |
| **Cons** | Full sort is overkill when only top 3000 needed |

## 5.5 Spatial Clustering (Grid Bucketing)

| Property | Value |
|---|---|
| **Why chosen** | Prevent all candidates clustering in one small region |
| **How** | Divide map into 25×25 cell buckets; keep first (best) per bucket |
| **Time** | O(K) after sort |
| **Space** | O(num_buckets) = O((R/25) × (C/25)) ≈ 400 booleans |
| **Alternatives** | K-means, max-min diversity, simulated annealing |
| **Pros** | O(1) bucket lookup, geographic spread |
| **Cons** | May discard better sites in same bucket; bucket size is arbitrary |

## 5.6 A* (A-Star) Pathfinding

| Property | Value |
|---|---|
| **Why chosen** | Optimal shortest path with heuristic guidance; handles slope constraints naturally |
| **How** | Best-first search with `f = g + h`; Manhattan `h` |
| **Time** | O(E log V) with binary heap; V = R×C = 250,000 |
| **Space** | O(V) for `g` and `seen` arrays |
| **Best case** | O(path_length × log V) when heuristic is very informative |
| **Average case** | Explores substantially fewer nodes than Dijkstra |
| **Worst case** | O(V log V) — degrades to Dijkstra if heuristic unhelpful |
| **Alternatives** | Dijkstra (no heuristic), BFS (unweighted), JPS, RRT for continuous terrain |
| **Pros** | Optimal for unit-cost grid; admissible heuristic |
| **Cons** | No path reconstruction; duplicate PQ entries; slope can block many routes |

## 5.7 Branch-and-Bound Pruning (Optimistic Score)

| Property | Value |
|---|---|
| **Why chosen** | Up to 50×50 = 2,500 pairs; each A* is expensive |
| **How** | Skip pair if `optimistic_score ≤ bestScore` where optimistic uses Manhattan distance (underestimates path penalty) |
| **Time saved** | Skips A* on provably suboptimal pairs |
| **Correctness** | Valid because Manhattan ≤ actual path length → optimistic ≥ true score |
| **Alternatives** | Pre-sort pairs by Manhattan dist; spatial indexing |

## 5.8 Evaluation Cap (`MAX_EVAL = 2000`)

| Property | Value |
|---|---|
| **Why chosen** | Hard runtime limit |
| **Trade-off** | May miss optimal pair if it's beyond the 2000th evaluation |
| **Risk** | Nested loop order is fixed (habitat index outer); later pairs may not be evaluated |

---

# 6. Data Structures Used

| Data Structure | Where Used | Why Appropriate |
|---|---|---|
| `vector<vector<float>>` | CSV grids | Natural 2D raster representation; dynamic sizing |
| `vector<vector<double>>` | Prefix tables `S`, `SS` | Double precision reduces accumulation error in sums |
| `struct Candidate` | Site records | Groups related fields (position, avg, rough) |
| `struct Prefix2D` | Statistics engine | Encapsulates prefix tables + query methods |
| `struct AStarNode` | A* priority queue | Bundles position + costs for heap ordering |
| `vector<float>` (flat) | `g[]` cost array | O(1) index via `r*C+c`; cache-friendly |
| `vector<char>` | `seen[]` visited flags | Compact boolean storage |
| `priority_queue` (min-heap) | A* open set | Efficient extract-min for lowest `f` |
| `vector<vector<bool>>` | Cluster `used` grid | Fast O(1) bucket occupancy check |
| `pair<int,int>` | Start/goal coordinates | Lightweight 2D point |
| `lambda` functions | Comparators, heuristics, index | Local logic without extra named functions |

### Why not `map` or `set`?

- Grid indices are dense integers 0..R×C-1 → **arrays beat hash maps**
- No need for ordered traversal of keys → **no `map`**

### Why not `queue` (BFS)?

- All edges cost 1 but **A* with Manhattan heuristic** explores fewer nodes while remaining optimal.

---

# 7. Mathematical Concepts

## 7.1 Prefix sum (inclusion–exclusion)

For prefix table `P` where `P[i][j] = sum of rectangle (0,0) to (i-1,j-1)`:

```
rect_sum(r1,c1,r2,c2) = P[r2+1][c2+1] - P[r1][c2+1] - P[r2+1][c1] + P[r1][c1]
```

*(Code shifts indices internally by +1.)*

## 7.2 Mean of 5×5 block

```
μ = (1/25) × Σ xᵢ
```

Computed via one `rect_sum` on table `S`.

## 7.3 Population variance and standard deviation

```
σ² = (1/25) × Σ xᵢ² − μ²
σ  = √max(0, σ²)
```

`Σ xᵢ²` comes from `rect_sum` on table `SS`.

**Interpretation in this project:** σ of **elevation** = terrain roughness in meters.

## 7.4 Combined score function

```
Score = 0.5 × I_hab + 0.5 × W_min − 0.001 × L_path
```

Where:
- `I_hab` = mean illumination in [0, 1]
- `W_min` = mean water-ice probability in [0, 1]
- `L_path` = A* path length in cells

**Example (from actual output):**

```
Habitat (303,264): I = 0.5656
Mining  (311,200): W = 0.9510
Path length: 72 cells

Score = 0.5(0.5656) + 0.5(0.9510) - 0.001(72)
      = 0.2828 + 0.4755 - 0.072
      = 0.6863 ✓
```

## 7.5 Manhattan distance

```
d_manhattan((r1,c1), (r2,c2)) = |r1−r2| + |c1−c2|
```

Used as A* heuristic and pruning lower bound on path length.

## 7.6 Slope constraint

For adjacent cells with elevations `h₁`, `h₂`:

```
|h₁ − h₂| ≤ 22 meters  ⟹  move allowed
```

This is a **hard constraint** — not a soft penalty.

## 7.7 Optimistic pruning bound

```
optimistic = 0.5×I + 0.5×W − 0.001×d_manhattan
```

Since `L_path ≥ d_manhattan` (shortest path on grid ≥ Manhattan distance):

```
true_score ≤ optimistic
```

If `optimistic ≤ bestScore`, the pair cannot beat the current best → skip A*.

## 7.8 Cell-to-meter conversion (inferred)

Output writes: `path_len * 100` meters.

**Inferred:** each grid cell ≈ 100 m × 100 m → 500×500 grid ≈ 50 km × 50 km region.

*This is inferred from `write_result()` formatting, not stated in comments.*

---

# 8. Input and Output

## Expected input

Four CSV files in the **working directory** (same folder as the executable):

| File | Format | Observed range | Used? |
|---|---|---|---|
| `elevation.csv` | 500 rows × 500 comma-separated floats | approx −64 to +64 m | ✅ Yes |
| `illumination.csv` | 500 × 500 floats | 0.01 to ~0.11 (fraction) | ✅ Yes |
| `water_ice.csv` | 500 × 500 floats | 0.62 to 0.951 (probability) | ✅ Yes |
| `signal_occultation.csv` | 500 × 500 floats | 0.0 or 1.7–3.0 | ❌ Loaded only |

### Input format rules (from `load_csv`)

- Comma-separated values, one row per line
- All rows must have the **same number of columns**
- Empty cells → `0.0`
- Non-numeric cells → fatal error
- No header row

### Input NOT accepted by code

- Command-line arguments for file paths
- Interactive input via `stdin`
- Different grid sizes between files (not validated — would cause subtle bugs)

## Output format

### Console (`stdout`)

- 5×5 sample of each map with dimensions
- Top 5 habitat and mining candidates
- Combined score
- Status messages

### `candidates.txt`

Top 20 habitat and top 20 mining candidates with coordinates, avg metric, roughness.

### `result.txt`

```
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

## Sample walkthrough

**Question:** Why isn't habitat (129,424) with illumination 0.594 chosen?

Top habitat by illumination alone is (129,424) with avg 0.594. But the optimizer picks **pairs**, not isolated sites.

When paired with mining candidates via A*:

- High-illumination northern sites may have longer or blocked paths to the best mining zones
- Score subtracts `0.001 × path_length` — a 200-cell path costs 0.2 score points
- Slope constraints may make some pairs unreachable entirely

(303,264) with illum 0.5656 wins because its **combined score** with (311,200) is highest: great water (0.951), moderate illum (0.566), short path (72 cells).

---

# 9. Major Features

## Feature 1: Multi-map CSV ingestion

| Aspect | Detail |
|---|---|
| **What** | Loads 4 geospatial raster files into memory |
| **Why** | All analysis depends on gridded sensor/simulation data |
| **How** | Line-by-line parse, validate, store as `vector<vector<float>>` |
| **Files** | `step1_csv.hpp`, `main.cpp` |

## Feature 2: Fast 5×5 window statistics

| Aspect | Detail |
|---|---|
| **What** | O(1) mean and std dev for any 5×5 region |
| **Why** | 246K windows would be too slow with naive summing |
| **How** | Dual prefix tables (sum + sum-of-squares) |
| **Files** | `step2_stats.hpp` (`Prefix2D`) |

## Feature 3: Dual candidate pools (habitat vs mining)

| Aspect | Detail |
|---|---|
| **What** | Separate ranked lists optimized for different metrics |
| **Why** | Habitat and mining have conflicting spatial requirements |
| **How** | `select_candidates` called with illumination vs water prefix |
| **Files** | `main.cpp`, `step2_stats.hpp` |

## Feature 4: Spatial clustering / deduplication

| Aspect | Detail |
|---|---|
| **What** | At most one candidate per 25×25 cell region |
| **Why** | Top-K by score alone would pick many neighbors in same peak |
| **How** | Grid bucketing with `used[][]` boolean mask |
| **Files** | `step2_stats.hpp` |

## Feature 5: Slope-constrained A* pathfinding

| Aspect | Detail |
|---|---|
| **What** | Shortest path respecting 22 m max step elevation change |
| **Why** | Models rover/cable feasibility on lunar terrain |
| **How** | A* with neighbor filter via `slope_ok()` |
| **Files** | `step3_pathfinding.hpp` |

## Feature 6: Multi-objective pair scoring

| Aspect | Detail |
|---|---|
| **What** | Weighted sum of illumination, water, and path penalty |
| **Why** | Single metric can't capture trade-offs |
| **How** | `score_pair()` linear combination |
| **Files** | `step4_result.hpp` |

## Feature 7: Branch-and-bound pruning

| Aspect | Detail |
|---|---|
| **What** | Skip A* when optimistic bound can't beat current best |
| **Why** | A* is the performance bottleneck |
| **How** | Compare `0.5×I + 0.5×W − 0.001×manhattan` to `bestScore` |
| **Files** | `main.cpp` |

## Feature 8: Evaluation budget cap

| Aspect | Detail |
|---|---|
| **What** | Stop after 2,000 A* runs |
| **Why** | Bound worst-case runtime |
| **How** | `evals` counter with early `break` |
| **Files** | `main.cpp` |

## Feature 9: Candidate and result file export

| Aspect | Detail |
|---|---|
| **What** | Persist intermediate and final results to disk |
| **Why** | Inspection, reporting, reproducibility |
| **How** | `write_candidates_to_file()`, `write_result()` |
| **Files** | `step2_stats.hpp`, `step4_result.hpp` |

## Feature 10: Console data preview

| Aspect | Detail |
|---|---|
| **What** | Print 5×5 corner of each loaded map |
| **Why** | Quick sanity check that files loaded correctly |
| **How** | `print_sample()` |
| **Files** | `step1_csv.hpp` |

---

# 10. Challenges Faced

*Inferred from implementation patterns — not documented by the author.*

## Edge cases

| Challenge | How addressed |
|---|---|
| Non-rectangular CSV | Validate column count per row; exit on mismatch |
| Empty / missing files | `exit(1)` with error message |
| Negative variance from float error | `max(0.0, variance)` before `sqrt` |
| No feasible path between pair | A* returns INF; pair skipped |
| No valid pair at all | Fallback message in `result.txt` |
| Grid edges for 5×5 windows | Loop `i <= R-5`, `j <= C-5` only |

## Algorithm design

| Challenge | How addressed |
|---|---|
| 250K cells × 246K windows | Prefix sums for O(1) queries |
| Candidate redundancy | Top-3000 + spatial clustering |
| 2,500 possible pairs × costly A* | Optimistic pruning + 2000 eval cap |
| Water-ice values nearly identical at top | Roughness tie-breaker + path length in score |

## Performance

| Challenge | How addressed |
|---|---|
| A* on 250K nodes | Manhattan heuristic; flat arrays |
| Repeated A* calls | Pruning + eval cap |
| Full sort of 246K items | Acceptable (~instant); could use `nth_element` |

## Precision

| Challenge | How addressed |
|---|---|
| Sum accumulation error | `double` for prefix tables, `float` for grid values |
| Comparing INF sentinel | `1e9f` threshold for unreachable paths |

## Code organization

| Challenge | How addressed |
|---|---|
| Growing complexity | Split into 4 step headers by pipeline stage |
| Header-only functions | `inline` on all functions to avoid linker errors |

---

# 11. Design Decisions

## Modular step headers (`step1` – `step4`)

| Choice | Trade-off |
|---|---|
| ✅ Clear pipeline stages, easy to explain | ❌ Not true compilation units; everything included in `main.cpp` |
| ✅ No build system needed | ❌ `bits/stdc++.h` reduces portability |

## Header-only with `inline`

| Choice | Trade-off |
|---|---|
| ✅ Simple single-command compile | ❌ Longer compile if split across TUs later |
| ✅ No linker setup | ❌ Larger translation unit |

## 5×5 analysis window

| Choice | Trade-off |
|---|---|
| ✅ Smooths cell noise | ❌ Arbitrary size; not configurable |
| ✅ Center-point representation | ❌ Loses sub-cell precision |

## Separate habitat and mining pools

| Choice | Trade-off |
|---|---|
| ✅ Models real dual-site architecture | ❌ Pair search needed; can't optimize single site |
| ✅ Different metrics per role | ❌ Doesn't consider co-located combined base |

## Linear score function

| Choice | Trade-off |
|---|---|
| ✅ Simple, interpretable weights | ❌ Can't express non-linear preferences (e.g., minimum illumination threshold) |
| ✅ Fast to compute | ❌ Weights (0.5, 0.5, 0.001) are hardcoded |

## A* over BFS/Dijkstra

| Choice | Trade-off |
|---|---|
| ✅ Fewer node expansions with good heuristic | ❌ More complex; duplicate PQ entries |
| ✅ Optimal for unit-cost grid | ❌ No path reconstruction for visualization |

## `signal_occultation` loaded but unused

| Likely reason | Trade-off |
|---|---|
| Planned feature (communication quality) not implemented | ❌ Wasted I/O and memory (~1 MB) |
| Competition data provided but deprioritized | Could add comm score to habitat selection |

## What could have been done differently

- Use `nth_element` instead of full sort for top-K
- Reconstruct and export actual path coordinates
- Validate all 4 grids have identical dimensions
- Make weights and thresholds configurable via CLI args
- Integrate `signal_occultation` into habitat scoring
- Use consistent integer type for path length (avoid `1e8f` float comparison)

---

# 12. Learning Outcomes

## Programming

- C++17 features: structured bindings `auto [avg, rough] = ...`, lambdas, `inline` headers
- File I/O with error handling (`ifstream`, `ofstream`)
- I/O performance tuning (`sync_with_stdio`, `cin.tie`)

## Algorithms

- 2D prefix sums / summed-area tables
- Top-K selection and spatial bucketing
- A* pathfinding with admissible heuristics
- Branch-and-bound optimization

## Problem solving

- Reducing intractable search (250K cells) to manageable candidates (~hundreds)
- Multi-criteria decision making with weighted scores
- Constraint modeling (slope limits as hard filters)

## Debugging

- Sample printing for data validation
- Intermediate output (`candidates.txt`) for inspecting pipeline stages
- Sentinel values (INF) for unreachable states

## Software engineering

- Pipeline decomposition into logical modules
- Separation of I/O, algorithm, and output concerns

## Optimization

- Time–accuracy trade-offs (`MAX_EVAL` cap)
- Pruning strategies to avoid redundant computation
- Flat arrays vs 2D for cache performance in A*

## Mathematics

- Prefix sum inclusion–exclusion
- Variance via sum of squares
- Manhattan distance as admissible heuristic

## Project organization

- Flat project structure suitable for competitions/prototypes
- Data files co-located with code

---

# 13. Interview Preparation

## 20 Beginner Questions

### Q1: What does this project do?
**A:** It analyzes a 500×500 lunar terrain grid to find the best habitat site (high sunlight), best mining site (high water ice), and the shortest feasible cable path between them, then writes the result to a file.

### Q2: What language is it written in?
**A:** C++17, compiled as a single translation unit including four header files.

### Q3: What files does the program read?
**A:** `elevation.csv`, `illumination.csv`, `water_ice.csv`, and `signal_occultation.csv` — all expected in the working directory.

### Q4: What files does the program write?
**A:** `result.txt` (final answer) and `candidates.txt` (top 20 shortlists).

### Q5: Where does execution start?
**A:** In `main()` in `main.cpp`.

### Q6: What is a `Candidate`?
**A:** A struct with row `r`, column `c`, average metric value `avg` (illumination or water), and terrain `rough` (elevation standard deviation).

### Q7: What does `avg` represent for habitat sites?
**A:** The mean illumination value in a 5×5 window centered at `(r,c)`, ranging from 0 to 1.

### Q8: What does `rough` represent?
**A:** The standard deviation of elevation in the same 5×5 window — a proxy for terrain roughness in meters.

### Q9: How do you compile the project?
**A:** `g++ -O2 -std=c++17 main.cpp -o moon_landing` — no Makefile is provided.

### Q10: What is the grid size?
**A:** 500 rows × 500 columns, inferred from the CSV data and confirmed at runtime.

### Q11: What pathfinding algorithm is used?
**A:** A* (A-star) with Manhattan distance heuristic.

### Q12: What movements are allowed on the grid?
**A:** 4-connected: up, down, left, right (no diagonals).

### Q13: What is the slope constraint?
**A:** Adjacent cells can only be connected if their elevation difference is at most 22 meters.

### Q14: What is the scoring formula?
**A:** `0.5 × habitat_illumination + 0.5 × mining_water − 0.001 × path_length`.

### Q15: Why are there two candidate pools?
**A:** Habitat and mining optimize different metrics (sunlight vs water ice), so they're ranked separately then paired.

### Q16: What is `K_keep`?
**A:** 3000 — the number of top candidates kept before spatial clustering.

### Q17: What is `clusterSize`?
**A:** 25 — the bucket size (in cells) for spatial deduplication so candidates spread across the map.

### Q18: Is `signal_occultation.csv` used?
**A:** No. It's loaded and printed as a sample, but no algorithm uses it.

### Q19: What happens if a CSV file is missing?
**A:** The program prints an error to `stderr` and exits with code 1.

### Q20: What does the program print to the console?
**A:** Data samples, top 5 candidates per pool, combined score, and status messages.

---

## 20 Intermediate Questions

### Q1: How does `Prefix2D` achieve O(1) rectangle queries?
**A:** It precomputes cumulative sum tables. Any rectangle sum is computed with 4 lookups using inclusion-exclusion: `P[r2][c2] - P[r1-1][c2] - P[r2][c1-1] + P[r1-1][c1-1]`.

### Q2: Why are there two prefix tables (`S` and `SS`)?
**A:** `S` stores sums for computing the mean. `SS` stores sums of squares for computing variance: `σ² = E[X²] - E[X]²`.

### Q3: Why use `double` for prefix tables but `float` for the grid?
**A:** Prefix sums over 500 values accumulate rounding error. `double` reduces error in statistical computations; `float` saves memory for the raw grid.

### Q4: Explain the spatial clustering algorithm.
**A:** After sorting candidates by score, the map is divided into 25×25 cell buckets. Walking the sorted list, the first candidate in each bucket is kept; subsequent candidates in the same bucket are discarded. This ensures geographic diversity.

### Q5: Why is the candidate coordinate `(i+2, j+2)` for a window starting at `(i,j)`?
**A:** A 5×5 window from `(i,j)` to `(i+4,j+4)` has center at `(i+2, j+2)` — integer center of the block.

### Q6: Why does A* use Manhattan distance as the heuristic?
**A:** With 4-connected unit-cost moves, Manhattan distance never overestimates the true shortest path, making it **admissible**. A* with an admissible heuristic guarantees optimal path length.

### Q7: Explain the optimistic pruning in the pair loop.
**A:** Before running A*, compute `0.5×I + 0.5×W − 0.001×manhattan`. Since actual path ≥ Manhattan distance, true score ≤ optimistic. If optimistic ≤ current best, skip the expensive A* call.

### Q8: What is `MAX_EVAL` and why does it exist?
**A:** A cap of 2000 A* evaluations to bound runtime. The nested loop may not evaluate all 2500 pairs if this limit is hit first.

### Q9: Why might the top habitat by illumination not be in the final answer?
**A:** The optimizer maximizes **pair score**, not individual habitat quality. A slightly dimmer site with a shorter or feasible path to a great mining site can win.

### Q10: How many 5×5 windows are evaluated?
**A:** `(R-4) × (C-4) = 496 × 496 = 246,016` for a 500×500 grid.

### Q11: What does A* return when no path exists?
**A:** `1e9f` (INF). The pair loop treats `path_len >= 1e8f` as unreachable.

### Q12: Does A* reconstruct the actual path?
**A:** No. It only returns the path **length** (`cur.g` at goal). Parent pointers are not stored.

### Q13: What is the tie-breaking rule when sorting candidates?
**A:** Higher `avg` wins. If `avg` is equal, lower `rough` (smoother terrain) wins.

### Q14: Why is `bits/stdc++.h` used?
**A:** Competitive programming convenience — includes most standard library headers in one line. Not portable for production code.

### Q15: How is illumination displayed differently in output vs internal storage?
**A:** Stored as fraction (0.5656). `write_result` multiplies by 100 to show "56.56%".

### Q16: What is the inferred real-world size of each cell?
**A:** Output multiplies cell count by 100 for meters, implying ~100 m per cell → ~50 km × 50 km total area.

### Q17: What complexity class is the overall pipeline?
**A:** Dominated by O(evaluations × A* cost) ≈ O(2000 × V log V) in practice, plus O(N log N) for initial sorting where N = 246K.

### Q18: Why might many mining candidates have `avg = 0.951`?
**A:** The water-ice map appears to plateau at 0.951 in many regions. When averages tie, roughness and path length become the differentiators.

### Q19: What error handling exists in CSV loading?
**A:** Missing file, non-numeric value, non-rectangular rows, empty file — all cause `exit(1)`.

### Q20: How would you verify the program works correctly?
**A:** Compile and run; check `result.txt` against manual score calculation; verify path length ≥ Manhattan distance; test with modified CSVs where optimal is known.

---

## 20 Advanced Questions

### Q1: Is the optimistic pruning bound always safe?
**A:** Yes. Path length on a 4-connected grid with positive edge costs is always ≥ Manhattan distance. So `−0.001×manhattan` is ≥ `−0.001×path_length`, meaning optimistic ≥ true score. Skipping when `optimistic ≤ best` is safe.

### Q2: Can `MAX_EVAL` cause a suboptimal answer?
**A:** Yes. If the optimal pair would be evaluated after 2000 other A* calls, it's never considered. The outer loop breaks on `evals`, not on exhausting promising pairs.

### Q3: How would you integrate `signal_occultation`?
**A:** Add it to habitat scoring: e.g., `score = 0.4×illum + 0.1×(1-occultation) + 0.5×water − 0.001×path`. Or filter habitat candidates where occultation < threshold. Requires defining what occultation values mean (appears to be 0 = clear, ~1.7–3.0 = obstructed).

### Q4: What's wrong with comparing `path_len >= 1e8f` when `path_len` is `int`?
**A:** `astar_path` returns `float`, cast to `int` in main. The comparison `path_len >= 1e8f` mixes types. It works because INF is `1e9f` and int max is much smaller, but it's a code smell — should use consistent types and named constant.

### Q5: How would you reduce memory usage for prefix tables?
**A:** Three `Prefix2D` objects each hold 2 × (R+1) × (C+1) doubles ≈ 3 × 2 × 501² × 8 bytes ≈ 12 MB. Acceptable. Could compute elevation roughness once and share, or use single-precision prefixes.

### Q6: Alternative to grid bucketing for spatial diversity?
**A:** Maximal Marginal Relevance (MMR), farthest-point sampling, or explicitly adding a diversity penalty term to the score. K-means on candidate coordinates with per-cluster top-1.

### Q7: When does A* degrade to Dijkstra?
**A:** When the heuristic is zero everywhere, or when obstacles force exploration of most nodes. Heavy slope constraints creating maze-like barriers can approach worst case.

### Q8: Could dijkstra be faster than A* here?
**A:** Unlikely for single source-target pairs on a 500×500 grid where Manhattan heuristic is informative. A* typically expands fewer nodes. For all-pairs shortest paths, different approaches apply.

### Q9: How to make this multi-threaded?
**A:** Parallelize the pair evaluation loop — each `(hi, mi)` A* is independent. Need thread-safe updates to `bestScore` (atomic or mutex). Prefix tables are read-only → safe to share.

### Q10: What's the impact of the 22 m slope threshold?
**A:** It's a hard filter on edges. Too strict → many unreachable pairs. Too loose → unrealistic paths. 22 m across 100 m cells implies max ~12.5° slope — plausible for rover planning (inferred).

### Q11: How would you add path reconstruction?
**A:** Maintain `parent[R*C]` array in A*. On reaching goal, backtrack from goal to start. Export as coordinate list or GeoJSON.

### Q12: Is the 5×5 window variance population or sample variance?
**A:** Population: divides by n=25, not n-1. Appropriate for describing the window itself, not estimating population statistics.

### Q13: How would you validate cross-file consistency?
**A:** After loading all grids, assert `illumination.size() == elevation.size()` and same for columns. Currently only elevation dimensions are used.

### Q14: Express the problem as formal optimization.
**A:** Maximize `f(h,m) = 0.5×I(h) + 0.5×W(m) − 0.001×d*(h,m)` subject to `d*(h,m)` being the shortest path length in graph G where vertices are cells, edges connect 4-neighbors with `|Δelevation| ≤ 22`, and edge weight 1.

### Q15: What if you needed globally optimal site selection without pair limits?
**A:** Current approach is heuristic (top-K + cap). Exact solution would require evaluating all habitat × mining pairs with A* — up to 246K² pairs, intractable. Could use spatial indexing, precomputed distance fields, or ILP formulations.

### Q16: Why might `priority_queue` with lazy deletion be used?
**A:** When a node is re-discovered with lower `g`, a new entry is pushed without removing the old one. `seen[]` check skips stale entries. Simpler than decrease-key but uses more heap space.

### Q17: How does the elevation data affect path length vs Manhattan?
**A:** Slope constraints force detours around steep cliffs. Actual path > Manhattan when obstacles exist. When terrain is smooth, path ≈ Manhattan.

### Q18: Propose a better weighting scheme.
**A:** Use configurable weights summing to 1. Add hard constraints (min illumination ≥ 0.4). Use Pareto frontier for multi-objective analysis instead of single weighted sum.

### Q19: What's the time complexity of `select_candidates`?
**A:** O((R-4)(C-4)) for window enumeration + O(N log N) for sort + O(K) for clustering = O(R×C + N log N) dominated by sort with N ≈ R×C.

### Q20: How would you unit test `slope_ok` and `astar_path`?
**A:** Create tiny 3×3 elevation grids: test flat terrain (path = Manhattan), test cliff blocking (returns INF), test detour required (path > Manhattan). Test `slope_ok(0, 22) = true`, `slope_ok(0, 23) = false`.

---

# 14. Resume Explanation

## Polished resume bullet

> **Optimal Moon Landing Site Selector** — Built a C++ geospatial optimization pipeline that processes 500×500 lunar terrain rasters (elevation, illumination, water-ice) to recommend optimal habitat and mining locations connected by a slope-constrained shortest path. Implemented 2D prefix-sum statistics, top-K spatial clustering, A* pathfinding with admissible heuristics, and branch-and-bound pruning — reducing search from 246K candidate windows to a tractable paired evaluation producing scored, reproducible mission plans.

## 30-second explanation

*"I built a C++ tool that reads lunar map data — elevation, sunlight, and water ice — and finds the best place for a habitat and a mining site. It uses prefix sums to quickly score thousands of 5×5 regions, clusters them for geographic diversity, then runs A* pathfinding between top candidates to find a feasible cable route. The final score balances sunlight, water access, and path length."*

## 2-minute explanation

*"The project solves lunar base planning: you want sunlight for power and water ice for resources, but they're not in the same place.*

*I load four CSV grids — 500 by 500 cells — representing elevation, illumination, water ice, and signal data. The core challenge is search space: a quarter-million cells, and pairing any two is expensive.*

*My pipeline has four stages. First, CSV parsing with validation. Second, I build 2D prefix-sum tables so I can compute the mean illumination or water ice and elevation roughness for any 5×5 window in constant time. I slide that window across the whole map, rank candidates, keep the top 3000, then apply spatial clustering — one winner per 25×25 bucket — so picks spread across the terrain.*

*Third, for the top 50 habitat and top 50 mining sites, I evaluate pairs. Before each A* run, I compute an optimistic score using Manhattan distance to prune hopeless pairs. A* itself runs on a 4-connected grid with a 22-meter max slope between neighbors — modeling what a rover or cable can handle.*

*The score is a weighted sum: half illumination, half water ice, minus a small penalty per path cell. The best pair goes to result.txt with coordinates, metrics, and path length. Runtime is about 15–20 seconds thanks to pruning and a 2000-evaluation cap."*

## 5-minute deep explanation

Cover all of the 2-minute content, then add:

- **Architecture:** Single `main.cpp` orchestrator with four header modules (`step1` CSV, `step2` stats, `step3` pathfinding, `step4` output). Header-only `inline` functions — compile with one `g++` command.
- **Prefix sums math:** Two tables — sum and sum-of-squares — enable mean and population standard deviation via `σ = sqrt(E[X²] - E[X]²)`. Variance clamped to non-negative before sqrt.
- **Why A*:** Admissible Manhattan heuristic on unit-cost grid guarantees optimal path length. Slope constraint applied as edge filter, not penalty.
- **Why not brute force:** 246K windows × pairing × A* each ≈ billions of operations. Top-K + clustering + pruning + cap makes it practical.
- **Key result:** Habitat at (303,264) with 56.6% illumination, mining at (311,200) with 95.1% water ice, 72-cell path (7.2 km). Score 0.6863. Top-illumination site (129,424) at 59.4% loses because pair optimization favors shorter feasible routes to high-water sites.
- **Known limitations:** `signal_occultation` unused; `MAX_EVAL` may miss global optimum; no path coordinate export; hardcoded weights; no build system; `bits/stdc++.h` non-portable.
- **Extensions you'd propose:** CLI config, parallel A*, path visualization, Pareto analysis, communication score integration.

---

# 15. Future Improvements

| Improvement | Why | Benefits | Difficulty | Effort |
|---|---|---|---|---|
| Use `signal_occultation` in scoring | Data already loaded | Better habitat comm quality | Medium | 4–8 hrs |
| CLI arguments for weights/thresholds | Hardcoded constants limit flexibility | Tunable without recompile | Easy | 2–4 hrs |
| Path reconstruction + export | Only length returned | Visualization, mission planning | Medium | 4–6 hrs |
| `CMakeLists.txt` / Makefile | Manual compile only | Reproducible builds, CI | Easy | 1–2 hrs |
| Cross-validate grid dimensions | Only elevation dims used | Prevent silent bugs | Easy | 1 hr |
| Replace full sort with `nth_element` | Sort 246K for top 3000 | Faster candidate phase | Easy | 1–2 hrs |
| Parallel pair evaluation | A* calls independent | Near-linear speedup on multi-core | Medium | 4–8 hrs |
| Remove `MAX_EVAL` or make adaptive | May miss optimal pair | Better answer quality | Easy | 1 hr |
| Web/GUI visualization | CLI only | Stakeholder demos | Hard | 2–5 days |
| Diagonal movement (8-connected) | More realistic rover motion | Shorter paths | Medium | 3–6 hrs |
| Unit tests (Catch2/GoogleTest) | No tests exist | Regression safety | Medium | 1–2 days |
| Replace `bits/stdc++.h` | Non-standard, slow compile | Portability | Easy | 1–2 hrs |
| Pareto frontier analysis | Single score hides trade-offs | Better decision support | Hard | 1–3 days |
| Precompute distance transform | Repeated A* from similar sources | Faster pairing | Hard | 2–4 days |

---

# 16. Code Quality Review

## Good practices ✅

- Clear pipeline separation into step headers
- Input validation with informative error messages
- `reserve()` on vectors where size is known
- Prefix sums for algorithmic efficiency
- Admissible A* heuristic (correctness)
- Optimistic pruning (performance)
- Intermediate output for debugging (`candidates.txt`)
- `max(0, variance)` guards numerical edge case
- I/O optimization in `main`

## Bad practices / code smells ⚠️

- `bits/stdc++.h` — non-portable, hides dependencies
- `using namespace std` in headers — pollutes includer namespace
- `signal_occultation` loaded but unused — dead work
- Magic numbers hardcoded (`3000`, `25`, `50`, `2000`, `22`, `0.001`)
- `exit(1)` in library functions — no recovery, hard to test
- Mixed `float`/`int` for path length
- `path_len >= 1e8f` compared after `int` cast — type inconsistency
- No dimension validation across input grids
- A* pushes duplicate nodes instead of decrease-key
- `bestPath` stored as `int` but updated from `float` path_len

## Possible bugs 🐛

| Issue | Severity | Detail |
|---|---|---|
| `MAX_EVAL` early exit | Medium | May return suboptimal pair |
| Grid size mismatch between CSVs | Medium | Not checked; out-of-bounds or wrong stats |
| Cluster index `c.r / clusterSize` | Low | Integer division — correct for bucket assignment |
| Converting INF float to int | Low | `1e9f` → int overflow on some platforms; comparison uses `1e8f` threshold before meaningful use |
| No check that `hab_pool`/`min_pool` non-empty | Low | Loops handle size 0 gracefully |

## Maintainability

- **Readable** for someone familiar with competitive programming style
- **Comments** sparse but function names are descriptive
- **No tests** — changes are risky
- **No config** — tuning requires code edits

## Naming

- Generally clear: `select_candidates`, `astar_path`, `score_pair`
- `P_illum`, `P_water`, `P_elev` — concise prefix table names
- `rough` could be `elev_stddev` for clarity
- `NH`, `NM` — acceptable local constants

## Scalability

| Scale | Behavior |
|---|---|
| 500×500 (current) | ~15–20 sec |
| 2000×2000 | Prefix build ~16× slower; A* per call ~16× more nodes; likely minutes without more pruning |
| Many maps | Linear in number of raster layers |

## Security issues

- **None significant** — local CLI tool, no network, no user shell injection
- CSV parsing could be abused with enormous files (memory exhaustion) — no size limits

## Performance improvements

1. `nth_element` for top-3000 instead of full sort
2. OpenMP parallel for loop over pairs
3. Cache A* results for repeated endpoints
4. Bitset instead of `vector<char>` for `seen` (marginal)
5. Early terminate A* when `g + H > best_known_path_length` (not implemented)

---

# 17. File Dependency Map

```
                    ┌─────────────────────────────────────────────┐
                    │                  main.cpp                    │
                    └─────────────────────────────────────────────┘
                           │           │           │           │
              ┌────────────┘           │           │           └────────────┐
              ▼                        ▼           ▼                        ▼
     ┌────────────────┐     ┌────────────────┐  ┌───────────────┐  ┌───────────────┐
     │ step1_csv.hpp  │     │ step2_stats.hpp│  │step3_pathfind │  │ step4_result  │
     └────────────────┘     └────────────────┘  │    .hpp       │  │    .hpp       │
              │                        ▲          └───────────────┘  └───────┬───────┘
              │                        │                                   │
              │                        └───────────────────────────────────┘
              │                                    (step4 includes step2)
              ▼
     ┌────────────────────────────────────────────────────────┐
     │  elevation.csv   illumination.csv   water_ice.csv      │
     │  signal_occultation.csv                                │
     └────────────────────────────────────────────────────────┘

     OUTPUT DEPENDENCIES:
     main.cpp ──► candidates.txt  (via write_candidates_to_file in step2)
     main.cpp ──► result.txt       (via write_result in step4)
     main.cpp ──► stdout           (via print_sample, cout)
```

### Include graph

```
main.cpp
 ├── step1_csv.hpp  → <bits/stdc++.h>
 ├── step2_stats.hpp → <bits/stdc++.h>
 ├── step3_pathfinding.hpp → <bits/stdc++.h>
 └── step4_result.hpp → <bits/stdc++.h>
                      → step2_stats.hpp
```

### Runtime dependency table

| From | To | Relationship |
|---|---|---|
| `main.cpp` | `load_csv` | Reads CSV files |
| `main.cpp` | `Prefix2D` | Builds stat tables |
| `main.cpp` | `select_candidates` | Creates candidate pools |
| `main.cpp` | `astar_path` | Path length per pair |
| `main.cpp` | `score_pair` | Evaluates pair quality |
| `main.cpp` | `write_result` | Final output |
| `step4_result.hpp` | `Candidate` | Uses struct from step2 |

---

# 18. Execution Walkthrough

**Scenario:** Run with bundled CSV data. Trace the winning pair.

### Phase 1: Load (main.cpp:14–19)

```
elevation     → 500×500, values ~ -64 to +64
illumination  → 500×500, values ~ 0.01 to 0.11
water_ice     → 500×500, values ~ 0.62 to 0.951
signal        → 500×500 (unused thereafter)
R=500, C=500
```

### Phase 2: Prefix build (main.cpp:28)

```
P_illum.S, P_illum.SS   ← from illumination
P_water.S, P_water.SS   ← from water_ice
P_elev.S, P_elev.SS     ← from elevation
```

Each: 501×501 `double` tables.

### Phase 3: Habitat candidates (main.cpp:34)

`select_candidates(P_illum, P_elev, 500, 500, 3000, 25)`:

```
Loop i=0..495, j=0..495:
  Example at (127,422):
    block_stats5x5 on illum → avg ≈ 0.594
    block_stats5x5 on elev  → rough ≈ 2.31
    push Candidate{129, 424, 0.594, 2.31}

After all 246016 pushes → sort → keep 3000 → cluster → hab_pool
```

After re-sort in main, top entry: `(129,424)` illum=0.594.

### Phase 4: Mining candidates (main.cpp:35)

Same with `P_water`. Top after sort: `(5,86)` water=0.951, rough=1.47.

### Phase 5: Pair evaluation — trace winning pair

Eventually evaluates `hi` pointing to `(303,264)` and `mi` pointing to `(311,200)`:

```
Hc = {r=303, c=264, avg=0.5656, rough=2.60}
Mc = {r=311, c=200, avg=0.9510, rough=2.08}

Manhattan = |303-311| + |264-200| = 8 + 64 = 72

optimistic = 0.5×0.5656 + 0.5×0.9510 - 0.001×72
           = 0.2828 + 0.4755 - 0.072 = 0.6863

Assume this exceeds prior bestScore → run A*:
  start=(303,264), goal=(311,200)
  A* explores nodes respecting slope ≤ 22m
  Returns g = 72 (equals Manhattan → straight-ish path exists)

score_pair = 0.5×0.5656 + 0.5×0.9510 - 0.001×72 = 0.6863

Update: bestScore=0.6863, bestPath=72, bestHab=Hc, bestMin=Mc
```

### Phase 6: Output (main.cpp:114–116)

```
write_result("result.txt", 0.6863, Hc, Mc, 72)
  → "Avg Illumination: 56.56%"
  → "Avg Water-Ice Probability: 0.9510"
  → "Path Length: 72 cells (7200 m)"
```

**Program exits 0.**

---

# 19. Key Takeaways

## What this project teaches

- How to **reduce a massive spatial search** into a practical pipeline using classic algorithms
- How **prefix sums** turn repeated range queries from O(n) to O(1)
- How **A*** combines Dijkstra correctness with heuristic-guided efficiency
- How **multi-objective problems** are often solved via weighted sums + constraints
- How **engineering trade-offs** (eval cap, clustering) sacrifice optimality for speed

## Why it is interesting

- Combines **data science** (raster statistics), **graph algorithms** (A*), and **mission design** (lunar base planning) in one cohesive program
- Real output with interpretable coordinates and metrics — not a toy sort/search demo
- Demonstrates both **algorithmic sophistication** and **pragmatic shortcuts** found in real systems

## What makes it unique

- Dual-site optimization (habitat ≠ mining) with **connectivity constraint** — harder than single-site selection
- Slope-aware pathfinding integrated into site scoring, not a post-hoc check
- Spatial clustering ensures recommendations **spread across the map**, not one hotspot
- Uses actual geospatial raster data at meaningful scale (250K cells)

---

# 20. README Quality

This document provides:

- ✅ Complete folder and file inventory
- ✅ Architecture diagrams (ASCII)
- ✅ Step-by-step execution narrative
- ✅ Per-file function documentation
- ✅ Algorithm complexity analysis
- ✅ Data structure rationale
- ✅ Mathematical formulas with worked example
- ✅ I/O specification with sample output
- ✅ 60 interview Q&As (beginner + intermediate + advanced)
- ✅ Resume-ready descriptions
- ✅ Code quality review
- ✅ Dependency map
- ✅ Full execution trace
- ✅ Explicit notes where information cannot be determined from code

### Build & run reference

```bash
# Compile
g++ -O2 -std=c++17 main.cpp -o moon_landing

# Run (CSV files must be in current directory)
./moon_landing        # Linux/macOS
.\moon_landing.exe    # Windows
```

### Project stats

| Metric | Value |
|---|---|
| Source files | 5 (1 `.cpp` + 4 `.hpp`) |
| Lines of code (approx) | ~350 |
| Input grid size | 500 × 500 |
| Valid 5×5 windows | 246,016 |
| Typical runtime | 15–20 seconds |
| External dependencies | None (only C++ standard library via `bits/stdc++.h`) |

---

# Interview Cheat Sheet

The **30 most important things** to remember before explaining this project:

1. **One-liner:** C++ lunar site selector — finds best habitat + mining pair with a slope-safe connecting path.

2. **Input:** Four 500×500 CSV rasters; only three are actually used (elevation, illumination, water_ice).

3. **`signal_occultation.csv` is loaded but never used** — mention proactively; shows you read all the code.

4. **Output:** `result.txt` (best pair + path), `candidates.txt` (top 20 shortlists), console preview.

5. **Score formula:** `0.5×illum + 0.5×water − 0.001×path_length`.

6. **Slope constraint:** Adjacent cells must differ by ≤ **22 meters** in elevation.

7. **Pathfinding:** **A*** with **Manhattan heuristic** on a **4-connected** grid.

8. **Why A*:** Admissible heuristic → optimal path length; faster than Dijkstra.

9. **Prefix sums:** Enable O(1) mean and std dev over any 5×5 window.

10. **Variance trick:** Two tables — sum (`S`) and sum-of-squares (`SS`); `σ = sqrt(E[X²] − E[X]²)`.

11. **5×5 window:** 25 cells; center stored as candidate coordinate `(i+2, j+2)`.

12. **`rough`:** Standard deviation of **elevation** in the window — terrain smoothness proxy.

13. **Top-K:** Keep best 3000 candidates before clustering (`K_keep = 3000`).

14. **Spatial clustering:** One candidate per 25×25 cell bucket — prevents all picks in one area.

15. **Pair search:** Top 50 habitat × top 50 mining = up to 2500 pairs.

16. **Pruning:** Skip A* if optimistic score (using Manhattan dist) can't beat current best.

17. **`MAX_EVAL = 2000`:** Hard cap on A* calls — trades optimality for speed.

18. **Actual result:** Habitat (303,264), Mining (311,200), path 72 cells, score 0.6863.

19. **Why not top habitat (129,424)?** Pair optimization — path length and mining match matter, not illum alone.

20. **Cell size inference:** Output says 72 cells = 7200 m → **100 m per cell**.

21. **Pipeline stages:** CSV load → prefix stats → candidate select → A* pairing → write result.

22. **Four headers:** `step1` CSV, `step2` stats, `step3` pathfinding, `step4` result.

23. **Compile:** `g++ -O2 -std=c++17 main.cpp -o moon_landing` — no build system.

24. **No path reconstruction:** A* returns only length, not the actual route.

25. **Error handling:** CSV errors → print message and `exit(1)` — no recovery.

26. **Complexity bottleneck:** O(2000 × V log V) for A* evaluations where V = 250,000.

27. **Water-ice ties:** Many top mining sites have avg = 0.951 — roughness and path break ties.

28. **Design trade-off:** Weighted linear score is simple but can't express hard minimums or Pareto trade-offs.

29. **Best improvement to mention:** Integrate signal occultation, add path export, parallelize A*, or CLI config.

30. **Skills demonstrated:** Prefix sums, A*, heuristic search, spatial clustering, multi-criteria optimization, geospatial raster processing.

---

# 22. Complete Learning Guide (Learn the Project From Scratch)

> **Who is this section for?** You are the original developer who forgot how you built it — or anyone preparing for an interview. This section teaches the project **in the order you should learn the code**, not the order functions appear in `main.cpp`. Each part has: **beginner explanation → technical details → self-test questions**. Answers are in the [Answer Key](#answer-key-for-all-self-test-questions) at the end.

---

## Recommended README Reading Order

If your goal is **complete mastery**, read the README in this sequence:

| Pass | Sections | Goal |
|------|----------|------|
| **Pass 1 — Story** | §1 Overview → §2 Architecture → §3 Project Flow | Understand *what* happens end-to-end |
| **Pass 2 — Code** | §22 (this section) file-by-file | Understand *how* each file works |
| **Pass 3 — Depth** | §4 Detailed Code → §5 Algorithms → §6 Data Structures → §7 Math | Interview-level technical depth |
| **Pass 4 — Practice** | §13 Interview Q&A → §22 Final Exam → §21 Cheat Sheet | Test yourself; memorize key facts |
| **Pass 5 — Polish** | §14 Resume → §16 Code Review → §18 Execution Walkthrough | Explain confidently; discuss trade-offs |

**Do not skip Section 22** if you want the tutorial-style “teach me from scratch” experience with practice questions.

---

## Step 0: The Data Files (Before Any Code)

### Beginner explanation

The program has **no keyboard input**. It reads **four CSV files** from the folder where you run it. Each file is a **500×500 grid** of numbers — like a spreadsheet with 500 rows and 500 columns.

| File | What each number means |
|------|------------------------|
| `elevation.csv` | Ground height in **meters** (roughly −64 to +64 in the bundled data) |
| `illumination.csv` | **Sunlight** level as a fraction from **0 to 1** (e.g. 0.56 = 56%) |
| `water_ice.csv` | **Water ice probability** from **0 to 1** (e.g. 0.951 = 95.1% chance) |
| `signal_occultation.csv` | Signal / communication obstruction data (mostly 0, some 1.7–3.0) |

Each cell represents roughly **100 m × 100 m** of lunar surface (inferred from output: 72 cells → 7200 m). The full map is about **50 km × 50 km**.

### Technical details

- Format: comma-separated floats, **no header row**, one row per line.
- `load_csv` enforces **rectangular** grids (same column count every row).
- **Only three maps are used in algorithms:** elevation, illumination, water_ice.
- **`signal_occultation.csv` is loaded and printed but never used** in scoring, candidates, or pathfinding.

### 📝 Self-test (Step 0)

1. How many cells are in one map, and what real-world area does that roughly cover?
2. Which three maps drive the final answer, and which is loaded but ignored?
3. If illumination is `0.56` internally, how does `result.txt` display it?

---

## File 1: `step1_csv.hpp`

**Learn this first** — everything else needs grids in memory.

### Beginner explanation

This file turns CSV files into C++ data and lets you preview them.

| Function | What it does |
|----------|--------------|
| `load_csv(path)` | Opens a file, reads line by line, splits on commas, converts to `float`, stores in a 2D list |
| `print_sample(A, name, k=5)` | Prints map name, size (e.g. `500x500`), and top-left `k×k` corner |

**Error handling:** missing file, bad number, uneven rows, or empty file → print error to `stderr` and **`exit(1)`** (program stops immediately).

**Special cases:** empty cell → `0.0`; whitespace around numbers is trimmed.

### Technical details

```cpp
vector<vector<float>> load_csv(const string &path)
```

**Algorithm per line:**
1. `getline` → skip empty lines
2. Split on `,` via `stringstream`
3. `trim` each cell; empty → `0.0f`; else `stof(cell)` in try/catch
4. First row sets `expected_cols`; later rows must match
5. `push_back(move(row))` into `data`

```cpp
void print_sample(const vector<vector<float>>& A, const string &name, int k=5)
```

Prints `name => R x C`, then `min(R,k) × min(C,k)` values with 2 decimal places.

**Design:** header-only, `#ifndef` guard, all functions `inline`, uses `bits/stdc++.h`.

### 📝 Self-test (File 1)

1. What happens if row 100 has 499 values but row 1 has 500?
2. What does `load_csv` return, and what type is each cell?
3. Why does `print_sample` only show 5×5 when the map is 500×500?
4. If `elevation.csv` is missing, does the program continue or stop?

---

## File 2: `step2_stats.hpp`

**Learn this second** — this shrinks 250,000 cells to a manageable candidate list.

### Beginner explanation

You slide a **5×5 window** over the map. For each window you compute:

- **`avg`** — average illumination *or* water (depending on which analysis you run)
- **`rough`** — how bumpy the terrain is (standard deviation of **elevation** in that window)

You store the **center** of the window as the site coordinate `(r, c)`.

Then you:
1. **Sort** all candidates: higher `avg` wins; if tied, **lower** `rough` wins
2. Keep top **3000** (`K_keep`)
3. **Spread them out:** divide the map into **25×25 cell buckets**; keep only the **best candidate per bucket**

`write_candidates_to_file` writes top 20 habitat + top 20 mining entries to `candidates.txt`.

### The `Candidate` struct

```cpp
struct Candidate {
    int r, c;      // grid coordinates (center of 5×5 window)
    float avg;     // mean illumination OR water ice
    float rough;   // elevation std dev in same window (meters)
};
```

Same struct for habitat and mining — only the meaning of `avg` changes.

### Technical details — `Prefix2D`

**Problem:** ~246,000 windows × 25 cells each = too many naive sums.

**Solution:** Two prefix tables `(R+1)×(C+1)` in `double`:
- `S` — prefix sum of values
- `SS` — prefix sum of **squares**

**`block_stats5x5(r,c)`** for window `(r,c)` to `(r+4,c+4)`:

```
mean = sum/25
var  = max(0, sum2/25 - mean²)
return (mean, sqrt(var))
```

**`select_candidates(P_metric, P_elev, R, C, K_keep, clusterSize)`:**

| Step | Detail |
|------|--------|
| Loop | `i = 0..R-5`, `j = 0..C-5` → 246,016 windows on 500×500 |
| Store | `Candidate{i+2, j+2, avg, rough}` — center of 5×5 |
| Sort | `avg` descending, `rough` ascending |
| Trim | `resize(K_keep)` → 3000 |
| Cluster | `cr = r/25`, `cc = c/25`; first in bucket wins |

### 📝 Self-test (File 2)

1. Why two prefix tables (`S` and `SS`) instead of one?
2. Window starts at `(100, 200)` — what `(r,c)` is stored?
3. Why one candidate per 25×25 bucket after top-3000?
4. Mining selection: what is `avg` computed from?
5. How many 5×5 windows on 500×500? Why doesn't `i` go to 499?

---

## File 3: `step3_pathfinding.hpp`

**Learn this third** — answers “can we connect these two sites?”

### Beginner explanation

Movement: **up, down, left, right only** (4 directions). Each step costs **1**.

You may only step to a neighbor if **elevation difference ≤ 22 meters** — otherwise too steep.

**A\*** explores promising cells first using **Manhattan distance** to the goal as a guess. When you reach the goal, return **number of steps**.

If impossible → return `1e9` (infinity sentinel).

**Important:** Returns **path length only** — not which cells form the path.

### Technical details

```cpp
struct AStarNode { int r, c; float f, g; };  // f = g + h
```

| Piece | Implementation |
|-------|----------------|
| `g[]` | Best cost from start, size `R*C`, init `INF=1e9` |
| `seen[]` | Visited flags |
| Heuristic `H` | `\|r - goal_r\| + \|c - goal_c\|` — **admissible** for 4-neighbor unit cost |
| Neighbors | `dr={1,-1,0,0}`, `dc={0,0,1,-1}` |
| Edge rule | `slope_ok(elev[r][c], elev[nr][nc])` → `\|h1-h2\| ≤ 22` |
| PQ | `priority_queue` with `greater<>` → min-heap on `f` |
| Goal | Return `cur.g`; else return `INF` |

**Lazy deletion:** duplicate PQ entries; skip if `seen[k]`.

**Not implemented:** `parent[]` for path reconstruction.

### 📝 Self-test (File 3)

1. Why 4 directions, not 8?
2. What does `slope_ok` check?
3. What is returned if the goal is surrounded by cliffs?
4. Why is Manhattan distance a safe heuristic?
5. Does this function return the actual path cells?

---

## File 4: `step4_result.hpp`

**Learn this fourth** — defines “best” and writes the report.

### Beginner explanation

Combine habitat quality, mining quality, and distance into **one score**:

```
score = half sunlight + half water − tiny penalty per path cell
```

Higher is better. `write_result` saves a human-readable report to `result.txt`.

### Technical details

```cpp
float score_pair(const Candidate& Hc, const Candidate& Mc, float path_len) {
    return 0.5f*Hc.avg + 0.5f*Mc.avg - 0.001f*path_len;
}
```

**Worked example (actual output):**

```
0.5 × 0.5656 + 0.5 × 0.9510 − 0.001 × 72 = 0.6863
```

**`write_result` formatting:**
- Illumination: `habitat.avg * 100` + `%`
- Water: raw `mining.avg`
- Path: `int(path_len)` cells, `int(path_len * 100)` meters

Includes `step2_stats.hpp` for `Candidate`.

### 📝 Self-test (File 4)

1. Why multiply illumination by 100 in output but not water?
2. Same illum and water — what breaks the tie?
3. Why subtract `0.001 * path_len`?
4. What are the four main sections of `result.txt`?

---

## File 5: `main.cpp`

**Learn this last** — wires everything together.

### Beginner explanation

`main()` runs the full pipeline:

```
Load 4 CSVs → print samples → build 3 prefix tables
→ habitat candidates (illum) + mining candidates (water)
→ sort both → write candidates.txt → print top 5 each
→ try top 50 × top 50 pairs:
     optimistic prune → A* → score_pair → track best
     (max 2000 A* runs)
→ write result.txt
```

**Optimistic prune:** Before A*, compute score using **Manhattan distance** instead of real path. Since real path ≥ Manhattan, true score ≤ optimistic. If optimistic ≤ current best, skip A*.

**Why top habitat might lose:** `(129,424)` has best illumination (0.594) but final pick is `(303,264)` (0.566) because scoring is **pair + path**, not illum alone.

### Technical details — key constants

| Constant | Value | Role |
|----------|-------|------|
| `K_keep` | 3000 | Candidates before clustering |
| `clusterSize` | 25 | Spatial bucket size |
| `NH`, `NM` | 50 | Top candidates per pool for pairing |
| `MAX_EVAL` | 2000 | A* call budget |
| `bestScore` | init `-1e9` | Running maximum |

**Pair loop (pseudocode):**

```
for hi in 0..NH-1:
  for mi in 0..NM-1:
    if evals >= MAX_EVAL: break
    optimistic = 0.5*Hc.avg + 0.5*Mc.avg - 0.001*manhattan(Hc, Mc)
    if optimistic <= bestScore: continue
    path_len = astar_path(elevation, Hc, Mc)
    evals++
    if path_len >= 1e8: continue
    if score_pair(...) > bestScore: update best
```

**I/O:** `ios::sync_with_stdio(false); cin.tie(nullptr);`

**Dead code path:** `signal_occultation` loaded, never used in logic.

### 📝 Self-test (File 5)

1. Learning order vs `main` include order — why differ?
2. What is `optimistic`, and why is pruning safe?
3. Why might best illumination site not win?
4. What happens when `evals` reaches 2000?
5. Winning pair (303,264)+(311,200): Manhattan distance and score with path 72?

---

## Final Exam — Tie Everything Together

Test yourself **without looking at code or the answer key first.**

### Big picture

1. Explain the project in 30 seconds to a non-technical person.
2. Draw the pipeline: CSV → ? → ? → ? → `result.txt`.
3. What problem does each `step*.hpp` file solve?

### Algorithms

4. How do prefix sums make 5×5 stats fast? Build vs query complexity?
5. Explain spatial clustering. Why 25×25?
6. Explain A* on this grid: moves, costs, heuristic, stop condition.
7. Why is branch-and-bound pruning safe?

### Numbers & trade-offs

8. Recalculate score for illum=0.5656, water=0.9510, path=72.
9. Name three hardcoded constants you'd expose as CLI flags.
10. What is the risk of `MAX_EVAL = 2000`?

### Gaps & honesty (strong in interviews)

11. What does `signal_occultation.csv` do today?
12. What doesn't A* return that a planner might want?
13. How big is the map in kilometers?

---

## Your Interview Script (Memorize This Flow)

When explaining aloud, follow this **7-step script**:

1. **Problem** — Dual lunar sites (habitat + mining) with a connectable cable route.
2. **Input** — Three 500×500 rasters used; ~100 m/cell; fourth loaded but unused.
3. **Candidate generation** — 5×5 windows, prefix sums, top 3000, 25×25 clustering.
4. **Pairing** — Top 50×50, optimistic prune, A* with 22 m slope cap.
5. **Objective** — `0.5·I + 0.5·W − 0.001·L`.
6. **Output** — `result.txt` + `candidates.txt`.
7. **Limitations** — Unused signal map, no path coordinates, eval cap, hardcoded weights.

---

## Answer Key for All Self-test Questions

Use this **after** attempting each question yourself.

### Step 0 answers

| # | Answer |
|---|--------|
| 1 | **250,000 cells** per map (500×500). At ~100 m/cell → **~50 km × 50 km** region. |
| 2 | **Used:** elevation, illumination, water_ice. **Ignored in logic:** signal_occultation (only loaded + printed). |
| 3 | Stored as fraction `0.56`; `write_result` multiplies by 100 → **"56.56%"** in `result.txt`. |

### File 1 (`step1_csv.hpp`) answers

| # | Answer |
|---|--------|
| 1 | Program prints `[ERROR] Non-rectangular CSV` and **`exit(1)`** — does not continue. |
| 2 | Returns **`vector<vector<float>>`** — 2D grid; each cell is **`float`**. |
| 3 | It's a **sanity check** — full 500×500 print would be huge; 5×5 confirms load/format. |
| 4 | **Stops immediately** — `cerr` message + `exit(1)`. |

### File 2 (`step2_stats.hpp`) answers

| # | Answer |
|---|--------|
| 1 | `S` gives **sum** (for mean). `SS` gives **sum of squares** (for variance: E[X²]−E[X]²). One table alone cannot compute std dev in O(1). |
| 2 | Center of 5×5 starting at (100,200) is **(102, 202)** — i.e. `(i+2, j+2)`. |
| 3 | Top-3000 by score alone can cluster in one bright/icy region. Buckets force **geographic diversity**. |
| 4 | **`avg` from water_ice** (`P_water`); **`rough` still from elevation** (`P_elev`). |
| 5 | **`(500−4) × (500−4) = 496×496 = 246,016`**. Window needs 5 rows/cols; last start index is 495 so window fits through index 499. |

### File 3 (`step3_pathfinding.hpp`) answers

| # | Answer |
|---|--------|
| 1 | Code models **4-connected** grid with **unit cost per edge**; diagonals would need different cost/heuristic. |
| 2 | **Adjacent pair only:** `|elevation[current] − elevation[neighbor]| ≤ 22` meters. |
| 3 | Returns **`1e9f` (INF)**; `main` treats `path_len >= 1e8` as unreachable and skips pair. |
| 4 | On 4-neighbor unit-cost grid, any path has length **≥ Manhattan distance** → heuristic **never overestimates** → admissible → A* optimal. |
| 5 | **No** — only **path length** (`cur.g` at goal). No `parent` array. |

### File 4 (`step4_result.hpp`) answers

| # | Answer |
|---|--------|
| 1 | Illumination is a **fraction** (0–1); ×100 makes intuitive **percent**. Water is already described as "probability" in 0–1 form. |
| 2 | **Lower `path_len`** → higher score (via −0.001×path). Also different `rough` if comparing single-pool candidates. |
| 3 | Encourages **shorter cable routes** — distance matters, not just resource quality. |
| 4 | (1) Combined score, (2) Optimal habitat site, (3) Optimal mining site, (4) Power cable path length. |

### File 5 (`main.cpp`) answers

| # | Answer |
|---|--------|
| 1 | **Includes in main:** step1→step2→step3→step4. **Learn order:** step1→step2→step3→step4→main because you need grids before stats, stats before pairing, pathfinding before scoring, all before orchestration. |
| 2 | `optimistic = 0.5·I + 0.5·W − 0.001×manhattan`. Real path ≥ Manhattan → real score ≤ optimistic. If optimistic ≤ best, **no pair can beat best** → safe to skip A*. |
| 3 | Optimizer maximizes **pair score**, not solo habitat illum. Shorter/feasible path to strong mining site can beat slightly dimmer habitat. |
| 4 | Loop **breaks** — remaining pairs **not evaluated**; answer may be **suboptimal** if best pair wasn't in first 2000 evals. |
| 5 | Manhattan = \|303−311\|+\|264−200\| = **8+64 = 72**. Score = 0.5×0.5656+0.5×0.9510−0.001×72 = **0.6863**. |

### Final exam answers

| # | Answer |
|---|--------|
| 1 | *Example:* "It reads Moon maps and picks the best sunny base spot and icy mining spot that can be connected by a safe cable route, then writes the answer to a file." |
| 2 | CSV → **prefix sums** → **candidate selection** → **A* pair evaluation** → **result.txt** |
| 3 | step1: load CSV; step2: stats + candidates; step3: A* pathfinding; step4: score + write result |
| 4 | Build **O(R×C)**; each 5×5 query **O(1)** via inclusion–exclusion on prefix tables |
| 5 | One best site per **25×25** bucket → prevents all picks in same peak; spreads candidates geographically |
| 6 | 4-neighbor moves, cost 1, heuristic Manhattan, stop when goal popped with minimal `g` |
| 7 | Manhattan ≤ true path length → optimistic score ≥ true score; pruning never discards a potential winner |
| 8 | **0.6863** (see File 4 worked example) |
| 9 | *Examples:* slope limit (22), score weights (0.5/0.5/0.001), K_keep (3000), MAX_EVAL (2000), clusterSize (25) |
| 10 | May **miss global optimum** if best pair would be evaluated after 2000th A* call |
| 11 | **Loaded and printed only** — not used in scoring, clustering, or pathfinding |
| 12 | **Actual path coordinates** — only length returned; no GeoJSON/polyline export |
| 13 | **~50 km × 50 km** (500 cells × ~100 m/cell, inferred from output formatting) |

---

## Quick Map: “Where Do I Find X in This README?”

| If you need to understand… | Read |
|----------------------------|------|
| What the project does (30 sec) | §1, §22 Interview Script |
| Folder / file layout | §2 |
| Step-by-step runtime story | §3, §18 |
| Every function explained | §4 |
| Why prefix sums / A* / clustering | §5, §22 File 2–3 |
| Formulas and score math | §7 |
| CSV format and sample output | §8 |
| Interview questions with answers | §13 + §22 Answer Key |
| Resume bullets | §14 |
| Code smells / bugs to mention | §16 |
| 30 facts to memorize | §21 Cheat Sheet |
| Learn files in correct order | **§22 (this section)** |

---

*Documentation generated from full source and data analysis of the Optimal-Moon-Landing project. Grid dimensions and runtime verified by compilation and execution.*
