
#include "melodia.h"
#include <cstdlib>

std::default_random_engine melodia::_generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

melodia::melodia(affective &p)
 {
    //std::iota(_melodia.begin(), _affective.end(), 0);
    std::uniform_int_distribution<int> pitch(-5, 15);
    std::uniform_int_distribution<int> duration(0,4);
    std::vector<int> _duracoes ={1,2,4,8,16};

    //int r = *select_randomly(foo.begin(), foo.end());
    /*
    duration
    Note Duration Integer Used
    whole note 0
    dotted half 1
    half 2
    dotted quarter 3
    quarter 4
    dotted eigth 5
    eigth 6
    sixteenth 7
    */
    int duracao = 16;

    //std::vector<int> _escala{1, 3, 5, 6, 8, 10, 12, 13};

     for(int i=0; i < p.size(); ++i){
        divideNota(p.getArousal(), duracao);
    }

    for (int i = 0; i < _melodia.size(); ++i) {
        this->_melodia[i].first = pitch(this->_generator);
        //this->_melodia[i].second = _duracoes[duration(this->_generator)];
    }

    //std::shuffle(_melodia.begin(), _melodia.end(), this->_generator);
}

void melodia::disp(affective &p) {
    std::cout << "----------------------Solution---------------------" << std::endl;
    int compasso = 0;
    for(int i=0; i<_melodia.size(); ++i){
        std::cout << "" << this->_melodia[i].first << "/";
        std::cout << "" << this->_melodia[i].second << "\t";
        compasso += _melodia[i].second;
        if(compasso == 16){
            compasso = 0;
            std::cout << "\n";
        }
    }
    std::cout << "----------------------------------------------------\n";

}


void melodia::divideNota(double arousal, int duracao){
    std::uniform_real_distribution<double> d(0, 20);
    chanceDividir = d(this->_generator);
    if(chanceDividir > duracao || duracao == 1){
        _melodia.push_back(std::make_pair(0,duracao));
    }
    else{
         divideNota(arousal, duracao/2);
         divideNota(arousal, duracao/2) ;

    }
}


//----------------------------------------------------------------------
//                          AVALIA A MELODIA
//----------------------------------------------------------------------

