// Copyright © 2022 Giorgio Audrito. All Rights Reserved.

/**
 * @file election_compare.hpp
 * @brief Implementation of the case study comparing election algorithms.
 */

#ifndef FCPP_ELECTION_COMPARE_H_
#define FCPP_ELECTION_COMPARE_H_

#include "lib/beautify.hpp"
#include "lib/coordination/election.hpp"
#include "lib/coordination/geometry.hpp"


/**
 * @brief Namespace containing all the objects in the FCPP library.
 */
namespace fcpp {


//! @brief Namespace containing the libraries of coordination routines.
namespace coordination {


namespace tags {
    //! @brief The side of the area where devices are located.
    struct side {};
    //! @brief The movement speed of devices.
    struct speed {};
    //! @brief The probability for a node to be prematurely terminated.
    struct crash {};
    //! @brief The time when node 0 should remove itself.
    struct die_time {};
    //! @brief The time of simulation end.
    struct end_time {};

    //! @brief The size of the node.
    struct node_size {};
    //! @brief The shape of the node.
    struct node_shape {};
    //! @brief The color of the node based on GCF__filtered leader.
    struct gcf_color {};
    //! @brief The color of the node based on Datta__filtered leader.
    struct datta_color {};

    //! @brief Output values.
    //! @{
    template <typename T>
    struct leaders {};
    template <typename T>
    struct correct {};
    template <typename T>
    struct spurious {};

    struct GCF {};
    struct Datta {};
    struct GCF__filtered {};
    struct Datta__filtered {};
    //! @}
}

//! @brief Converts a device uid to a color.
color uid2col(device_t i) {
    real_t h = i * 0.06 + 1;
    h = 320 * (1 - 1 / h);
    real_t s = (i & 1) > 0 ? 0.5 : 1;
    real_t v = (i & 2) > 0 ? 0.5 : 1;
    return color::hsva(h,s,v);
}

//! @brief Stabilise a value, accepting changes only after a number of rounds with the same value given by the delay.
GEN(T) T stabiliser(ARGS, T value, int delay) { CODE
    return get<0>(old(CALL, make_tuple(value,value,0), [&](tuple<T,T,int> o) {
        if (value == get<1>(o)) ++get<2>(o);
        else get<2>(o) = 1;
        get<1>(o) = value;
        if (get<2>(o) > delay) get<0>(o) = value;
        return o;
    }));
}

//! @brief Computes several election algorithms for comparing them.
MAIN() {
    device_t GCF = wave_election(CALL);
    device_t Datta = color_election(CALL);
    device_t GCF__filtered = stabiliser(CALL, GCF, 4);
    device_t Datta__filtered = stabiliser(CALL, Datta, 4);

    node.storage(tags::node_size{}) = 0.10;
    node.storage(tags::node_shape{}) = shape::cube;
    if (Datta__filtered == node.uid) {
        node.storage(tags::node_size{}) = 0.15;
        node.storage(tags::node_shape{}) = shape::icosahedron;
    }
    if (GCF__filtered == node.uid) {
        node.storage(tags::node_size{}) = 0.20;
        node.storage(tags::node_shape{}) = shape::sphere;
    }
    node.storage(tags::gcf_color{}) = uid2col(GCF__filtered);
    node.storage(tags::datta_color{}) = uid2col(Datta__filtered);

    rectangle_walk(CALL, make_vec(0,0), make_vec(node.storage(tags::side{}),2), node.storage(tags::speed{})*0.01, 1);
    bool perturbation = node.current_time() >= node.storage(tags::die_time{});
    if (node.uid % 10 == 0 and perturbation) node.terminate();
    double E = node.storage(tags::end_time{});
    if (node.current_time() > E + 2) node.terminate();
    if (node.uid > 1 and node.current_time() < E - 20 and node.next_real()*100 < node.storage(tags::crash{})) {
        node.velocity() /= 20;
        node.next_time(node.current_time() + 20);
        node.storage(tags::node_shape{}) = shape::tetrahedron;
    }

    node.storage(tags::leaders<tags::GCF>{}) = GCF;
    node.storage(tags::leaders<tags::Datta>{}) = Datta;
    node.storage(tags::leaders<tags::GCF__filtered>{}) = GCF__filtered;
    node.storage(tags::leaders<tags::Datta__filtered>{}) = Datta__filtered;

    node.storage(tags::correct<tags::GCF>{}) = GCF == perturbation;
    node.storage(tags::correct<tags::Datta>{}) = Datta == perturbation;
    node.storage(tags::correct<tags::GCF__filtered>{}) = GCF__filtered == perturbation;
    node.storage(tags::correct<tags::Datta__filtered>{}) = Datta__filtered == perturbation;

    node.storage(tags::spurious<tags::GCF>{}) = GCF > perturbation;
    node.storage(tags::spurious<tags::Datta>{}) = Datta > perturbation;
    node.storage(tags::spurious<tags::GCF__filtered>{}) = GCF__filtered > perturbation;
    node.storage(tags::spurious<tags::Datta__filtered>{}) = Datta__filtered > perturbation;
}


}


}

#endif // FCPP_ELECTION_COMPARE_H_
