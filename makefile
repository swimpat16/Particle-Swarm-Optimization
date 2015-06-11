all: gen_config lab6 run

gen_config: gen_config.cc
	g++ -g -O2 -o gen_config gen_config.cc

lab6: lab6.cc
	g++ -g -O2 -o lab6 lab6.cc

run: run.cc
	g++ -g -O2 -o run run.cc

force:
	g++ -g -O2 -o gen_config gen_config.cc
	g++ -g -O2 -o lab6 lab6.cc
	g++ -g -O2 -o run run.cc

clean:
	rm -f gen_config lab6 run debug.csv

imgclean:
	rm -f img/*
	
superclean:
	rm -f gen_config lab6 run debug.csv config/* out/* img/*
