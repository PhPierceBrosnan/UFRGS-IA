CC = gcc
CXX = g++
OPT = -g -Wall -O3 -Wno-unused-function -Wno-unused-variable
PSVNOPT = --no_state_map --no_abstraction --no_backwards_moves --history_len=0

psvn2c_core.c:
	cp ../psvn2c_core.c ./psvn2c_core.c

psvn2c_state_map.c:
	cp ../psvn2c_state_map.c ./psvn2c_state_map.c

psvn2c_abstraction.c:
	cp ../psvn2c_abstraction.c ./psvn2c_abstraction.c

%.c: %.psvn  psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
	../psvn2c  $(PSVNOPT) --name=$(*F)  < $< > $@
	rm -f ./psvn2c_core.c ./psvn2c_state_map.c ./psvn2c_abstraction.c

.PRECIOUS: %.c

%.succ: %.c succ.cpp
	$(CXX) $(OPT) succ.cpp -include $< -o $@

.PHONY : clean
clean:
	rm -f *.succ *.o sliding_tile1x3.c psvn2c_core.c psvn2c_state_map.c psvn2c_abstraction.c
