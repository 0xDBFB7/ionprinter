### Simulation

### Salome-Meca

A really excellent program, with one fatal flaw.

#### IBSimu

A really nice ion optics simulation library. I'm a novice in numerical methods, so I encountered some stability issues at high current densities. 

https://sourceforge.net/p/ibsimu/mailman/message/31283601/

Change the solver number to be 1 higher than the previous solver.

#### CAELinux

#### ElmerFEM also has support for charged particle tracing.

To export from ElmerFEM, insert the following code into the .sif file: 

```
Solver 2 ! Set the correct solver number here.
              ! Usually you want to use the highest number so the results are saved at the end of a timestep.
  Exec Solver = String "after timestep"
  exec interval = 1
  Equation = String "ResultOutput"
  Procedure = File "ResultOutputSolve" "ResultOutputSolver"
  Output File Name = String "paraver." !or any other output file name of your choice
  Output Format = String "vtk"
  Vtk Format = Logical True
End
```

If your mesh is really big, you can save via Sif -> Generate + Sif -> Edit -> Save (in the text editor)

http://www.elmerfem.org/forum/viewtopic.php?f=7&t=3264&hilit=mortar#p10380

StatCurrentSolve can output the current and heating power values; just set

`  Calculate Volume Current = True`
  `Calculate Joule Heating = True`

http://www.elmerfem.org/forum/viewtopic.php?f=3&t=3246&p=10282&hilit=find+current+potential#p10282

#### Sparselizard

#### COMSOL

Maybe someday this company will be able to afford a COMSOL license.

That day is not today.



Paraview - sometimes you've gotta hit color map > rescale to data range after reloading new .vtks.

https://github.com/ElmerCSC/elmerfem/tree/devel/fem/tests

https://barbagroup.github.io/essential_skills_RRC/laplace/1/