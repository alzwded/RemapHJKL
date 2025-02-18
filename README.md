RemapHJKL
=========

the down key on my keyboard broke, so I've improvised

This app toggles the keyboard state into *vim navigation mode* which remaps HJKL to the arrow keys. That's it. This is only because my down arrow key broke and I really need that key.

To switch the layout into *vim navigation mode* press `Ctrl`-`Alt`-`3`. To switch back out, press `Ctrl`-`Alt`-`3` again.

Keys
----

| Keys                | Effect                       |
|---------------------|------------------------------|
| `Ctrl` `Alt` `3`    | Enter/leave hijack mode      |
| `H`, `J`, `K`, `L`  | Arrows                       |
| `Bksp`              | Left arrow                   |
| `Space`             | Right arrow                  |
| `6`, `0`, `A`       | `Home`                       |
| `4`, `E`            | `End`                        |
| `B`, `F`            | `PgUp`, `PgDwn`              |
| `C`                 | `Pause/Brk`                  |

All other letters and numbers get silently gobbled up to avoid confusion.

TODO
====

More vimness! In particular:

* [x] smarter icon that reflects the current state (e.g. `i` and `n`)
* [x] ~~`^D` & `^U`~~ as F and B for now
* [x] `^` & `$`
* [ ] `G`
* [ ] `y`, `d`, `p`
* [ ] maybe registers someday
* [ ] visual mode
