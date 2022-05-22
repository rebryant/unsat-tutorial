#!/usr/bin/python3

# Generate CNF encoding of Crawford's Minimal Disagreement Parity benchmark
# http://www.cs.cornell.edu/selman/docs/crawford-parity.pdf
# Use different encoding than presented in paper:
# - Incorporate constants directly into encoding
# - Use unary counter for at-most-k constraint

import sys
import getopt

import writer


def usage(name):
    print("Usage: %s [-h] [-v] [-x] [-f] [-a] [-O] [-p] [-r ROOT] -n N [-m M] [-d D] [-k K] [-y CPROB] [-t T] [-s SEED] [-X XARGS]" % name)
    print("  -h       Print this message")
    print("  -v       Put comments in file")
    print("  -x       Exclude expected solution (should make formula unsatisfiable")
    print("  -f       Use fixed solution and corruption bits")
    print("  -r ROOT  Set root of output file.  Will generate ROOT.cnf and possible ROOT.order")
    print("  -a       Anonymize.  Don't include solution information in file")
    print("  -O       Optimize.  Use Plaisted-Greenbaum encoding")
    print("  -p       Generate permutation file for BDD variable ordering")
    print("  -n N     Number of variables")
    print("  -m M     Number of samples (default = 2*N) to generate")
    print("  -d D     Select D final samples for problem (default = M)")
    print("  -y CPROP Probability of corrupting sample")
    print("  -k K     Number of corrupted samples (default = M/8 if -y not given)")
    print("  -t T     Number of mismatches tolerated (default = K).  When T <= 0 tolerate K+T mismatches")
    print("  -s SEED  Set random seed")
    print("  -X XARGS  Set maximum Xor size (must be >= 3)")


cwriter = None

#### Encoding options #######################
# Optimize: Use Plaisted-Greenbaum encoding.
# Don't optimize: Use Tseitin encoding
optimize = False
# Maximum xor length.  Must be >= 3
# Insert encoding variables to split longer ones
maxArgs = 4

# Parameters
verbose = False
seed = 123456
numVariables = None
numOriginalSamples = None
numSamples = None
numDownSamples = None
numCorrupt = None
numTolerated = None


# Bit vectors
solutionBits = []
corruptionBits = []

# numSamples random bit sequences, each of length numVariables
sampleBitSet = []
# The computed parity for each sample
parityBits = []
# The target phase for each sample
phaseBits = []

# Problem variables
solutionVariables = []
corruptionVariables = []

# Added variables
# Intermediate variables in Xors
xorVariables = []
# Intermediate variables in At-Most-K.  Set of indexed lists,
# where index specifies max input
amkVariables = {}

# Keep RNG as global
rng = None

# Different phases where RNG gets used
rngPhaseCount = 6
rngPhaseInit, rngPhaseCounting, rngPhaseSelection, rngPhaseBits, rngPhaseSamples, rngPhaseDownSamples = range(rngPhaseCount)


def initRng(seed):
    global rng
    rng = RNG(seed=seed, seedCount=rngPhaseCount)


# The following random number generator isn't especially good, but it
# provides control over the seed, and it can be easily translated into
# other languages.

# CONSTANTS
# These are the parameters used in the C++ minstd_rand random number
# generator It is based on a class of linear congruential generators
# due to D. H. Lehmer The parameters were suggested by Stephen K. Park
# and Keith W. Miller, and Paul K. Stockmeyer in CACM, July 1988.

GROUPSIZE = 2147483647 # 2^31 - 1, a prime number
MVAL = 48271
VVAL = 16807
INITSEED = 418
DEFAULTSEED = 618

