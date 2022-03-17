#include <iomanip>
#include <iostream>
#include <sstream>

#include "lib/func.hpp"
#include "lib/common/plot.hpp"

// double checks that the constraints are satisfied
frac double_check(const func& g, int L) {
    frac K = 1;
    for (int x=0; x<L; ++x)
        K = std::max(K, frac(g.recovery(x), g.ideal(x)));
    return K;
}

// searches for the best competitiveness within [a,b]
std::pair<frac,frac> best_competitiveness(frac a, frac b) {
    // invariant: func(a) fails, func(b) succeeds with competitiveness k
    frac k = func(b).competitiveness();
    while (k > a) { // when k == a, k is minimum and b upper bound
        frac c = double(b-a) > 1e-7 ? (a+b)/2 : k;
        if (c > k) {
            b = c;
            continue;
        }
        func g(c);
        frac r = g.competitiveness();
        if (r < c) {
            std::cout << "success for " << c << " with " << r << " = " << double(r) << " at x = " << g.size() << std::endl;
            b = c;
            k = r;
        } else {
            std::cout << "failure for " << c << " with " << r << " = " << double(r) << " at x = " << g.size() << std::endl;
            a = c;
        }
    }
    return {k, b};
}

template <typename T>
struct y {};

std::string to_graph(func const& g) {
    using namespace fcpp;
    struct best {};
    struct asymptotic {};
    struct x {};
    using row_t = common::type_sequence<x, aggregator::mean<double>, y<best>, aggregator::mean<double>, y<asymptotic>, aggregator::mean<double>>;
    using plot_t = plot::split<x, plot::values<row_t, common::type_sequence<>, plot::unit<y>>, std::ratio<100,1>>;

    plot_t p;
    int maxx = g.size() * 6 / 5;
    double offs = (double)g.offset();
    for (int i=0; i<maxx; ++i) {
        p << common::make_tagged_tuple<x, aggregator::mean<y<best>, true>, aggregator::mean<y<asymptotic>, true>>(i, g.dir(i), offs + (1 + sqrt(2))*i);
    }
    std::stringstream ss;
    ss << plot::file("parameter", p.build(), {{"styles[1]", "dotted"}, {"colors[1]", "heavyred"}});
    return ss.str();
}

int main() {
    std::string plot;
    int L = 10000000;
    std::cout.precision(17);
    std::cout << "/*\n";
    {
        std::cout << "SEARCHING BEST COMPETITIVENESS" << std::endl;
        auto p = best_competitiveness(frac(29,12), frac(25,10));
        frac K = p.first;  // 32/13
        frac U = p.second; // 1230757/499995
        std::cout << "BEST COMPETITIVENESS POSSIBLE:  " << K << " = " << double(K) << std::endl;
        std::cout << "UPPER BOUND TO COMPETITIVENESS: " << U << " = " << double(U) << std::endl << std::endl;
        
        func g(U);
        K = g.competitiveness();
        std::cout << "DOUBLE CHECK: " << K << " = " << double(K) << ", " << g.size() << " custom values, " << g.offset() << " offset" << std::endl;
        K = double_check(g, L);
        std::cout << "TRIPLE CHECK: " << K << " = " << double(K) << ", checked up to " << L << std::endl;
        std::cout << g << std::endl << std::endl;
        plot = to_graph(g);
    }
    {
        std::cout << "SIMPLER GOOD-ENOUGH FUNCTION" << std::endl;
        frac K(5,2);
        func g(K);
        K = g.competitiveness();
        std::cout << "DOUBLE CHECK: " << K << " = " << double(K) << ", " << g.size() << " custom values, " << g.offset() << " offset" << std::endl;
        K = double_check(g, L);
        std::cout << "TRIPLE CHECK: " << K << " = " << double(K) << ", checked up to " << L << std::endl;
        std::cout << g << std::endl << std::endl;
    }
    std::cout << "*/\n" << plot;
}
