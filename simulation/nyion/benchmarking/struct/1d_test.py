
potentials = [0]*40
refined_indices = [0]*40

SIZES = [8,8,8]

potentials[14] = 10
refined_indices[1] = 8
refined_indices[2] = 16
refined_indices[5] = 24
refined_indices[21] = 32



current_depth = 0

x = 0
buffer_ref_queue = [0,0,0]
buffer_x_queue = [0,0,0]

sync_depth = 2

for i in range(0,50):        
    buffer_point = buffer_ref_queue[current_depth]

    print("Buf: {} Buffer pointer: {} x: {} buffer_ref_queue: {} current_depth: {}".format(buffer_point+x,buffer_point,x,buffer_ref_queue,current_depth))
    if(current_depth != sync_depth-1 and refined_indices[buffer_point+x]):
        print("Descending to",current_depth+1)
        buffer_x_queue[current_depth] = x
        current_depth += 1
        buffer_ref_queue[current_depth] = refined_indices[buffer_point+x]
        x = 0
        continue
    
    if(x == SIZES[current_depth]-1):
        if(current_depth == 0): # done!
            break
        print("Ascending to",current_depth-1)
        current_depth-=1;
        x = buffer_x_queue[current_depth]+1
        continue
        
    x+=1 
    


        
        #    for x1 in range(0,l1_length):
        #        if(refined_indices[x1]):
        #            if(refined_indices[x1-1])
        #                data[x2] = [x1-1][]

