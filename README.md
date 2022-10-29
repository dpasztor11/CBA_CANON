# Colouring sets of cubic 6-poles

Cubic k-pole P is a graph that has k ordered vertices of degree one and remaining vertices are of degree three. Given a proper colouring of P with colours {0, 1, 2}, the boundary colouring is the k-tuple whose i-th element is the colour of the edge incident to i-th vertex of degree one. A colouring set of P is the set of possible boundary colourings of P. There are two obvious necessary conditions when an element from {0, 1, 2}^k may be a colouring set of a k-pole, one is given by parity lemma, second is given by the fact that the colouring set needs to be strongly Kempe closed. We call such colouring sets plausible. An equivalence relation on plausible colouring sets can be defined as follows: two colouring sets are equivalent if they have the same set of plausible colouring sets in the complement. This relation behaves nicely with respect to two other equivalence relation obtained by reordering vertices of degree one and color swapping, which gives rise to one unifying equivalence relation. We want to study which equivalence classes of the unifying equivalence contain a coloring set of some cubic 6-pole. 

# Terminology

**Cubic k-pole** = graph P with k ordered vertices of degree 1 and remaining vertices of degree 3

**Boundary colouring** = given a proper colouring of P with colours {0, 1, 2}, the coundary colouring is a k-tuple whose i-th element is the colour of the edge incident to i-th vertex of degree one.

**Colouring set of cubic k-pole** = set of all possible boundary colourings

**CBA** = ColouringBitArray (represents a colouring set of a cubic k-pole)

**Plausible CBA** = CBA that satisfies Parity Lemma and is Strongly kempe closed (we know that non-plausible CBAs can't be realized, i.e. they don't have a graph, so we only care about plausible ones)

**Parity lemma** = given a proper colouring, every colour in boundary colouring is used an even number of times

**Strongly kempe closed** = for every k-tuple from the colouring set and every set of colours {a, b} there exists index pairing of all a or b colours from k-tuple so that no matter whether we swap or not swap every pair, the k-tuple will still be in our colouring set

**C-ekvivalence** = swapping colours
(so CBA1 is ekvivalent to CBA2 iff CBA1 = {a | a is created by swapping 2 colours of b AND b is 6-tuple from CBA2})

**S-ekvivalence** = tuple permutation
(so CBA1 is ekvivalent to CBA2 iff CBA1 = {a | a = phi(b) AND b is k-tuple from CBA2 AND phi is some permutation})

**K-ekvivalence** = same Kempe closed components
(so CBA1 is ekvivalent to CBA2 iff KempeClosedComponent(CBA1) = KempeClosedComponent(CBA2), where KempeClosedComponent(CBA) is maximal plausible CBA that is a subset of complement of CBA)
