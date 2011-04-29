#include "garage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG printf("here\n"); fflush(stdout);

Dir base;
char pwd[512];
Dir current;

int main(int argc, char *argv[]){
    base = (Dir)malloc(sizeof(struct dir));
    strcpy(base->name, ":");
    base->files = NULL;
    base->subDirs = NULL;
    base->parent = NULL;
    
    current = base;
    strncpy(pwd, ":", 2);
    
    Path retPath;
    Dir refDir;
    char *args;
    char *command = (char *)malloc(sizeof(char)*256);
    while(!feof(stdin)){
        fgets(command, 255, stdin);
        
        if(!strncmp(command, "add", 3)){
            args = clean(command+4);
            retPath = parsePath(args);
            refDir = navTo(retPath, 1);
            if(refDir != NULL){           
                add(refDir, retPath);
            }
        }else if(!strncmp(command, "cf", 2)){
            args = clean(command+3);
            retPath = parsePath(args);
            refDir = navTo(retPath, 0);
            if(refDir != NULL){
                cf(refDir, retPath);
            }
        }else if(!strncmp(command, "end", 3)){
            return 0;
        }else if(!strncmp(command, "ls", 2)){
            args = clean(command+2);
            retPath = parsePath(args);
            if(!strncmp(args, "", 2)){
                refDir = current;
            }else{
                refDir = navTo(retPath, 0);
            }
            if(refDir != NULL){
                ls(refDir);
            }
        }else if(!strncmp(command, "mkfloor", 7)){
            args = clean(command+8);
            retPath = parsePath(args);
            refDir = navTo(retPath, 1);
            if(refDir != NULL){
                mkfloor(refDir, retPath);
            }
        }else if(!strncmp(command, "pwf", 3)){
            pwf();
        }else if(!strncmp(command, "rm", 2)){
            args = clean(command+3);
            retPath = parsePath(args);
            refDir = navTo(retPath, 1);
            if(refDir != NULL){
                rm(refDir, retPath);
            }
        }else{
            printf("Error: Unknown command %s\n", clean(command));
        }
    }
    return 0;
}

char *clean(char *string){
    char *retString = (char *)malloc(sizeof(char)*strlen(string));
    int i = 0;
    while(*string != '\0'){
        if(*string != ' ' && *string != '\t' && *string != '\n' && *string){
            retString[i] = *string;
            i++;
        }
        string++;
    }
    retString[i] = '\0';
    return retString;
}

Dir navTo(Path toFile, int butOne){
    Dir dirTrav = current;
    LL llTrav;
    int start = 0;
    int end = (butOne?toFile->numLevel-1:toFile->numLevel);
    if(!strncmp(toFile->hr[0], ":", 2)){
        if(toFile->numLevel == 1){
            return base;
        }
        start = 1;
    }else if(!strncmp(toFile->hr[0], "..", 2)){
        dirTrav = current;
        if(dirTrav->parent == NULL){
            printf("Error, [%s] has no parent lot\n", dirTrav->name);
            return NULL;
        }else{
            dirTrav = dirTrav->parent;
            start = 1;
        }
    }
    //printf("start: %d, end: %d\n", start, end);
    for(int i = start; i < end; i++){
        llTrav = dirTrav->subDirs;
        while(llTrav != NULL){
            if(!strncmp(toFile->hr[i], "..", 2)){
                if(dirTrav->parent == NULL){
                    printf("Error, [%s] has no parent lot\n", dirTrav->name);
                    return NULL;
                }else{
                    dirTrav = dirTrav->parent;
                    break;
                }
            }else if(!strcmp(toFile->hr[i], llTrav->obj.directory->name)){
                dirTrav = llTrav->obj.directory;
                break;
            }
            llTrav = llTrav->next;
        }
        if(llTrav != NULL){
            continue;
        }else{
            printf("Error: Cannot find subfloor [%s]\n", toFile->hr[i]);
            return NULL;
        }
    }
    //printf("%s\n", dirTrav->name);
    return dirTrav;
}    

Path parsePath(char *string){
    Path retPath = (Path)malloc(sizeof(struct path));
    char *iter = string;
    char *copy = string;
    int dirs = 1;
    while(*iter != '\0'){
        if(*iter == ':'){
            dirs++;
        }
        iter++;
    }
    int start = 0;
    if(*copy == ':'){
        retPath->numLevel = dirs;
        retPath->hr = (char **)malloc(sizeof(char*)*dirs);
        retPath->hr[0] = (char*)malloc(sizeof(char)*256);
        strncpy(retPath->hr[0], ":", 2);
        if(!strcmp(copy, ":")){
            retPath->numLevel--;
            return retPath;
        }
        start = 1;
    }else{
        retPath->numLevel = dirs;
        retPath->hr = (char **)malloc(sizeof(char*)*dirs);
    }
    copy = copy+start;
    for(int i = start; i < dirs; i++){
        retPath->hr[i] = (char*)malloc(sizeof(char)*256);
        iter = strchr(copy, ':');
        if(iter != NULL){
            strncpy(retPath->hr[i], copy, iter-copy);
        }else{
            strcpy(retPath->hr[i], copy);
        }
        iter++;
        copy = iter;
    }
    return retPath;
}  
        
