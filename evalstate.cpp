#include "evalstate.h"

EvalState::EvalState() {}

EvalState::~EvalState() {}

void EvalState::setValue(QString var, int value) { symbolTable[var] = value; }

int EvalState::getValue(QString var) { return symbolTable[var]; }

bool EvalState::isDefined(QString var) { return symbolTable.contains(var); }

void EvalState::clear() { symbolTable.clear(); }

QMap<QString, int>* EvalState::getSymbolTable() { return &symbolTable; }
