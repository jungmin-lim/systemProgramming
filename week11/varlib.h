int VLstore(char* name, char* val);
char* VLlookup(char* name);
int VLexport(char* name);
static struct var* find_item(char* name, int first_blank);
char* replace_variable(char* buf);
void VLlist();
int VLenviron2table(char* evn[]);
char** VLtable2environ();
