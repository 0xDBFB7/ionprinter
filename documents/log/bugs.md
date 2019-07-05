Going to try to emulate NASA's software methodology and note the root cause of any especially tedious bugs that I encounter. This may help improve codebase quality, or it may end up being a waste of time.

GitHub's issues page may be better suited to these reports.



<hr>

Vector pass-by-value causes huge mesh copy for every call of every function:

```c++
relative_mesh_value(std::vector<std::vector<T>> input_mesh);
relative_mesh_value(mesh);//~10 us 

relative_mesh_value(std::vector<std::vector<T>>& input_mesh);
relative_mesh_value(mesh);//~0.8 us 


```





