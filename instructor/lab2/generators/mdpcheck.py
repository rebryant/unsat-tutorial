#!/usr/bin/python3

# Check solution for Crawford's Minimal Disagreement Parity benchmark
# Input files: Annotated CNF file + solution file (can take from stdin)

# CNF file should contain lines of form:
# c Parity sampling problem.  n=8, m=16, k=4, t=4, seed=1
# c Target Solution: 0 0 1 0 1 1 1 1
# c Solution variables: 1 2 3 4 5 6 7 8
# c Corrupted samples: 1 1 0 0 0 0 1 1 0 0 0 0 0 0 0 0
# c Corruption variables: 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
# c Sample #01: 1 1 0 1 1 0 0 1 | 1 (Corrupted)
# c Sample #02: 0 0 0 0 1 0 1 0 | 1 (Corrupted)
# c Sample #03: 0 1 1 0 1 0 0 1 | 1
# c Sample #04: 1 0 1 0 0 1 0 1 | 1
# c Sample #05: 1 0 1 1 1 1 0 1 | 0
# c Sample #06: 0 1 1 0 1 1 0 1 | 0
# c Sample #07: 0 0 1 1 1 0 1 0 | 0 (Corrupted)
# c Sample #08: 1 1 0 0 0 0 0 0 | 1 (Corrupted)
# c Sample #09: 1 1 1 1 1 0 1 0 | 1
# c Sample #10: 0 1 0 1 1 0 1 1 | 1
# c Sample #11: 1 1 1 0 1 1 0 0 | 1
# c Sample #12: 0 0 0 1 0 0 0 0 | 0
# c Sample #13: 1 0 1 0 1 0 0 1 | 1
# c Sample #14: 1 0 0 0 1 0 1 1 | 1
# c Sample #15: 1 1 1 1 0 0 1 1 | 1
# c Sample #16: 1 1 0 1 0 1 0 1 | 0

# Solution file should contain one or more solutions of form
# v -1 2 3 -4 -5 6 7 -8 -9 -10 11 12 -13 14 -15 -16 -17 18 -19 -20 -21 -22 -23 -24 
# v -25 26 -27 28 -29 30 31 -32 33 34 -35 -36 -37 -38 -39 -40 -41 -42 -43 -44 -45 
# v 46 -47 48 49 -50 -51 -52 -53 -54 -55 -56 -57 -58 -59 60 61 -62 -63 -64 -65 
# v -66 -67 -68 -69 -70 -71 72 73 74 -75 -76 -77 -78 -79 -80 -81 -82 -83 84 85 86 
# v 87 88 89 -90 -91 -92 -93 -94 -95 96 97 98 99 100 101 102 103 104 105 106 107 0

import sys
import re

# Problem parameters
numVariables = 0
numSamples = 0
numTolerated = 0

variableIds = []
samples = []
phases = []

def trim(s):
    while len(s) > 0 and s[-1] == '\n':
        s = s[:-1]
    return s

# Look for field of form P=NNN, where P is specified character
def efind(pchar, line):
    p = pchar + "=([\d]+)"
    m = re.search(p, line)
    if m is None:
        return None
    try:
        val = int(m.group(1))
    except:
        return None
    return val

# c Sample #01: 1 1 0 1 1 0 0 1 | 1 
def getSample(line):
    sample = []
    phase = None
    started = False
    ended = False
    for field in line.split():
        if not started and field[-1] == ':':
            started = True
        elif started and  field[0] == '|':
            ended = True
        elif started or ended:
            try:
                val = int(field)
                if val < 0 or val > 1:
                    return None
                if ended:
                    phase = val
                    return (sample, phase)
                else:
                    sample.append(val)
            except:
                return None
    return None

# c Solution variables: 1 2 3 4 5 6 7 8
def getVariables(line):
    vlist = []
    started = False
    for field in line.split():
        if not started and field[-1] == ":":
            started = True
        elif started:
            try:
                val = int(field)
                vlist.append(val)
            except:
                return None
    return vlist
            