void add(Dir addTo, Path toFile){
    LL newFile;
    LL fileTrav = addTo->files;
    int end = toFile->numLevel-1;
    if(addTo->files == NULL){
        newFile = (LL)malloc(sizeof(struct linklist));
        newFile->next = NULL;
        strcpy(newFile->obj.file, toFile->hr[end]);
        addTo->files = newFile;
        return;
    }
    while(fileTrav->next != NULL){
        if(!strcmp(fileTrav->obj.file, toFile->hr[end])){
            printf("Error: Duplicate item \"%s\" in [%s]\n", toFile->hr[end], addTo->name);
            return;
        }
        fileTrav = fileTrav->next;
    }
    if(!strcmp(fileTrav->obj.file, toFile->hr[end])){
        printf("Error: Duplicate item \"%s\" in [%s]\n", toFile->hr[end], addTo->name);
        return;
    }
    newFile = (LL)malloc(sizeof(struct linklist));
    newFile->next = NULL;
    strcpy(newFile->obj.file, toFile->hr[end]);
    fileTrav->next = newFile;
    fflush(stdout);
    return;
}
                
void cf(Dir newDir, Path toDir){
    char **absPath = NULL;
    int start = 0;
    int end = 0;
    int total = end;
    if(strncmp(toDir->hr[0], ":", 1)){
        Path curPath = parsePath(pwd);
        end = curPath->numLevel+toDir->numLevel;
        absPath = (char**)malloc(sizeof(char*)*end);
        for(int i = 0; i < curPath->numLevel; i++){
            absPath[i] = curPath->hr[i];
        }
        start = curPath->numLevel;
    }else{
        end = toDir->numLevel;
        absPath = (char**)malloc(sizeof(char*)*end);
        start = 0;
    }
    int i = start;
    total = end;
    for(int j = 0; j < toDir->numLevel; j++){
        if(!strncmp(toDir->hr[j], "..", 2)){
            /*if( i == 1 ){
                printf("Error, [:] has no parent lot\n");
                return;
            } Should never happen */
            i--;
            total--;
            continue;
        }else{
            absPath[i] = toDir->hr[j];
            i++;
        }
    }
    pwd[0] = '\0';
    for(int j = 0; j < i; j++){
        if(j == 0 || j == 1){
            strcat(pwd, absPath[j]);
        }else{
            strcat(pwd, ":");
            strcat(pwd, absPath[j]);
        }
    }
    current = newDir;        
}

void ls(Dir refDir){
    LL llTrav = refDir->files;
    while(llTrav != NULL){
        printf("%s\n", llTrav->obj.file);
        llTrav = llTrav->next;
    }
    llTrav = refDir->subDirs;
    while(llTrav != NULL){
        printf("[%s]\n", llTrav->obj.directory->name);
        llTrav = llTrav->next;
    }
    return;
}

void mkfloor(Dir refDir, Path toDir){
    LL llTrav = refDir->subDirs;
    LL newDirLL = NULL;
    Dir newDir = NULL;
    while(llTrav != NULL && llTrav->next != NULL){
        if(!strcmp(llTrav->obj.directory->name, toDir->hr[toDir->numLevel-1])){
            printf("Error: duplicate floor [%s]\n", llTrav->obj.directory->name);
            return;
        }
        llTrav = llTrav->next;
    }
    
    if(llTrav != NULL && !strcmp(llTrav->obj.directory->name, toDir->hr[toDir->numLevel-1])){
        printf("Error: duplicate floor [%s]\n", llTrav->obj.directory->name);
        return;
    }    
    newDir = (Dir)malloc(sizeof(struct dir));
    strcpy(newDir->name, toDir->hr[toDir->numLevel-1]);
    newDir->parent = refDir;
    newDir->files = NULL;
    newDir->subDirs = NULL;
    
    newDirLL = (LL)malloc(sizeof(struct linklist));
    newDirLL->obj.directory = newDir;
    newDirLL->next = NULL;
    
    if(refDir->subDirs == NULL){
        refDir->subDirs = newDirLL;
    }else{
        llTrav->next = newDirLL;
    }
    return;
}

void pwf(){
    printf("[%s]\n", pwd);
}

void rm(Dir refDir, Path refPath){
    int end = refPath->numLevel-1;
    LL iter = refDir->files;
    LL last = iter;
    LL remove = NULL;
    while(iter != NULL){
        if(!strcmp(iter->obj.file, refPath->hr[end])){
            if(iter == refDir->files){
                last = iter;
                refDir->files = iter->next;
                //free(last);
                last = NULL;
                return;
            }else{
                last->next = iter->next;
                //free(iter);
                return;
            }
        }
        last = iter;
        iter = iter->next;
    }
    iter = refDir->subDirs;
    last = iter;
    while(iter != NULL){
        if(!strcmp(iter->obj.directory->name, refPath->hr[end])){
            if(iter == refDir->subDirs){
                refDir->subDirs = iter->next;
                remove = iter;
                recDel(remove->obj.directory);
                free(remove->obj.directory);
                remove->obj.directory = NULL;
                //printf("Clean: %s\n", clean(pwd));
                //printf("Current Dir: %s\n", current->name);
                free(remove);
                remove = NULL;
                return;
            }else{
                last->next = iter->next;
                recDel(iter->obj.directory);
                free(iter->obj.directory);
                free(iter);
                iter = NULL;
                return;
            }
        }
        last = iter;
        iter = iter->next;
    }
    printf("Error: Cannot find %s\n", refPath->hr[end]);
    return;
}

void recDel(Dir refDir){
    LL iter = refDir->files;
    LL last = iter;
    LL remove = NULL;
    while(iter != NULL){
        remove = iter;
        last = iter;
        iter = iter->next;
        free(remove);
        remove = NULL;
    }
    iter = refDir->subDirs;
    last = iter;
    while(iter != NULL){
        remove = iter;
        last = iter;
        iter = iter->next;
        recDel(remove->obj.directory);
        free(remove->obj.directory);
        remove->obj.directory = NULL;
        free(remove);
        remove = NULL;
    }
    return;
}
            
            
            
            
            
            
            
            
            
