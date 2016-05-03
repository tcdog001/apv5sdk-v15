#include <auteos.h>
#include<stdlib.h>
#include<stdio.h>

char mac[18];
char mac_temp[13];

int set_mac_form(char *mac_org)
{
    int i=0, j=0;
    if( NULL == mac_org )
    {
        return -1;
    }
    memset(mac, 0x00, sizeof(mac));
    for(; i<13; i++)
    {
        if( '\0' != mac_org[i] )
        {
            mac[j++] = mac_org[i];
            if( 0 == (j+1)%3 )
            {
                mac[j++] = ':';
            }
        }
        else
        {
            break;
        }
    }
    mac[17] = '\0';
}

int set_mac_add(char *mac_org, unsigned int pos, unsigned int addend)
{
    char *mac_head = mac_org + 6;
    char *mac_end = mac_org + 11;
    unsigned int mac_base = strtol(mac_head, mac_end, 16);
    unsigned int mac_add = 0;//addend * pow(16 ,(6-pos)*2);
    unsigned int mac_add_base = 1;
    int i = 1;
    for(; i<= ((6-pos)*2); i++)
    {
        mac_add_base *= 16;
    }
    mac_add = addend * mac_add_base;
    mac_base += mac_add;
    if(mac_base > 0xFFFFFF)
    {
        mac_base -= 0x1000000;
    }
    char test[7];
    autelan_sprintf(mac_org+6, "%06x", mac_base);
    
}

int main(int argc, char *argv[])
{
    int i=0; 
    unsigned int addend = 1;
    if( argc != 3 && argc != 4 )
    {
        autelan_printf("error:need correct parameter!\n");
        return -1;
    }
    if( (argc == 4) && (argv[3] != NULL) )
        addend = atoi(argv[3]);
    strcpy(mac_temp, argv[2]);    

    if(!strcmp(argv[1],"upper"))
    {
        set_mac_add(mac_temp, 6, addend);
    }
    if(!strcmp(argv[1],"mid-upper"))
    {
        set_mac_add(mac_temp, 4, addend);
    }
    set_mac_form( mac_temp );
    autelan_printf("%s\n", mac); 
}
