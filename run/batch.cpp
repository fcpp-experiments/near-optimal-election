// Copyright © 2022 Giorgio Audrito. All Rights Reserved.

/**
 * @file batch.cpp
 * @brief Runs batch executions of the case study comparing election algorithms producing plots.
 */


#include "lib/simulation_setup.hpp"

using namespace fcpp;

//! @brief Number of identical runs to be averaged.
constexpr int runs = 50;

//! @brief The plotter object.
option::plot_t p;

//! @brief Builds a sequence of parameters initialising the simulation.
auto make_parameters(bool is_sync, int runs, std::string var = "none") {
    using namespace option;
    return batch::make_tagged_tuple_sequence(
        batch::arithmetic<seed>(0, runs-1, 1),
        batch::constant<sync>(is_sync),
        batch::arithmetic<speed>(0, 59, var == "speed" ? 1 : var == "none" ? 30 : 100),
        batch::arithmetic<crash>(0.0, 5.99, var == "prob" ? 0.1 : var == "none" ? 3 : 10),
        batch::arithmetic<dens>(10 + 10 * (var != "dens"), 40, 30),
        batch::arithmetic<side>(10 + 10 * (var != "side"), 40, 30),
        batch::constant<simtype>(var == "none" ? 0 : var == "speed" ? 1 : var == "prob" ? 2 : -1),
        batch::stringify<output>("output/batch", "txt"),
        batch::constant<plotter>(&p),
        batch::filter([&](auto const& t){
            if (var != "none") return false;
            return abs(common::get<speed>(t) - 10*common::get<crash>(t)) > 0.01;
        }),
        batch::formula<round_dev>([&](auto const& t){ return is_sync ? 0 : 0.25; }),
        batch::formula<dev_num  >([ ](auto const& t){ return (common::get<dens>(t)*common::get<side>(t)*200)/314; }),
        batch::formula<end_time >([ ](auto const& t){ return common::get<side>(t)*15; }),
        batch::formula<die_time >([ ](auto const& t){ return common::get<side>(t)*5; })
    );
}

//! @brief The main function.
int main() {
    // Runs the synchronous simulation.
    batch::run(component::batch_simulator<option::list<true>>{},
               make_parameters(true, runs*10));
    // Runs the asynchronous simulation.
    batch::run(component::batch_simulator<option::list<false>>{},
               make_parameters(false, runs*10),
               make_parameters(false, runs, "prob"),
               make_parameters(false, runs, "speed"));
    // Builds the resulting plots.
    std::cout << plot::file("batch", p.build(), {{"MAX_CROP", "1"}, {"LOG_LIN", "10"}});
    return 0;
}
