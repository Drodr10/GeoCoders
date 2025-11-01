# CLI Wireframe

This file documents the command-line menu and expected user interactions.

Example session (text UI):

```plaintext
Loading restaurant data from file...
Building k-d tree... Done. (Time: X ms)
Building Quadtree... Done. (Time: Y ms)

Please select an option:

1. Find nearest restaurants using k-d Tree
2. Find nearest restaurants using Quadtree
3. Run performance comparison benchmark
4. Exit

|
```

When the user selects a search option, prompt:

```plaintext
Enter latitude:
Enter longitude:
Enter number of restaurants to find (k):

Output example:
Searching with Quadtree...
(Time: Z microseconds)
The 5 closest restaurants are:

1. McDonald’s
2. Chili’s
...
```
