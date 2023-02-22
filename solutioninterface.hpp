#ifndef SOLUTIONINTERFACE_HPP
#define SOLUTIONINTERFACE_HPP

enum OP{ADD, SUB, DIV, MUL};

struct CountdownSolutionPiece{
    bool isOperation;
    int value;
    OP op;
};

#endif //SOLUTIONINTERFACE_HPP
