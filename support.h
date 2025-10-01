#ifndef SUPPORT_H
#define SUPPORT_H

#include "provided.h"

bool checkVertical(const Scaffold& s, int N, int color);

bool checkHorizontal(const Scaffold& s, int N, int color);

bool checkDiagonal1(const Scaffold& s, int N, int color);

bool checkDiagonal2(const Scaffold& s, int N, int color);

bool checkAll(const Scaffold& s, int N, int color);

int rating(const Scaffold& s, int N, int color);

#endif