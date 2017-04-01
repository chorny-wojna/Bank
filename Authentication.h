#pragma once

#include <stdbool.h>

#define HASHMAX		39883
#define LOGIN		32364
#define PASSWORD	32364

bool checkLogin(char* login);

bool checkPassword(char* password);

unsigned int hashData(const char* line);