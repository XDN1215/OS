#include <stdio.h>

int main(int argc,char *argv[])
{
    FILE * fp;
    fp = fopen(argv[1], "r");
    if(fp != NULL)
    {
        char buf[1000];
        int n = fread(buf,1,1000,fp);
        fclose(fp);
        if(n > 512)
        {     
            printf("ERROR: boot block too large: %d bytes (max 510)\n", n);
            return 0;
        }
        else
        {
            printf("OK: boot block is %d bytes (max 510)\n", n);
            FILE * fp1;
            fp1 = fopen(argv[1], "a+");
            while(n < 510)
            {     
                fwrite("\x0", 1, 1, fp1);
                n++;
            }
            if(n = 510)
            {
                fwrite("\x55\xAA", 1, 2, fp1);
            }
            fclose(fp1);
        }
    }
}