

pdf: dodd lab1.pdf lab1-solution.pdf lab2.pdf lab2-solution.pdf

dodd:
	(pushd dd ; make ; popd)

lab1.pdf: lab1.tex lab1-body.tex exercise.sty
	pdflatex lab1
	pdflatex lab1

lab1-solution.pdf: lab1-solution.tex lab1-body.tex exercise.sty
	pdflatex lab1-solution
	pdflatex lab1-solution


lab2.pdf: lab2.tex lab2-body.tex exercise.sty
	pdflatex lab2
	pdflatex lab2

lab2-solution.pdf: lab2-solution.tex lab2-body.tex exercise.sty
	pdflatex lab2-solution
	pdflatex lab2-solution


clean:
	rm -f *.{aux,log}
	rm -f *~
