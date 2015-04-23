//Moonder V.1 (22.03.14) By: Kevin Huynh (currently)



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//------------------------------------- Global Variables -----------------------------------------**
void get_usr(void);										// gets info from file and puts name + details into user struct
char *profile;											// just a pointer to get file name (is used like once but is necessary)
struct profile_msgbuf user;								// contains name and details for user
//------------------------------------------------------------------------------------------------**

struct profile_msgbuf {
    long mtype;  /* must be positive */
    struct profile_info {
        char usr_name[100];
        char details[300];
		int status;			// Status being 0 = unmatched | 1 = matched | 2 = liked
    } pinfo;
};

int main(int argc, char *argv[])
{
    if (argc == 1 || argc > 2) {
		printf("ERROR: Please input one profile file (including its extension)\n");
		exit(1);
	}
	profile =(char *) malloc(strlen(argv[1])+1);
	strcpy(profile, argv[1]);
	//----------------------- Above are file ops so don't worry too much --------------------------**

	get_usr();
	
	int size = sizeof(struct profile_msgbuf) - sizeof(long);
	user.mtype = 1;
	user.pinfo.status = 0;
	
	long key = 1108488947;
	int msqid;
	if ((msqid = msgget(key, 0666)) == -1) {
        perror("msgget");
        exit(1);
    }
	msgsnd(msqid, &user, size, 0);
	
	printf("Welcome to Moonder, %s. Perhaps you want to know:\n", user.pinfo.usr_name);
	
	
	free(profile);
	return 0;
}

void get_usr(void)
{
	FILE * inp ;
	if ((inp = fopen(profile, "r")) == NULL)
	{
		perror("fopen");
		exit(1);
	}
	fgets(user.pinfo.usr_name,sizeof user.pinfo.usr_name, inp);
    if (user.pinfo.usr_name[strlen(user.pinfo.usr_name)-1] == '\n') user.pinfo.usr_name[strlen(user.pinfo.usr_name)-1] = '\0';
	fgets(user.pinfo.details,sizeof user.pinfo.details, inp);
}