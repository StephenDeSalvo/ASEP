#include <iostream>
#include <string>

#include "../../desalvo_standard_library/polymorphic_graph.h"
#include "../../desalvo_standard_library/timer.h"
#include "../ASEP.h"
#include "../particle.h"

namespace dsl = desalvo_standard_library;


typedef dsl::Particle<dsl::PolymorphicGraph, dsl::Node> Particle;
typedef dsl::Node Position;

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "Catch2/catch.hpp"


namespace dsl = desalvo_standard_library;


// this is the traditional exclusion function for the ASEP.
bool exclusion_function(const Position* proposed_position, std::vector<Particle*>& particles) {
    bool block = false;

    // Iterate through each particle to check the block condition.
    for (auto& particle : particles) {
        if (particle->get_position() == proposed_position)
            block = true;
    }
    return block;
}

// Tests for fraction.h.
TEST_CASE( "ASEP", "[asep]" ) {

    unsigned int seed = 42;
    std::default_random_engine generator(seed);

    dsl::PolymorphicGraph graph = dsl::complete_graph(6);

    ASEP::ASEP<dsl::PolymorphicGraph, Particle, dsl::timer<> > asep(&graph, 2, exclusion_function, generator);

    auto positions = asep.get_positions();
    std::vector<std::string> labels(positions.size());
    size_t counter = 0;
    for (auto& position : positions) {
        labels[counter] = (position->get_position()->get_label());
        ++counter;
    }

    REQUIRE (labels == std::vector<std::string>{"0","0"});

    asep.run(2);

    positions = asep.get_positions();
    counter = 0;
    for (auto& position : positions) {
        labels[counter] = (position->get_position()->get_label());
        ++counter;
    }

    REQUIRE (labels == std::vector<std::string>{"5","0"});


    //dsl::PolymorphicGraph graph = dsl::random_weighted_graph(6, .8, generator);
    //dsl::PolymorphicGraph graph = dsl::random_weighted_line_with_source_sink(10, generator);
    //dsl::PolymorphicGraph graph = dsl::random_weighted_complete_directed_graph(6, generator);
    //dsl::PolymorphicGraph graph = dsl::random_weighted_complete_directed_graph_with_source_sink(100, generator);

    //std::string filename = "input/graph.txt"; //argv[1];
    //dsl::PolymorphicGraph graph = dsl::graph_from_file(filename);
    //graph.print(std::cout);


    //return 0;
}



