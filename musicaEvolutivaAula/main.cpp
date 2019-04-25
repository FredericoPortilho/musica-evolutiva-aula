#include <iostream>

#include "affective.h"
#include "melodia.h"
#include "EA.hpp"
#include "island_EA.hpp"
#include "naive_EA.hpp"
#include "selection_EA.hpp"
#include "parameter_control_EA.h"
#include "island2_EA.h"
#include "parameter_control2_EA.h"


template <typename problem_class, typename solution_class>
void solve_problem(size_t problem_size){
    // Create a random problem
    problem_class problem(100);
    problem.disp();

    for (int i = 0; i < 3; ++i) {
        // Create a solver for the problem
        using solver_t = parameter_control2_EA<problem_class,solution_class>;
        solver_t solver(problem);

        // Choose an algorithm
        enum solver_t::algorithm alg = (i==0) ? solver_t::algorithm::DEFAULT :
                                  (i==1) ? solver_t::algorithm::GA : solver_t::algorithm::EE;
        solver.algorithm(alg);

        // Run!
        solver.run();

        // Print final statistics
        int idx = 1;
        for (auto iter = solver.best_solutions_begin(); iter != solver.best_solutions_end(); ++iter) {
            std::cout << "Solution " << idx << ": " << std::endl;
            (*iter)->disp(problem);
            std::cout << "Objetive function " << idx << ": " << (*iter)->fx << std::endl;
            idx++;
        }
    }

};

int main() {
    const size_t problem_size = 20;
    solve_problem<affective,melodia>(problem_size);

    return 0;
}
