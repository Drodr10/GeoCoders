# Architecture — GeoQuery

## Overview

This document outlines the high-level architecture and responsibilities for components in GeoQuery.

## Components

- `Restaurant`: simple POD (plain old data) struct representing an entry from GeoJSON.
- `GeoJSONParser`: parses GeoJSON to `std::vector<Restaurant>`. Handles point and polygon geometries.
- `SpatialIndex` (interface): abstract class exposing `build()` and `knn()` methods.
  - `KDTree`: 2-D k-d tree with alternating split axis.
  - `Quadtree`: 4-ary tree partitioning bounding boxes.
- `Benchmark`: measures build and query times and returns `Benchmark::Result`.
- `CLI`: command-line interaction and menu handling.

## Data flow

1. Parser reads GeoJSON and returns `std::vector<Restaurant>`.
2. The same vector is passed to `KDTree::build()` and `Quadtree::build()`.
3. `knn()` queries return sorted lists of (distance, Restaurant).
4. `Benchmark` times builds and queries; `CLI` presents results.
