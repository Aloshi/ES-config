ES-config
=========

A flexible tool for configuring multiple emulators simultaneously.

Building
========

As some of the dependencies are not in the Debian repositories, getting dependencies is a little bit complicated.
To solve that, **I've written a short Bash script named "get_dependencies.sh" that does everything for you**, including install AngelScript.

OR, If you want it install everything yourself, you'll need these dependencies:
```
sudo apt-get install libsdl1.2-dev libsdl-ttf2.0-dev libsdl-image1.2-dev
```

You'll also need [AngelScript](http://www.angelcode.com/angelscript/downloads.html), which is inconveniently absent from the Debian repositories!
At the time of writing, the newest version is 2.26.1.

After all that's over, just run `make`. The resulting binary is named `es-config`, and is not copied anywhere.

Usage
=====

Command line arguments:
```
--scriptdir [path]		search for scripts at [path]
--resourcedir [path]	load images, etc from [path]
--help								pierce the heavens
```

As this is primarily an input configuration interface, it is not currently navigatable with the keyboard.
F4 will close the application if it hasn't frozen.

-Aloshi
http://www.aloshi.com
