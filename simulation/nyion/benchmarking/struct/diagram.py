from graphviz import Digraph

g = Digraph(comment='The Round Table')




def new_block(scale,origin,address,length):
    with g.subgraph(name='cluster_'+str(address), node_attr={'shape': 'record'}) as a:
        #a.attr(rank = 'same')
        for i in range(length-1,-1,-1):
            a.node(str(address+i),
                label='<{{X={} | W<sub>x</sub> = {} | Buf {} | Ref 0}}>'
                .format(i,((i-1)*scale+origin)*(not(i == 0 or i == length-1)),address+i),    
                style="filled", fillcolor=('green' if i==0 or i==length-1 else 'white'))        

        a.body.append('{rank=same;' + ';'.join([str(address+i) for i in range(0,length)]) + '}')      
        a.body.append(' -> '.join([str(address+i) for i in range(0,length)]) + '[style=invis]')


new_block(0.01,0.0,0,8)

new_block(0.01,0.0,8,8)

g.edge('1','9',color='red',minlen='20',penwidth='3')
g.edge('2','14',color='red',minlen='20',penwidth='3')
    
print(g.source)  
g.format = 'svg'
g.render()