GDIR = ../tools
GEN = $(GDIR)/ddgen.py
SCALE=25pt

tex: cdcl.tex cdcl-flip.tex

cdcl.tex: cdcl.dd
	$(GEN) -i cdcl.dd -o cdcl.tex -u $(SCALE) -S c

cdcl-flip.tex: cdcl-flip.dd
	$(GEN) -i cdcl-flip.dd -o cdcl-flip.tex -u $(SCALE) -S c

cdcl-flip-doc.pdf: cdcl-flip.dd
	$(GEN) -w -i cdcl-flip.dd -o cdcl-flip-doc.tex -u $(SCALE) -S c
	pdflatex cdcl-flip-doc.tex

clean:
	rm -f *.tex *.pdf *~ *.log *.aux

