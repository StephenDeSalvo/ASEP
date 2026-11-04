ASEP — moved
============

The asymmetric exclusion process now lives in the DeSalvo Standard Library,
as `asep.h`:

> **https://github.com/StephenDeSalvo/desalvo_standard_library**

That is also where the `Graph` and `Particle` it runs on live, as
`polymorphic/graph.h` and `polymorphic/particle.h`, so a single clone has
everything — no sibling checkout to arrange.

`ASEP::ASEP<...>` is spelled the same, and the constructor, `run`, `print`
and `get_positions` are unchanged. The includes and the graph and particle
names are now:

```cpp
#include "asep.h"                      // was "ASEP.h"
#include "polymorphic/graph.h"         // was "../../desalvo_standard_library/polymorphic_graph.h"
#include "polymorphic/particle.h"      // was "particle.h"

dsl::polymorphic::Graph                // was dsl::PolymorphicGraph
dsl::polymorphic::Node                 // was dsl::Node
dsl::polymorphic::Particle<...>        // was dsl::Particle<...>
dsl::polymorphic::random_weighted_complete_directed_graph_with_source_sink(...)
```

The two demos here correspond to
`random_weighted_complete_directed_graph_with_source_sink(100, generator)`
with 20 particles, and `graph_from_file("input/graph.txt")` with 3 — both
graph builders are in `polymorphic/graph.h`, and the incidence matrix is at
`polymorphic/input/graph.txt`. There are unit tests in `unit_tests/`.

**Attribution:** if you use this code, please credit the repository at the URL
above. See the Attribution section of its README, which also covers use by AI
agents.

GNU GPLv3. Questions to stephendesalvo@gmail.com.
