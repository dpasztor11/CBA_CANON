FLAGS_COMMON = -std=c++17 -pedantic -Wall -Wextra -fmax-errors=3
FLAGS_DEBUG = -g -O0 -DDEBUG -DBA_GRAPH_DEBUG $(FLAGS_COMMON)
FLAGS_RELEASE = -O3 -ltbb $(FLAGS_COMMON)

FINDING_CBAS_TARGETS = graphJoin graphJoin2 generateCSKSolutions generateCSKSolutions2
EQUIVALENCE_GENERATION_TARGETS = generateCSKEq generateCEqCSEq
EQUIVALENCE_GENERATION_OLD_TARGETS = generatefeasibleCBA generatefeasibleCBA_parallel
OTHER_TARGETS = unfoundCsEq generateCskPoset

define generate_compile_rule
$(2): | createBuildFolders/$(1)
	g++ $(FLAGS_RELEASE) src/$(1)/$(2).cpp -o build/$(1)/$(2).out -ltbb
	./build/$(1)/$(2).out
endef

#define generate_compile_test_rule
#$(1):
#	g++ $(FLAGS_RELEASE) tests/$(1).cpp -o build/$(1)/$(2).out -ltbb
#	./build/$(1)/$(2).out
#endef

# Generates rules for all targets
$(foreach target, $(FINDING_CBAS_TARGETS), $(eval $(call generate_compile_rule,findingCBAs,$(target))))
$(foreach target, $(EQUIVALENCE_GENERATION_TARGETS), $(eval $(call generate_compile_rule,EquivalenceGeneration,$(target))))
$(foreach target, $(EQUIVALENCE_GENERATION_OLD_TARGETS), $(eval $(call generate_compile_rule,EquivalenceGeneration/old,$(target))))
$(foreach target, $(OTHER_TARGETS), $(eval $(call generate_compile_rule,other,$(target))))


createBuildFolders/%:
	mkdir -p build/$*

test:
	g++ $(FLAGS_DEBUG) test_colouring_bit_array_canon.cpp -o build/test.out
	./build/test.out

test_kempe:
	g++ $(FLAGS_DEBUG) test_kempe.cpp -o build/test_kempe.out
	./build/test_kempe.out

5poles:
	g++ $(FLAGS_DEBUG) -fconcepts -I../../Git/ba-graph/include 5polestest.cpp -o build/5polestest.out
	./build/5polestest.out

.PHONY: test 5poles test_kempe createBuildFolders/%
