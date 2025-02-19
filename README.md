RemapHJKL
=========

the down key on my keyboard broke, so I've improvised

This app toggles the keyboard state into *vim navigation mode* which remaps HJKL to the arrow keys. That's it. This is only because my down arrow key broke and I really need that key.

To switch the layout into *vim navigation mode* press `LControl`-`LWin`-`3`. To switch back out, press `LControl`-`LWin`-`3` again.

Keys
----

| Keys                  | Effect                                        |
|-----------------------|-----------------------------------------------|
| `LControl` `LWin` `3` | Enter/leave hijack mode                       |
| `H`, `J`, `K`, `L`    | Arrows                                        |
| `6`, `0`, `A`         | `Home`                                        |
| `4`, `E`              | `End`                                         |
| `B`, `F`              | `PgUp`, `PgDwn`                               |
| `R`                   | `Break` (use with Control)                    |
| `P`                   | `Pause` (the weirdest scancode)               |
| `I`                   | When in hijack mode, leaves hijack mode       |
| `S`                   | HJKL and Arrow keys send mouse scroll events instead

All other letters and numbers get silently gobbled up to avoid confusion.

And yes, I override a global hotkey reserved by Windows because it reserves it for something I never use.

The windows key by itself cannot be used as a modifier in a very easy way.
If you never noticed, the start menu opens only if there were no other keys
between key down and key up. So even if you capture Win+whatever, you still
have to issue the key up event. You can mix in some random modifier, but then
you need to check the state of the other modifiers to synthesize them before
getting to key up. It's too much of a faff, so use Super + Control and call
it a day.

The hotkey used to be Ctrl Alt 3, but the new Windows Terminal actually made
me use that key :-)

Also, be warned, Ctrl + Alt on windows is *also* AltGr, except when it isn't
and applications read the keys at a lower level. Actually, this is the problem,
it sometimes is, sometimes isn't AltGr.

Caveats
-------

Under heavy system load, this will break and keys will get stuck.

Because it's no longer using `RegisterHotkey`, you can now run it multiple
times, which will be confusing. I'll need to get around to making sure
it only runs once...

Building
--------

If you are very familiar with native development, you can skip this section and do what you normally do, there are no special build requirements.

You need some version of Visual Studio C++ Build Tools (or Visual Studio Community Edition, probably easier to get the latter).

It doesn't really depend strongly on any particular build tools version. I just go into the [.vcxproj](./RemapHJKL/RemapHJKL.vcxproj) and change `PlatformToolset` to whatever I randomly have installed. Visual Studio itself will ask if you want to upgrade the solution or to retarget the platform toolset if you happen to have a different one. I think the newest API quirks I use hail from the Vista era, so pretty much any extant version of the build tools will do.

Anyway, once that's out of the way, just build the release version for `x64`.

If you only installed the build tools without visual studio, there's a [build script](./build.bat) which calls `msbuild`.

The Release version is the only one worth running. This is not really debugabble, as the hook will still be registered even while the program is stopped. And you want all the speed optimizations the compiler will throw, this is ultimately adding lag to every input event on the system.

Build the app with the right amount of bitness (usually x64).

TODO
====

More vimness! In particular:

* [ ] check if it's already running
* [x] smarter icon that reflects the current state (e.g. `i` and `n`)
* [x] ~~`^D` & `^U`~~ as F and B for now
* [x] `^` & `$`
* [ ] `G`
* [ ] `y`, `d`, `p`
* [ ] maybe registers someday
* [ ] visual mode
