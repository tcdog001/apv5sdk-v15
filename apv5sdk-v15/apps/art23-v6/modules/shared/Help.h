

extern void HelpClose();

extern int HelpOpen(char *filename);

//
// find the specified topic and print the description
//
extern int Help(char *name, void (*print)(char *buffer));

//
// print a list of topics in the help file
//
extern int HelpIndex(void (*print)(char *buffer));
