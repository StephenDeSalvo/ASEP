//
//  ASEP.h
//  DeSalvo
//
//  Created by Stephen DeSalvo on 01/24/19.
//  Copyright (c) 2019 Stephen DeSalvo. All rights reserved.
//


#ifndef DeSalvoASEP_h
#define DeSalvoASEP_h



#include <iostream>
#include <vector>

#include "../desalvo_standard_library/timer.h"
#include "../desalvo_standard_library/std_cout.h"

namespace dsl = desalvo_standard_library;

/** @namespace ASEP
    @brief namespace to keep track of the various parameterizations and variations of the asymmetric exclusion process.

    There are many variations of ASEP depending on the dynamics of the particles, the dimension of the lattice, and the exclusion function.
    We have just implemented the standard one with a bit of flexibility in the Universe, or graph, that the particles are allowed to live on.
    For any graph you want, just add in a source and a sink with holding probabilities in order to create the birth and death states.
*/
namespace ASEP {


    /** @class ASEP
        @brief Creates an object representing a run of the classical ASEP on a graph.

        @tparam Universe is a Graph
        @tparam Particle is an object which can live on a Graph
        @tparam URNG is the standard uniform random number generator type
        @tparam Timer is the internal clock on the particles.

        REQUIRES:
        Very important, the Universe class must have a member function with signature
            initial_state() -> const Position*
        where Position is the equivalent of a Node type for the Graph type.

        This is important in two places, the first is to initialize the particles.  The second
        is because the exclusion function requires knowing the type of the proposed state to 
        move to, in order to compare with the other particles.  This way only the Universe object
        needs to reference both the initial_state for the particles as well as the type for the 
        exclusion function. 

        DO NOT Forget this member function or you will incur the wrath of the compiler's template errors.

        @code
            // Sample code to create and run an ASEP.
            #include <iostream>
            #include <string>

            #include "desalvo/polymorphic_graph.h"
            #include "desalvo/particle.h"
            #include "desalvo/timer.h"
            #include "ASEP/ASEP.h"

            namespace dsl = desalvo_standard_library;


            typedef dsl::Particle<dsl::PolymorphicGraph, dsl::Node> Particle;
            typedef dsl::Node Position;


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

                unsigned int seed = 42;
                std::default_random_engine generator(seed);

                // Use any of these out-of-the box graphs
                //dsl::PolymorphicGraph graph = dsl::complete_graph(6);
                //dsl::PolymorphicGraph graph = dsl::random_weighted_graph(6, .8, generator);
                //dsl::PolymorphicGraph graph = dsl::random_weighted_line_with_source_sink(10, generator);
                //dsl::PolymorphicGraph graph = dsl::random_weighted_complete_directed_graph(6, generator);
                dsl::PolymorphicGraph graph = dsl::random_weighted_complete_directed_graph_with_source_sink(100, generator);

                // Or load in the incidence matrix from a file 
                //std::string filename = "input/graph.txt"; //argv[1];
                //dsl::PolymorphicGraph graph = dsl::graph_from_file(filename);

                //graph.print(std::cout);

                // Create a classical ASEP with 20 particles and standard exponential timer for particle movements.
                ASEP::ASEP<dsl::PolymorphicGraph, Particle, dsl::timer<> > asep(&graph, 20, exclusion_function);

                // Print out the initial states.
                std::cout << "Initial States: " << std::endl;
                asep.print(std::cout);
                std::cout << std::endl;

                // Run the process forward 100 time epochs.
                asep.run(100);

                // Print out the final states.
                std::cout << "Final States: " << std::endl;
                asep.print(std::cout);
                std::cout << std::endl;

                return 0;
            }

        @endcode
    */
    template<typename Universe, typename Particle, typename Timer=dsl::timer<>, typename URNG=std::default_random_engine>
    class ASEP {
    public:

        ASEP() = delete;
        ASEP(ASEP&) = delete;

        /** Constructor for standard ASEP.
            @param initial_universe is the universe to run the process on.
            @param initial_particles is the initial number of particles to start the process with.
            @param initial_exclusion_function is the initial exclusion function to apply to proposed moves.
            @param initial_generator is the random number generator
        */
        ASEP(Universe* initial_universe, size_t initial_particles, std::function<bool(decltype(Universe().initial_state()), std::vector<Particle*>&)> initial_exclusion_function, URNG& initial_generator) : universe(initial_universe), exclusion_function(initial_exclusion_function), generator(initial_generator) {

            auto initial_state = universe->initial_state();

            // Creates particles at initial state and seeds the random generator.
            for (size_t i=0; i<initial_particles; ++i)
                particles.push_back(new Particle(universe, initial_state, generator));

        }

        /** Start your engines!  This function runs the ASEP until time time epochs have transpired.
            @param time is the time in which to stop the simulation.
        */
        void run(double time) {

            // First get the vector of jump times and indexes
            std::vector<std::pair<size_t, double>> jump_times;
            size_t index = 0;
            for (auto& particle : particles) {
                for (double x : particle->jump_times(time))
                    jump_times.push_back(std::make_pair(index, x));
                ++index;
            }

            // Make a lambda function to do the comparison.
            typedef std::pair<size_t, double> vec_pair;
            std::function< bool(vec_pair, vec_pair) > vec_pair_compare = [](const vec_pair& a, const vec_pair& b) { return a.second < b.second; };

            // Sort them by jump times.
            std::sort(std::begin(jump_times), std::end(jump_times), vec_pair_compare);

            //std::cout << jump_times << std::endl;

            for (auto& x : jump_times) {
                particles[x.first]->update(generator, particles, exclusion_function);
            }

        }


        /** Show the positions of each particle.
            @param out is a stream object, by default std::cout
        */
        void print(std::ostream& out=std::cout) const {
            for (auto& particle : particles) {
                particle->print(out);
                out << " ";
            }
        }


    private:
        Universe* universe;
        std::vector<Particle*> particles;
        std::function<bool(decltype(Universe().initial_state()), std::vector<Particle*>&)> exclusion_function;
        URNG generator;
    };




}


#endif