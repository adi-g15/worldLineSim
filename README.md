# World Line Simulator
=======================

Wait... if you are thinking it's some big rendering project, sorry but it's not,
it enjoys this condition ->

_Think about a world, in which 4 snakes keep moving automatically to get food_

And, here's what this project adds ->
_And... you can go to a particular time in **past**, and change the position of one of the snakes... Baammm, a new world line is created in which this event happened, and the older world had the event not taken place... so there is a high chance, current state(lengths of the snakes) of the world will be different than what had been in the previous world line_

That's it, this project does this much mainly (though no limitations on what it may add later).

It also features a constantly growing matrix (the world_plot).

### Compiling
NA - Not complete for a successful compile yet

### NOTE - 
1. 

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

## Source of Idea

  Steins Gate (Anime)

  and...

  My Interest, and thinking 😎😎


> `@future` - Since this project HEAVILY depends on asynchronous and multithreading approach, so better use a thread pool too in later time, for removing some of the overhead of creating and destroying threads again and again


:copyright: Aditya Gupta 2020
