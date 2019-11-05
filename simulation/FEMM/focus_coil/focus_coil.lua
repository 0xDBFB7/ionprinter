
showconsole()
mydir="./"
open(mydir .. "dyneboard10.fem")
mi_saveas(mydir .. "temp.fem")
mi_seteditmode("group")
for n=0,10 do
    mi_analyze()
    mi_loadsolution()
    mo_groupselectblock(1)
    fz=mo_blockintegral(19)
    print((10-n)/10,fz)
    if (n<10) then
        mi_selectgroup(1)
        mi_movetranslate(0,0.49)
    end
end
mo_close()
mi_close()
