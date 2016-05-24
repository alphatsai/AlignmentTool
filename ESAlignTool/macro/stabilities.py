#!/usr/bin/python
import os, re, sys, shutil
import math, time
from ROOT import *

### DB file base points
X00={'mR':0.,        'mF':0.,        'pF':0.,       'pR':0.      }
Y00={'mR':0.,        'mF':0.,        'pF':0.,       'pR':0.      }
Z00={'mR':-308.306,  'mF':-303.846,  'pF':303.846,  'pR':308.306 }
A00={'mR':0.,        'mF':0.,        'pF':0.,       'pR':0.      }
B00={'mR':0.,        'mF':0.,        'pF':0.,       'pR':0.      }
G00={'mR':0.,        'mF':0.,        'pF':0.,       'pR':0.      }

### Starting points ( aligment tag IVO + base point on db )
## 1. 2015 summer propmt reco
#X0={'mR':-0.06,    'mF':-0.05,    'pF':0.52,    'pR':0.54    }
#Y0={'mR':-0.749,   'mF':-0.779,   'pF':-0.97,   'pR':-0.959  }
#Z0={'mR':-309.286, 'mF':-304.646, 'pF':304.466, 'pR':309.116 }
#A0={'mR':1.9,      'mF':2.2,      'pF':0.1,     'pR':-1.6    }
#B0={'mR':-0.2,     'mF':0.5,      'pF':0.3,     'pR':-0.5    }
#G0={'mR':-0.1,     'mF':0.5,      'pF':0.8,     'pR':0.5     }

## 2. 2015 RunC,D propmt reco
#X0={'mR':-0.303,   'mF':-0.284,   'pF':0.324,   'pR':0.357   }
#Y0={'mR':-0.462,   'mF':-0.477,   'pF':-0.932,  'pR':-0.905  }
#Z0={'mR':-308.922, 'mF':-304.289, 'pF':304.167, 'pR':308.770 }
#A0={'mR':1.4,      'mF':1.4,      'pF':-1.3,    'pR':-1.7    }
#B0={'mR':0.5,      'mF':0.8,      'pF':0.1,     'pR':0.4     }
#G0={'mR':-0.,      'mF':0.6,      'pF':0.8,     'pR':0.6     }

## 2. 2016 RunB, 80X_dataRun2_Prompt_v8 (used Run2015 Boff)
X0={'mR':-0.225,   'mF':-0.242,   'pF':0.331,   'pR':0.351   }
Y0={'mR':-0.503,   'mF':-0.534,   'pF':-0.927,  'pR':-0.898  }
Z0={'mR':-310.274, 'mF':-305.657, 'pF':305.510, 'pR':310.086 }
A0={'mR':1.4,      'mF':1.2,      'pF':-1.2,    'pR':-1.2    }
B0={'mR':0.2,      'mF':0.6,      'pF':0.0,     'pR':0.4     }
G0={'mR':0.1,      'mF':0.6,      'pF':0.8,     'pR':0.6     }

### Input files
#inputFile='pdf/inputMatrixElementsRunB.txt'
#inputFile='pdf/inputMatrixElementsRunC25ns.txt'
#inputFile='pdf/inputMatrixElementsRunDv3.txt'
#inputFile='pdf/inputMatrixElementsRunDv4.txt'
inputFile='results/17May_Run2016B/inputMatrixElements_cfi.txt'

#output='pdf/Stabilities_RunB'
#output='pdf/Stabilities_RunC25ns'
#output='pdf/Stabilities_RunDv3'
#output='pdf/Stabilities_RunDv4'
output='results/17May_Run2016B/pdf/Stabilities'


### Main functions
print '>> [INFO] Read file '+inputFile
lines0 = open(inputFile)
lines1 = filter( None, ( line.strip() for line in lines0 ))                             # Remove empty line
lines2 = filter( lambda x: not x.startswith('#'), ( line.strip() for line in lines1 )) # Remove # comments lines
lines  = filter( lambda x: not x.find('MatrixElementsTmp.Iter'), ( line.strip() for line in lines2 )) # Remain MatrixElementsTmp.Iter*

