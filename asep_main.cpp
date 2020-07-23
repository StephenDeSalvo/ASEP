#include <iostream>
#include <string>

#include "../../desalvo_standard_library/polymorphic_graph.h"
#include "../../desalvo_standard_library/timer.h"
#include "ASEP.h"
#include "particle.h"

namespace dsl = desalvo_standard_library;


typedef dsl::Particle<dsl::PolymorphicGraph, dsl::Node> Particle;
typedef dsl::Node Position;


/** @function exclusion_function
    @brief this is the traditional exclusion function for the ASEP.
    @param proposed_position is the position the particle tries to go to.
    @param particles is the collection of all particles, which is needed to determine whether the move is legal.
    @returns true if the move is legal, false otherwise.
*/
bool exclusion_function(const Position* proposed_position, std::vector<Particle*>& particles) {
    bool block = false;

    // Iterate through each particle to check the block condition.
    for (auto& particle : particles) {
        if (particle->get_position() == proposed_position)
            block = true;
    }
    return block;
}

int main(int argc, const char * argv[]) {

    unsigned int seed = time(nullptr);
    std::default_random_engine generator(seed);

    //dsl::PolymorphicGraph graph = dsl::complete_graph(6);
    //dsl::PolymorphicGraph graph = dsl::random_weighted_graph(6, .8, generator);
    //dsl::PolymorphicGraph graph = dsl::random_weighted_line_with_source_sink(10, generator);
    //dsl::PolymorphicGraph graph = dsl::random_weighted_complete_directed_graph(6, generator);
    dsl::PolymorphicGraph graph = dsl::random_weighted_complete_directed_graph_with_source_sink(100, generator);

    //std::string filename = "input/graph.txt"; //argv[1];
    //dsl::PolymorphicGraph graph = dsl::graph_from_file(filename);
    //graph.print(std::cout);


    ASEP::ASEP<dsl::PolymorphicGraph, Particle, dsl::timer<> > asep(&graph, 20, exclusion_function, generator);

    std::cout << "Initial States: " << std::endl;
    asep.print(std::cout);
    std::cout << std::endl;

    asep.run(10000);

    std::cout << "Final States: " << std::endl;
    asep.print(std::cout);
    std::cout << std::endl;


    return 0;
}



