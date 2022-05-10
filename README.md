# unsat-tutorial
Exploring the ability of Boolean satisfiability solvers to produce a proofs of unsatisfiability

Files:

  tools:
    A set of useful tools for supporting SAT generation and analyzing results

  kissat:
    Code for SAT solver kissat.
    Extracted from https://github.com/arminbiere/kissat
    Licensing terms shown in kissat/LICENSE

  drat-trim:
    Code for the proof checkers drat-trim and lrat-check.  Extracted from https://github.com/marijnheule/drat-trim
    Licensing terms shown in drat-trim/LICENSE

  generators:
    Code for generating CNF encodings of SAT problems

Compiling the tools:

Kissat should be compiled using its generated makefile:
  cd kissat
  ./configure
  make

The executable code will be written to the file kissat/build/kissat

The others can be compiled using 'make install'


Other Makefile options:

  clean:
    Clean generated stuff from directories
    