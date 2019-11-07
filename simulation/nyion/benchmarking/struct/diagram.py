from graphviz import Digraph

g = Digraph(comment='The Round Table')




def new_block(scale,origin,address,length,label):
    with g.subgraph(name='cluster_'+str(address), node_attr={'shape': 'record'}) as a:
        
        a.attr(label = label)
        for i in range(0,length):
            a.node(str(address+i),
                label='<{{X={0} | W<sub>x</sub> = {1:.4f} | Buf {2} | Ref 0}}>'
                .format(i,((i-1)*scale+origin)*(not(i == 0 or i == length-1)),address+i),    
                style="filled", fillcolor=('green' if i==0 or i==length-1 else 'white'))        

        a.body.append('{rank=same;' + ';'.join([str(address+i) for i in range(0,length)]) + '}')
        a.body.append(' -> '.join([str(address+i) for i in range(0,length)]) + '[style=invis,weight=4.0]')


new_block(0.01,0.0,0,8,'')

new_block(0.01/6,0.0,8,8,'')

new_block(0.01/6,0.0083,16,8,'')

g.edge('1','9',color='red',minlen='5',penwidth='3')
g.edge('2','14',color='red',minlen='5',penwidth='3')

g.edge('14','16',color='blue',minlen='2',penwidth='3')
g.edge('17','15',color='blue',minlen='2',penwidth='3')

g.edge('2','17',color='red',minlen='2',penwidth='3')
g.edge('3','22',color='red',minlen='2',penwidth='3')


print(g.source)  
g.format = 'svg'
g.render()
