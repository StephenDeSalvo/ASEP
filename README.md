ASEP — moved
============

**This repository is retired.**  The asymmetric exclusion process now lives in
the DeSalvo Standard Library, as `asep.h`:

> **https://github.com/StephenDeSalvo/desalvo_standard_library**

This repository always depended on that one, and half of it had already moved:
the `Graph` and `Particle` the process runs on went across in 2020 as
`polymorphic/graph.h` and `polymorphic/particle.h`.  `ASEP.h` was the piece
left behind, which made this repo a header that could not stand on its own and
a library that was missing its process.  Now there is one place.

What changed in the move — the demos here need all of it:

```cpp
#include "asep.h"                      // was "ASEP.h"
#include "polymorphic/graph.h"         // was "../../desalvo_standard_library/polymorphic_graph.h"
#include "polymorphic/particle.h"      // was "particle.h"

dsl::polymorphic::Graph                // was dsl::PolymorphicGraph
dsl::polymorphic::Node                 // was dsl::Node
dsl::polymorphic::Particle<...>        // was dsl::Particle<...>
dsl::polymorphic::random_weighted_complete_directed_graph_with_source_sink(...)
```

`ASEP::ASEP<...>` is spelled the same — an alias keeps the top-level `ASEP::`
namespace working — and the constructor and `run` / `print` / `get_positions`
are unchanged.  No sibling checkout is needed any more; one clone has
everything.

Three bugs were fixed on the way, each of which stopped the process running on
a current compiler:

- `ASEP.h` used `std::function`, `std::sort` and `std::default_random_engine`
  without including `<functional>`, `<algorithm>` or `<random>`.
- `particle.h` declared `std::discrete_distribution<double>`; that
  distribution returns an index, so its argument must be integral.  The
  casualty was `update()`, the one function that moves a particle.
- `file.h` used an `operator>>` it did not include, so whether it compiled
  depended on include order.

There are tests now, in `unit_tests/asep_tests.cpp`, which run the process
rather than just instantiating it — none of the three was visible otherwise.

This repository stays readable for the sake of any link that points at it.
