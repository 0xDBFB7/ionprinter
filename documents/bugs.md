Going to try to emulate NASA's software methodology and note the root cause of any especially tedious bugs that I encounter. This may help improve codebase quality, or it may end up being a waste of time.

GitHub's issues page may be better suited to these reports.



<hr>

#### Issue:

Vector passed by value causes huge mesh copy at every call of a function:

```c++
relative_mesh_value(std::vector<std::vector<T>> input_mesh);
relative_mesh_value(mesh);//~10 us 

relative_mesh_value(std::vector<std::vector<T>>& input_mesh);
relative_mesh_value(mesh);//~0.11 us 
```

##### Discovery

relative_mesh_value() seemed to run exceptionally slowly. Expensive cube root calls were initially blamed, but the problem persisted even when all the contents of relative_mesh_value were removed.

##### Solution

Most other functions had the proper dereference in place. This problem could be solved procedurally by incorporating timing metrics into unit tests.

<hr>

#### Issue:

submesh_side_length returned incorrectly rounded results for certain specific values.

##### Discovery

Mesh decoarsen call failed for certain lengths. This was traced to a failure in submesh_side_length.

##### Solution

Exhaustive test procedures should be implemented wherever possible.

<Hr>

#### Issue:

```c++
int argc;
char *argv[0];
glutInit(&argc, argv); //segfaults
```



```c++
int argc = 0;
char *argv[1];
glutInit(&argc, argv);
```

##### Discovery

Commenting out an unrelated bit of code triggered this error.

##### Solution

Memory fuzzer testing would probably have caught this earlier.

<hr>

#### Issue:

Trying to switch OpenGL mode between Perspective to Ortho expended several hours of testing. It seemed that the mode change ruined the perspective rotation somehow.

##### Discovery

Removing all functions and writing a minimal test case in a separate inline loop clearly demonstrated that the 3d renderer was fine; rather, the viewpoint code was broken.

##### Solution

Particularly persistent bugs should be isolated from the codebase for closer examination if possible.

<hr>

#### Issue:

Return value neglected for custom min and max functions:

```c++
float mesh_max(std::vector<std::vector<T>> &input_mesh, root_mesh_geometry mesh_geometry){
  float max = 1e-30;
  for(int root = 0; root < mesh_geometry.root_size; root++){
      ...
  }
  ...nothing, you idiot!
}
```

##### Discovery

These functions always returned zero, which made draw_mesh very uncomfortable indeed.

Surprisingly, g++ did not take issue with this code, despite the use of `-Wall -Wextra -pedantic -Werror`.

##### Solution

All compiler warnings were disabled; they were promptly re-enabled.

<hr>

#### Issue:

Relative mesh indexing functions handle root boundaries incorrectly.


This bug somehow survived some ten rewrites of the relevant function.

##### Discovery

Convergence was very slow, even with multigrid methods.

The issue was obvious once the 3D opengl viewer was functional.

##### Solution

Ensure that unit tests also test for physical correctness. Ensure also that the success state can only be attained in the correct way; that no other combination of errors can lead to the asserted result.

For instance, if all values in an array are set to 1, any malformed array index will yield 1. Instead, all values should be initialized to zero, and only the requested point should be set.

<hr>

#### Issue:

Math within macro call subtly broken by order-of-operations

```
#define idx(x,y,z,x_len,y_len) ((x_len*y_len*z) + (x_len*y) + x)
idx(r_x,r_y,r_z+1,root_x_len,root_y_len) //13
idx(r_x,r_y,r_z+1,root_x_len,root_y_len) //14
r_z + 1 //22
idx(r_x,r_y,,root_x_len,root_y_len) 
```

##### Discovery

Rewritten relative mesh indexing function broke for edge cases. Issue only discovered due to extra time spent on unit test function; must be more disciplined in the future. 

Sprinkling printfs made the issue apparent.

##### Solution

Convert from macro to inline function to avoid such issues in the future. 

-E would be a useful troubleshooting feature, but it's breaking cmake.



<hr>

#### Issue:

Nearly a month of testing still did not yield a functional multigrid implementation.

#### Solution

Writing a proof-of-concept recipe in Python without regard to optimization very rapidly exposed the problem. This is definitely something to keep in mind; it's much easier to troubleshoot something if you know precisely the expected behavior. 

<hr>

#### Issue:

 A constant defined as double fully halved a kernel's performance.

```
(6.0*U[idx(x,y,z,X_SIZE,Y_SIZE)]
5000 us
```


```
(6.0f*U[idx(x,y,z,X_SIZE,Y_SIZE)]
2200us
```


<hr>


#### Issue:

Unit testing may be a hindrance in the early rapid prototyping stages of a project. 

##### Discovery 

Many hundreds of hours were spent developing the first (very dumb) data structure; hundreds of lines of unit tests were written and thrown out as the API changed almost hourly. Eventually it became apparent that this structure wouldn't work. The new linear-stack adaptive block-structured tree shares none of the tests.



#### Issue:

Sneaky uninitialized buffers can lead to undefined behavior:

```
void sync_ghosts(){

...

    int buffer_ref_queue[MAX_DEPTH]; //store previous block reference
    int buffer_x_queue[MAX_DEPTH];  //store value index in block 

```

is broken. 


```

void sync_ghosts(){

    int buffer_ref_queue[MAX_DEPTH] = {0}; //store previous block reference
    int buffer_x_queue[MAX_DEPTH] = {0};  //store value index in block 
    
```

works.

#### Solution

A fuzzer would be helpful. Static analysis, too.

