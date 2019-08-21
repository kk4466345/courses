#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
int match(char *cwd,char *filename,char *str) {
    char file[254]="";

    strcat(file,cwd);
    strcat(file,"/");
    strcat(file,filename);

    FILE *fp=fopen(file,"r");
    char tmp[256]={0x0};
    while(fp && fgets(tmp,sizeof(tmp),fp)) {
        if(strstr(tmp,str)) {
            return 2;
        }
    }
    if(fp) {
        fclose(fp);
    }
    return 0;
}

void listdir(const char *name, int indent,char *cwd,char *kkm)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            strcpy(path,name);
            strcat(path,"/");
            strcat(path,entry->d_name);
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            strcpy(path,name);
            strcat(path,"/");
            strcat(path,entry->d_name);

            listdir(path, indent + 2,cwd,kkm);
           
        } else if(entry->d_type == 8) {
            // printf("%*s- %s\t", indent, "", entry->d_name);
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));

             strcat(cwd, "/");
             strcat(cwd, name);

            


            if(match(cwd,entry->d_name,kkm) ) {
                printf("%s/%s:\n",name,entry->d_name );
                FILE *f;
                   char s;
                   
                   char file[254]="";
                   strcat(file,name);
                   strcat(file,"/");
                   strcat(file,entry->d_name);
                   f=fopen(file,"r");
                   while((s=fgetc(f))!=EOF) {
                      printf("%c",s);
                   }
                   fclose(f);
                   
            }
            


        }
    }
    closedir(dir);
}

int main(void) {
    char cwd[1024];
    char kkm[254];
    scanf("%s",kkm);
    getcwd(cwd, sizeof(cwd));
    listdir("..", 0,cwd,kkm);
    return 0;
}
