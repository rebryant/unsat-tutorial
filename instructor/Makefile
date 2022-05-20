DEST = ../student
SRC = .
LECSRC = $(SRC)/slides
LECDEST = $(DEST)/lectures
LABSRC= $(SRC)/labs
L1DEST= $(DEST)/lab1
L2DEST= $(DEST)/lab2
FILTER= 'OMIT'

LECFILES = $(LECSRC)/lecture-1a-intro.pdf $(LECSRC)/lecture-1b-proof.pdf $(LECSRC)/lecture-2a-bdd-intro.pdf $(LECSRC)/lecture-2b-bdd-proof.pdf
L1FILES = $(LABSRC)/writeups/lab1.pdf $(LABSRC)/writeups/lab1-solution.pdf $(LABSRC)/files/eg-1.cnf $(LABSRC)/files/phpd-02.cnf $(LABSRC)/files/phpd-03.cnf \
	$(LABSRC)/generators/gen_pigeon.py $(LABSRC)/generators/writer.py
L1SPECIALFILE = cnf_utilities.py
L1SPECIALPATH =  $(LABSRC)/generators/$(L1SPECIALFILE)
L1README = $(LABSRC)/README-L1.txt

# L2FILES = $(LABSRC)/writeups/lab2.pdf $(LABSRC)/writeups/lab2-solution.pdf

setup:
	rm -rf $(LECDEST) $(L1DEST) $(L2DEST)
	mkdir $(LECDEST) $(L1DEST) $(L2DEST)


dist: $(LECFILES) $(L1FILES) $(L1SPECIALPATH) $(L2FILES)
	cp -p $(LECFILES) $(LECDEST)
	cp -p $(L1FILES) $(L1DEST)
	grep -v $(FILTER) $(L1SPECIALPATH) > $(L1DEST)/$(L1SPECIALFILE)
	cp -p $(L1README) $(L1DEST)/README.txt
#	cp -p $(L2FILES) $(L2DEST)
