from root_numpy import array2root
import numpy as np
import h5py
from ROOT import TFile, TTree
from array import array
import sys

fname = []
if len(sys.argv) != 1:
    for fn in range(1,len(sys.argv)):
        fname.append(sys.argv[fn])
        
else:
    print ('Type -h or --help to see other usage')
    fin = raw_input('please enter the file name (must be .hdf5): ')
    fname.append(fin)
    if(fin == '-h' or fin == '--help'):
        print ('There are 2 usages:')
        print ('1. python convertmultivar.py A.hdf5 B.hdf5 C.hdf5 ....')
        print ('2. python convertmultivar.py'+ ' to input a single file.')
        sys.exit(0)
        
for fn in fname:
    print ('Opening file '+fn+' ...')
    g = h5py.File(fn, 'r')
    datac = g['events/clock']
    datas = g['events/signal']
    datah = g['events/header']
    dataT = g['events/temperature']
    datat = g['events/time']
    enum = datac.shape[0]
    tar_c  = []
    tar_s  = []
    tar_h  = []
    tar_T  = []
    tar_t  = []
    tar_e  = []
    print ('writing a tree...')
    for i in range(enum):
        if i%1000 == 0:
            print ('processing',i,'/',enum,' of data...')
        A1 = (datac[i],)
        A2 = (datas[i],)
        A3 = (datah[i],)
        A4 = (dataT[i],)
        A5 = (datat[i],)
        A6 = (i,)
        tar_c.append(A1)
        tar_s.append(A2)
        tar_h.append(A3)
        tar_T.append(A4)
        tar_t.append(A5)
        tar_e.append(A6)


    out_e = np.array(tar_e ,dtype=[('event',np.int32,(1,))])
    out_c = np.array(tar_c ,dtype=[('clock',np.float64,(1,))])
    out_s = np.array(tar_s ,dtype=[('signal',np.int32,(128,))])
    out_h = np.array(tar_h ,dtype=[('header',np.int32,(16,))])
    out_T = np.array(tar_T ,dtype=[('temperature',np.float64,(1,))])
    out_t = np.array(tar_t ,dtype=[('time',np.float64,(1,))])

    kstart = fn.rfind('/')+1
    kend   = fn.find('.hdf5')
    fout   = fn[kstart:kend]
    print ('Output file will be '+'data/'+fout+'.root')

    array2root(out_e,'data/'+fout+'.root','tree', mode='recreate')
    array2root(out_c,'data/'+fout+'.root','tree', mode='update')
    array2root(out_s,'data/'+fout+'.root','tree', mode='update')
    array2root(out_h,'data/'+fout+'.root','tree', mode='update')
    array2root(out_T,'data/'+fout+'.root','tree', mode='update')
    array2root(out_t,'data/'+fout+'.root','tree', mode='update')

    g.close()
    print ('Closing file '+fn+' ...')
print ('done!')

