# GeoQuery: Geo-Coders

Team: Geo-Coders

Team Members:

- Diego Rodriguez
- Xavier Ruiz
- Ehan Shah

Project Title: GeoQuery

## Short description

GeoQuery is a command-line C++ application that builds and benchmarks two spatial indices—a k-d Tree and a Quadtree—over a large GeoJSON dataset of restaurants. The project supports parsing, spatial indexing, k-nearest neighbors queries, and performance comparison between data structures.

## How to Build and Run

### Prerequisites

- C++17 compiler (e.g., g++, clang++, MSVC)
- CMake 3.22 or newer
- [nlohmann/json](https://github.com/nlohmann/json) (automatically fetched by CMake)

### Build Instructions

1. Clone the repository:

```sh
git clone https://github.com/Drodr10/GeoCoders.git
cd GeoCoders
```

2. Create a build directory and run CMake:

```sh
  mkdir build
  cd build
  cmake ..
  cmake --build .
```

3. Place your GeoJSON data file in the `data/` directory (default: `data/restaurants.geojson`).

### Run Instructions

From the `build` directory, run:

```sh
./geoquery.exe
```

or (on Unix):

```sh
./geoquery
```

The parser defaults to `data/restaurants.geojson`, but you can change the path in `src/main.cpp` (see line 12).

### Command-Line Usage

The CLI supports k-nearest neighbor queries and benchmarking. See [wireframe.md](docs/wireframe.md) for available options.

### Architecture

For a high-level overview of the system and its components, see [architecture.md](docs/architecture.md).

### Data

Place your GeoJSON data files in the `data/` directory. See [data/README.md](data/README.md) for details.

## Project layout

```plaintext
GeoCoders/
├─ .gitignore
├─ CMakeLists.txt
├─ README.md
├─ data/
│  └─ README.md
├─ docs/
│  ├─ architecture.md
│  └─ wireframe.md
├─ include/
│  ├─ benchmark.h
│  ├─ cli.h
|  ├─ distance.h
│  ├─ kdtree.h
│  ├─ parser.h
│  ├─ quadtree.h
│  ├─ restaurant.h
│  ├─ spatial_index.h
├─ src/
│  ├─ benchmark.cpp
│  ├─ cli.cpp
|  ├─ distance.cpp
│  ├─ kdtree.cpp
│  ├─ main.cpp
│  ├─ parser.cpp
│  ├─ quadtree.cpp
```
