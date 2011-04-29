
typedef struct linklist *LL;

typedef struct dir{
    char name[256];
    struct dir *parent;
    LL files;
    LL subDirs;
} *Dir;

struct linklist{
    union{
        Dir directory;
        char file[256];
    } obj;
    struct linklist *next;
};

typedef struct path{
    char **hr;
    int numLevel;
} *Path;

char *clean(char *string);
Path parsePath(char *string);
Dir navTo(Path toFile, int butOne);
void add(Dir addTo, Path toFile);
void cf(Dir newDir, Path toDir);
void ls(Dir refDir);
void mkfloor(Dir refDir, Path toDir);
void pwf();
void rm(Dir refDir, Path refPath);
void recDel(Dir refDir);