dX={'mR':{}, 'mF':{}, 'pF':{}, 'pR':{}}
dY={'mR':{}, 'mF':{}, 'pF':{}, 'pR':{}}
dZ={'mR':{}, 'mF':{}, 'pF':{}, 'pR':{}}
dA={'mR':{}, 'mF':{}, 'pF':{}, 'pR':{}}
dB={'mR':{}, 'mF':{}, 'pF':{}, 'pR':{}}
dG={'mR':{}, 'mF':{}, 'pF':{}, 'pR':{}}

X={'mR':0., 'mF':0, 'pF':0, 'pR':0 }
Y={'mR':0., 'mF':0, 'pF':0, 'pR':0 }
Z={'mR':0., 'mF':0, 'pF':0, 'pR':0 }
A={'mR':0., 'mF':0, 'pF':0, 'pR':0 }
B={'mR':0., 'mF':0, 'pF':0, 'pR':0 }
G={'mR':0., 'mF':0, 'pF':0, 'pR':0 }

def fillValue( position, plain, niter, value):
    position[plain][int(niter)]=float(value)
    sorted(position[plain]) 

def checkSize( dX, dY, dZ, dA, dB, dG, position ):
    total = len(dX[position])+len(dY[position])+len(dZ[position])+len(dA[position])+len(dB[position])+len(dG[position])
    if total*10/6%10 != 0:
        print '>> [ERROR] IterN is '+position+' is different'
        return False
    return True 

def setStyle( gh, title, xtitle, ytitle, ymin, ymax ):
    gh.SetTitle(title)
    gh.GetXaxis().SetTitle(xtitle)
    gh.GetXaxis().SetLabelFont(42)
    gh.GetXaxis().SetLabelSize(0.04)
    gh.GetYaxis().SetTitle(ytitle)
    gh.SetMaximum(ymax)
    gh.SetMinimum(ymin)

for line in lines:
    niter = line.split('Iter')[1].split('_ES')[0]
    plain = line.split('_ES')[1].split('=')[0].split('d')[0]
    dpos  = line.split('_ES')[1].split('=')[0].split('d')[1].rstrip()
    value = line.split('_ES')[1].split('=')[1].split('(')[1].split(')')[0] 

    if dpos in 'X':
        fillValue( dX, plain, niter, value )
    elif dpos in 'Y':
        fillValue( dY, plain, niter, value )
    elif dpos in 'Z':
        fillValue( dZ, plain, niter, value )
    elif dpos in 'Alpha':
        fillValue( dA, plain, niter, value )
    elif dpos in 'Beta':
        fillValue( dB, plain, niter, value )
    elif dpos in 'Gamma':
        fillValue( dG, plain, niter, value )
    else:
        print 'Not found '+dpos

if not checkSize( dX, dY, dZ, dA, dB, dG, 'mR' ):
    exit() 
if not checkSize( dX, dY, dZ, dA, dB, dG, 'mF' ):
    exit()
if not checkSize( dX, dY, dZ, dA, dB, dG, 'pR' ):
    exit()
if not checkSize( dX, dY, dZ, dA, dB, dG, 'pF' ):
    exit()

