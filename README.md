# unsat-tutorial
Exploring the ability of Boolean satisfiability solvers to produce a proofs of unsatisfiability

## Files:

  - tools:
    A set of useful tools for supporting SAT generation and analyzing results
    These include versions of the following programs:

    - `kissat`
      Code for SAT solver kissat.
      Extracted from https://github.com/arminbiere/kissat.
      Licensing terms shown in tools/kissat/LICENSE.

    - `drat-trim`
      Code for the proof checkers drat-trim and lrat-check.  Extracted from https://github.com/marijnheule/drat-trim.
      Licensing terms shown in drat-trim/LICENSE.

    - `tbuddy`
      Code for the TBUDDY trusted BDD package and the TBSAT solver
      Extracted from https://github.com/rebryant/tbuddy-artifact

  - `instructor`
    Master copies of the lecture and lab material

  - `student`
    Material for students to use.  Populated from instructor directory

## Make options

 - Compiling the tools and setting up student directory structure:

    `make install`

 - Reverting back to original files

    `make clean`


## Lab materials
  - [Obtaining Lectures and Labs](instructor/writeups/downloading.pdf)