UNIT=40pt
SCALE=0.65
SSCALE=0.90
PDIR = ../tools
GEN=$(PDIR)/ddgen.py

all: bdd-build-tree-path.tex bdd-build-tree.tex bdd-build-r4.tex  bdd-build-r3.tex  bdd-build-r2.tex bdd-build-canonical.tex \
	ordering-good.tex ordering-bad.tex bdd-eg.tex \
	apply-argA-include.pdf apply-argB-include.pdf apply-recurse-include.pdf apply-result-include.pdf \
	apply-recurse-step1-include.pdf apply-recurse-step2-include.pdf apply-recurse-step3-include.pdf \
	apply-recurse-step4-include.pdf apply-recurse-step5-include.pdf \
	eg-integer-10-include.pdf 


.SUFFIXES: .dd .idd .sdd .wdd .tex .pdf

.dd.tex:
	$(GEN) -i $< -o $@ -u $(UNIT) -s $(SCALE) -S c

.idd.tex:
	$(GEN) -i $< -o $@ -u $(UNIT) -s $(SCALE) -w -S c

.sdd.tex:
	$(GEN) -i $< -o $@ -u $(UNIT) -s $(SSCALE) -S c

.wdd.tex:
	$(GEN) -i $< -o $@ -u $(UNIT) -w -s $(SSCALE) -S c

.tex.pdf:
	pdflatex $< 

bdd-build-tree-proto.wdd: bdd-build-tree.sdd
	cp bdd-build-tree.sdd bdd-build-tree-proto.wdd

bdd-build-r4-proto.wdd: bdd-build-r4.sdd
	cp bdd-build-r4.sdd bdd-build-r4-proto.wdd

bdd-build-r3-proto.wdd: bdd-build-r3.sdd
	cp bdd-build-r3.sdd bdd-build-r3-proto.wdd

bdd-build-r2-proto.wdd: bdd-build-r2.sdd
	cp bdd-build-r2.sdd bdd-build-r2-proto.wdd

ordering-good-proto.idd: ordering-good.dd
	cp ordering-good.dd ordering-good-proto.idd

ordering-bad-proto.idd: ordering-bad.dd
	cp ordering-bad.dd ordering-bad-proto.idd

apply-argA-include.wdd: apply-argA.sdd
	cp apply-argA.sdd apply-argA-include.wdd

apply-argB-include.wdd: apply-argB.sdd
	cp apply-argB.sdd apply-argB-include.wdd

apply-result-include.wdd: apply-result.sdd
	cp apply-result.sdd apply-result-include.wdd

apply-recurse-include.wdd: apply-recurse.sdd
	cp apply-recurse.sdd apply-recurse-include.wdd

apply-recurse-step1-include.wdd: apply-recurse-step1.sdd
	cp apply-recurse-step1.sdd apply-recurse-step1-include.wdd

apply-recurse-step2-include.wdd: apply-recurse-step2.sdd
	cp apply-recurse-step2.sdd apply-recurse-step2-include.wdd

apply-recurse-step3-include.wdd: apply-recurse-step3.sdd
	cp apply-recurse-step3.sdd apply-recurse-step3-include.wdd

apply-recurse-step4-include.wdd: apply-recurse-step4.sdd
	cp apply-recurse-step4.sdd apply-recurse-step4-include.wdd

apply-recurse-step5-include.wdd: apply-recurse-step5.sdd
	cp apply-recurse-step5.sdd apply-recurse-step5-include.wdd

eg-integer-10-include.idd: eg-integer-10.dd
	cp eg-integer-10.dd eg-integer-10-include.idd


clean:
	rm -f *~ *.aux *.log *.tex *.wdd *.idd *.pdf



