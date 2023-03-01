test:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG test_colouring_bit_array_canon.cpp -o build/test.out
	./build/test.out

test_kempe:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG test_kempe.cpp -o build/test_kempe.out
	./build/test_kempe.out

debug:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG generateCBAs.cpp -o build/debug.out
	./build/debug.out

run:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  generateCBAs.cpp -o build/run.out
	./build/run.out

debug2:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG generateCBAs2.cpp -o build/debug2.out
	./build/debug2.out

run2:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  generateCBAs2.cpp -o build/run2.out
	./build/run2.out

feasible_debug:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fmax-errors=3 -DDEBUG -DBA_GRAPH_DEBUG generatefeasibleCBA.cpp -o build/feasibledebug.out
	./build/feasibledebug.out

feasible_run:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  generatefeasibleCBA.cpp -o build/feasiblerun.out
	./build/feasiblerun.out

feasible_run_p:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3 generatefeasibleCBA_parallel.cpp -o build/feasiblerun_p.out -ltbb
	./build/feasiblerun_p.out

5poles:
	g++ -std=c++17 -g -O0 -pedantic -Wall -Wextra -fconcepts -fmax-errors=3 -I../../Git/ba-graph/include -DDEBUG -DBA_GRAPH_DEBUG 5polestest.cpp -o build/5polestest.out
	./build/5polestest.out

reducedComplement:
	g++ -std=c++17 -g -O3 -pedantic -Wall -Wextra -fmax-errors=3 src/EquivalenceGeneration/generateCSKEq.cpp -o build/generateCSKEq.out
	./build/generateCSKEq.out

generateMinimal:
	g++ -std=c++17 -g -O3 -pedantic -Wall -Wextra -fmax-errors=3 generateCBAMinimal_parallel.cpp -o build/generateCBAMinimal_parallel.out -ltbb
	./build/generateCBAMinimal_parallel.out

generateFeasibleToFile:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3 generateCBAToFile_parallel.cpp -o build/generateCBAToFile_parallel.out -ltbb
	./build/generateCBAToFile_parallel.out

generateCEq:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3 generateCEq.cpp -o build/generateCEq.out -ltbb
	./build/generateCEq.out

generateCEqCSEq:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3 src/EquivalenceGeneration/generateCEqCSEq.cpp -o build/generateCEqCSEq.out -ltbb
	./build/generateCEqCSEq.out

generatePoset:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3 posetCSKEq.cpp -o build/posetCSKEq.out
	./build/posetCSKEq.out

generateSolutions:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  src/findingCBAs/generateCSKSolutions.cpp -o build/run.out
	./build/run.out

graphJoin6x6:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  src/findingCBAs/wip/graphJoin6x6.cpp -o build/run.out
	./build/run.out

graphJoin7:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  src/findingCBAs/wip/graphJoin7.cpp -o build/run.out
	./build/run.out

graphJoin7x5:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  src/findingCBAs/wip/graphJoin7x5.cpp -o build/run.out
	./build/run.out

graphJoin:
	g++ -std=c++17 -O3 -pedantic -Wall -Wextra -fmax-errors=3  src/findingCBAs/graphJoin.cpp -o build/run.out
	./build/run.out


.PHONY: test debug run feasible_debug feasible_run 5poles run2 debug2 feasible_run_p test_kempe
