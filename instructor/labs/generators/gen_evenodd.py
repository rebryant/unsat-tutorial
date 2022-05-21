#!/usr/bin/python3

#####################################################################################
# Copyright (c) 2021 Marijn Heule, Randal E. Bryant, Carnegie Mellon University
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
# associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
# OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
########################################################################################

import sys
import getopt
import random
import writer
import cnf_utilities


# Generate CNF file for two parity encodings, one even and the other odd
def usage(name):
    print("Usage: %s [-h] [-v] -n N -r ROOT -E MODE -O MODE [-s SEED]" % name) 
    print("  -h       Print this message")
    print("  -v       Run in verbose mode")
    print("  -n N     Specify number of variables in constraints")
    print("  -r ROOT  Specify root name for files.  Will generate ROOT.cnf")
    print("  -E MODE  Specify mode for even parity constraint (l=left linear, r=right linear, t=tree, R=random)")
    print("  -O MODE  Specify mode for odd parity constraint (l=left linear, r=right linear, t=tree, R=random)")
    print("  -s SEED  Set random seed")


class ParityMode:
    mcount = 4
    left, right, tree, random = range(mcount)
    tokens = ['l', 'r', 't', 'R']
    names = ["left", "right", "tree", "Random"]

    def parse(self, token):
        for m in range(self.mcount):
            if token == self.tokens[m]:
                return m
        return None

    def name(self, mode):
        if mode < 0 or mode >= self.mcount:
            return None
        return self.names[mode]

verbose = False

# When generating random instances, use different seeds for the even and odd trees
# When -s argument given, these seeds generated from the provided starting seed
seeds = [1, 2]

def reseed(seed):
    global evenSeed, oddSeed
    random.seed(seed)
    modulus = (1<<31)-1  # Prime
    seeds[0] = random.randrange(1,modulus)
    seeds[1] = random.randrange(1,modulus)

def makeLits(vars, mode, seed):
    lits = (list(vars))
    if mode == ParityMode.right:
        lits.reverse()
    if mode == ParityMode.random:
        random.seed(seed)
        random.shuffle(lits)
    return lits

def encode(writer, vars, mode, phase):
    seed = seeds[phase]
    lits = makeLits(vars, mode, seed)
    if mode == ParityMode.tree:
        cnf_utilities.parityTree(writer, lits, phase, verbose=verbose)
    else:
        cnf_utilities.parityChain(writer, lits, phase, verbose=verbose)

#     print("Usage: %s [-h] [-v] -r ROOT -E MODE -O MODE -n N [-s SEED]" % name) 

def run(name, args):
    global verbose
    froot = None
    emode = None
    omode = None
    n = None
    pm = ParityMode()

    optlist, args = getopt.getopt(args, "hvn:r:E:O:s:")
    for (opt, val) in optlist:
        if opt == '-h':
            usage(name)
            return
        elif opt == '-v':
            verbose = True
        elif opt == '-n':
            n = int(val)
        elif opt == '-r':
            froot = val
        elif opt == '-E':
            emode = pm.parse(val)
            if emode is None:
                print("Unknown even mode '%s'" % val)
                return
        elif opt == '-O':
            omode = pm.parse(val)
            if omode is None:
                print("Unknown odd mode '%s'" % val)
                return
        elif opt == '-s':
            reseed(int(val))

    if n is None:
        print("Must have value for n")
        usage(name)
        return
    if froot is None:
        print("Must have root name")
        usage(name)
        return
    if emode is None or omode is None:
        print("Must have both even and odd modes")
        return
    cwriter = writer.LazyCnfWriter(froot, verbose)
    if verbose:
        ename = pm.name(emode)
        if emode == pm.random:
            ename += " (seed=%d)" % seeds[0]
        oname = pm.name(omode)
        if omode == pm.random:
            oname += " (seed=%d)" % seeds[1]
        cwriter.doComment("Encoding of even and odd parity constraints over %d variables" % (n))
        cwriter.doComment("Even mode = %s, Odd mode = %s" % (ename, oname))
    vars = cwriter.newVariables(n)
    encode(cwriter, vars, emode, 0)
    encode(cwriter, vars, omode, 1)
    cwriter.finish()

if __name__ == "__main__":
    run(sys.argv[0], sys.argv[1:])

        
