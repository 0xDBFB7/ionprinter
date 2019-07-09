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

<hr>



