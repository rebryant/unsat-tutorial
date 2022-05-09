RDIR = ../..
SDIR = $(RDIR)/kissat/build
SOLVE = $(SDIR)/kissat
CDIR = $(RDIR)/drat-trim
DRAT = $(CDIR)/drat-trim
LRAT = $(CDIR)/lrat-check

clean:
	rm -f *~
	rm -f *.*drat *.*lrat *data 

.SUFFIXES: .cnf .gen-drat .gen-lrat .unsat-check-data .sat-gen-data

.cnf.gen-drat:
	-$(SOLVE) --unsat --no-binary $< $@ | tee $*.unsat-gen-data

.cnf.sat-gen-data:
	-$(SOLVE) $< | tee $@

.gen-drat.unsat-check-data:
	$(DRAT) $*.cnf $< | tee $@

.gen-drat.gen-lrat:
	$(DRAT) $*.cnf $< -L $@ | tee $*.gen-lrat-data
