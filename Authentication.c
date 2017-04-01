#include "Authentication.h"

bool checkLogin(const char* login)
{
	return (hashData(login) == LOGIN);
}

bool checkPassword(const char* password)
{
	return (hashData(password) == PASSWORD);
}

unsigned int hashData(const char* line)
{
	unsigned int hash = 0;

	for (; *line; line++)
		hash = (hash * 1664525) + (unsigned char)(*line) + 1013904223;

	return hash % HASHMAX;
}