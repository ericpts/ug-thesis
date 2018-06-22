all: build doc

.PHONY: build
build:
	./build.sh

.PHONY: doc
doc:
	doxygen Doxyfile

	cp docs/latex/classMethod__coll__graph.pdf paper/
	cd paper; convert classMethod__coll__graph.pdf classMethod__coll__graph.eps

	cp docs/latex/classProject__coll__graph.pdf paper/
	cd paper; convert classProject__coll__graph.pdf classProject__coll__graph.eps

	cd paper; make clean; make

.PHONY: test
test:
	./test.py

.PHONY: format
format:
	./format.sh

