#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_space(char * str)
{
  int i = 0, j = 0;

   for(i = 0; i < strlen(str); i++)
	 {
	    if(str[i] == ' ' || str[i] == '\n')
		    continue;

		  str[j] = str[i];
		    j++;
			 }

    str[j] = '\0';
}



int main()
{
	FILE * fp;
	char buffer[4096] = {0, };
	char * ptr;
	int i = 0;
	unsigned long long int fields[5];

	fp = fopen("/proc/meminfo", "r");

	for(i = 0; i < 5; i++)
	{
		if(!fgets(buffer, sizeof(buffer), fp))
		{
			printf("fgets error\n");
			fclose(fp);
		}			
	
		printf("%s\n", buffer);
		
		remove_space(buffer);

		ptr = strchr(buffer, ':');
		sscanf(++ptr, "%Lu", &fields[i]);
		
		printf("fields[i] : %Lu\n\n", fields[i]);
	}


	fclose(fp);

	return 0;
}