def getProblem(infile):
    global numVariables, numSamples, numTolerated
    global variableIds, samples, phases
    samples = []
    phase = []
    for line in infile:
        if line[0] != 'c':
            continue
        if "Parity sampling problem" in line:
            numVariables = efind('n', line)
            numSamples = efind('m', line)
            numTolerated = efind('t', line)
            if numVariables is None:
                print("ERROR: Couldn't find number of variables in line '%s'" % trim(line))
                return False
            if numSamples is None:
                print("ERROR: Couldn't find number of samples in line '%s'" % trim(line))
                return False
            if numTolerated is None:
                print("ERROR: Couldn't find number of allowed mismatches in line '%s'" % trim(line))
                return False
        elif "Sample #" in line:
            if numVariables == 0:
                print("ERROR.  Found sample in line '%s', but don't yet know number of variables" % trim(line))
                return False
            sample, phase = getSample(line)
            if sample is None or len(sample) != numVariables or phase is None:
                print("ERROR.  Invalid sample in line '%s'" % trim(line))
                return False
            samples.append(sample)
            phases.append(phase)
        elif "Solution variables" in line:
            if numVariables == 0:
                print("ERROR.  Found variable list in line '%s', but don't yet know number of variables" % trim(line))
                return False
            vlist = getVariables(line)
            if vlist is None or len(vlist) != numVariables:
                print("ERROR.  Couldn't extract list of solution variables from line '%s'" % trim(line))
                return False
            variableIds = vlist
        else:
            pass
    if len(samples) != numSamples:
        print("ERROR.  Got %d samples.  Expected %d" % (len(samples), numSamples))
        return False
    return True
        
def getSolutions(infile):
    allAssignments = []
    sofar = []
    for line in infile:
        terminated = False
        if line[0] != 'v':
            continue
        for field in line[1:].split():
            try:
                val = int(field)
            except:
                print("ERROR.  Invalid field '%s' found in solution line '%s'" % (field, trim(line)))
                return None
            if val == 0:
                terminated = True
                break
            else:
                sofar.append(val)
        if terminated:
            allAssignments.append(sofar)
            sofar = []
    # Now reduce these to the actual assignments to the solution variables
    allSolutions = []
    for alist in allAssignments:
        bits = []
        for v in variableIds:
            if v < 1 or v > len(alist):
                print("ERROR: Out of range input variable index %d.  Have %d total variables" % (v, len(alist)))
                return None
            b = None
            if v in alist:
                b = 1
            elif -v in alist:
                b = 0
            if b is None:
                print("ERROR: Couldn't find assignment to input variable %d in assignment list %s" % (v, str(alist)))
                return None
            bits.append(b)
        allSolutions.append(bits)
    return allSolutions
                
def matchCount(bits):
    count = 0
    for sample,phase in zip(samples, phases):
        p = 0
        for b, s in zip(bits, sample):
            if s:
                p = p ^ b
        match = p == phase
        if match:
            count += 1
    return count
        
def check(pfile, sfile):
    failCount = 0
    if not getProblem(pfile):
        return False
    allSolutions = getSolutions(sfile)
    if allSolutions is None:
        return False
    scount = len(allSolutions)
    for i in range(scount):
        bits = allSolutions[i]
        m = matchCount(bits)
        if numSamples - m <= numTolerated:
            print("Solution #%d.  %d mismatches.  Passed" % (i+1, numSamples - m))
        else:
            print("Solution #%d.  %d mismatches.  Failed" % (i+1, numSamples - m))
            failCount += 1
    if scount == 0:
        print("NO SOLUTION FOUND")
    elif failCount == 0:
        if scount == 1:
            print("SOLUTION VERIFIED")
        else:
            print("ALL %d SOLUTIONS VERIFIED" % scount)
    else:
        if scount == 1:
            print("SOLUTION FAILED")
            return False
        else:
            print("%d/%d SOLUTIONS FAILED" % (failCount, scount))
    return failCount > 0
    
def run(name, args):
    if len(args) == 0 or len(args) == 1 and args[0] == '-h':
        print("Usage %s CNFFILE [SOLUTIONFILE]" % name)
        return
    try:
        pfile = open(args[0], 'r')
    except:
        print("Couldn't open file '%s'" % args[0])
        return
    sfile = sys.stdin
    if len(args) > 1:
        try:
            sfile = open(args[1], 'r')
        except:
            print("Couldn't open file '%s'" % args[1])
            return
    if check(pfile, sfile):
        sys.exit(0)
    else:
        sys.exit(1)

if __name__ == "__main__":
    run(sys.argv[0], sys.argv[1:])
    
    
    

