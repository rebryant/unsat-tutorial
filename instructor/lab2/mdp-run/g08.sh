#!/bin/bash

N=08

# Execution of MDParity benchmark for N=24.  No proof generation attempted
for M in {16,24}
do
    K=$((M/8))
    for S in {101..110}
    do
	make cnf N=$N M=$M K=$K SEED=$S
	make knp N=$N M=$M K=$K SEED=$S
	make gnp N=$N M=$M K=$K SEED=$S
    done
done    
