#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Authentication.h"
#include "sqlite3.h"
#include <stdlib.h>

static int callback(void *data, int argc, char **argv, char **azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}


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

		printf("Your password: ");
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

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char sql[512];
	const char* data = "Callback function called";

	rc = sqlite3_open("bank.db", &db);

	if (rc) 
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(1);
	}

	if (user == ADMIN)
	{
		while (true)
		{
			printf("\nMain Menu\n");
			printf("1) Manage clients\n");
			printf("2) Manage accounts\n");
			printf("Any other character to exit\n");

			char c = getch();

			switch (c)
			{
				case '1':
				{
					printf("Parameters\n");
					printf("1) Show clients info\n");
					printf("2) Change first name\n");
					printf("3) Change last name\n");
					printf("4) Change address\n");
					printf("Any other character to exit\n");

					char p = getch();
					switch (p)
					{
					case '1':
						strcpy(sql, "SELECT * from BANK_CLIENTS");
						break;

					case '2':
					{
						printf("Enter current first name: ");
						char fname[40];
						gets(fname);
						printf("Enter current last name: ");
						char lname[40];
						gets(lname);

						printf("Enter new first name: ");
						char nname[40];
						gets(nname);

						strcpy(sql, "UPDATE BANK_CLIENTS set FIRST_NAME = '");
						strcat(sql, nname);
						strcat(sql, "' where FIRST_NAME = '");
						strcat(sql, fname);
						strcat(sql, "' and LAST_NAME = '");
						strcat(sql, lname);
						strcat(sql, "';");
					}
					break;

					case '3':
					{
						printf("Enter current first name: ");
						char fname[40];
						gets(fname);
						printf("Enter current last name: ");
						char lname[40];
						gets(lname);

						printf("Enter new last name: ");
						char nname[40];
						gets(nname);

						strcpy(sql, "UPDATE BANK_CLIENTS set LAST_NAME = '");
						strcat(sql, nname);
						strcat(sql, "' where FIRST_NAME = '");
						strcat(sql, fname);
						strcat(sql, "' and LAST_NAME = '");
						strcat(sql, lname);
						strcat(sql, "';");
					}
					break;

					case '4':
					{
						printf("Enter current first name: ");
						char fname[40];
						gets(fname);
						printf("Enter current last name: ");
						char lname[40];
						gets(lname);

						printf("Enter new address: ");
						char naddr[40];
						gets(naddr);

						strcpy(sql, "UPDATE BANK_CLIENTS set ADDRESS = '");
						strcat(sql, naddr);
						strcat(sql, "' where FIRST_NAME = '");
						strcat(sql, fname);
						strcat(sql, "' and LAST_NAME = '");
						strcat(sql, lname);
						strcat(sql, "';");
					}
					break;

					default:
						sqlite3_close(db);
						return 1;
						break;
					}
				}
				break;


				case'2':
					printf("Parameters\n");
					printf("1) Show accounts info\n");
					printf("2) Change account type\n");
					printf("3) Change overdraft term\n");
					printf("4) Change max transactions number\n");
					printf("Any other character to exit\n");

					char p = getch();

					switch (p)
					{
					case '1':
					{
						strcpy(sql, "SELECT BANK_ACCOUNTS.ID, BANK_CLIENTS.FIRST_NAME, BANK_CLIENTS.LAST_NAME,\
							BANK_ACCOUNTS.TYPE, BANK_ACCOUNTS.SUM from BANK_ACCOUNTS,\
							BANK_CLIENTS WHERE BANK_CLIENTS.ID = BANK_ACCOUNTS.CLIENT_ID");
					}
					break;

					case '2':
					{
						printf("Enter account id: ");
						char id[20];
						gets(id);
						
						char d;
						do
						{
							printf("Choose new account type:\n");
							printf("1) current\n");
							printf("2) savings\n");

							d = getch();
						} while (d != '1' && d != '2');

						char type[15];

						if (d == '1')
							strcpy(type, "current");
						else
							strcpy(type, "savings");

						strcpy(sql, "UPDATE BANK_ACCOUNTS SET TYPE = '");
						strcat(sql, type);
						strcat(sql, "' WHERE ID = ");
						strcat(sql, id);
						strcat(sql, ";");
					}
					break;

					case '3':
					{
						printf("Enter account id: ");
						char id[20];
						gets(id);

						int term = 35;
						do
						{
							printf("Enter new overdraft term (<= 31 days): ");
							scanf("%d", &term);
						} while (term > 31);

						char sterm[10];
						sprintf(sterm, "%d", term);

						strcpy(sql, "UPDATE BANK_CONFIG SET OVERDRAFT_ACC_TERM = ");
						strcat(sql, sterm);
						strcat(sql, " WHERE BANK_ACCOUNTS_ACC_ID = ");
						strcat(sql, id);
						strcat(sql, ";");
					}
					break;

					case '4':
					{
						printf("Enter account id: ");
						char id[20];
						gets(id);

						int maxt = 10;

						printf("Enter new max transactions number: ");
						scanf("%d", &maxt);

						char smaxt[10];
						sprintf(smaxt, "%d", maxt);

						strcpy(sql, "UPDATE BANK_CONFIG SET MAX_TRANSACTIONS = ");
						strcat(sql, smaxt);
						strcat(sql, " WHERE BANK_ACCOUNTS_ACC_ID = ");
						strcat(sql, id);
						strcat(sql, ";");
					}
					break;

					default:
					{
						sqlite3_close(db);
						return 1;
					}
					break;
					}
				break;


				default:
					sqlite3_close(db);
					return 1;
				break;
			}

			rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
			if (rc != SQLITE_OK) 
			{
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			else 
			{
				fprintf(stdout, "Operation done successfully\n");
			}
		}
	}
	else if (user == OPER)
	{
		while (true)
		{
			printf("\nMain Menu\n");
			printf("1) Credit\n");
			printf("2) Debit\n");
			printf("3) Transaction\n");
			printf("4) Transaction percent\n");

			char c = getch();

			switch (c)
			{
				case '1':
				{
					printf("Enter account id");
					char id[20];
					char ID[20];
					gets(id);
					strcpy(ID, id);
					strcpy(sql, "select BANK_ACCOUNTS.SUM from BANK_ACCOUNTS where ID = ");
					strcat(sql, id);
					strcat(sql, ";");
					sqlite3_stmt *pStmt;
					rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "Failed to prepare statement\n");
						fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
						sqlite3_close(db);
						return 1;
					}
					rc = sqlite3_step(pStmt);
					double sum = sqlite3_column_double(pStmt, 0);
					printf("Enter credit\n");
					gets(id);
					sum += atof(id);
					sprintf(id, "%f", sum);
					strcpy(sql, "UPDATE BANK_ACCOUNTS SET SUM = '");
					strcat(sql, id);
					strcat(sql, "' WHERE ID = ");
					strcat(sql, ID);
					strcat(sql, ";");

				}
				break;
				case '2':
				{
					printf("Enter account id");
					char id[20];
					char ID[20];
					char debit[20];
					gets(id);
					strcpy(ID, id);
					strcpy(sql, "select BANK_ACCOUNTS.SUM from BANK_ACCOUNTS where ID = ");
					strcat(sql, id);
					strcat(sql, ";");
					sqlite3_stmt *pStmt;
					rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "Failed to prepare statement\n");
						fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
						sqlite3_close(db);
						return 1;
					}
					rc = sqlite3_step(pStmt);
					double sum = sqlite3_column_double(pStmt, 0);
					printf("Enter debit\n");
					gets(debit);
					double scr = atof(debit);
					sum -= scr;
					if (sum < 0)
					{
						printf("Limit is exceeded\n");
						break;
					}
					sprintf(debit, "%f", sum);
					strcpy(sql, "UPDATE BANK_ACCOUNTS SET SUM = '");
					strcat(sql, debit);
					strcat(sql, "' WHERE ID = ");
					strcat(sql, ID);
					strcat(sql, ";");
				
				}
				break;
				case '3':
				{
					printf("Enter account id - source");
					char id[20];
					char ID[20];
					gets(id);
					strcpy(ID, id);
					strcpy(sql, "select BANK_ACCOUNTS.SUM from BANK_ACCOUNTS where ID = ");
					strcat(sql, id);
					strcat(sql, ";");
					sqlite3_stmt *pStmt;
					rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "Failed to prepare statement\n");
						fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
						sqlite3_close(db);
						return 1;
					}
					rc = sqlite3_step(pStmt);
					double sum = sqlite3_column_double(pStmt, 0);
					printf("Enter sum\n");
					char cSum[20];
					char destSum[20];
					gets(cSum);
					strcpy(destSum, cSum);
					sum -= atof(cSum);
					if (sum < 0)
					{
						printf("Limit is exceeded\n");
						break;
					}
					sprintf(cSum, "%f", sum);
					strcpy(sql, "UPDATE BANK_ACCOUNTS SET SUM = '");
					strcat(sql, cSum);
					strcat(sql, "' WHERE ID = ");
					strcat(sql, ID);
					strcat(sql, ";");
					char updSource[200] ;
					strcpy(updSource, sql);
					printf("Enter account destination id");
					gets(id);
					strcpy(ID, id);
					strcpy(sql, "select BANK_ACCOUNTS.SUM from BANK_ACCOUNTS where ID = ");
					strcat(sql, id);
					strcat(sql, ";");
					rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "Failed to prepare statement\n");
						fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
						sqlite3_close(db);
						return 1;
					}
					rc = sqlite3_step(pStmt);
					sum = sqlite3_column_double(pStmt, 0);
					sum += atof(destSum);
					sprintf(id, "%f", sum);
					strcpy(sql, "UPDATE BANK_ACCOUNTS SET SUM = '");
					strcat(sql, id);
					strcat(sql, "' WHERE ID = ");
					strcat(sql, ID);
					strcat(sql, "; ");
				//	rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
					strcat(sql, updSource);
				}
				break;
				case '4':
				{
					printf("Enter account id - source");
					char id[20];
					char ID[20];
					gets(id);
					strcpy(ID, id);
					strcpy(sql, "select BANK_ACCOUNTS.SUM,BANK_CONFIG.PERCENT from BANK_ACCOUNTS,BANK_CONFIG where BANK_ACCOUNTS.ID = ");
					strcat(sql, id);
					strcat(sql, ";");
					sqlite3_stmt *pStmt;
					rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "Failed to prepare statement\n");
						fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
						sqlite3_close(db);
						return 1;
					}
					rc = sqlite3_step(pStmt);
					double sum = sqlite3_column_double(pStmt, 0);
					double percentage = (double)sqlite3_column_int(pStmt,1)/100 +1;
					double percSum = (double)sum / percentage;
					double diff = sum - percSum;
					if (sum < 0)
					{
						printf("Limit is exceeded\n");
						break;
					}
					char cSum[20];
					sprintf(cSum, "%f", percSum);
					strcpy(sql, "UPDATE BANK_ACCOUNTS SET SUM = '");
					strcat(sql, cSum);
					strcat(sql, "' WHERE ID = ");
					strcat(sql, ID);
					strcat(sql, "; ");
					char updSource[200];
					strcpy(updSource, sql);
					printf("Enter account destination id");
					gets(id);
					strcpy(sql, "select BANK_ACCOUNTS.SUM from BANK_ACCOUNTS where ID = ");
					strcat(sql, id);
					strcpy(ID, id);
					strcat(sql, ";");
					rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
					if (rc != SQLITE_OK) {
						fprintf(stderr, "Failed to prepare statement\n");
						fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
						sqlite3_close(db);
						return 1;
					}
					rc = sqlite3_step(pStmt);
					sum = sqlite3_column_double(pStmt, 0);
					sum += diff;
					sprintf(id, "%f", sum);
					strcpy(sql, "UPDATE BANK_ACCOUNTS SET SUM = '");
					strcat(sql, id);
					strcat(sql, "' WHERE ID = ");
					strcat(sql, ID);
					strcat(sql, "; ");
					strcat(sql, updSource);
				}
				break;
				default:
					sqlite3_close(db);
					return 1;
					break;
			}
			rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
			if (rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			else
			{
				fprintf(stdout, "Operation done successfully\n");
			}
		}
	}

	sqlite3_close(db);
	return(0);
}