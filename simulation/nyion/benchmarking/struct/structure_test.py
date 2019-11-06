import numpy
import matplotlib.pyplot as plt
import math

CELL_SIZES = [16,16,16,4096,128,128,128,2097152] #l0 x,y,z,len, l1 x,y,z,len, 

def idx(x, y, z, x_len, y_len):
    return ((x_len*y_len*z) + (x_len*y) + x);
    
def is_inside_boundary(world_x,world_y,world_z):
    return False    

X = 0
Y = 1
Z = 2
LEN = 3

BUFFER_SIZE = 100000

WORLD_X_SIZE = 0.1
WORLD_Y_SIZE = 0.1
WORLD_Z_SIZE = 0.1

MAX_DEPTH = 1

particles = [0]*BUFFER_SIZE
electric_potential = [0]*BUFFER_SIZE
boundary_conditions = [0]*BUFFER_SIZE
refined_indices = [0]*BUFFER_SIZE
space_charge = [0]*BUFFER_SIZE



# if(buffer_end_pointer == 0): # if the root block hasn't been initialized yet.
    

def define_blocks(buffer_end_pointer):
    # called once on startup.
        
    buffer_end_pointer = 0
    
    for depth in range(0,MAX_DEPTH):    
        
        world_cell_origin_x = 0 
        world_cell_origin_y = 0
        world_cell_origin_z = 0
    
        xlen = CELL_SIZES[(depth*4)+X]
        ylen = CELL_SIZES[(depth*4)+Y]
        zlen = CELL_SIZES[(depth*4)+Z]
    
        world_x_scale = WORLD_X_SIZE
        world_y_scale = WORLD_Y_SIZE
        world_z_scale = WORLD_Z_SIZE
    
        for i in range(0,depth):
            world_x_scale /= CELL_SIZES[(i*4)+X]
            world_y_scale /= CELL_SIZES[(i*4)+Y]
            world_z_scale /= CELL_SIZES[(i*4)+Z]

        
        '''
        Initialize block.
        '''
        for x in range(1, xlen-1):
            for y in range(1, ylen-1):
                for z in range(1, zlen-1):
                    world_x = world_cell_origin_x+(x*world_x_scale)
                    world_y = world_cell_origin_y+(y*world_y_scale)
                    world_z = world_cell_origin_z+(z*world_z_scale)
                    
                    boundary_conditions[buffer_end_pointer + idx(x,y,z,xlen,ylen)] = is_inside_boundary(world_x,world_y,world_z)
                
        
        '''
        Refine cells that need to be refined.
        '''
        for x in range(1, xlen-1): #even the root has ghost points
            for y in range(1, ylen-1):
                for z in range(1, zlen-1):
                    index = buffer_end_pointer + idx(x,y,z,xlen,ylen)
                    
                    if(not (boundary_conditions[index+idx(0,0,0,xlen,ylen)]
                             == boundary_conditions[index+idx(0,0,1,xlen,ylen)]
                             == boundary_conditions[index+idx(0,1,0,xlen,ylen)]
                             == boundary_conditions[index+idx(0,1,1,xlen,ylen)]
                             == boundary_conditions[index+idx(1,0,0,xlen,ylen)]
                             == boundary_conditions[index+idx(1,0,1,xlen,ylen)]
                             == boundary_conditions[index+idx(1,1,0,xlen,ylen)]
                             == boundary_conditions[index+idx(1,1,1,xlen,ylen)]) or # check if this cell is homogenous; otherwise, it needs to be refined.
                                not space_charge[idx] == 0.0 or 
                                is_inside_special_boundary(world_x,world_y,world_z)):
                                                
                            refined_indices[index] = 0;

        buffer_end_pointer += idx(xlen-1,ylen-1,zlen-1,xlen,ylen) 
    
                
    return buffer_end_pointer


def sync_ghosts(depth):
    for true:
        for x,y,z in bla:
            

        
                


