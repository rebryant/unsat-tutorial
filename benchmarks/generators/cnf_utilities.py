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

import writer

# Utilities for encoding problems into CNF

# Each of the following will add clauses to a CNF file (using the
# writer.LazyCnfWriter class) for a specified type of constraint
# If the verbose flag is set, then comments will be also added to the CNF file
# Argument lits is a list of literals (each given as a signed integer)

def atLeastOne(writer, lits, verbose = False):
    if verbose:
        writer.doComment("At-least-one constraint")
    writer.doClause(lits)

def atMostOneDirect(writer, lits, verbose = False):
    if verbose:
        slist = [str(lit) for lit in lits]
        writer.doComment("Direct encoding of at-most-one constraint for literals [%s]" % (", ".join(slist)))
    for i in range(len(lits)):
        for j in range(i+1, len(lits)):
            writer.doClause([-lits[i], -lits[j]])
                       
def exactlyOneDirect(writer, lits, verbose = False):    
    if verbose:
        slist = [str(lit) for lit in lits]
        writer.doComment("Direct encoding of exactly-one constraint for literals [%s]" % (", ".join(slist)))
    atLeastOne(writer, lits, False)
    atMostOne(writer, lits, False)


def atMostOneSinz(writer, lits, verbose = False):
    if verbose:
        slist = [str(lit) for lit in lits]
        writer.doComment("Sinz encoding of at-most-one constraint for literals [%s]" % (", ".join(slist)))
    if len(lits) <= 3:
        atMostOneDirect(writer, lits, False)
    else:
        sv = writer.newVariable()
        atMostOneDirect(writer, [lits[0], lits[1], -sv], False)
        atMostOneSinz(writer, [sv] + lits[2:], False)

# Determine parity of binary representation of integer w
def bitParity(w):
    value = 0
    while w != 0:
        value ^=  w & 0x1
        w = w >> 1
    return value


def parityDirect(writer, lits, phase, verbose = False):
    if verbose:
        slist = [str(lit) for lit in lits]
        ptype = "odd" if phase == 1 else "even"
        writer.doComment("Direct encoding of %s parity for literals [%s]" % (ptype, ", ".join(slist)))
    n = len(lits)
    for i in range(1 << n):
        # Count number of 0's in i
        zparity = bitParity(~i & ((1 << n) - 1))
        if zparity == phase:
            continue
        nlits = []
        for j in range(n):
            weight = 1 if ((i>>j)&0x1) == 1 else -1
            nlits.append(lits[j] * weight)
        writer.doClause(lits)
    
def parityTree(writer, lits, phase, group = 3, verbose = False):
    if verbose:
        slist = [str(lit) for lit in lits]
        ptype = "odd" if phase == 1 else "even"
        writer.doComment("Tree encoding (group size = %d) of %s parity for literals [%s]" % (group, ptype, ", ".join(slist)))
    if len(lits) <= group:
        writer.parityDirect(writer, lits, phase, group = group, verbose = False)
    else:
        xv = writer.newVariable()
        parityDirect(writer, lits[0:group-1] + [xv], phase=0, group = group, verbose = False)
        parityDirect(writer, lits[group-1:]  + [xv], phase=phase, group = group, verbose = False)

def parityChain(writer, lits, phase, group = 3, verbose = False):
    if verbose:
        slist = [str(lit) for lit in lits]
        ptype = "odd" if phase == 1 else "even"
        writer.doComment("Chain encoding (group size = %d) of %s parity for literals [%s]" % (group, ptype, ", ".join(slist)))
    if len(lits) <= group:
        writer.parityDirect(writer, lits, phase, group = group, verbose = False)
    else:
        xv = writer.newVariable()
        parityDirect(writer, lits[0:group-1] + [xv], phase=0, group = group, verbose = False)
        parityDirect(writer, [xv] + lits[group-1:], phase=phase, group = group, verbose = False)        

def biconditionalChain(writer, lits, verbose = False):
    if verbose:
        slist = [str(lit) for lit in lits]
        writer.doComment("Create chain of biconditionals for literals [%s]" % (", ".join(slist)))
        for i in range(len(lits)-1):
            writer.doClause([lits[i], -lits[i+1]])
            writer.doClause([-lits[i], lits[i+1]])
                         
