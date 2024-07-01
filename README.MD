# Welcome to GDScript++

A rewrite of original gdscript that allows for better modifications, and will hopefully be faster.

### motivation:
gdscript is currently not well suited for implementing external libraries and functionality, missing a lot of featurs such as operator overloading.
And with the core of godots functionality being centered around It's own types I found it hard to work with, for my very specific usecases.

And with main godot development being focused on **improving the engine** I've decided to make my own version of GDscript that will allow me to more comfortably develop my own apps.
That said this project is most likely **not going to be a full replacement** for gd script, but rather an addition that helps with dealing with external libraries.

With the focus being on speed and external features, this **deviates quite a lot from what seems to be godots core design**, which is centered around portability of it's own types.


### IMPORTANT NOTE

***DO NOT HARASS GODOT DEVELOPERS!*** <br>
**Don't ask them to use this as a replacement** for gdscript in the engine it self. <br>
Far too often have I seen people being mad that some plugin isn't part of the engine.<br>
This not being part of the engine is fine, In fact, I like it this way more, I have complete control and freedom to develop and document this in the way that suits me more.

Remember, this is like a minecraft mod, while it can be cool and useful, it's, in the grand scheme of things, a large change that is to run in it's own direction.

Please be nice to all the devs, everyone is trying their best.


### core functionality:
#### VM:
gdspp VM is in fact only used for saving and loading data, and then calling this data. 