class RNG:
    # Maintain set of seeds, allowing them to be used independently
    # Seed 0 used to generate other seeds
    # Other seeds can be used for application
    seedList = [INITSEED]

    def __init__(self, seed = DEFAULTSEED, seedCount = 10):
        self.seedList[0] = seed
        for i in range(seedCount):
            self.seedList.append(self.next(sidx=0))

    def next(self, sidx = 0):
        val = (VVAL + self.seedList[sidx] * MVAL) % GROUPSIZE
        self.seedList[sidx] = val
        return val

    # Return random float, distributed uniformly in interval [0, upperLimit)
    def randFloat(self, upperLimit = 1.0, sidx = 0):
        val = self.next(sidx)
        rval = (float(val)/GROUPSIZE) * upperLimit
        return rval

    # Return random integer, distributed uniformly in interval [lower, upper]
    def randInt(self, lower, upper, sidx = 0):
        rval = self.randFloat(sidx=sidx)
        return lower + int(rval * (upper + 1 - lower))

    # Choose maxSample elements (without replacement) from sequence at random
    # and return as list.
    def sample(self, seq, maxSample, sidx = 0):
        if len(seq) <= maxSample:
            return seq
        result = [0] * maxSample
        swaps = [0] * maxSample
        for i in range(maxSample):
            w = self.randFloat(sidx = sidx)
            idx = i + int(w * float(len(seq)-i))
            swaps[i] = idx
            result[i] = seq[idx]
            seq[idx], seq[i] = seq[i], seq[idx]
        # Use list of swaps to restore input sequence
        for i in range(maxSample-1, -1, -1):
            idx = swaps[i]
            seq[idx], seq[i] = seq[i], seq[idx]
        return result

    def randomBits(self, length, sidx = 0):
        return [self.randInt(0, 1, sidx) for i in range(length)]

def vectorParity(vec):
    parity = 0
    for bit in vec:
        parity ^= bit
    return parity

def vectorString(vec):
    slist = [str(v) for v in vec]
    return " ".join(slist)


def bitParity(w):
    value = 0
    while w != 0:
        value ^=  w & 0x1
        w = w >> 1
    return value

# Add list of clauses for encoding parity
def genParity(vars, phase):
    n = len(vars)
    for i in range(1 << n):
        # Count number of 0's in i
        zcount = bitParity(~i & ((1 << n) - 1))
        if zcount == phase:
            continue
        lits = []
        for j in range(n):
            weight = 1 if ((i>>j)&0x1) == 1 else -1
            lits.append(vars[j] * weight)
        cwriter.doClause(lits)

def genParityChain(vars, phase):
    global xorVariables
    if len(vars) <= maxArgs:
        genParity(vars, phase)
    else:
        var = cwriter.newVariable()
        xorVariables.append(var)
        left = vars[0:maxArgs-1] + [var]
        right = vars[maxArgs-1:] + [var]
        genParity(left, 0)
        genParityChain(right, phase)

# Generate atMost-k ladder and assert that it holds
# When global flag "optimize" set, only enforce one side of
# constraints (Plaisted-Greenbaum)
# When not set, use Tseitin encoding
def enforceAmk(vars, k):
    global amkVariables
    amkVariables = { i+1 : [] for i in range(1,len(vars)) }
    m = len(vars)
    # Assume 0 <= k < m
    # Lits of variables encoding counts.
    # entry (i,j) gives variable for vars[:i] having at most j 1's
    slist = [str(v) for v in vars]
    if verbose:
        cwriter.doComment("Encoding of at-most-%d over variables [%s]" % (k, ", ".join(slist)))
    c = {}
    # Count = 0
    c[(0,0)] = -vars[0]
    cwriter.doComment("Encode at-most-0 conditions")
    for i in range(1, m-k):
        here = cwriter.newVariable()
        amkVariables[i+1].append(here)
        if verbose:
            cwriter.doComment("Variable %d encodes that vars 1..%d have at most %d 1's" % (here, i+1,0))
        c[(i,0)] = here
        local = vars[i]
        prev = c[(i-1,0)]
        cwriter.doClause([-local, -here])
        cwriter.doClause([prev, -here])
        # Don't need constraint:
        if not optimize:
            cwriter.doClause([local, -prev, here])
    for j in range(1, k+1):
        if verbose:
            cwriter.doComment("Encode at-most-%d conditions" % j)
        i = j
        here = cwriter.newVariable()
        amkVariables[i+1].append(here)
        if verbose:
            cwriter.doComment("Variable %d encodes that vars 1..%d have at most %d 1's" % (here,i+1,j))
        c[(i,j)] = here
        local = vars[i]
        prevJm1 = c[(i-1,j-1)]
        cwriter.doClause([-local, prevJm1, -here])
        # Don't need constraint:
        if not optimize:
            cwriter.doClause([local, here])
            cwriter.doClause([-prevJm1, here])
        for i in range(j+1, m+j-k):
            here = cwriter.newVariable()
            amkVariables[i+1].append(here)
            if verbose:
                cwriter.doComment("Variable %d encodes that vars 1..%d have at most %d 1's" % (here,i+1,j))
            c[(i,j)] = here
            local = vars[i]
            prevJ = c[(i-1,j)]
            prevJm1 = c[(i-1,j-1)]
            cwriter.doClause([prevJ, -here])
            cwriter.doClause([-local, prevJm1, -here])
            # Don't need constraint:
            if not optimize:
                cwriter.doClause([-prevJm1, -prevJ, here])
                cwriter.doClause([local, -prevJ, here])
    topVar = c[(m-1,k)]
    if verbose:
        cwriter.doComment("Assert that at-most-%d condition holds" % numTolerated)
    cwriter.doClause([topVar])


