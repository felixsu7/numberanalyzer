# Design / Architecture

*(note: currently messy)*

## Code Architecture

TODO

## The GUI (or precisely the TUI)

First, when we say GUI, it means "Graphical User Interface", so technically, what we have implemented is still technically a GUI. However, the specific type we did that is in a terminal is called a TUI, or "Terminal User Interface".

It's not as powerful as real GUIs, but is still useful and actually common for developer tools.

But how can you display something like that when we can seemingly only output lines?

### ANSI Sequences.

Basically, terminals (at least in UNIX and Linux world) had an interesting history, and people came up with a way to give programs more control over the terminal.

By outputting special kinds of sequences, called ANSI sequences, you can do many things, output styled text (bold, italics, colored, etc.), or even access the mouse.

Not all terminals are equal and this especially shows for windows, as they don't share the same root as UNIX and Linux. However, modern windows terminals, except in some cases, does support ANSI sequences, so we are kinda fine anyways.

There's actually one more part we need to do to achieve absolute control: Raw Mode.

### Raw Mode

To understand raw mode, you first need to be aware that terminals already does a lot of work for you, you don't receive lines until user presses enter, keys actually popping up on screen (echo), scrollback history (you can see input/output above, like you can scroll back), etc. This is called "Cooked Mode", it exists because of history. Terminals that we use in computers actually emulate real terminals and that is why some behavior exists. What we want is much more control over the terminal, we do that by telling the terminal to enter Raw Mode.

Except Raw Mode means different for linux and windows. I am not that familiar with windows.

In linux though, without going into too much, each terminal has special settings programs can just modify, and through that, we can enter raw mode, along with other useful things.

The term "Raw Mode" is loose and probably not official coined, but basically, it just means the program is telling the terminal that stop doing certain useful things and let the program take more control.

You can see the code at `src/platform.cpp`, specifically the `platform_term_activate()` functions.

Listing exactly what it did is long and doesn't belong here anyways.

Windows, idk (yet) lol

But yeah, since in Cooked Mode, the terminal already does a lot of work for us. If we do need these things again while in Raw Mode, most of the times, it's much better if we do implement theses by ourselves again.

### What we do now

By entering the raw mode and being aware of the power ANSI sequences brings, what exactly can we do?

At least what's relevant are:
- Control exactly where is a character placed in the terminal, like pixels in a screen (not quite)
- Style text, the "letter" or character itself (foreground), it's color and if it's bold, italic, strikethrough, etc.; and the color of the background behind it
- Keys won't pop up automatically when we type (echo), we need to control that behavior exactly.
- Know the size of the terminal, it's width and height.
- Access the mouse position and if something was pressed.
- Alternative screens, basically when we switch to TUI mode, we can save the previous state and then load it after exiting. Easy switching between text mode and TUI mode basically.
- Tell the terminal to wait for us to finish outputting something, so "screen tearing" doesn't happen, basically V-Sync, but not quite.
- Even beep, but this is unreliable.

And much more, but that's what it matters.

Great, we basically have a mini version of a GUI, in a terminal. There's still more things to understand and do before we get to drawing buttons and stuff.

We can now think of the terminal as a screen, able to draw text anywhere.

We can directly draw out text if we want to, but to make certain things easier, we need a better representation of this screen: Framebuffers

### Framebuffer

Framebuffers are basically arrays of size (width x height), and they will be painted to the screen every frame. It makes handling drawing easier.

How drawing works in general, is that you have a framebuffer, you draw to it, once we are done, then we paint that framework to the screen, then we clear the framebuffer, and loop again; that how it works even in real graphical applications, especially games.

However, doing this in the terminal, despite needed, is expensive, whole screen for maybe 20 FPS is expensive and will slow the whole program down.

We don't need to paint the whole framebuffer, we only need to paint back the differences between the last frame and the current frame. (idk if this is also done in real apps)

Getting data from the screen is expensive though, so we need another framebuffer to represent the current state of the screen.

So we have two framebuffers now:
- Front Framebuffer: represents the current screen or terminal itself, must stay in sync, otherwise rendering errors happen.
- Back Framebuffer: where we do draw to.

When we want to paint a new frame to the screen, we compare the two framebuffers, and only send back their differences to the terminal. Now the back framebuffer now represents the screen and the front framebuffer is outdated, we swap them (C pointer stuffs basically), then clear the now back framebuffer, for the new frame to begin.

This is technically called Double-buffering, common in real apps, but the reason we use it here is probably different from theirs.

### Sync

We cannot directly paint the whole framebuffer to the screen though, since the differences are strings containing sequences telling the terminal what to do, well under the hood, these sequences are interpreted one by one. Could be fast, but we can still see "tearing" in the terminal in the middle of the process of a frame being painted.

We can easily fix that by telling the terminal that to wait for us to finish painting back the frame.

Basically, at the beginning of a frame, we tell the terminal to stop drawing anything for awhile and wait, remembering what we told it while it waits. This is the sync begin.

We paint a new frame, the terminal remembers what we tell it, but doesn't really update anything on screen yet.

Then, we give the sync end command, the terminal does what we told while it waited, not one by one anymore, but basically all at the same time.

No more screen tearing, and it's easy to do.

### One quirk, and optimization for it.

How a terminal works is that it has a cursor pointing to a character, basically, anything we print out goes to where the cursor points, so we move it, that's basically two commands, one for the movement, and the content itself. So we can't really directly draw something at somewhere.

Though, it moves itself to the right when we draw something, so printing "Hello", does draw "Hello", no need to move the cursor by ourselves. Just like how it works in normal terminal text applications.

Currently, everytime we want to draw something somewhere, it's always two commands. It would be a waste if we are drawing something like a row, and we do that a lot.

With the knowledge of the cursor automatically moving to the right, we can optimize the code.

If we only need to move one time to the right, then we don't say the cursor needs to move to the right exactly one time. That's one command saved.

This cursor movement command is also somewhat long and thus takes a bit relatively long to process, not much, but it stacks up. We have many "pixels" to be painted many times per second, it's a great saving really alongside the frame difference optimization.

In fact, since we have easy access to the front framebuffer, we can simply "run over" the "pixels" by redrawing them with the data from the front framebuffer. No differences, but we just avoided the long cursor movement command. 

Before we go to the higher levels of drawing commands, we need to handle the inputs first.

### Input handling

TODO

### Higher drawing commands

TODO

### Widgets

TODO

### How it works as a whole

TODO

### Summary

TODO

## Math Expression Parser

TODO
