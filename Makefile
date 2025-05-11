.PHONY: all cexamples clean

all: cexamples

cexamples:
	cd code/CExamples && $(MAKE)

clean:
	cd code/CExamples && $(MAKE) clean
