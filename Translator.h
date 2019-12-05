#pragma once
#include "Precompiled.h"

void Terminal_Single_Thread(string_view input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2);
void Terminal_Double_Thread(string_view input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2);

int OperChecker(const char Operator);

bool InfixSyntaxCheker(string_view view);
bool PostfSyntaxCheker(string_view view);
bool PrefiSyntaxCheker(string_view view);

void InfToPost(string_view  _string, unique_ptr<char[]> &ptr);
void InfToPref(string_view  _string, unique_ptr<char[]> &ptr);

void PostToInf(string_view  _string, unique_ptr<char[]> &ptr);
void PostToPref(string_view _string, unique_ptr<char[]> &ptr);

void PrefToPost(string_view _string, unique_ptr<char[]> &ptr);
void PrefToInf(string_view  _string, unique_ptr<char[]> &ptr);

//void PrefToInfMyOwn(string_view _string, unique_ptr<char[]> &ptr);