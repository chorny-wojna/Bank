#pragma once

#include <stdbool.h>

#define HASHMAX			39883
#define ADMIN_LOGIN		32364	//admin
#define ADMIN_PASSWORD	32364	//admin
#define OPER_LOGIN		24908	//oper
#define OPER_PASSWORD	24908	//oper

enum role {ADMIN, OPER, OTHER};

int checkLogin(const char* login);

int checkPassword(const char* password);

unsigned int hashData(const char* line);