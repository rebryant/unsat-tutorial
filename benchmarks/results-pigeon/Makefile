INTERP=python3
VERB = -v
RDIR = ../..
GDIR = $(RDIR)/benchmarks/generators
GENERATE = $(GDIR)/gen_pigeon.py
SDIR = $(RDIR)/kissat/build
SOLVE = $(SDIR)/kissat
CDIR = $(RDIR)/drat-trim
DRAT = $(CDIR)/drat-trim
LRAT = $(CDIR)/lrat-check
TDIR = $(RDIR)/tools
GRAB = $(TDIR)/grab_data.py
N=8

pdurun: pigeon-direct-unsat-$(N).cnf pigeon-direct-unsat-$(N).drat pigeon-direct-unsat-$(N).unsat-check-data
pdsrun: pigeon-direct-sat-$(N).cnf pigeon-direct-sat-$(N).sat-gen-data
psurun: pigeon-sinz-unsat-$(N).cnf pigeon-sinz-unsat-$(N).drat pigeon-sinz-unsat-$(N).unsat-check-data
pssrun: pigeon-sinz-sat-$(N).cnf pigeon-sinz-sat-$(N).sat-gen-data

pigeon-direct-sat-$(N).cnf:
	$(INTERP) $(GENERATE) $(VERB) -n $(N) -p $(N) -r pigeon-direct-sat-$(N)

pigeon-direct-unsat-$(N).cnf:
	$(INTERP) $(GENERATE) $(VERB) -n $(N) -r pigeon-direct-unsat-$(N)

pigeon-sinz-sat-$(N).cnf:
	$(INTERP) $(GENERATE) $(VERB) -S -n $(N) -p $(N) -r pigeon-sinz-sat-$(N)

pigeon-sinz-unsat-$(N).cnf:
	$(INTERP) $(GENERATE) $(VERB) -S -n $(N) -r pigeon-sinz-unsat-$(N)


.SUFFIXES: .cnf .drat .lrat .unsat-check-data .sat-gen-data

.cnf.drat:
	-$(SOLVE) --unsat --no-binary $< $@ | tee $*.unsat-gen-data

.cnf.sat-gen-data:
	-$(SOLVE) $< | tee $@

.drat.unsat-check-data:
	$(DRAT) $*.cnf $< | tee $@

.drat.lrat:
	$(DRAT) $*.cnf $< -L $@ | tee $*.lrat-data

data:
	# Direct/SAT
	$(INTERP) $(GRAB) 2 "p cnf" pigeon-direct-sat-*.cnf > pigeon-direct-sat-input-clauses.csv
	$(INTERP) $(GRAB) -1 'process-time' pigeon-direct-sat-*.sat-gen-data > pigeon-direct-sat-solver-seconds.csv
	# Direct/UNSAT
	$(INTERP) $(GRAB) 2 "p cnf" pigeon-direct-unsat-*.cnf > pigeon-direct-unsat-input-clauses.csv
	$(INTERP) $(GRAB) -1 'process-time' pigeon-direct-unsat-*.unsat-gen-data > pigeon-direct-unsat-solver-seconds.csv
	$(INTERP) $(GRAB)  'proof_added' pigeon-direct-unsat-*.unsat-gen-data > pigeon-direct-proof-clauses.csv
	$(INTERP) $(GRAB)  'verification time' pigeon-direct-unsat-*.unsat-check-data > pigeon-direct-check-seconds.csv
	# Sinz/SAT
	$(INTERP) $(GRAB) 2 "p cnf" pigeon-sinz-sat-*.cnf > pigeon-sinz-sat-input-clauses.csv
	$(INTERP) $(GRAB) -1 'process-time' pigeon-sinz-sat-*.sat-gen-data > pigeon-sinz-sat-solver-seconds.csv
	# Sinz/UNSAT
	$(INTERP) $(GRAB) 2 "p cnf" pigeon-sinz-unsat-*.cnf > pigeon-sinz-unsat-input-clauses.csv
	$(INTERP) $(GRAB) -1 'process-time' pigeon-sinz-unsat-*.unsat-gen-data > pigeon-sinz-unsat-solver-seconds.csv
	$(INTERP) $(GRAB)  'proof_added' pigeon-sinz-unsat-*.unsat-gen-data > pigeon-sinz-proof-clauses.csv
	$(INTERP) $(GRAB)  'verification time' pigeon-sinz-unsat-*.unsat-check-data > pigeon-sinz-check-seconds.csv

clean:
	rm -f *~ *.cnf *data *drat *lrat