for p in ['mR', 'mF', 'pF', 'pR']:
    print '>> [INFO] Filling '+p
    X[p]=X0[p]
    Y[p]=Y0[p]
    Z[p]=Z0[p]
    A[p]=A0[p]
    B[p]=B0[p]
    G[p]=G0[p]

    c = TCanvas( str('c_'+p), '', 200, 10, 700, 500 )
    c.GetFrame().SetFillColor( 21 )
    c.GetFrame().SetBorderSize( 6 )
    c.GetFrame().SetBorderMode( -1 )
    c.Divide(3,2)

    size=len(dX[p])
    gX = TGraph(size)
    gY = TGraph(size)
    gZ = TGraph(size)
    gA = TGraph(size)
    gB = TGraph(size)
    gG = TGraph(size)
    for i in range(1,size+1):
        X[p]-=dX[p][i]
        Y[p]-=dY[p][i]
        Z[p]-=dZ[p][i]
        A[p]-=dA[p][i]*1000
        B[p]-=dB[p][i]*1000
        G[p]-=dG[p][i]*1000

        gX.SetPoint(int(i-1),Double(i),Double(dX[p][i]))
        gY.SetPoint(int(i-1),Double(i),Double(dY[p][i]))
        gZ.SetPoint(int(i-1),Double(i),Double(dZ[p][i]))
        gA.SetPoint(int(i-1),Double(i),Double(dA[p][i])*1000)
        gB.SetPoint(int(i-1),Double(i),Double(dB[p][i])*1000)
        gG.SetPoint(int(i-1),Double(i),Double(dG[p][i])*1000)

    setStyle( gX, 'dX(cm)',        'iteration', '', -0.05, 0.05)
    setStyle( gY, 'dY(cm)',        'iteration', '', -0.05, 0.05)
    setStyle( gZ, 'dZ(cm)',        'iteration', '', -0.05, 0.05)
    setStyle( gA, 'd#alpha(mrad)', 'iteration', '', -0.1,  0.1)
    setStyle( gB, 'd#beta(mrad)',  'iteration', '', -0.1,  0.1)
    setStyle( gG, 'd#gamma(mrad)', 'iteration', '', -0.1,  0.1)

    tl = TLine(1,0,size,0)
    tl.SetLineColor(2)
    c.cd(1)
    gX.Draw('A*L')
    tl.Draw('SAME')
    c.cd(2)
    gY.Draw('A*L')
    tl.Draw('SAME')
    c.cd(3)
    gZ.Draw('A*L')
    tl.Draw('SAME')
    c.cd(4)
    gA.Draw('A*L')
    tl.Draw('SAME')
    c.cd(5)
    gB.Draw('A*L')
    tl.Draw('SAME')
    c.cd(6)
    gG.Draw('A*L')
    tl.Draw('SAME')
    c.SaveAs(str(output+'_'+p+'.pdf'))

outputFile = open(output+'_FinalPositions.txt', 'w')

### Print out base point on db
print  ">> [INFO] Ideal P0"  
print  "          %5s %6s %6s %8s %5s %5s %5s  "%( 'plain', 'X(cm)', 'Y(cm)', 'Z(cm)', 'Alpha', 'Beta', 'Gamma' )
outputFile.write("Ideal P0:\n") 
outputFile.write("%5s %6s %6s %8s %5s %5s %5s\n"%( 'plain', 'X(cm)', 'Y(cm)', 'Z(cm)', 'Alpha', 'Beta', 'Gamma' ))
for p in ['mR', 'mF', 'pF', 'pR']:
    print  "          %5s %6.3f %6.3f %8.3f %5.1f %5.1f %5.1f  "%( p, X00[p], Y00[p], Z00[p], A00[p], B00[p], G00[p] )
    outputFile.write("%5s %6.3f %6.3f %8.3f %5.1f %5.1f %5.1f\n"%( p, X00[p], Y00[p], Z00[p], A00[p], B00[p], G00[p] ))

outputFile.write("\n")
 
### Print out starting point  
print  ">> [INFO] Un-Aligned P"  
print  "          %5s %6s %6s %8s %5s %5s %5s  "%( 'plain', 'X(cm)', 'Y(cm)', 'Z(cm)', 'Alpha', 'Beta', 'Gamma' )
outputFile.write("Un-Aligned:\n") 
outputFile.write("%5s %6s %6s %8s %5s %5s %5s\n"%( 'plain', 'X(cm)', 'Y(cm)', 'Z(cm)', 'Alpha', 'Beta', 'Gamma' ))
for p in ['mR', 'mF', 'pF', 'pR']:
    print  "          %5s %6.3f %6.3f %8.3f %5.1f %5.1f %5.1f  "%( p, X0[p], Y0[p], Z0[p], A0[p], B0[p], G0[p] )
    outputFile.write("%5s %6.3f %6.3f %8.3f %5.1f %5.1f %5.1f\n"%( p, X0[p], Y0[p], Z0[p], A0[p], B0[p], G0[p] ))

outputFile.write("\n")
 
