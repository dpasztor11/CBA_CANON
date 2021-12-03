test:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG test_colouring_bit_array_canon.cpp -o test.out
	./test.out

test_kempe:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG test_kempe.cpp -o test_kempe.out
	./test_kempe.out

debug:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG generateCBAs.cpp -o debug.out
	./debug.out

run:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  generateCBAs.cpp -o run.out
	./run.out

debug2:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG generateCBAs2.cpp -o debug2.out
	./debug2.out

run2:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  generateCBAs2.cpp -o run2.out
	./run2.out

feasible_debug:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG generatefeasibleCBA.cpp -o feasibledebug.out
	./feasibledebug.out

feasible_run:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  generatefeasibleCBA.cpp -o feasiblerun.out
	./feasiblerun.out

feasible_run_p:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3 generatefeasibleCBA_parallel.cpp -o feasiblerun_p.out -ltbb
	./feasiblerun_p.out

    
5poles:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fconcepts -fmax-errors=3 -I../../Git/ba-graph/include -DDEBUG -DBA_GRAPH_DEBUG 5polestest.cpp -o 5polestest.out
	./5polestest.out


.PHONY: test debug run feasible_debug feasible_run 5poles run2 debug2 feasible_run_p test_kempe
