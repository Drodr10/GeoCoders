# GeoQuery — Geo-Coders

Team: Geo-Coders

Team Members:

- Diego Rodriguez
- Xavier Ruiz
- Ehan Shah

Project Title: GeoQuery

## Short description

GeoQuery is a command-line project blueprint (C++) to build and benchmark two spatial indices — a k-d Tree and a Quadtree — over a large GeoJSON dataset of restaurants. This repository contains the project scaffold and design blueprints; no implementation code is included here.

## Goals

- Parse and store 154,119 restaurants from OSM-derived GeoJSON.
- Build a 2D k-d Tree and a Quadtree from the dataset.
- Support k-nearest neighbors queries for a query coordinate.
- Compare performance between data structures.

## Project layout (blueprint)

```plaintext

GeoCoders/
├─ CMakeLists.txt
├─ README.md
├─ .gitignore
├─ include/
│  ├─ restaurant.h
│  ├─ spatial_index.h
│  ├─ kdtree.h
│  ├─ quadtree.h
│  ├─ parser.h
│  ├─ benchmark.h
│  └─ cli.h
├─ src/
│  ├─ main.cpp                # program entry (skeleton)
│  ├─ parser.cpp              # GeoJSON parsing implementation stub
+│  ├─ kdtree.cpp             # KDTree implementation stub
│  ├─ quadtree.cpp           # Quadtree implementation stub
│  ├─ benchmark.cpp          # Benchmark harness stub
│  └─ cli.cpp                # Command-line UI stub
├─ docs/
│  ├─ architecture.md
│  └─ wireframe.md
└─ data/
   └─ README.md
```

Next steps for implementation are listed in the docs.
