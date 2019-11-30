
MAX_DEPTH = 3

potentials = [0]*40
refined_indices = [0]*40

SIZES = [8,8,8]


potentials[14] = 14
potentials[17] = 17




refined_indices[1] = 8
refined_indices[2] = 16
refined_indices[5] = 24
refined_indices[21] = 32




def traverse_all(sync_depth, ignore_ghosts=0):

    current_depth = 0
    
    x = ignore_ghosts
    buffer_ref_queue = [0,0,0]
    buffer_x_queue = [0,0,0]
          
    while True:        
        buffer_point = buffer_ref_queue[current_depth]
    
        #print("Buf: {} Buffer pointer: {} x: {} buffer_ref_queue: {} current_depth: {}".format(buffer_point+x,buffer_point,x,buffer_ref_queue,current_depth))
        if(current_depth != sync_depth-1 and refined_indices[buffer_point+x]):
            #print("Descending to",current_depth+1)
            #Descend to next level down
            
            buffer_x_queue[current_depth] = x
            current_depth += 1
            buffer_ref_queue[current_depth] = refined_indices[buffer_point+x]
            x = ignore_ghosts
            continue
        
        if(x == SIZES[current_depth]-ignore_ghosts):
            #Finished with this depth; ascend back to previous layer.
            if(current_depth == 0): # done!
            
                break
            #print("Ascending to",current_depth-1)
            current_depth-=1;
            x = buffer_x_queue[current_depth]+1
            continue
    
        print(current_depth,x)
        ### Logic goes here.
        x+=1

traverse_all(3,ignore_ghosts=1)

def sync_ghosts(array, sync_depth):
    '''
    Transfers ghost values between blocks at sync_depth. 
    Performs a breadth-first search of the structure tree
    '''

    current_depth = 0
    
    x = 1
    
    buffer_ref_queue = [0,0,0]
    buffer_x_queue = [0,0,0]
           
    while True: 
       
        buffer_point = buffer_ref_queue[current_depth]
    
        if(current_depth != sync_depth-1 and refined_indices[buffer_point+x]):
            # Descend to block below
            buffer_x_queue[current_depth] = x
            current_depth += 1
            buffer_ref_queue[current_depth] = refined_indices[buffer_point+x]
            x = 1
            continue
        
        if(x == SIZES[current_depth]-1):
            # Ascend to block above
            if(current_depth == 0): # done!
                break
            current_depth -= 1;
            x = buffer_x_queue[current_depth]+1
            continue
    
        print(current_depth,x)

        if(current_depth == sync_depth-1 and refined_indices[buffer_point+x]): #actually sync
            # 0 0 0  
            # G G G     potentials[refined_indices[buffer_point+x]+0]
            #-^-^-^--
            # 4 2 6     potentials[refined_indices[buffer_point+x-1]+sizes[current_depth+1]-2]
            
            if(refined_indices[buffer_point+x-1]): #shouldn't have to worry about buffer overrun, because ghosts are ignored
                                                    #index block before
                #Update bottom ghost points                                                    
                array[refined_indices[buffer_point+x]] = potentials[refined_indices[buffer_point+x-1]+SIZES[current_depth+1]-2]
            else:
                pass
            
            if(refined_indices[buffer_point+x+1]):
                #Update top ghost points
                array[refined_indices[buffer_point+x]+SIZES[current_depth]-1] = potentials[refined_indices[buffer_point+x+1]+1]            
            else:
                pass # Ghosts at the edge are currently ignored; it must be ensured that no E-field lookups occur near the edges. 
                
        x+=1    


def transfer_down_interpolate(array):
    '''

        
    '''

    
    current_depth = 0
    
    x = 1
    
    buffer_ref_queue = [0,0,0]
    buffer_x_queue = [0,0,0]
           
    while True: 
       
        buffer_point = buffer_ref_queue[current_depth]
    
        if(current_depth != MAX_DEPTH-1 and refined_indices[buffer_point+x]):
            # Descend to block below
            buffer_x_queue[current_depth] = x
            current_depth += 1
            buffer_ref_queue[current_depth] = refined_indices[buffer_point+x]
            x = 1
            continue
        
        if(x == SIZES[current_depth]-1):
            # Ascend to block above
            if(current_depth == 0): # done!
                break
            current_depth -= 1;
            x = buffer_x_queue[current_depth]+1
            continue

    
        if(refined_indices[buffer_point+x]):
        
            xlen = SIZES[current_depth]
            for i in range(0,xlen-2):
                array[refined_indices[buffer_point+x]+i+1] = array[buffer_point+x]*((xlen-i)/xlen) + array[buffer_point+x+1]*(i/xlen) 
                
        x+=1    




#print(potentials)
#sync_ghosts(potentials,1)
#print(potentials)

print(potentials)
transfer_down_interpolate(potentials)
print(potentials)


