#ifndef NHF3SKELETON_COUNTDOWNSOLVER_HPP
#define NHF3SKELETON_COUNTDOWNSOLVER_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include "solutioninterface.hpp"

CountdownSolutionPiece Cases(char op){
    CountdownSolutionPiece piece;
    piece.isOperation = 1;

    if (op == '+') {
        piece.op = ADD;
    } else if (op == '-') {
        piece.op = SUB;
    } else if (op == '*') {
        piece.op = MUL;
    } else if (op == ':') {
        piece.op = DIV;
    }
    return piece;
}

int Operation(int n1, int n2, char op){
    int result;
    if (op == '+') {
        result = n1 + n2;
    } else if (op == '-' and n1 > n2) {
        result = n1 - n2;
    } else if (op == '*') {
        result = n1 * n2;
    } else if (op == ':' and n2 != 0 and n1%n2 == 0) {
        result = n1 / n2;
    }
    else {result = -10;}

    return result;
}

std::pair<std::vector<CountdownSolutionPiece>, int> brackets(std::vector<int> numbers_, std::vector<char> operations, char last, int sum, const int &target, int min_sum){
    int result = min_sum;
    int min_ = -2;
    std::vector<CountdownSolutionPiece> vect = {};
    CountdownSolutionPiece piece;
    piece.value = numbers_.back(); //vektorokat hasznalok verem megvalositashoz
    piece.isOperation = 0;
    vect.push_back(piece);
    std::vector<CountdownSolutionPiece> vect_min = vect;

    while (!operations.empty()) {
        if (numbers_.size() >= 2) {
            int subresult = Operation(numbers_[numbers_.size() - 1], numbers_[numbers_.size() - 2], operations.back());

            if (subresult == -10){ //invalid operation
                std::pair<std::vector<CountdownSolutionPiece>, int> v = std::make_pair(vect_min, -2);
                return v;
            }

            CountdownSolutionPiece piece_op;
            piece_op = Cases(operations.back());

            piece.value = numbers_[numbers_.size() - 2];
            vect.push_back(piece);
            vect.push_back(piece_op);

            if (abs(target-(Operation(sum, subresult, last))) < abs(target - min_sum)){
                vect_min = vect;
                result = Operation(sum, subresult, last);
                piece_op = Cases(last);
                min_ = result;
                vect_min.push_back(piece_op);
                std::pair<std::vector<CountdownSolutionPiece>, int> v = std::make_pair(vect_min, min_);
                return v;
            }
            numbers_.pop_back(); //kivesszul a "verembol"
            numbers_.pop_back();
            numbers_.push_back(sum); //visszarakjuk az eddigi muvelet/ek eredmenyet
            operations.pop_back();
        }
    }
    std::pair<std::vector<CountdownSolutionPiece>, int> v = std::make_pair(vect_min, -2); //ha nem talaltunk az eddiginel jobb megoldast, invalid ertekkel terunk vissza
    return v;
}

std::pair<std::vector<CountdownSolutionPiece>, int> VariationResult(std::vector<int> numbers_, std::vector<char> operations, const int &target_){ //visszaadja az adott permutacio a minimalis elterest eredmenyezo muveleti sorrendu kifejezest
    int res_ = numbers_.back(); //vektorokat hasznalok verem megvalositashoz
    int min_ = res_;
    std::vector<CountdownSolutionPiece> vect = {};
    CountdownSolutionPiece piece;
    piece.value = numbers_.back();
    piece.isOperation = 0;
    vect.push_back(piece);
    std::vector<CountdownSolutionPiece> vect_min = vect;

    while (!operations.empty()) {
        if (numbers_.size() >= 2) {
            CountdownSolutionPiece piece_op;
            piece_op = Cases(operations.back());
            int osszeg = Operation(numbers_[numbers_.size() - 1], numbers_[numbers_.size() - 2], operations.back());

            if (osszeg == -10){ //invalid operation, addigi minimalis elteresuvel terunk vissza
                std::pair<std::vector<CountdownSolutionPiece>, int> v = std::make_pair(vect_min, min_);
                return v;
            }

            piece.value = numbers_[numbers_.size() - 2];
            res_ = osszeg;
            vect.push_back(piece);
            vect.push_back(piece_op);

            if (abs(target_ - min_) > abs(target_ - osszeg)) {
                min_ = res_;
                vect_min = vect;
                if (min_ == target_){ //ha elertuk a targatet, nem nezzuk tovabb
                    std::pair<std::vector<CountdownSolutionPiece>, int> v = std::make_pair(vect_min, min_);
                    return v;
                }
            }
            numbers_.pop_back(); //kivesszul a "verembol"
            numbers_.pop_back();
            operations.pop_back();

            if (numbers_.size()>=2){ //zarojelezes hianyanak kikuszobolesere megnezzuk a maradek tagokra is eloszor a reszeredmenyt, ha kisebb az elteres a targettol, elmentjuk
                std::vector<CountdownSolutionPiece> aktual = vect;
                std::vector<int> num = numbers_;
                std::vector<char> muv = operations;
                char last = muv.back();
                muv.pop_back();

                std::pair<std::vector<CountdownSolutionPiece>, int> brakets_ = brackets(num, muv, last, osszeg, target_, min_);

                if (brakets_.second != -2){
                    for (std::size_t s = 0; s<brakets_.first.size(); s++){
                        aktual.push_back(brakets_.first[s]);
                    }
                    vect_min = aktual;
                    min_ = brakets_.second;
                    //std::cout << min_ << std::endl;
                }
            }
            numbers_.push_back(osszeg); //visszarakjuk a "verembe" az addigi muvelet/ek eredmenyet
        } else { //vegigertunk a vektorokon, csak a vegeredmeny van a vektorban
            res_ = numbers_.back();
            if (abs(target_ - min_) > abs(target_ - numbers_.back())) { //minimalis elteresu frissitese
                min_ = res_;
                vect_min = vect;
                if (min_ == target_){ //ha elertuk a targatet, nem nezzuk tovabb
                    std::pair<std::vector<CountdownSolutionPiece>, int> v = std::make_pair(vect_min, min_);
                    return v;
                }
            }
        }
    }
    std::pair<std::vector<CountdownSolutionPiece>, int> v = std::make_pair(vect_min, min_);
    return v;
}

