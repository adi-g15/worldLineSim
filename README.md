# World Line Simulator

A BIG overhaul on the way... ab is duniya me bolne, sun ne, dekhne, sochne wale log honge... :D

<div align="center">
  <img src="https://api.codacy.com/project/badge/Grade/652ba837de78439f90a144db9b6cf4ac"/><!--(https://app.codacy.com/gh/adi-g15/worldLineSim?utm_source=github.com&utm_medium=referral&utm_content=adi-g15/worldLineSim&utm_campaign=Badge_Grade)-->
  <img src="https://www.deepcode.ai/api/gh/badge?key=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwbGF0Zm9ybTEiOiJnaCIsIm93bmVyMSI6ImFkaS1nMTUiLCJyZXBvMSI6IndvcmxkTGluZVNpbSIsImluY2x1ZGVMaW50IjpmYWxzZSwiYXV0aG9ySWQiOjIyMzM5LCJpYXQiOjE2MTM5OTgxMTN9.PrWhRLumHS9kjRO8KXnY3Yd-1bJJZ_-c07kptEyCQPs"/><!--)](https://www.deepcode.ai/app/gh/adi-g15/worldLineSim/_/dashboard?utm_content=gh%2Fadi-g15%2FworldLineSim)-->
  <img src="https://www.codefactor.io/repository/github/adi-g15/worldlinesim/badge"/><!--)](https://www.codefactor.io/repository/github/adi-g15/worldlinesim)-->

  <br/>
  <img src="https://github.com/adi-g15/worldLineSim/actions/workflows/cmake_ubuntu.yml/badge.svg"/><!--)](https://github.com/adi-g15/worldLineSim/actions/workflows/cmake_ubuntu.yml)-->
  <img src="https://github.com/adi-g15/worldLineSim/actions/workflows/cmake_win.yml/badge.svg"/><!--)](https://github.com/adi-g15/worldLineSim/actions/workflows/cmake_win.yml)-->
</div>

As of now, it `can` be said that it simulates a simple 3D multiverse

And... it currently doesn't have 3D rendering, since that wasn't of importance earlier, the theoretical implementation was the goal, but it's interesting,
think of the very basic idea as this ->

_Think about a world, in which some snakes keep moving automatically to get food, any \`time*\` the universe has some state; You go back in time, change anything, that then changes the state then, and hence the present and the future_

ie. Time Travel following the WorldLine theory, stated in the Anime `Steins Gate`

It has a 3D auto expanding space, like a simple form of our universe
**Now the project offers much more**, since you can now have as many entities as memory allows, and can CREATE your own entities too.

This project generalises the idea to entities and actions, in different worlds and worldlines, all independently on different threads, as well theoretically different verses too, other than this ->

It also features a constantly growing matrix (the world_plot), using another project [GraphMat3D](https://github.com/adi-g15/graphMat) that was actually a part of this one itself

## Building

First do a recursive clone, then

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

## Current State

> Note: Currently the focus is that _internally_ the simulation happens, but maybe the display won't be very great now, so it just logs on console (Since, I don't have a ground in rendering things currently, if you have ideas, or suggestions do put in that pinned issue)

> **NOTE: The time is integral now**

![Current State](ss.png "This is a part of the logging")

## Class Diagram

> Many entities have been added after this, leaving it so, as it clarifies the base things
>
> Made with draw.io

![Class Diagram](class_diagram.png "WorldLine Sim")

## Future Ideas

> There have been lots of ideas, all just in code, from now on, adding here, what i think...

* 3D Rendering ->
  Currently, EVERYTHING happens, and is correctly simulating, devs working on this maybe satisfied, though actually being able see them moving will absolutely be a great thing

* Sparse Matrix for space ->
  Currently it uses the graphMat library for the 3D matrix, ie, the space here, in current scenario a sparse data structure will be more beneficial

> 3D Rendering wasn't the aim at beginning; Developed graphMat as dense matrix as per the eariler needs, they have slightly changed with the current ways entities exist

## Source of Idea

  Steins Gate (Anime)

  and...

  My Interest, and thinking 😎😎


:copyright: Aditya Gupta 2020
