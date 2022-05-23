DTARGET = ../../student
WDIR = ../writeups

tar: lab2.tar

lab2.tar:
	rm -rf lab2
	# Create replica of student subdirectory	
	mkdir lab2
	mkdir lab2/files lab2/generators lab2/mdp-run
	cp -p README.txt lab2
	cp -p files/*.{cnf,lrat} lab2/files
	cp -p generators/mdparity.py lab2/generators
	cp -p generators/mdpcheck.py lab2/generators
	cp -p generators/writer.py lab2/generators
	cp -p mdp-run/Makefile lab2/mdp-run
	cp -p mdp-run/{g08,g24}.sh lab2/mdp-run
	cp -p $(WDIR)/lab2.pdf lab2
	cp -p $(WDIR)/lab2-solution.pdf lab2
	# Tar and copy it to student directory tree
	tar cf lab2.tar lab2
	rm -rf lab2


dist: lab2.tar
	rm -rf $(DTARGET)/lab2
	cp lab2.tar $(DTARGET)
	(cd $(DTARGET); tar xf lab2.tar ; rm lab2.tar)
