DGEN = ./tools/gen-data.py
YMIN=0.011


data:
	rm -rf data-formatted
	mkdir data-formatted
	# Chess
	$(DGEN) < data-raw/chess-kissat-clauses.csv > data-formatted/chess-kissat-clauses.tex
	$(DGEN) < data-raw/chess-tbsat-bucket-clauses.csv > data-formatted/chess-tbsat-bucket-clauses.tex
	$(DGEN) < data-raw/chess-tbsat-linear-clauses.csv > data-formatted/chess-tbsat-linear-clauses.tex
	$(DGEN) < data-raw/chess-tbsat-scan-clauses.csv > data-formatted/chess-tbsat-scan-clauses.tex
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



clean:
	rm -f *~ *.log