### Print out aligned point  
print  ">> [INFO] Aligned P-dP" 
print  "          %5s %6s %6s %8s %5s %5s %5s "%(  'plain', 'X(cm)', 'Y(cm)', 'Z(cm)', 'Alpha', 'Beta', 'Gamma')
outputFile.write("Aligned P-dP:\n") 
outputFile.write("%5s %6s %6s %8s %5s %5s %5s\n"%( 'plain', 'X(cm)', 'Y(cm)', 'Z(cm)', 'Alpha', 'Beta', 'Gamma' ))
for p in ['mR', 'mF', 'pF', 'pR']:
    print  "          %5s %6.3f %6.3f %8.3f %5.1f %5.1f %5.1f  "%( p, X[p], Y[p], Z[p], A[p], B[p], G[p] )
    outputFile.write("%5s %6.3f %6.3f %8.3f %5.1f %5.1f %5.1f\n"%( p, X[p], Y[p], Z[p], A[p], B[p], G[p] ))

outputFile.write("\n")

### Print out aligned point w.r.t base point 
print ">> [INFO] Aligned (P-dP)-P0" 
print "          %5s %6s %6s %8s %5s %5s %5s "%(   'plain', 'X(cm)', 'Y(cm)', 'Z(cm)', 'Alpha', 'Beta', 'Gamma')
outputFile.write("Aligned (P-dP)-P0:\n") 
outputFile.write("%5s %6s %6s %8s %5s %5s %5s\n"%( 'plain', 'X(cm)', 'Y(cm)', 'Z(cm)', 'Alpha', 'Beta', 'Gamma' ))
for p in ['mR', 'mF', 'pF', 'pR']:
    print  "          %5s %6.3f %6.3f %8.3f %5.1f %5.1f %5.1f  "%( p, X[p]-X00[p], Y[p]-Y00[p], Z[p]-Z00[p], A[p]-A00[p], B[p]-B00[p], G[p]-G00[p] )
    outputFile.write("%5s %6.3f %6.3f %8.3f %5.1f %5.1f %5.1f\n"%( p, X[p]-X00[p], Y[p]-Y00[p], Z[p]-Z00[p], A[p]-A00[p], B[p]-B00[p], G[p]-G00[p] ))

outputFile.write("\n")

### Print out info for macro 
Axis={  'X':X,   'Y':Y,   'Z':Z,   'Alpha':A,   'Beta':B,   'Gamma':G   }
Axis0={ 'X':X00, 'Y':Y00, 'Z':Z00, 'Alpha':A00, 'Beta':B00, 'Gamma':G00 }
print  ">> [INFO] For producing db file macro (angle unit is mrad)" 
print  "          %6s  %7s, %7s, %7s, %7s, %7s, %7s, %7s, %7s  "%( 'Axis', 'mR', 'mR', 'mF', 'mF', 'pF', 'pF', 'pR', 'pR')
outputFile.write("For producing db file macro:\n") 
outputFile.write("%6s  %7s, %7s, %7s, %7s, %7s, %7s, %7s, %7s\n"%( 'Axis', 'mR', 'mR', 'mF', 'mF', 'pF', 'pF', 'pR', 'pR'))
for a in [ 'X', 'Y', 'Z', 'Alpha', 'Beta', 'Gamma' ]:
    print  "          %6s  %7.4f, %7.4f, %7.4f, %7.4f, %7.4f, %7.4f, %7.4f, %7.4f  "%( a, Axis[a]['mR']-Axis0[a]['mR'], Axis[a]['mR']-Axis0[a]['mR'], Axis[a]['mF']-Axis0[a]['mF'], Axis[a]['mF']-Axis0[a]['mF'], Axis[a]['pF']-Axis0[a]['pF'], Axis[a]['pF']-Axis0[a]['pF'], Axis[a]['pR']-Axis0[a]['pR'], Axis[a]['pR']-Axis0[a]['pR'] )
    outputFile.write("%6s  %7.4f, %7.4f, %7.4f, %7.4f, %7.4f, %7.4f, %7.4f, %7.4f\n"%( a, Axis[a]['mR']-Axis0[a]['mR'], Axis[a]['mR']-Axis0[a]['mR'], Axis[a]['mF']-Axis0[a]['mF'], Axis[a]['mF']-Axis0[a]['mF'], Axis[a]['pF']-Axis0[a]['pF'], Axis[a]['pF']-Axis0[a]['pF'], Axis[a]['pR']-Axis0[a]['pR'], Axis[a]['pR']-Axis0[a]['pR'] ))

outputFile.close()

