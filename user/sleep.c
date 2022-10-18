#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void main(int argc, char *argv[]){
	if(argc != 2){
		exit(-1);
	}
	else{
		printf("%s", "pls wait...\n");
		sleep(atoi(argv[1]));
		exit(0);
	}//printf("%d%d", argc, atoi(argv[1]));
}
