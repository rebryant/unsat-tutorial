DEST = ../student
SRC = .
LECSRC = $(SRC)/slides
LECDEST = $(DEST)/lectures
LECFILES = $(LECSRC)/lecture-1a-intro.pdf $(LECSRC)/lecture-1b-proof.pdf $(LECSRC)/lecture-2a-bdd-intro.pdf $(LECSRC)/lecture-2b-bdd-proof.pdf

setup:
	rm -rf $(DEST)
	mkdir $(DEST)
	mkdir $(LECDEST)
	cp -p $(LECFILES) $(LECDEST)	
	(cd lab1; make setup)
	(cd lab2; make setup)
