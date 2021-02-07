# World Line Simulator

Wait... if you are thinking it's some big rendering project, sorry but it's not but it's interesting,
think of the very basic idea as this ->

_Think about a world, in which 4 snakes keep moving automatically to get food_

Now the project offers mcuh more, since you can now have as many entities as memory allows, and can CREATE your own entities too.

This project generalises the idea to entities and actions, in different worlds and worldlines, all independently on different threads, as well theoretically different verses too, other than this ->
_And... you can go to a particular time in **past**, and change the position of one of the entities... Baammm, a new world line is created in which *this event* happened, and the older world had *this event* had not taken place... so there is a high chance, current state(lengths of the snakes, positions of entities, etc. combined) of the world will be different than what had been in the previous world line_

It also features a constantly growing matrix (the world_plot), using another project [GraphMat3D](https://github.com/adi-g15/graphMat) that was actually a part of this one itself

## Building

```sh
cd build   # if doesn't exist, then create
cmake ..
cmake --build .
```

> It WILL build using MSVC compiler
>
> the github action is failing, since it's not building on GCC due to SFINAE errors, which work on MSVC though GCC maybe more strict, i am not focusing on that for now

## Creating your own Custom ENTITIES (for example, horses)

* Inherit from the Entity class (Interface actually), override/implement the `startSimulation` and `pauseSimulation` virtual member functions, and others that are pure virtuals, and need to be overloaded in Entity class

* In `World` constructors, make sure your push your new entities to the entities vector with `entities.push_back(new Horse(this));`

> Constructor - An entities constructor takes in the World*, so that it has access to the worldplot (consider it the space fabric here, just a matrix), for eg, calling world->getRandomCoord() to get a position in the world
> See the constructor of other entity, for eg. Human, Rock, Snake for reference

## Current State (Logs)

> Note: Currently the focus is that _internally_ the simulation happens, but maybe the display won't be very great now, so it just logs on console (Since, I don't have a ground in rendering things currently, if you have ideas, or suggestions do put in that pinned issue)

![Current State](https://github.com/adi-g15/Sources/blob/main/worldLineSim_state.png?raw=true "This is a part of the logging")

## Class Chart

> Made with draw.io

![Class Chart](https://github.com/adi-g15/Sources/blob/main/worldLineSim.png?raw=true "WorldLine Sim")

## Future Ideas

> There have been lots of ideas, all just in code, from now on, adding here, what i feel...

* UI libraries ->
  Consider using libraries that have been made for multi threaded applications, or better handle those cases. Some considerables are ->
  1. https://github.com/wjakob/nanogui
  2. https://github.com/dankamongmen/notcurses

* I/O ->
  Likely not required with the UI libraries, but likely for logging another stream maybe required. Some considerations are ->
  1. https://github.com/expnkx/fast_io

* Multi-Threading ->
  Since this project HEAVILY depends on asynchronous and multithreading approach, so better use a thread pool too in later time, for removing some of the overhead of creating and destroying threads again and again
  There are likely multiple thread pool implementations, use one of them.

## Source of Idea

  Steins Gate (Anime)

  and...

  My Interest, and thinking 😎😎


:copyright: Aditya Gupta 2020
