It really isn't that special, the most that this code does is just calling other functions or storing data.

# internals
## processor
processor is focused on storing and reading data, and on calling functions.
Which makes it much closer to the high level implementation, but also means that technically what is processed by it, is more similar to micro code rather than operations.

While this will power the gdscript++ it can be used separately, or even with other languages, that is dependent on the JIT.

## calling functions
I've stumbled upon a very cool design:
which deals with varargs
```cpp
#include <type_traits>

template<typename R, typename... Args>
auto call(R(*function)(Args...), Args... args) -> typename std::enable_if<!std::is_same<R, void>::value, R>::type {
    return function(args...);
}


template<typename... Args>
void call(void (*function)(Args...), Args... args) {
    function(args...);
}


int funcTestA(int a, float b, int c)
{
    return a * b * c;
}

int funcTestB(int a, int c)
{
    return a * c;
}

int funcTestC()
{
    return 0;
}

void funcTestE()
{
    volatile int a = 0;
}

int main() {
    int a = 1;
    float b = 2;
    int c = 3;

    volatile int result;

    result = call(funcTestA, a, b, c);
    result = call(funcTestB, a, c);
    result = call(funcTestC);
    call(funcTestE);

    return 0;
}
```


## buffers and storage
We need somewhere to store our data, so this is it 
### Function buffer
Stores function pointers, in a vector, nothing special
### Parameter buffer
Stores parameters for functions, these are taken as varargs, and stored here.
Parameter buffer comes with parameter hints, they say if parameters are actually in varargs, or if they're indexes to storage buffer.

### Storage buffer
Stores pointer to data, could be classes too.
### Local Stack
Stack... used for stack yeah lol

## operations
Operators have 32 bit value although only 8 bits are used.
This is to improve cache sizing

Operators are always followed by 3 32 bit arguments, with these colors:
- <font color="#C2F261">first</font>
- <font color="#FAA613">second</font>
- <font color="#F44708">third</font>
If operator takes different arguments it will be specified inside of it.

### [0x00] NOP - no operation
Not really necessary but it's implementation doesn't cost anything and we could use it for debugging or small delays, other data can be used for custom debugs etc.

### [0x01] NEW - create new object/type/variable
Creates a new thing.
<font color="#C2F261">class constructor</font> - (index in function buffer) function that is called when constructing class.
<font color="#FAA613">parameters</font> - (index in parameter buffer) parameters used to create the new object.
<font color="#F44708">storage</font> - (index in storage buffer) where to store the reference to class
if storage == 0, the object pointer is passed onto local stack

### [0x02] DEL - delete object/type/variable
deletes a thing.
deleting here just calls a destructor, and removes a reference to the object, in case where the object manages it's own lifetime, this will have no effect on it.
<font color="#C2F261">class destructor</font> - (index in function buffer) destructor
<font color="#FAA613">storage</font> - (index in storage buffer) 

### [0x03] EQ - equals
runs `==` on two storage items
<font color="#C2F261">first item</font>
<font color="#FAA613">second item</font>
### [0x04] NEQ - not equals
runs `!=` on two storage items
<font color="#C2F261">first item</font>
<font color="#FAA613">second item</font>
### [0x05] LOQ - less or equal
runs `<=` on two storage items
<font color="#C2F261">first item</font>
<font color="#FAA613">second item</font>
### [0x06] MOQ - more or equal
runs `=>` on two storage items
<font color="#C2F261">first item</font>
<font color="#FAA613">second item</font>
### [0x07] LES - less
runs `<` on two storage items
<font color="#C2F261">first item</font>
<font color="#FAA613">second item</font>
### [0x08] MOR - more
runs `>` on two storage items
<font color="#C2F261">first item</font>
<font color="#FAA613">second item</font>

### [0x09] BR - branch
Branch will store current position onto stack
<font color="#C2F261">address to branch to</font> 
### [0x0a] CBR - conditional branch
Branch if last stack value is above 0
Branch will store current position onto stack
<font color="#C2F261">address to branch to</font> 

### [0x0b] RET - return
Will return from current position onto one stored on stack

### [0x0c] JMP - jump
<font color="#C2F261">address to jump to</font> 
### [0x0d] CJMP - conditional jump
Jump if last stack value is above 0
<font color="#C2F261">address to jump to</font> 

### [0x0e] RJMP - relative jump
<font color="#C2F261">address to jump to, based from current address</font> 
### [0x0f] RCJMP - relative conditional jump
Jump if last stack value is above 0
<font color="#C2F261">address to jump to, based from current address</font> 

### [0x10] EXIT - return
Returns back to whatever called the VM
<font color="#C2F261">return value type</font>
- 0 means void
- 1 means value from stack
- 2 means value from storage buffer
<font color="#FAA613">storage buffer index</font> - used only if type is 2