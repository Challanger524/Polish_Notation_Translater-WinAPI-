#pragma once
#include "Precompiled.h"

bool Terminal_Single_Thread(const char *input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2);
bool Terminal_Double_Thread(const char *input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2);

int OperChecker(const char Operator);

bool InfixSyntaxCheker(const char *, const size_t);
bool PostfSyntaxCheker(const char *, const size_t);
bool PrefiSyntaxCheker(const char *, const size_t);

void InfToPost(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);
void InfToPref(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);

void PostToInf(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);
void PostToPref(const char*_string, unique_ptr<char[]> &ptr, const size_t _str_size);

void PrefToPost(const char*_string, unique_ptr<char[]> &ptr, const size_t _str_size);
void PrefToInf(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);

//void PrefToInfMyOwn(const char *_string, unique_ptr<char[]> &ptr, const size_t _str_size);