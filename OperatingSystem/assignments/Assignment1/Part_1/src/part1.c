#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
int match(char *cwd,char *filename,char *str,char *path) {
    char file[254]="";
    char a[1024]="";
    strcat(a,path);
    strcat(a,"/");
    strcat(a,filename);

    strcat(file,cwd);
    strcat(file,"/");
    strcat(file,filename);

    FILE *fp=fopen(file,"r");
    char tmp[256]={0x0};
    printf("%s:",a );
    while(fp && fgets(tmp,sizeof(tmp),fp)) {
        if(strstr(tmp,str)) {
            printf("%s\n",tmp );
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
            char pwd[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            strcpy(path,name);
            strcpy(pwd,cwd);

            strcat(path,"/");
            strcat(pwd,"/");

            strcat(path,entry->d_name);
            strcat(pwd,entry->d_name);
            listdir(path, indent + 2,pwd,kkm);

        } else if(entry->d_type == 8) {
            // printf("%*s- %s\t", indent, "", entry->d_name);
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));

             strcat(cwd, "/");
             strcat(cwd, name);

            if(match(cwd,entry->d_name,kkm,name) ) {

                // FILE *f;
                //    char s;
                //
                //    char file[254]="";
                //    strcat(file,name);
                //    strcat(file,"/");
                //    strcat(file,entry->d_name);
                //    f=fopen(file,"r");
                //    while((s=fgetc(f))!=EOF) {
                //       printf("%c",s);
                //    }
                //    fclose(f);

            }



        }
    }
    closedir(dir);
}

int main(int argv,char *argc[]) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    if(argv==3) {

        listdir(argc[2], 0,cwd,argc[1]);
    }
    return 0;
}
