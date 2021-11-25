#  Buzz Requirements – Requirements for Buzz

## Descriptions

This repo holds dynamic libraries and header files for Buzz.

## Setting environment variables for Buzz

Note: -**CLONEDDIR** points to the root directory of the this repo.  
Example: `d:\buzztest\buzzrequirements`

### For Windows:

```cmd
set BUZZREQS=CLONEDDIR
; for example d:\buzztest\buzzrequirements
set CGO_CFLAGS=-I%BUZZREQS%\include
set CGO_CPPFLAGS=-I%BUZZREQS%\include
set CGO_LDFLAGS=-L%BUZZREQS%\lib
```


### For linux, etc:



Ok, now we can install Buzz with **Go**.
