// Copyright © 2022 Giorgio Audrito. All Rights Reserved.

/**
 * @file simulation_setup.hpp
 * @brief Simulation setup for the case study comparing election algorithms.
 */

#ifndef FCPP_SIMULATION_SETUP_H_
#define FCPP_SIMULATION_SETUP_H_

#include "lib/fcpp.hpp"
#include "lib/election_compare.hpp"


/**
 * @brief Namespace containing all the objects in the FCPP library.
 */
namespace fcpp {

namespace option {

using namespace common::tags;
using namespace component::tags;
using namespace coordination::tags;


struct sync {};      // whether it is synchronous           = true, false
struct dens {};      // average density                     = 10, 20, 30
//     side          // number of hops                      = 10, 20, 40
//     speed         // maximum movement speed              = 0, 0.25, 0.5
struct dev_num {};   // total number of devices             = dens*side*2/π
struct round_dev {}; // standard deviation in round length  = sync ? 0 : 0.25
//     end_time      // time for end simulation             = 10*side
//     die_time      // time for disruption                 = 5*side
struct simtype {};   // type of the simulation

                     // total complexity of simulation      = (2*dens*side)^2

using d0 = distribution::constant_n<times_t, 0>;
using d1 = distribution::constant_n<times_t, 1>;
using d2 = distribution::constant_n<times_t, 2>;

template <bool is_sync>
using spawn_s = sequence::multiple<
    distribution::constant_i<size_t, dev_num>,
    distribution::interval_n<times_t, 0, is_sync ? 0 : 20>,
    is_sync
>;

using round_s = sequence::periodic<
    distribution::weibull<d1, d0, void, round_dev>,
    distribution::weibull<d1, d0, void, round_dev>
>;

using export_s = sequence::periodic<d0, d1, distribution::constant_i<times_t, end_time>>;

using rectangle_d = distribution::rect<d0, d0, distribution::constant_i<double, side>, d2>;

using aggregator_t = aggregators<
    leaders<GCF>,               aggregator::distinct<device_t>,
    leaders<Datta>,             aggregator::distinct<device_t>,
    leaders<GCF__filtered>,     aggregator::distinct<device_t>,
    leaders<Datta__filtered>,   aggregator::distinct<device_t>,

    correct<GCF>,               aggregator::sum<int>,
    correct<Datta>,             aggregator::sum<int>,
    correct<GCF__filtered>,     aggregator::sum<int>,
    correct<Datta__filtered>,   aggregator::sum<int>,

    spurious<GCF>,              aggregator::sum<int>,
    spurious<Datta>,            aggregator::sum<int>,
    spurious<GCF__filtered>,    aggregator::sum<int>,
    spurious<Datta__filtered>,  aggregator::sum<int>
>;

template <typename xvar>
using plot_row_t = plot::join<plot::plotter<aggregator_t, xvar, leaders>, plot::plotter<aggregator_t, xvar, correct>, plot::plotter<aggregator_t, xvar, spurious>>;

using plot_time_t = plot::split<sync, plot::filter<simtype, filter::equal<0>, plot::split<common::type_sequence<sync, speed, crash>, plot_row_t<plot::time>>>>;

template <typename xvar>
using plot_var_row_t = plot::filter<plot::time, filter::above<100>, plot_row_t<xvar>>;

template <typename xvar, int n, typename svar>
using plot_var_t = plot::filter<simtype, filter::equal<n>, plot::split<common::type_sequence<sync, svar>, plot_var_row_t<xvar>>>;

using plot_t = plot::join<plot_var_t<speed, 1, crash>, plot_var_t<crash, 2, speed>, plot_time_t>;


template <bool is_sync>
DECLARE_OPTIONS(list,
    synchronised<is_sync>,
    parallel<false>,
    program<coordination::main>,
    retain<metric::retain<2>>,
    round_schedule<round_s>,
    exports<
        tuple<device_t, device_t, int>, vec<2>,
        tuple<device_t, int>, tuple<device_t, int, int, int>,
        tuple<bool,device_t,int,device_t>, tuple<bool,device_t,int,device_t,bool>,
        tuple<bool, unsigned int, short, unsigned int>,
        tuple<unsigned int, short, short, short>
    >,
    log_schedule<export_s>,
    aggregator_t,
    tuple_store<
        side,                       double,
        speed,                      double,
        crash,                      double,
        die_time,                   times_t,
        end_time,                   times_t,

        node_size,                  double,
        node_shape,                 shape,
        gcf_color,                  color,
        datta_color,                color,

        leaders<GCF>,               device_t,
        leaders<Datta>,             device_t,
        leaders<GCF__filtered>,     device_t,
        leaders<Datta__filtered>,   device_t,

        correct<GCF>,               int,
        correct<Datta>,             int,
        correct<GCF__filtered>,     int,
        correct<Datta__filtered>,   int,

        spurious<GCF>,              int,
        spurious<Datta>,            int,
        spurious<GCF__filtered>,    int,
        spurious<Datta__filtered>,  int
    >,
    extra_info<sync, int, speed, double, crash, double, simtype, int>,
    plot_type<plot_t>,
    spawn_schedule<spawn_s<is_sync>>,
    init<
        x,          rectangle_d,
        seed,       functor::cast<distribution::interval_n<double, 0, 1<<30>, uint_fast32_t>,
        side,       distribution::constant_i<double, side>,
        speed,      distribution::constant_i<double, speed>,
        round_dev,  distribution::constant_i<double, round_dev>,
        crash,      distribution::constant_i<double, crash>,
        die_time,   distribution::constant_i<times_t, die_time>,
        end_time,   distribution::constant_i<times_t, end_time>
    >,
    connector<connect::fixed<>>,
    size_tag<node_size>,
    shape_tag<node_shape>,
    color_tag<gcf_color, datta_color>
);

}

}

#endif // FCPP_SIMULATION_SETUP_H_
