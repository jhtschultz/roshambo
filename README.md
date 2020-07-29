# roshambo

This branch contains a modified version of the test suite used in the first
International RoShamBo Programming Competition, intended for inclusion in
OpenSpiel. `main()` has been commented out, as has the declaration of `bzero()`
on line 2159, which caused an error when building on Mac OS X. The bot
Greenberg, winner of the second competition held in 2000, has been added to
the list of bots. All symbols that could potentially cause name collisions
have been prefixed with `ROSHAMBO_BOT_`. Finally, a C++ map interface is
included to facilitate importing the bots into OpenSpiel.

The original, unaltered versions of the code here are stored in the
master branch of this repository.
