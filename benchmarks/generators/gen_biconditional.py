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
import  getopt
import writer
import cnf_utilities

# Generate CNF file for biconditional cycle
def usage(name):
    print("Usage: %s [-h] [-v] [-s] -r ROOT -n N" % name) 
    print("  -h       Print this message")
    print("  -v       Run in verbose mode")
    print("  -s       Generate satisfiable instance")
    print("  -r ROOT  Specify root name for files.  Will generate ROOT.cnf")
    print("  -n N     Specify number of variables in cycle")

verbose = False
varCount = 8
satisfiable = False

def generate(froot):
    cwriter = writer.LazyCnfWriter(froot, verbose)
    if verbose:
        ctype = "satisfiable" if satisfiable else "unsatisfiable"
        cwriter.doComment("Encoding of %s biconditional cycle for %d variables" % (ctype, varCount))
    vars = cwriter.newVariables(varCount)
    cnf_utilities.biconditionalChain(cwriter, vars, verbose)
    lastLit = vars[-1] if satisfiable else -vars[-1]
    wrap = [vars[0], lastLit]
    cnf_utilities.biconditionalChain(cwriter, wrap, verbose)
    cwriter.finish()

def run(name, args):
    global verbose, varCount, satisfiable
    froot = None
    varCount = None
    optlist, args = getopt.getopt(args, "hvsr:n:")
    for (opt, val) in optlist:
        if opt == '-h':
            usage(name)
            return
        elif opt == '-v':
            verbose = True
        elif opt == '-s':
            satisfiable = True
        elif opt == '-r':
            froot = val
        elif opt == '-n':
            varCount = int(val)
    if varCount is None:
        print("Must have value for n")
        usage(name)
        return
    if froot is None:
        print("Must have root name")
        usage(name)
        return
    generate(froot)

if __name__ == "__main__":
    run(sys.argv[0], sys.argv[1:])

        
