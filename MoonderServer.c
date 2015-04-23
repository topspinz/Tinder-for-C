// MoonderServer V.1 (22.03.14) By: Kevin Huynh (currently)

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct profile_msgbuf {
    long mtype;  /* must be positive */
    struct profile_info {
        char usr_name[100];
        char details[300];
		int status;			// Status being 0 = unmatched | 1 = matched | 2 = liked
    } pinfo;
};

int main(void)
{
	long key = 1108488947;
	int msqid;
	struct profile_msgbuf person;
	int size = sizeof(struct profile_msgbuf) - sizeof(long);
	if ((msqid = msgget(key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
	
	while (person.pinfo.status < 0)
	{
		if (msgrcv(msqid, &person, size, 1, 0) == -1){
		perror("msgrcv");
		exit(1);
		}
		printf("%s\n", person.pinfo.details);
	}
	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }
	
}