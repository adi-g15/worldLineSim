# World Line Simulator

As of now, it `can` be said that it simulates a simple 3D multiverse

And... it currently doesn't have 3D rendering, since that wasn't of importance earlier, the theoretical implementation was the goal, but it's interesting,
think of the very basic idea as this ->

_Think about a world, in which 4 snakes keep moving automatically to get food, any \`time*\` the universe has some state; You go back in time, change anything, that then changes the state then, and hence the present and the future_

ie. Time Travel following the WorldLine theory, stated in the Anime `Steins Gate`
It has a 3D auto expanding space, like a simple form of our universe
Now the project offers much more, since you can now have as many entities as memory allows, and can CREATE your own entities too.

This project generalises the idea to entities and actions, in different worlds and worldlines, all independently on different threads, as well theoretically different verses too, other than this ->

It also features a constantly growing matrix (the world_plot), using another project [GraphMat3D](https://github.com/adi-g15/graphMat) that was actually a part of this one itself

## Building

```sh
cd build   # if doesn't exist, then create
cmake ..
cmake --build .
```

> You may set CMAKE_Build_Type as Release

## Creating your own Custom ENTITIES (for example, horses)

* Inherit from the Entity class, override/implement the `startSimulation` and `pauseSimulation` virtual member functions, and others that are pure virtuals, and need to be overloaded in Entity class

* In `World` constructors, make sure your push your new entities to the entities vector with `entities.push_back(new Horse(this));`

> Constructor - An entities constructor takes in the World*, so that it has access to the worldplot (consider it the space fabric here, just a matrix), for eg, calling world->getRandomCoord() to get a position in the world
> See the constructor of other entity, for eg. Human, Rock, Snake for reference

## Current State (Logs)

> Note: Currently the focus is that _internally_ the simulation happens, but maybe the display won't be very great now, so it just logs on console (Since, I don't have a ground in rendering things currently, if you have ideas, or suggestions do put in that pinned issue)

![Current State](https://raw.githubusercontent.com/adi-g15/Sources/main/worldLineSim_state.png?token=AI4LBINZKQFMNVC5BRCO7S3AFU3KO "This is a part of the logging")

## Class Chart

> Made with draw.io

![Class Chart](https://raw.githubusercontent.com/adi-g15/Sources/main/worldLineSim.png?token=AI4LBIN5MOCOAM4ODLI3NCLAFU3TS "WorldLine Sim")

## Future Ideas

> There have been lots of ideas, all just in code, from now on, adding here, what i think...

* 3D Rendering ->
  Currently, EVERYTHING happens, and is correctly simulating, devs working on this maybe satisfied, though actually being able see them moving will absolutely be a great thing

* Logging ->
  For logging purposes, another open source logging library maybe utilised. Some considerations are ->
  1. https://github.com/expnkx/fast_io

* Sparse Matrix for space ->
  Currently it uses the graphMat library for the 3D matrix, ie, the space here, in current scenario a sparse data structure will be more beneficial

> 3D Rendering wasn't the aim at beginning; Developed graphMat as dense matrix as per the eariler needs, they have slightly changed with the current ways entities exist

## Source of Idea

  Steins Gate (Anime)

  and...

  My Interest, and thinking 😎😎


:copyright: Aditya Gupta 2020