std::pair<std::vector<CountdownSolutionPiece>, int> PermutationResult(std::vector<int> numbers, const int &target_){ //visszaadja az adott permutaciora a minimalis elterest eredmenyezo muveleti sorrendu kifejezest
    std::vector<char> ops = {'+', '-', '*', ':'};
    std::vector<char> operations = {}; //maga a muveleti sorrend
    std::vector<int> VariationNum = {}; //szamoljuk, hogy hanyfele muveleti jel/csere volt mar az adott kombinacioban, ha az osszes helyen 4 az ertek, akkor vagyunk kesz
    std::pair<std::vector<CountdownSolutionPiece>, int> res_;
    std::pair<std::vector<CountdownSolutionPiece>, int> min_;

    int i = 0;
    while (i < pow(4, numbers.size()-1)){
        for (int j = 0; j<4; j++){ //mindig csak az utolso muveleti jelet cserelgetjuk a for ciklusban, ha mar az osszes kombinacio volt, "felbuborekoltatjuk"
            if (operations.empty()){

                for(std::size_t k = 0; k<numbers.size()-1; k++){ //kezdoertek: ++...+
                    operations.push_back('+');
                    VariationNum.push_back(1);
                }
                res_ = VariationResult(numbers, operations, target_);
                if (abs(target_-res_.second) < abs(target_-min_.second) or (res_.second == min_.second and res_.first.size() < min_.first.size())){
                    min_ = res_;
                    if (min_.second == target_){ //ha elertuk a targatet, nem nezzuk tovabb
                        return min_;
                    }
                }
                i++;
            } else {
                if (VariationNum.back() == 4 ){ //ops.size() == 4
                    //addig visszalepunk, amig nem talalunk olyan elemet, amelynek nem 4 az erteke, azaz nem vizsgaltuk meg az osszes utana levo jel kombinaciojat
                    std::size_t k = VariationNum.size()-1;
                    while (VariationNum[k] == 4){ //ops.size() == 4
                        operations.pop_back();
                        VariationNum.pop_back();
                        k--;
                    }
                    operations.back() = ops[VariationNum.back()];  //atallitjuk a kovetkezo operatorra a legutolso olyan elemet, amelynek meg valamely lehetoseget nem neztuk
                    VariationNum.back()++;

                    for(std::size_t l = operations.size(); l < numbers.size() - 1; l++){
                        operations.push_back('+');
                        VariationNum.push_back(1);
                    }
                    i++;
                    res_ = VariationResult(numbers, operations, target_);
                    if (abs(target_-res_.second) < abs(target_-min_.second)){
                        min_ = res_;
                        if (min_.second == target_){ //ha elertuk a targatet, nem nezzuk tovabb
                            return min_;
                        }
                    }
                }
                else {
                    operations.back() = ops[VariationNum.back()];
                    VariationNum.back()++;
                    i++;
                    res_ = VariationResult(numbers, operations, target_);
                    if (abs(target_-res_.second) < abs(target_-min_.second)){
                        min_ = res_;
                        if (min_.second == target_){ //ha elertuk a targatet, nem nezzuk tovabb
                            return min_;
                        }
                    }
                }
            }
        }
    }
    return min_;
}

std::vector<CountdownSolutionPiece> solveNumberProblem(std::vector<int> numbers, int target){
    std::vector<CountdownSolutionPiece> vect_ = {};
    std::pair<std::vector<CountdownSolutionPiece>, int> min_ = std::make_pair(vect_, -1);

    std::sort(numbers.begin(), numbers.end());
    do {
std::pair<std::vector<CountdownSolutionPiece>, int> result = PermutationResult(numbers, target);
        if (min_.second == -1 or abs(target-result.second) < abs(target-min_.second)){
            min_ = result;
        }

        if (min_.second == target){
            //std::cout << min_.second << std::endl;
            return min_.first;
        }
    } while (std::next_permutation(numbers.begin(), numbers.end())); //vegigmegy az osszes permutacion

    return min_.first;
}

#endif //NHF3SKELETON_COUNTDOWNSOLVER_HPP
