It really isn't that special, the most that this code does is just calling other functions or storing data.

# internals
## processor
processor is focused on storing and reading data, and on calling functions.
Which makes it much closer to the high level implementation, but also means that technically what is processed by it, is more similar to micro code rather than operations.

While this will power the gdscript++ it can be used separately, or even with other languages, that is dependent on the JIT.

## buffers and storage
We need somewhere to store our data, so this is it 
### Function buffer
Stores function pointers, in a vector, nothing special
### Parameter buffer
Stores parameters for functions, these are taken as varargs, and stored here.
### Storage buffer
Stores pointer to data, could be classes too.
### Local Stack
Stack... used for stack yeah lol

## operations
Operators have 32 bit value separated into different parts.
First 8 bits contain the main operation category.
Second 8 bits contain the actual operation, see descriptions below.
The last 16 bits contain variations, these can be anything like size of of data etc.. haven't thought this part trough yet.

Operators are always followed by 3 arguments, with these colors:
- <font color="#C2F261">first</font>
- <font color="#FAA613">second</font>
- <font color="#F44708">third</font>
If operator takes different arguments it will be specified inside of it.

### [0x00] NOP - no operation
Not really necessary but it's implementation doesn't cost anything and we could use it for debugging or small delays, other data can be used for custom debugs etc.
### [0x01] NEW - create new object/type/variable
Creates a new thing.
#### [0x0100] NEW class - creates new class
<font color="#C2F261">class constructor</font> - (index in function buffer) function that is called when constructing class.
<font color="#FAA613">parameters</font> - (index in parameter buffer) parameters used to create the new object.
<font color="#F44708">storage</font> - (index in storage buffer) where to store the reference to class
if storage == 0, the 