int melodia::evaluate(affective &p) {
    //std::vector<int> _escala{1, 3, 5, 6, 8, 10, 12, 13};//escala maior
    std::vector<int>::iterator it;

    //Metricas Avaliação Melodia Comum
    /*
1. SAME NOTE: Fitness Points: 17. The scale degree of the next note has not
changed.
2. ONE STEP: Fitness Points: 17. The scale degree of the next note has gone up
or down one step.
3. ONE THIRD: Fitness Points: 15. The scale degree of the next note has gone up
or down two steps.
4. ONE FOURTH: Fitness Points: 12. The scale degree of the next note has gone
up or down three steps.
5. ONE FIFTH: Fitness Points: 10. The scale degree of the next note has gone up
or down four steps.
6. OVER FIFTH: Fitness Points: -25. The scale degree of the next note is greater
than four steps away.
7. FOUR SEVEN: Fitness Points: -25. The current note is scale degree four and
the next note is scale degree seven.
8. SIXTEENTH NOTE: Fitness Points: -10. The current note is a sixteenth note.
9. DRASTIC DURATION CHANGE: Fitness Points: -20. The duration change
between the current note and the next note is more than four steps in table 1.
10. BEGIN TONIC: Fitness Points: 50. The melody begins with the tonic note
(scale degree 1).
11. END TONIC: Fitness Points: 50. The melody ends in the tonic note (scale
degree 1).
*/

//Metricas relacionadas a arousal valencia
/*

*/


int total = 0;
    /*
    //Dentro da escala
    for (int i = 0; i < p.size() - 1; ++i) {
        it = find(_escala.begin(), _escala.end(), this->_melodia[i]);
        if ( it != _escala.end()){
            total++;
        }
    }
    */
int compasso = _melodia[0].second;
int numNotasCompassoAnterior =0;
int numNotasCompassoAtual =0;

    //Avaliações referentes a nota seguinte
    for (int i = 1; i < p.size(); ++i) {
        int difPitch = std::abs(_melodia[i].first - _melodia[i-1].first); //diferenca entre notas
        int difDur = std::abs(_melodia[i].second - _melodia[i-1].second); //diferenca entre notas


        if (difPitch <= 4){
            total+=50 + p.getArousal()*-10;
            if (difPitch <= 3) total += 20 + p.getArousal()*-10;
            if (difPitch <= 2) total += 30 + p.getArousal()*-10;
            if (difPitch <= 1) total += 20 + p.getArousal()*-10;
        }
        else{
            total -=30 + p.getArousal()*difPitch*10;
        }

        //nota muito curta
        if (_melodia[i].second == 1 || _melodia[i].second == 16)
            total -= 50;

        /*if(_melodia[i].second == _melodia[i-1].second)
                total += 100 + p.getValencia()*-100;
        */
        //tritono
        if(_melodia[i-1].first == 4 && _melodia[i].first == 7)
            total -= 100;


        //mudança drastica de duração
        if(difDur > 4)
            total -=50;

        if(_melodia[i].second == 0){
            total = (p.getArousal()*-10)*_melodia[i].second;
        }

        //comeca na tonica
        if(numNotasCompassoAtual == 0 && (_melodia[0].first == 1 || _melodia[i].first == 0 || _melodia[i].first == 0))
            total += 50+10*p.getValencia();


        numNotasCompassoAtual ++;
        compasso += _melodia[i].second;


        if(compasso == 16){
            if(numNotasCompassoAnterior !=0){
                total += std::abs(numNotasCompassoAnterior - numNotasCompassoAtual)*p.getValencia()*-10;
            }
            numNotasCompassoAnterior = numNotasCompassoAtual;
            numNotasCompassoAtual = 0;
            compasso = 0;


            //termina na tonica
            if(_melodia[i].first == 1 || _melodia[i].first == 8 || _melodia[i].first == 15)
                total += 10+10*p.getValencia();
        }

    }



    //Ultima nota maior que a primeira

    if ( _melodia[p.size() - 1] > _melodia[0]){
        total = total + p.getArousal()*100;//premia arousal positivo, desconta arousal negativo
    }
    else    {
        total = total + p.getArousal()*-100; //premia arousal negativo, desconta arousal positivo
    }


    //total = total/_melodia.size();

    return total;
}



void melodia::mutation(affective &p, double mutation_strength) {
    std::uniform_int_distribution<int> pos(0.0,1.0);
    std::uniform_int_distribution<size_t> d(0,p.size()-1);
    for (int i = 0; i < (mutation_strength / 2) * p.size(); ++i) {
        //altera uma das notas em +1 ou -1
        size_t posPitchMuta = d(_generator);
        size_t posDurMuta = d(_generator);

        if((d(_generator)%2 == 0 || _melodia[posPitchMuta].first == 0) && _melodia[posPitchMuta].first != 15)
            this->_melodia[posPitchMuta].first += 1;
        else
            this->_melodia[posPitchMuta].first -= 1;

        //divideNota(p.getArousal(), _melodia[posDurMuta].second);
        /*
        if((d(_generator)%2 == 0 || _melodia[posDurMuta].second == 0) && _melodia[posDurMuta].second != 7)
            .second += 1;
        else
            this->_melodia[posDurMuta].second -= 1;
        */
    }



    // Smart approach:
    // std::binomial_distribution<double> d(p.size(), mutation_strength)
    // int n = d(_generator)
    // flip $n$ bits
}

melodia melodia::crossover(affective &p, melodia &rhs) {
    std::uniform_int_distribution<int> d(0,1);
    melodia child(p);
    for (int i = 0; i < p.size(); ++i) {
        if (d(_generator)<p.size()/2) {
            child._melodia[i].first = this->_melodia[i].first;

        } else {
            child._melodia[i].first = rhs._melodia[i].first;
        }
    }
    return child;
}





