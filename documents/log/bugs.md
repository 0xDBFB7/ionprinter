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

###Trying to switch OpenGL mode between Perspective to Ortho expended several hours of testing. It seemed that the mode change ruined the perspective rotation somehow.

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

Ensure that unit tests also test for physical correctness. Ensure also that the success state can only be attained in the correct way.