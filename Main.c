
#include <stdio.h>
#include "Authentication.h"

int main(int argc, char* argv[])
{
	puts("*** Bank application ***");
	puts("");

	char login[40], password[40];
	enum role user = OTHER;
	
	for (int i = 0; i < 3; i++)
	{
		printf("Your login: ");
		gets(login);

		printf("Your password: %s", login);
		gets(password);

		if (checkLogin(login) == ADMIN &&
			checkPassword(password) == ADMIN)
		{
			user = ADMIN;
			break;
		}
		else if (checkLogin(login) == OPER &&
			checkPassword(password) == OPER)
		{
			user = OPER;
			break;
		}
		else
		{
			printf("Try again. %d tries left\n", 2 - i);
		}
	}

	if (user == OTHER)
	{
		puts("Access denied. Exit");
		return 1;
	}


}