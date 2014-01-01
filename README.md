What is this?
=============
It is an 4KB remake of the classic puzzle game, "Box World" which in turn is a remake of the 
classic [Sokoban](http://en.wikipedia.org/wiki/Sokoban). The goal of the game is to move all the 
boxes to their storage locations in the warehouse to andvance to the next level. You control the 
player character with the arrow keys.

If you want to try this 4KB Windows version out, you can download it [here](https://github.com/osjoberg/boxworld4k/releases/download/1.1/BoxWorld4K.exe).

Features
========
Some of the highlights:
* 40+ awesome levels
* Seven beautiful tiles in seven mindbogglingly realistic colours
* Blazingly fast Windows user interface complete with menus and everything
* Autosave of your achivements
* Undo and restart when you are in a tight corner
* Complete keyboard support
* Level selection of completed levels
* Hours or even days of puzzling fun
* 4KB

How can all this goodness be so tiny?
=====================================
In approximate order of importance:
* The game has good qualifications to be tiny
* The standard library functionality was removed
* Graphics for the tiles are stored as graphics drawing instructions instead of being a bitmap
* Levels are stored in a cumbersome but compact way
* An unbelivably good linker/executable compressor is used, Crinkler
* Code was written to be compact first, maintainable second and not fast at all
* Code was rearranged to please the compressor
* Fiddled with the compiler and linker settings to find the optimal settings for tinyness

History
=======
I think it was in the summer of 2002 when I had lot´s of spare time between semesters that I got 
the idea of trying to recreate a fun game with the goal of cramming in as many fun play hours as 
possible in 32KB. At the time I was playing "Box World" which is a game that I really love. It 
turns out it was a really good candidate for this project because it is a puzzle game which takes 
time to play, it is addictive and as a bonus it has really simple mechanics, small levels and a low 
tile count.

I started churning away and a few weeks later my first version was done with a few levels. Without 
compression it weighed in around 32KB. I knew that adding compression would likely half that number 
and I would succeed without doing any further optimizations. With half of the summer left I started 
the painstaking process of making it into a 4KB game. Once I got the game to be smaller than 4KB 
compressed without levels I was able to make one small optimization at a time and for every major 
successful optimization I was able to add a few more levels. After struggling during the second 
half of the summer and a few more weeks into the autumn I finally had the game complete with 40+ 
levels in 4095 bytes. Mission complete!

During the winter of 2013 a friend thought that I should put the source up on GitHub for the world 
to see. However since the state of the art executable compression technology for 4KB demos that I 
used back in 2002 no longer works, I had to switch compression technology. Thankfully I found 
Crinkler which is doing an impressive job of doing a lot better compression while at the same time 
preserving the PE header enough to make it run. That is why the game now weighs in in just under 
3.5KB!

Future
======
Since there is a few hundred more bytes available now, I think it would be best to use it properly. 
Some things that I really would like to see:
* More levels (is it possible to cram in all 100 levels in 4KB?)
* More space saving optimizations
* A rotating player character
* Number of moves counter
* Current level indication
* Improved graphics
* Something else?

Pull requests are more than welcome!