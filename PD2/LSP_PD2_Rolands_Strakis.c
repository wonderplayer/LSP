#include <stdio.h>
#include <errno.h>
#include <string.h>

int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *fin;
    FILE *fout;
    unsigned char buffer[5000];
    char c;
    size_t nolasits, ierakstits;

    if (argc < 3)
    {   
        return 0;
    }

    fin = fopen(argv[1], "rb");

    if (fin == NULL)
    {
        fprintf(stderr,"%s: %s\n", strerror(errno), argv[1]);
        return -1;
    }

    if (exists(argv[2]))
    {
        printf("Fails ar nosaukumu %s jau eksiste. Vai parrakstit? (Y/N)\n", argv[2]);
        c = getchar();
        if (c == 'N' || c == 'n') return 0;
    }

    fout = fopen(argv[2], "wb");

    do
    {
        nolasits = fread(buffer, 1, sizeof buffer, fin);
        if(nolasits) ierakstits = fwrite(buffer, 1, nolasits, fout);
        else ierakstits = 0;
    } while ((nolasits > 0) && (nolasits == ierakstits));

    if(ierakstits){
      perror("Kopesanas kluda");
      return -1;  
    } 
    
    fclose(fin);
    fclose(fout);

    return 0;
}