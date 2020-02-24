from graphviz import Digraph

g = Digraph(comment='')


def new_block(scale,origin,address,length,label,ref):
    with g.subgraph(name='cluster_'+str(address), node_attr={'shape': 'record'}) as a:

        a.attr(label = label)
        for i in range(0,length):
            a.node(str(address+i),
                label='<{{X={0} | W<sub>x</sub> = {1:.4f} | Buf {2} | Ref {3}}}>'
                .format(i,((i-1)*scale+origin)*(not(i == 0 or i == length-1)),address+i,ref[i]),
                style="filled", fillcolor=('green' if i==0 or i==length-1 else 'white'))

        a.body.append('{rank=same;' + ';'.join([str(address+i) for i in range(0,length)]) + '}')
        a.body.append(' -> '.join([str(address+i) for i in range(0,length)]) + '[style=invis,weight=100,len=0,K=0]')

def block_indices(address,ref,name,formatter):
    with g.subgraph(name='cluster_'+str(address), node_attr={'shape': 'record'}) as a:
        a.attr(label = name)
        for i in range(0,len(ref)):
            a.node(str(address+i),
                label=formatter.format(ref[i]),
                style="filled", fillcolor=('white'))

        a.body.append('{rank=same;' + ';'.join([str(address+i) for i in range(0,len(ref))]) + '}')
        a.body.append(' -> '.join([str(address+i) for i in range(0,len(ref))]) + '[style=invis,weight=4.0]')

def legend(address,text,colors):
    with g.subgraph(name='cluster_'+str(address), node_attr={'shape': 'record'}) as a:
        for i in range(0,len(text)):
            a.node(str(address+i),
                label=text[i],
                style="filled", fillcolor=(colors[i]))

        a.body.append('{rank=same;' + ';'.join([str(address+i) for i in range(0,len(text))]) + '}')
        a.body.append(' -> '.join([str(address+i) for i in range(0,len(text))]) + '[style=invis,weight=4.0]')


new_block(0.01,0.0,0,8,'',[0,8,17,0,0,24,0,0])

new_block(0.01/5,0.0,8,8,'',[0]*8)

new_block(0.01/5,0.012,16,8,'',[0,0,0,0,0,32,0,0])

new_block(0.01/5,0.04,24,8,'',[0]*8)

new_block((0.01/5)/5,0.02,32,8,'',[0]*8)


block_indices(40,[0,8,16,24,32],"block_indices", '<Ref {0}}}>')
block_indices(45,[0,1,4],"block_depth_lookup", '<{0}}}>')

legend(50,["reference linkages","ghosts","ghost updates","interpolated"],['orange','green','blue',"red"])


g.edge('1','8',color='orange',penwidth='3')
g.edge('2','16',color='orange',penwidth='3')
g.edge('5','24',color='orange',penwidth='3')
g.edge('21','32',color='orange',penwidth='3')

g.edge('1','9',color='red',minlen='5',penwidth='3')
g.edge('2','14',color='red',minlen='5',penwidth='3')

g.edge('14','16',color='blue',minlen='2',penwidth='3')
g.edge('17','15',color='blue',minlen='2',penwidth='3')

g.edge('2','17',color='red',minlen='2',penwidth='3')
g.edge('3','22',color='red',minlen='2',penwidth='3')

g.edge('6','30',color='red',minlen='2',penwidth='3')
g.edge('5','25',color='red',minlen='2',penwidth='3')

g.edge('21','33',color='red',minlen='2',penwidth='3')
g.edge('22','38',color='red',minlen='2',penwidth='3')

g.edge('45','40',color='purple',penwidth='3')
g.edge('46','41',color='purple',penwidth='3')
g.edge('47','44',color='purple',penwidth='3')

#g.edge('40','7',weight='10')

#g.edge('40','8',color='red',minlen='10',penwidth='3', weight='0')





print(g.source)
g.format = 'svg'
g.render()
