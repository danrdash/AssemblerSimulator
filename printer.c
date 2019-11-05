//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#define MAXCHAR 500
//#define MEM 2048
//#pragma warning(disable:4996)
//
//
//
//int main(int argc, const char * argv[]) {
//	FILE *fp = NULL, *fpmem = NULL;
//	int x = 0, counter=0, convert=0;
//	char f= 'c', memo[MEM][10], str[MAXCHAR], *ch=NULL;
//	fp = fopen(argv[1], "r+");
//	fpmem = fopen(argv[2], "w");
//	fprintf(fpmem, "{ ");	
//	while (fgets(str, MAXCHAR, fp)) { // first run
//		counter++;
//
//	}
//	fclose(fp);
//	fp = fopen(argv[1], "r+");
//	x = 0;
//	while (fgets(str, MAXCHAR, fp)) { //second run
//		ch = strchr(str, '\n');
//		*ch = '\0';
//		convert =strtol(str, NULL, 16);
//		fprintf(fpmem, "%d , ", convert);
//		x++;
//		if (x == counter - 1)
//			break;
//	}
//	fgets(str, MAXCHAR, fp);
//	ch = strchr(str, '\n');
//	*ch = '\0';
//	convert = strtol(str, NULL, 16);
//	fprintf(fpmem, "%d , ", convert);
//	fclose(fp);
//	fclose(fpmem);
//	scanf("%c", &f);
//}
//
//	