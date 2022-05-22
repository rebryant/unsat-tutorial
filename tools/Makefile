install:
	cd drat-trim ; make
	cd tbuddy ; make install
	echo "Note: KISSAT not installed"
	echo "You should do the following:"
	echo "   cd kissat"
	echo "   ./configure"
	echo "   make"


clean:
	cd drat-trim ; make clean
	cd kissat ; make clean
	cd tbuddy ; make clean
	rm -f *~ *.pyc
