RDIR=../../..

KDIR=$(RDIR)/kissat/build
KISSAT=$(KDIR)/kissat

DDIR=$(RDIR)/drat-trim
DRAT=$(DDIR)/drat-trim
LRAT=$(DDIR)/lrat-check

clean:
	rm -f *~ *.gen-drat *.gen-lrat *.*-check

.SUFFIXES: .cnf .manual-drat .manual-lrat .gen-drat .manual-drat-check .manual-lrat-check .gen-drat-check

.cnf.gen-drat:
	-$(KISSAT) --no-binary --plain $< $@


.gen-drat.gen-drat-check:
	$(DRAT) $*.cnf $< | tee $@

.manual-drat.manual-drat-check:
	$(DRAT) $*.cnf $< | tee $@


.manual-lrat.manual-lrat-check:
	$(LRAT) $*.cnf $< | tee $@
