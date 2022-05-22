UNIT=50pt
SCALE=0.75
PDIR = ../../slides/tools
GEN=$(PDIR)/ddgen.py

all: n21.tex n20.tex n21-22.tex

.SUFFIXES: .dd .tex

.dd.tex:
	$(GEN) -i $< -o $@ -u $(UNIT) -s $(SCALE)


clean:
	rm -f *~ *.tex 



