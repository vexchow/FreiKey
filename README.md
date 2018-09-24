# FreiKey: Firmware for my custom keyboards
I've now made 3 fully custom keyboards. [ErgoBlue](https://github.com/kevinfrei/ErgoBlue) was just a gateway drug!

The first two are the same keyboard (one for work, one for home!). The third,
however, is a keyboard I plan on taking with me when I travel. In order, they're
named "FreiKeys" and "Betterfly". FreiKeys is this beauty:

![Final Canvas XDA](docs/FinalCanvasXDA.jpg)

I designed it with AutoDesk Fusion360, and milled it (PCB, aluminum plate, and
walnut case) with my sweet, sweet [MillRight Power Route
CNC](https://millrightcnc.com/product/millright-cnc-power-route-kit-bundle/). I
can't say enough good things about MillRight. The sales process was marvelous,
providing me lots of support while I was assembling & testing out the machine
and the forums are a great resource. Since I enjoyed both the process and the
end result so much, I decided to look around and see if there was another place
I would like a nice custom keyboard. As luck would have it, my work laptop is a
[2017 MacBook 12"](https://support.apple.com/kb/sp757), which is a great laptop
for wandering around, walking between buildings at work, and unfolding fully
even when boxed into cramped airline seats. That
["butterfly"](https://bgr.com/2018/07/14/macbook-pro-2018-butterfly-keyboard-fixed-ifixit/)
keyboard, though? A steaming POS. Almost as noisy as MX Blue's, but (I'm *not*
exaggerating) only slightly more tactile than the original [Surface Touch
keyboard](https://support.microsoft.com/en-us/help/4039633/surface-touch-cover).

So, given how much I've come to enjoy my little thumb cluster, I stuck that
design on a single PCB. My only real complaint about the FreiKeys is that I have
only a single LED to display any sort of status. So, while designing the
Betterfly board, I decided to toss on a little OLED. I really did like having
that around on my old (now unused) Infinity ErgoDox. I picked up a [Teensy
3.2](https://www.pjrc.com/store/teensy32.html), and an [SSD1306 128x32 I2C OLED
from AdaFruit](https://www.adafruit.com/product/931), and away we go:

![Betterfly](docs/Betterfly.jpg)

That was a photo before I had any of the software working: I just wrote a little
display routine to validate that all of the key switches worked properly. Since
that photo, I have it working as a normal keyboard, albeit only as a single
layer. I've been cleaning up & restructuring the firmware quite a bit, in order
to support Teensy (and I hope to support a wireless OLED for FreiKey as some
point in the future). I'm still trying to figure out a pleasing way to both
abstract out all the differences between the Teensyduino keyboard stuff and the
AdaFruit Bluetooth keyboard stuff, and add more flexibility to my firmware. (I
know, I guess I could try to port to QMK or something, but I'm an engineering
manager and have been for more than half my career, now, so I'm actively looking
for ways to scratch the coding itch that keeps me well clear of doing any damage
to the code that my far-more-talented-software-developers-than-myself who I
support write :) So, in the mean time, I'm also putting together a write up of
both FreiKey and Betterfly.

## Quick Summary: Betterfly

This beauty is made with Kailh Choco Burnt Orange switches, though I have some
Choco Browns on the way: The Burnt Orange switches are just a little bit too
heavy: I'd love a middle grown between the two (MX Clears, and Zealios Purples
both fit this bill), but I'd rather have a little too light than a little too
heavy. I ordered them from
[NovelKeys](https://novelkeys.xyz/collections/switches), along with a set of
keycaps. I didn't bother with LED's under the keys (unsure if I'm going to
regret that...) because I've never done that, and manually routing the PCB for a
whole separate LED matrix sounds like a **lot** of work.

The cool thing here, however, is the stuff "around" the board itself. As I
mentioned above, I travel for work sometimes. When I travel, I'm stuck using
either some crappy keyboard laying around at work at some poor soul's desk where
I squat for a day or so, or I have to use the @#$% butterfly thing on the Mac.
After a day or so, my wrists get aggravated. This keyboard has the PCB bolted to
a slab of 1/8" UHMW plastic on the back (attached with 11 M3 recessed bolts, with lock washers and
cap nuts, which I think look moderately decent). The slab-of-plastic has some
clips (also UHMW), which then screw onto the 'side rails' you can see in the
photo (I should probably sand them: they look kind of rough) that slide onto the
MacBook, securing nicely with a hole in the left side for the USB-C connector,
and a hole on the right that will fit a narrow headphone jack. The key switches
mount directly on the PCB, which is exposed (and you can see my goofy PCB trace
design: the light spots are where I routed, so the traces are actually quite
fat!), then the back protects the keyboard (it doesn't seem to hit keys, so I
use Karabiner Elements to disable the builtin keyboard when it detects this guy
attached).

One final physical feature: Those side clips that slide it onto the laptop?
They're the only thing you have to change to adapt it to another device. They
attach with a pair of M1.6 screws that key the thing attached quite securely.
So, when I get a new laptop, I just have to design & mill a couple new small
parts, no new soldering, and \*poof\* Betterfly can carry on it's wrist-saving
ways!

I'll throw links to the AutoDesk Fusion360 project here in the near-future. In
the mean time, I'm procrastinating a bunch of stuff for work that I ought to be
doing. Later... (and here's what I'd already written about the FreiKeys)

## Quick Summary: FreiKeys

I designed & manufactured this thing all myself. It's using a pair of AdaFruit
Feather nRF52's, same as the ErgoBlue keyboard I wired together myself. The
reason for this keyboard is twofold:

1. A better fit for my thumbs. The ErgoDox thumb cluster took a couple weeks to
get accustomed to, and when I've gone back and tried it after having grown
accustomed to my new FreiKeys, I have the same problem: the cluster is too far
away. My thumb doesn't naturally 'rest' where the cluster is.
2. A reasonable place for an "Arrow-T". Yes, I could have mapped an arrow
cluster somewhere on the ErgoDox, but wherever it went, it would have been
moving around a key that I didn't want to move/relearn. FreiKeys has an arrow
cluster (and, on the other side, a pgup/pgdn/home/end cluster) directly below
the home row. You just have to slide your hand down about 2 inches!

Rather than cutting an aluminum plate and manually wiring the matrix, I designed
a PCB, with a spot to put the Feather controllers. This was the first time I've
designed (and milled) a PCB, as well as the first time I've milled aluminum, so
it's been an adventure. A few key points worth noting:

* Small end mills break easily (particularly on Aluminum)!
* Large 'V' bits **don't** break easily (certainly not while ripping through
  some copper clad PCB blanks)
* Clearing away the spare copper of a PCB isn't necessary. You just need to make
  sure each circuit that matters is isolated (much faster to mill.)
* Aluminum is much harder than wood
* Milling aluminum is messy (either using cutting fluid, thereby extending the
  life of your bits, or just those damn aluminum shavings, which aren't
  magnetically responsive and behave like glitter)
* New router bits cut aluminum much better
* I still haven't got the right speed/feed for milling the copper off of copper
  clad PCB's cleanly
* Sanding your finished PCB can get rid of the messy edges, but can also sand
  off pieces of the trace you care about!
* Surface mounting something that isn't really designed for it is awkward

## Narrative

With that, here's what I did:

First, I swiped the ErgoDox STL files to get a good 'starting point' for key
layout. I like the ortholinear, staggered fingers for the ErgoDox. I actually
shifted the index finger to be a little lower than the ring finger, and pulled
the G and H rows down a little bit more, because it made sense to me (at least
for my hand/finger shape). Next, I deleted the "inner" row of keys. They were
always too far a reach, and I didn't really find them that useful. I always had
to look to see what I was hitting. I then added an extra set of 3 keys in the
J/K/L columns to provide the Arrow-T locations. Finally, I put a few keys in a
rainbow shape near where I figured my thumb would be comfortable. Instead of
just milling it in aluminum (which I knew was going to be messy), I extruded the
key holes into 'fake' keys and milled the shape out of a chunk of old styrofoam
insulation I've been carrying around for years. It's super easy to mill, and
holds a shape nicely. This gave me an opportunity to sort of try the layout of
the thumb cluster to see if it was comfortable. Turns out, I needed the keys a
little closer. At the same time, I figured it wouldn't hurt to add another key
beside the arrow cluster.

Once I had the key layout, I milled a baseplate out of 1.6mm aluminum. This was
my first time milling aluminum, so I broke a lot of bits, but eventually got a
finished plate. I stuck a bunch of keyswitches in the plate to get a feel for
them, and decided I needed to change the thumbs a bit more. I turned the
'resting' key from a 1x1.5 to a simple 1x1 key, and pulled the 1.5 and 1.25 keys
even closer, leaving the solitary 1x1 at the bottom. In addition, I had started
designing the PCB & wooden cases and realized that I needed to carve out a space
for a battery, so I took out a chunk from the aluminum plate under the wrist
wrest to put it.

For the PCB's, I started out with a matrix, similar to what I did for the
ErgoBlue project. Once I had the matrix, I started running traces. I didn't
really optimize for anything, here. I'm sure I could have done a much better job
at preventing myself from needing to do so much soldering of wire on the
diode-side of the PCB if I'd spent more time. Maybe next project :)

The big, exciting addition to this project is an LED. While the ErgoBlue worked
fine, I found only being able to use the "typing stuff into a text window"
capability to communicate status a bit of a bummer. Each half has a solitary LED
in the upper 'inside' corner, near the '5' and '6' keys. As of this writing, the
only thing that it does is flicker when you're over 10% battery life (and you
hit a combination of 4 keys), or stay solid if you're under 10%. But I plan on
addition more capabilities "soon".

For the wood cases, there are a couple things of note. The first is that I had
the leg (almost) fully modeled, so I went ahead and had the CNC cut it out. It
was kind of slow, because of the tapers, but resulted in a completely consistent
leg length, which is somewhat difficult to do manually on such a small part. The
other interesting element of the case is that the leg is attached with a cheesy
little 1" x 1/2" hinge, which has a small cut-away on the "inner" edge of the
case, which allowed for a smaller case. I had milled one case with enough space
to attach the leg hinge on the underside of the case, but that resulted in a
rather chunky looking case. I prefer the new version.

A few final points of interest: I'm using 2500mAH batteries from AdaFruit. They
last about a week, even with basically *no* attention to power consumption.
***Update: with minor attention to "sleeping" (scan @ 4Hz instead of constant if
nothing's happened for a few minutes) the batteries last a bit over 3 weeks. I
should disable the voltage regulator. AdaFruit seems to imply that doing so
would further reduce power consumption...*** If you use different batteries,
you'll need to change the shape of the battery cut-outs to match. I carved the
cases out of walnut this go around. It's a gorgeous, dark wood, and is hard
enough that I didn't have *any* breaking pieces, even with connections of less
than a 50 square millimeters.

## The Good Stuff

Here's stuff that doesn't live on GitHub (A bunch of AutoDesk Fusion 360 projects)
* [The aluminum
keyplate](https://a360.co/2L1pxVX) is the same for both the left & right hand.
* [The left side PCB](https://a360.co/2KXEIiF).
* [The right side PCB](https://a360.co/2KZmqhe).
* [The wooden cases](https://a360.co/2L0rK3Y) in a single project.

Here's a view pictures of the various parts in various stages of creation:

### Milling Aluminum is Kinda Messy
![Doing this right takes time](docs/AluminumIsHard.jpg)

This was my practice sheet of aluminum. The first problem was bad "speeds and
feeds". The second was that a 1/8" bt is just too big. I didn't want the corners
that 'cleaned out'. The final key-hole was cut using a single-flute 2mm bit,
which can be had for cheap on Amazon.
