bom.csv:	mega65cr-r1.csv partsmap.csv bommerge
	./bommerge mega65cr-r1.csv partsmap.csv bom.csv

bommerge:	Makefile bommerge.c
	gcc -Wall -g -o bommerge bommerge.c
