#  Buzz Requirements – Requirements for Buzz

## Descriptions

Buzz is a Golang library built to be used out of the box.  
It is aimed to be used for interactive environments (games etc.).  
Uses Buzz CGO. This repo holds the header files and dynamic/static object files that Buzz needs. It is important that those who want to use Buzz do not search for a library and can make a quick start.

## Setting environment variables for Buzz

Note: -**CLONEDDIR** points to the root directory of the this repo.  
Example:  
`d:\buzztest\buzzrequirements`

### For Windows:

```cmd
set BUZZREQS=CLONEDDIR
; for example d:\buzztest\buzzrequirements

set CGO_CFLAGS=-I%BUZZREQS%\include
; Parameters to pass  to the C compiler on your platform. For example you can use "-O3".

set CGO_CPPFLAGS=-I%BUZZREQS%\include
; Parameters to pass  to the C++ compiler on your platform. For example you can use "-O3".

set CGO_LDFLAGS=-L%BUZZREQS%\lib
; Parameters to pass  to the linker on your platform. For example you can use "-lto" etc.
```

### For linux, etc:

TODOS.

***

Ok, now we can install Buzz with **Go**.
