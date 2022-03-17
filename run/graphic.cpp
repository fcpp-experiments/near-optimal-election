// Copyright © 2022 Giorgio Audrito. All Rights Reserved.

/**
 * @file graphic.cpp
 * @brief Runs a single execution of the case study comparing election algorithms with a graphical user interface.
 */

#include "lib/simulation_setup.hpp"

using namespace fcpp;

//! @brief The plotter object.
option::plot_t p;

//! @brief Performs a single graphic run.
template <bool is_sync>
void graphic_run(bool moving = false) {
    // The network object type (interactive simulator with given options).
    using net_t = typename component::interactive_simulator<option::list<is_sync>>::net;
    // The initialisation values (simulation name, texture of the reference plane, node movement speed).
    auto init_v = common::make_tagged_tuple<option::name, option::sync, option::seed, option::speed, option::dens, option::side, option::round_dev, option::dev_num, option::end_time, option::die_time, option::crash, option::plotter, option::area_min, option::area_max>(
        "Leader Election (" + std::string(is_sync ? "" : "a") + "synchronous" + std::string(moving ? ", moving" : "") + ")",
        is_sync,
        0,
        moving ? 30 : 0,
        20,
        20,
        is_sync ? 0 : 0.25,
        254,
        300,
        100,
        moving ? 3 : 0,
        &p,
        make_vec(0,0),
        make_vec(20,2)
    );
    // Construct the network object.
    net_t network{init_v};
    // Run the simulation until exit.
    network.run();
}

//! @brief The main function.
int main() {
    std::cout << "/*\n";
    // Runs the synchronous simulation.
    graphic_run<true>();
    // Runs the asynchronous simulation.
    graphic_run<false>();
    // Runs the synchronous moving simulation.
    graphic_run<true>(true);
    // Runs the asynchronous moving simulation.
    graphic_run<false>(true);
    // Builds the resulting plots.
    std::cout << "*/\n" << plot::file("graphic", p.build(), {{"MAX_CROP", "1"}, {"LOG_LIN", "10"}});
    return 0;
}
