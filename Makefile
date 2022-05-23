clean:
	cd tools ; make clean
	cd instructor ; make clean

install:
	cd tools ; make install
	cd instructor ; make setup