def initData(fixed):
    global solutionBits, corruptionBits, sampleBitSet, parityBits, phaseBits
    global numSamples, numOriginalSamples, numCorrupt, numTolerated
    if fixed:
        solutionBits = [i%2 for i in range(numVariables)]
        corruptionBits = [1 if i < numCorrupt else 0 for i in range(numSamples)]
    else:
        solutionBits = rng.randomBits(numVariables, rngPhaseBits)
        corruptionVars = rng.sample(list(range(numSamples)), numCorrupt, rngPhaseSelection)
        corruptionBits = [1 if i in corruptionVars else 0 for i in range(numSamples)]

    sampleBitSet = []
    parityBits = []
    phaseBits = []

    for i in range(numSamples):
        sbits = rng.randomBits(numVariables, rngPhaseSamples)
        sampleBitSet.append(sbits)
        samples = [solve & sample for solve, sample in zip(solutionBits, sbits)]
        parity = vectorParity(samples)
        parityBits.append(parity)
        phase = parity ^ corruptionBits[i]
        phaseBits.append(phase)

    if numDownSamples < numSamples:
        numOriginalSamples = numSamples
        numSamples = numDownSamples
        dlist = rng.sample(list(range(numOriginalSamples)), numSamples, rngPhaseDownSamples)
        dlist.sort()
        corruptionBits = [corruptionBits[i] for i in dlist]
        sampleBitSet = [sampleBitSet[i] for i in dlist]
        parityBits = [parityBits[i] for i in dlist]
        phaseBits = [phaseBits[i] for i in dlist]
        numOriginalCorrupt = numCorrupt
        numCorrupt = sum(corruptionBits)
        numTolerated = max(0,numTolerated - (numOriginalCorrupt - numCorrupt))

def initVariables():
    global solutionVariables, corruptionVariables
    solutionVariables = cwriter.newVariables(numVariables)
    corruptionVariables = cwriter.newVariables(numSamples)

# Print string on output and as comment in file
def docString(s, asComment = True, asOutput = True):
    if asComment:
        cwriter.doComment(s)
    if asOutput:
        print("c " + s)

def document(anonymous, exclude):
    docString("Parity sampling problem.  n=%d, m=%d, k=%d, t=%d, seed=%d" % (numVariables, numSamples, numCorrupt, numTolerated, seed))
    if numOriginalSamples is not None:
        docString("Downsampled from %d samples" % (numOriginalSamples))
    if exclude:
        docString("Target solution blocked.  Formula should be unsatisfiable")
    docString("Target Solution: %s" % vectorString(solutionBits), asComment = not anonymous)
    docString("Solution variables: %s" % vectorString(solutionVariables))
    docString("Corrupted samples: %s" % vectorString(corruptionBits), asComment = not anonymous)
    docString("Corruption variables: %s" % vectorString(corruptionVariables))
    if not anonymous:
        for i in range(numSamples):
            if corruptionBits[i]:
                docString("Sample #%.2d: %s | %d (Corrupted)" % (i+1, vectorString(sampleBitSet[i]), phaseBits[i]))
            else:
                docString("Sample #%.2d: %s | %d" % (i+1, vectorString(sampleBitSet[i]), phaseBits[i]))

