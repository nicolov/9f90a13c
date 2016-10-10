# Knightboard by Nico

Thanks, it feels great taking a test graded by a real human :)

```
Look at **test.cpp** first, it contains Google Test unit tests for the requested functionality. **main.cpp** only has a few usage examples.
```

**Tests**: using Google Test

## Running

I've developed and tested on Ubuntu 16.04 with CMake version 3.5.2 and gcc version 5.4.0, but OSX 10.10 works fine as well. For your convenience, I've built a Docker image that runs gtest:

```
docker run nicolov/knightboard:v1
```

If you want to compile and run locally, don't forget to copy the `knightboard.txt` file to your home directory.

## Thoughts

While developing, I kept a few ideas in the back of my mind:

- Algorithms should generalize well to large maps. In the real world, I expect each path to only cover a small part of the map (cars, anyone?). Thus, I've been careful to build the graph structure on the fly, instead of generating an adjancency list beforehand. On the same note, I've used hashsets to keep track of node data, instead of storing everything in a pre-allocated table. All this is probably unnoticeably slower than arrays, but would require much less memory in the real world.

- I like templates :). In this particular case, I could have used `std::vector` and dinamically allocate the board instead of templating over the dimensions and using std::array. In general, I like to allocate statically whenever possible. I enjoy compile-time computations with `constexpr`, and non-class template variables really come in handy for that.

- I've optimized for **code clarity** rather than reuse, since the questions were mostly building upon each other. In a real system, I would have factored out the common functionality more aggressively. The same goes for protecting member data and functions.

- I know I could have used a single integer for indexing the board, but using two was much nicer, and worth the whole of 64 extra bits for this exercise.

### Level 1

Not much to say here, but I went back and added the additional functionality, so it's a bit overengineered w.r.t this initial question.

### Level 2

This is slightly more general case of level 3 that wasn't really any easier to solve. I understand the progression though, and wrote DFS for the sake of variety (see Level 3 below). Both graph traversals eventually visit all nodes, so DFS will solve level 2 just as well.

### Level 3

This question (and the next) screams **graph**, and I duly submitted. Each square in the board is a vertex of the graph, and appropriate edges connect nodes that can be reached from each other under the constraint of "L" moves. The shortest path in this graph corresponds to the shortest path in the actual board.

For level 3, all edge weights are the same. BFS will thus be enough to find the shortest path. Again, this is a particular case of Level 4.

### Level 4

The goal boils down to a shortest path search on a weighted graph. I resorted to good old Dijkstra, using the STL's priority queue backed by a vector. Definitely good enough for this question.

For this question, I updated the candidate generation step to skip Rock and Barrier step. The teleport square pair is implemented as a direct edge with weight 0.

### Level 5

I had to lookup the algorithm for this one, and discovered that there is none :D It smells of Dynamic Programming, but the space is really huge due to the "visit once" constraint. I sketched out such a solution anyways, but its `O(N 2^N)` complexity makes it useless even for the small board, let alone the big one..

## Thanks!

Thanks for the challenge, it's been fun :) 
