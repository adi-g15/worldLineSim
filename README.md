# World Line Simulator

Wait... if you are thinking it's some big rendering project, sorry but it's not,
it enjoys this condition ->

_Think about a world, in which 4 snakes keep moving automatically to get food_

And, this project generalises this idea to entities and actions, in different worlds and worldlines, as well theoretically different verses too, other than this ->
_And... you can go to a particular time in **past**, and change the position of one of the snakes... Baammm, a new world line is created in which this event happened, and the older world had the event not taken place... so there is a high chance, current state(lengths of the snakes) of the world will be different than what had been in the previous world line_

That's it, this project does this much mainly (though no limitations on what it may add later).

It also features a constantly growing matrix (the world_plot).

### Building

**READ THE NOTE AFTER THESE COMMANDS**

```sh
cd worldLineSim/build   # if doesn't exist, then create
cmake ..
cmake --build .
```

> Now depending on your compiler, you will have a `simulator` or `simulator.exe`, which is the executable after building, you can run it then

_NOTE_ - 
  1. Still doesn't use all the functions.
  2. It has been built without linking(referencing) to all features(functions) just so to know how it works at that particular time
  3. There maybe commits in the master branch itself, that WON'T BUILD, but i will make sure to either build it, or remove the command area for the time being then

### NOTE - 
1. Currently all commits are on the master branch itself, and not on a separate dev branch, since i prefer to keep track of all my 'initial
 work on master itself (this thing will change once the initial idea is done)
2. Initially the focus is that _internally_ the simulation happens, but maybe the display won't be very great now (Since, I don't have a ground in rendering things currently, if you have ideas, or suggestions do put in that pinned issue)

## Flags used in code

Various flags (inside comments), have been used to convey some messages to the developer ->
* `@todo` - These comments show that particular thing is to be done
* `@future` - These are tasks to be done, AFTER the basic idea is done, say for example, an improvement to the begaviour or algorithm
* `@note` - Some information about the code given to a user, or a simple note to let you know how it might behave
* `@warning` - A caution note to the reader (generally telling that you should call other function for cleaning the resources used (pointers, thread etc.))
* `@debug` - These signify the part of code that is only there for debugging purposes, and should be removed later for actual accepted behaviour
* `@me` - Notes to myself
* `@deprecated` - Portions of codes, or functions that won't be used (likely not used in the code), and will be removed with the following commits

Other than these, usual single line comments, are 'low-priority comments' more general than these

And, multi line comments are generally for briefly telling about some struct or class

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
