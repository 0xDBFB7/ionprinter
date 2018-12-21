--bf_output.lua--
x1=0
x2=0.7
dx=0.001

y1=0.00
y2=0.2
dy=0.001

ni = (x2-x1)/dx+2
nj = (y2-y1)/dy+2

handle=openfile("../../IBSimu/simulations/magnetic_focusing_1/B.dat","w")

for j=0,nj-1,1 do
  for i=0,ni-1,1 do
  x=x1+i*dx
  y=y1+j*dy
  A,B1,B2=mo_getpointvalues(x,y)
  write(handle,x," ",y," ",0," ",0," ",B2,"\n")
 end
end

closefile(handle)