def generate(exclude):
    # Generate Parity Computations
    for i in range(numSamples):
        vars = [corruptionVariables[i]] + [solutionVariables[j] for j in range(numVariables) if sampleBitSet[i][j] == 1]
        phase = phaseBits[i]
        cstring = "Y" if corruptionBits[i] == 1 else "N"
        if verbose:
            cwriter.doComment("Encoding of Sample #%.2d.  Vars = %s.  Phase = %d (Corrupted: %s)" % (i+1, vectorString(vars), phase, cstring))
        genParityChain(vars, phase)
    # Generate at-most-k
    enforceAmk(corruptionVariables, numTolerated)
    if exclude:
        # Generate blocking clause
        xclause = [-v if b else v for (v,b) in zip(solutionVariables, solutionBits)]
        cwriter.doComment("Exclude target solution")
        cwriter.doClause(xclause)

# Generate ordering file
def generateOrder(pwriter):
    # Place amk variables, from highest to lowest
    klist = sorted(amkVariables.keys())
    klist.reverse()
    for k in klist:
        pwriter.doOrder(amkVariables[k])
    # Place the corruption variables
    pwriter.doOrder(corruptionVariables)
    # Place the Xor Variables
    pwriter.doOrder(xorVariables)
    # Place the solution variables
    pwriter.doOrder(solutionVariables)
    pwriter.finish()

def run(name, args):
    global verbose, numVariables, numSamples, numDownSamples, numCorrupt, numTolerated, seed
    global optimize, maxArgs
    global cwriter
    fixed = False
    anonymous = False
    numVariables = None
    numSamples = None
    numDownSamples = None
    numCorrupt = None
    numTolerated = None
    probCorrupt = None
    exclude = False
    order = False
    root = None

    optlist, args = getopt.getopt(args, "hvxfaOpr:n:m:d:k:t:s:X:y:")
    for (opt, val) in optlist:
        if opt == '-h':
            usage(name)
            return
        elif opt == '-v':
            verbose = True
        elif opt == '-x':
            exclude = True
        elif opt == '-f':
            fixed = True
        elif opt == '-a':
            anonymous = True
        elif opt == '-O':
            optimize = True
        elif opt == '-p':
            order = True
        elif opt == '-r':
            root = val
        elif opt == '-n':
            numVariables = int(val)
        elif opt == '-m':
            numSamples = int(val)
        elif opt == '-d':
            numDownSamples = int(val)
        elif opt == '-k':
            numCorrupt = int(val)
        elif opt == '-y':
            probCorrupt = float(val)
        elif opt == '-t':
            numTolerated = int(val)
        elif opt == '-s':
            seed = int(val)
        elif opt == '-X':
            maxArgs = int(val)



    if numVariables is None:
        print("Must specify number of variables with -n option")
        return
    if numSamples is None:
        numSamples = 2*numVariables
    if numDownSamples is None:
        numDownSamples = numSamples
    if numDownSamples > numSamples:
        print("Invalid number of downsamples.  Must be <= number of samples")
        return

    initRng(seed)

    if numCorrupt is None:
        if probCorrupt is None:
            # Crawford states that he used k = m / 8 for his benchmarks.
            # This seems to be a good number.  Larger values admit too many solutions
            numCorrupt = numSamples // 8
        else:
            while True:
                numCorrupt = 0
                for i in range(numSamples):
                    val = rng.randFloat(sidx=rngPhaseCounting)
                    if val <= probCorrupt:
                        numCorrupt += 1
                if numCorrupt != 0:
                    break
    if numTolerated is None:
        numTolerated = numCorrupt
    elif numTolerated <= 0:
        numTolerated += numCorrupt

    initData(fixed)        

    if root is None:
        base = "mdp"
        if fixed:
            base += "-fixed"
        if exclude:
            base += "-x"
        dstring = "" if numOriginalSamples is None else "d%d" % numOriginalSamples
        root = "%s-n%d-m%d%s-k%d-t%d-s%d" % (base, numVariables, numSamples, dstring, numCorrupt, numTolerated, seed)


    cwriter = writer.LazyCnfWriter(root, verbose=False)
    initVariables()
    document(anonymous, exclude)
    generate(exclude)
    vcount = cwriter.vcount()
    cwriter.finish()
    if order:
        pwriter = writer.OrderWriter(vcount, root)
        generateOrder(pwriter)


if __name__ == "__main__":
    run(sys.argv[0], sys.argv[1:])

