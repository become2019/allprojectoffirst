#include <stdio.h>

typedef struct grade {
	int time;
	char name[10];
}Grade;

int main(){
	FILE * fp;
	Grade tmp[4];
	fp = fopen("grade", "r");
	for(int i = 0; i < 4; i++){
		fread(&tmp[i], sizeof(Grade), 1, fp);
		printf("%d %s\n", tmp[i].time, tmp[i].name);
	}
	return 0;
}