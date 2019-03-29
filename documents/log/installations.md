MathGL:

Download source.

-fPIC error when cmaking with -D=enable-fltk=on

Added 

```
add_compile_options(-fPIC)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")
```

to CMakelists.txt

Recompiled and reinstalled FLTK with 

```
./configure --enable-shared CFLAGS=-fPIC CXXFLAGS=-fPIC
```

Then built mathgl normally.



