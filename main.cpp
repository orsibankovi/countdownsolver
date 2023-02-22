#include "woodpecker.hpp"
#include <vector>
#include <stack>
#include "solutioninterface.hpp"
#include "countdownsolver.hpp"

int evaluateReversePolishNotation(const std::vector<CountdownSolutionPiece>& v, std::vector<int> numbers){
    for(CountdownSolutionPiece n : v){
        if(!n.isOperation){
            auto idx = std::find(numbers.begin(), numbers.end(), n.value);
            if(idx == std::end(numbers)) return -1;
            numbers.erase(idx);
        }
    }
    std::stack<CountdownSolutionPiece> stack;
    for (const CountdownSolutionPiece& cd: v) {
        if (cd.isOperation) {
            if(stack.size() < 2) return -1;
            CountdownSolutionPiece r = stack.top();
            stack.pop();
            CountdownSolutionPiece l = stack.top();
            stack.pop();
            if (l.isOperation || r.isOperation) return -1;
            int result = -1;
            switch (cd.op) {
                case ADD:
                    result = l.value+r.value;
                    break;
                case SUB:
                    result = l.value-r.value;
                    break;
                case MUL:
                    result = l.value*r.value;
                    break;
                case DIV:
                    result = l.value/r.value;
                    if( r.value == 0 || l.value % r.value != 0) return -1;
                    break;
            }
            if(result < 0) return -1;
            CountdownSolutionPiece res{false, result, ADD};
            stack.push(res);
        } else {
            stack.push(cd);
        }
    }
    CountdownSolutionPiece result = stack.top();
    stack.pop();
    if(stack.empty() && !result.isOperation) return result.value;
    return -1;
}

TEST("Simple (3 numbers)") {
    std::vector<int> numbers{2, 8, 30}; /// Hint: 7 = (30/2)-8
    int target = 7;
    MEASURE("countdown", 30s){
        std::vector<CountdownSolutionPiece> resultInPolishNotation = solveNumberProblem(numbers, target);
        CHECK_GE(resultInPolishNotation.size(), (size_t) 1);
        int resultNumber = evaluateReversePolishNotation(resultInPolishNotation, numbers);
        CHECK_EQ(resultNumber, target);
    }
}


TEST("Simple (3 numbers) - No exact solution") {
    std::vector<int> numbers{5, 8, 19};
    int target = 9;
    MEASURE("countdown", 30s){
        std::vector<CountdownSolutionPiece> resultInPolishNotation = solveNumberProblem(numbers, target);
        CHECK_GE(resultInPolishNotation.size(), (size_t) 1);
        int resultNumber = evaluateReversePolishNotation(resultInPolishNotation, numbers);
        CHECK_EQ(resultNumber, 8);
    }
}

TEST("4 numbers - No exact solution") {
    std::vector<int> numbers{2, 8, 30,100};
    int target = 29;
    MEASURE("countdown", 30s){
        std::vector<CountdownSolutionPiece> resultInPolishNotation = solveNumberProblem(numbers, target);
        CHECK_GE(resultInPolishNotation.size(), (size_t) 1);
        int resultNumber = evaluateReversePolishNotation(resultInPolishNotation, numbers);
        CHECK_EQ(true, resultNumber == 28 || resultNumber == 30);
    }
}

TEST("4 numbers - 1") {
    std::vector<int> numbers{83, 45, 28, 7}; /// Hint: 42 = 83-45+(28/7)
    int target = 42;
    MEASURE("countdown", 45s){
        std::vector<CountdownSolutionPiece> resultInPolishNotation = solveNumberProblem(numbers, target);
        CHECK_GE(resultInPolishNotation.size(), (size_t) 1);
        int resultNumber = evaluateReversePolishNotation(resultInPolishNotation, numbers);
        CHECK_EQ(resultNumber, target);
    }
}

TEST("4 numbers - Very simple") {
    std::vector<int> numbers{7, 100, 5, 4};
    int target = 7;
    MEASURE("countdown", 45s){
        std::vector<CountdownSolutionPiece> resultInPolishNotation = solveNumberProblem(numbers, target);
        CHECK_GE(resultInPolishNotation.size(), (size_t) 1);
        int resultNumber = evaluateReversePolishNotation(resultInPolishNotation, numbers);
        CHECK_EQ(resultNumber, target);
    }
}

TEST("4 numbers - 2") {
    std::vector<int> numbers{727, 98, 7, 10}; ///Hint 275 = (727+98)/(10-7)
    int target = 275;
    MEASURE("countdown", 45s){
        std::vector<CountdownSolutionPiece> resultInPolishNotation = solveNumberProblem(numbers, target);
        CHECK_GE(resultInPolishNotation.size(), (size_t) 1);
        int resultNumber = evaluateReversePolishNotation(resultInPolishNotation, numbers);
        CHECK_EQ(resultNumber, target);
    }
}

TEST("5 numbers - 1") {
    std::vector<int> numbers{5,100,7,1,1}; /// Hint: 497 = 5*(100-(1+1))+7
    int target = 497;
    MEASURE("countdown", 60s){
        std::vector<CountdownSolutionPiece> resultInPolishNotation = solveNumberProblem(numbers, target);
        CHECK_GE(resultInPolishNotation.size(), (size_t) 1);
        int resultNumber = evaluateReversePolishNotation(resultInPolishNotation, numbers);
        CHECK_EQ(resultNumber, target);
    }
}

TEST("5 numbers - No exact solution") {
    std::vector<int> numbers{75,100,2,3,9};
    int target = 726;
    MEASURE("countdown", 60s){
        std::vector<CountdownSolutionPiece> resultInPolishNotation = solveNumberProblem(numbers, target);
        CHECK_GE(resultInPolishNotation.size(), (size_t) 1);
        int resultNumber = evaluateReversePolishNotation(resultInPolishNotation, numbers);
        CHECK_EQ(resultNumber, 725);
    }
}

TEST("5 numbers - 2") {
    std::vector<int> numbers{75,100,2,1,9};
    int target = 726;
    MEASURE("countdown", 60s){
        std::vector<CountdownSolutionPiece> resultInPolishNotation = solveNumberProblem(numbers, target);
        CHECK_GE(resultInPolishNotation.size(), (size_t) 1);
        int resultNumber = evaluateReversePolishNotation(resultInPolishNotation, numbers);
        CHECK_EQ(resultNumber, target);
    }
}

WOODPECKER_TEST_MAIN()