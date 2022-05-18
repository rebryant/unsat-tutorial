#!/usr/bin/python3

import sys
import getopt
import dd

# Generate LaTeX picture description of decision diagram

def usage(name):
    print("Usage: %s [-h] [-w] [-u UNIT] [-s SCALE] [-S SCHEME] [-i INFILE] [-o OUTFILE]" % name)
    print(" -h         Print this message")
    print(" -w         Wrap to generate standalone document")
    print(" -u UNIT    Declare unit length (e.g., 50pt)")
    print(" -s SCALE   Scale entire picture")
    print(" -S SCHEME  Specify color scheme (options = 'bw' or 'color')")
    print(" -i INFILE  Specify input file name")
    print(" -o OutFILE Specify output file name")

def declare(oname, scheme):
    ofile = sys.stdout
    if oname is not None:
        try:
            ofile = open(oname, 'w')
        except:
            print("Couldn't open output file '%s'" % oname)
            return
    dd.Scheme().declare(ofile, scheme)
    ofile.close()
    
    

def generate(iname, oname, unit, scale, scheme, wrap):
    try:
        ifile = open(iname, 'r')
    except:
        print("Couldn't open input file '%s'" % iname)
        return
    ofile = sys.stdout
    if oname is not None:
        try:
            ofile = open(oname, 'w')
        except:
            print("Couldn't open output file '%s'" % oname)
            return
    g = dd.Graph()
    try:
        g.load(ifile)
    except Exception as ex:
        print("Couldn't load input file: %s" % str(ex))
        return
    ifile.close()
    if wrap:
        ofile.write("\\documentclass[11pt]{standalone}\n")
        ofile.write("\\usepackage{tikz}\n")            
        if scheme == dd.Scheme.color:
            # When including in Beamer slides, want sans serif math
            ofile.write("\\usepackage{sansmathfonts}\n")
        ofile.write("\\begin{document}\n")
    if scale != 1.0:
        ofile.write("\\scalebox{%.2f}{%%\n" % scale)
    g.renderTikz(ofile, unit, scheme)
    if scale != 1.0:
        ofile.write("}\n")
    if wrap:
        ofile.write("\\end{document}\n")
    ofile.close()

def run(name, args):
    iname = None
    oname = None
    unit = "0.1cm"
    wrap = False
    scale = 1.0
    scheme = dd.Scheme.blackWhite
    optlist, args = getopt.getopt(args, "hwpu:s:S:i:o:")
    for (opt, val) in optlist:
        if opt == '-h':
            usage(name)
            return
        elif opt == '-w':
            wrap = True
        elif opt == '-u':
            unit = val
        elif opt ==  '-s':
            scale = float(val)
        elif opt == '-S':
            scheme = dd.Scheme().parse(val)
        elif opt == '-i':
            iname = val
        elif opt == '-o':
            oname = val
        else:
            print("Unknown option '%s'" % opt)
            usage(name)
            return
    if iname is None:
        print("Must have input file name")
        return
    generate(iname, oname, unit, scale, scheme, wrap)


if __name__ == "__main__":
    run(sys.argv[0], sys.argv[1:])
    
        
