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



Refbase:

Installed php, php extensions, bib extensions , moved everything to /var/www/html/

`mysql_connect has been removed from php 7. Installed php5.6.`

had to reset sql password with 

```update user set plugin='mysql_native_password' where User='root';```

also needed

`sudo sed -i 's/TYPE=/ENGINE=/g' install.sql`