#include <stdio.h>

int sv_garums(char *virkne)
{
    int lenght = 0;
    for (int i = 0; virkne[i] != '\0'; ++i)
        lenght++;

    return lenght;
}
 
void sv_kopet(char *no, char *uz)
{
    int i;
    for (i = 0; no[i] != '\0'; i++)
        uz[i] = no[i];

    uz[i] = '\0';
}

int sv_meklet(char *kur, char *ko)
{
    int sakums;
    for (int i = 0; kur[i] != '\0'; ++i)
    {
        sakums = i;
        for (int j = 0; kur[i] == ko[j]; ++j, ++i)
            if(ko[j+1] == '\0')
                return sakums;

        i = sakums;
    }
    return -1;
}

void sv_apgriezt(char *virkne)
{
    char temp;
    int len = sv_garums(virkne) - 1;
    for (int i = 0; i < len/2; ++i)
    {
            temp = virkne[i];
            virkne[i] = virkne[len-i];
            virkne[len-i] = temp;
    }
}

void sv_vapgriezt(char *virkne)
{
    // ...
    // TODO: PD1.5 kodu rakstīt te!
    // ...
}

void izvest(char *virkne){
    for (int i = 0; virkne[i] != '\0'; ++i)
        printf("%c", virkne[i]);
    printf("\n");
}

int main(void) {
    char buferis[100];

    printf("Tests uzdevumam PD1.1:\n");
    printf("%d\n", sv_garums("hello world"));
    printf("%d\n", sv_garums("123"));
    printf("%d\n", sv_garums(""));

    printf("Tests uzdevumam PD1.2:\n");
    char uz[100];
    sv_kopet("hello world", uz);
    izvest(uz);

    printf("Tests uzdevumam PD1.3:\n");
    printf("%d\n", sv_meklet("hello world", "lo"));
    
    printf("Tests uzdevumam PD1.4:\n");
    char v[] = "hello world";
    sv_apgriezt(v);
    izvest(v);    

    // Uzdevumus 4. un 5. testēt šādā veidā, lai izvairītos no access violation!
    // sv_kopet("abcd", buferis);
    // sv_apgriezt(buferis);
    // printf("%s\n", buferis);

    return 0;
}
