#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char g_str[] = "Cpu(s): 17.2%us,  0.4%sy,  0.2%ni, 81.8%id,  0.4%wa,  0.0%hi,  0.0%si,  0.0%st";

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
 char cpu_str[20] = {0, };
 double cpu;

 remove_space(g_str);
 printf("%s\n", g_str);

 sscanf(g_str, "%*[^:]:%*[^,],%*[^,],%*[^,],%[^%%],%*s", cpu_str);

 sscanf(cpu_str, "%lf", &cpu);

 printf("%f\n", cpu);

 return 0;
}
