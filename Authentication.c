#include "Authentication.h"

int checkLogin(const char* login)
{
	if (hashData(login) == ADMIN_LOGIN)
		return ADMIN;

	if (hashData(login) == OPER_LOGIN)
		return OPER;

	return OTHER;
}

int checkPassword(const char* password)
{
	if (hashData(password) == ADMIN_PASSWORD)
		return ADMIN;

	if (hashData(password) == OPER_PASSWORD)
		return OPER;

	return OTHER;
}

unsigned int hashData(const char* line)
{
	unsigned int hash = 0;

	for (; *line; line++)
		hash = (hash * 1664525) + (unsigned char)(*line) + 1013904223;

	return hash % HASHMAX;
}