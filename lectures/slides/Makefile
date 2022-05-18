DGEN = ./tools/gen-data.py
SUM = ./tools/summarize_slides.py
YMIN=0.005

all: data dd pdf

pdf: lecture-1a-intro.pdf  lecture-1b-proof.pdf  lecture-2a-bdd-intro.pdf  lecture-2b-bdd-proof.pdf \

spdf: lecture-1a-summary.pdf lecture-1b-summary.pdf lecture-2a-summary.pdf lecture-2b-summary.pdf 


dd:
	pushd bdd-dd ; make ; popd
	pushd cdcl-dd ; make ; popd


data:
	rm -rf data-formatted
	mkdir data-formatted
	# Chew
	$(DGEN) < data-raw/chew-kissat-clauses.csv | sed 's/addplot/addplot[only marks, color=red!50!blue, thick]/g' \
		> data-formatted/chew-kissat-clauses.tex
	$(DGEN) < data-raw/chew-tbsat-bucket-clauses.csv | sed 's/addplot/addplot[color=darkred, mark=triangle*, thick]/g' \
		> data-formatted/chew-tbsat-bucket-clauses.tex
	$(DGEN) < data-raw/chew-tbsat-direct-clauses.csv | sed 's/addplot/addplot[color=clearorange, mark=pentagon*, thick]/g' \
		> data-formatted/chew-tbsat-direct-clauses.tex
	$(DGEN) < data-raw/chew-tbsat-gauss-clauses.csv | sed 's/addplot/addplot[color=medgreen, mark=diamond*, thick]/g' \
		> data-formatted/chew-tbsat-gauss-clauses.tex
	$(DGEN) -L $(YMIN) < data-raw/chew-kissat-seconds.csv | sed 's/addplot/addplot[only marks, color=red!50!blue, thick]/g' \
		> data-formatted/chew-kissat-seconds.tex
	$(DGEN) -L $(YMIN) < data-raw/chew-tbsat-bucket-seconds.csv | sed 's/addplot/addplot[color=darkred, mark=triangle*, thick]/g' \
		> data-formatted/chew-tbsat-bucket-seconds.tex
	$(DGEN) -L $(YMIN) < data-raw/chew-tbsat-direct-seconds.csv | sed 's/addplot/addplot[color=clearorange, mark=pentagon*, thick]/g' \
		> data-formatted/chew-tbsat-direct-seconds.tex
	$(DGEN) -L $(YMIN) < data-raw/chew-tbsat-gauss-seconds.csv | sed 's/addplot/addplot[color=medgreen, mark=diamond*, thick]/g' \
		> data-formatted/chew-tbsat-gauss-seconds.tex

lecture-1a-summary.tex: lecture-1a-intro.tex
	$(SUM) lecture-1a-intro.tex > lecture-1a-summary.tex

lecture-1b-summary.tex: lecture-1b-proof.tex
	$(SUM) lecture-1b-proof.tex > lecture-1b-summary.tex

lecture-2a-summary.tex: lecture-2a-bdd-intro.tex
	$(SUM) lecture-2a-bdd-intro.tex > lecture-2a-summary.tex

lecture-2b-summary.tex: lecture-2b-bdd-proof.tex
	$(SUM) lecture-2b-bdd-proof.tex > lecture-2b-summary.tex


.SUFFIXES: .tex .stex .pdf

.tex.pdf:
	pdflatex $<
	pdflatex $<


clean:
	pushd bdd-dd ; make clean ; popd
	pushd cdcl-dd ; make clean ; popd
	rm -f *~ *.{log,aux,nav,out,pdf,snm,toc,vrb}

superclean: clean
	rm -f *~ *.{pdf}
