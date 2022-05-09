SDIR =~/repos/kissat/build
SOLVE = $(SDIR)/kissat
CDIR =~/repos/drat-trim
CHECK = $(CDIR)/drat-trim

clean:
	rm *~

.SUFFIXES: .cnf .drat .lrat

.cnf.drat:
	-$(SOLVE) --no-binary $< $@

.drat.lrat:
	$(CHECK) $*.cnf $< -L $@
