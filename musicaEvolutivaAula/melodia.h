
#ifndef EVOLUTIONARY_COMPUTATION_MELODIA_H
#define EVOLUTIONARY_COMPUTATION_MELODIA_H

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

#include "affective.h"

class melodia {
public:
    melodia(affective &p);
    void disp(affective &p);
    int evaluate(affective &p);
    void mutation(affective &p, double mutation_strength);
    void divideNota(double arousal, int duracao);

    melodia crossover(affective &p, melodia& rhs);
    double fx;
    double fitness;

    int MENOR_NOTA = 0;
    int MAIOR_NOTA = 15;

std::vector<std::pair <int, int> > _melodia;
private:
    double chanceDividir;
    size_t qtdNotas;
    static std::default_random_engine _generator;
};


#endif //EVOLUTIONARY_COMPUTATION_MELODIA_H


