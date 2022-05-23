DTARGET = ../../student
WDIR = ../writeups
FILTER = 'OMIT'

tar: lab1.tar

setup: lab1.tar
	rm -rf $(DTARGET)
	mkdir $(DTARGET)
	cp lab1.tar $(DTARGET)
	(cd $(DTARGET); tar xf lab1.tar ; rm lab1.tar)
	rm -f lab1.tar

lab1.tar:
	rm -rf lab1
	# Create replica of student subdirectory	
	mkdir lab1
	mkdir lab1/files lab1/generators
	cp -p README.txt lab1
	cp -p files/*.cnf lab1/files
	# Remove solution code from file
	cat generators/cnf_utilities.py | grep -v $(FILTER) > lab1/generators/cnf_utilities.py
	cp -p generators/gen_pigeon.py lab1/generators
	cp -p generators/writer.py lab1/generators
	cp -p $(WDIR)/lab1.pdf lab1
	cp -p $(WDIR)/lab1-solution.pdf lab1
	# Tar and copy it to student directory tree
	tar cf lab1.tar lab1
	rm -rf lab1



