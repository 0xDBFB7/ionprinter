Young, David A. *Soft-sphere model for liquid metals*. No. UCRL--52352. California Univ., 1977.

has some parameters for different metals n' stuff.



Had to change line 539 of pizza/dump.py to

 ```     if atoms is not None:```

to get it to run.



The output dump was exported to vtk by running 

```vtk(dump("bowtie_nozzle/dump.all")).many("output/new")```

```python -i pizza/src/pizza.py```