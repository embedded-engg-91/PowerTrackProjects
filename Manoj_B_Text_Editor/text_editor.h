#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
extern int wordsize;
extern char *tok1;
extern char *tok2;

typedef struct
{
    int pos;
    int line_no;
} cursor;

typedef struct node
{
    struct node *prev;
    struct node *next;
    char *str; 
} DLL;

typedef struct
{
    DLL *firstline;
    DLL *lastline;
    int linecount;
} document;

typedef struct
{
    char *data;
    int clipboard_size;
} Clipboard;

typedef struct history
{
    DLL *head;
    DLL *tail;
    int linecount;

    cursor curs;

    struct history *next;

} History;

typedef struct
{
    document doc;
    cursor curs;
    Clipboard clipboard;
    History *undo_stack;
    History *redo_stack;
} text_editor;

typedef enum
{
    INSERT_OP,
    DELETE_OP,
    NEWLINE_OP,
    DELETE_LINE_OP,
    REPLACE_OP
} Operation;

extern int modified;
extern char *commands[];

typedef enum
{
    SUCCESS,
    FAILURE,
    VALID,
    INVALID,
    INSERTED,
    LIST_EMPTY,
    DELETED,
    NEW_LINE,
    MOVED,
    COPIED,
    CUT,
    PASTE,
    LOADED,
    SAVED,
    CLOSED,
    FOUND,
    NOTFOUND,
    REPLACED,
    NOTREPLACED,
    EXIT
} Status;
int dl_insert_last(DLL **head, DLL **tail, char *s);
DLL *goto_cursor(DLL **head, DLL **tail, cursor *curs);
Status insert(text_editor *ted, char *str, DLL **head, DLL **tail);
void initialise(text_editor *ted);
void printMenu(void);
Status validate(char *s);
Status jump_to_fun(char *s, text_editor *ted, DLL **head, DLL **tail);
void print_list(DLL *head);
Status delete_chars(text_editor *ted, int count, DLL **head, DLL **tail);
Status dl_delete_node(text_editor *ted, DLL **head, DLL **tail, DLL *node_to_delete);
Status create_new_line(text_editor *ted, DLL **head, DLL **tail);
Status dl_insert_after(DLL **head, DLL **tail, DLL *new_node, DLL *temp); 
Status move_up(text_editor *ted, DLL **head, DLL **tail);
Status move_down(text_editor *ted, DLL **head, DLL **tail);
Status move_left(text_editor *ted, DLL **head, DLL **tail);
Status move_right(text_editor *ted, DLL **head, DLL **tail);
Status move_end(text_editor *ted, DLL **head, DLL **tail);
Status move_home(text_editor *ted, DLL **head, DLL **tail);
Status move_start(text_editor *ted, DLL **head, DLL **tail);
Status move_finish(text_editor *ted, DLL **head, DLL **tail);
Status copy_text(text_editor *ted, DLL **head, DLL **tail, int count);
Status cut_text(text_editor *ted, DLL **head, DLL **tail, int count);
Status paste_text(text_editor *ted, DLL **head, DLL **tail);
Status valid_filename(char *filename);
Status file_open(text_editor *ted, DLL **head, DLL **tail, char *filename);
Status file_save(text_editor *ted, DLL **head, DLL **tail, char *filename);

Status file_close(text_editor *ted, DLL **head, DLL **tail);
Status editor_find(text_editor *ted, DLL **head, DLL **tail, char *text);
Status editor_replace(text_editor *ted, DLL **head, DLL **tail, char *find_text, char *replacer);
Status save_undo_state(text_editor *ted);
DLL *copy_document(DLL *head, DLL **tail); 
Status save_redo_state(text_editor *ted);
void clear_redo_stack(text_editor *ted);
Status editor_undo(text_editor *ted);
Status editor_redo(text_editor *ted);
void free_document(DLL **head, DLL **tail);

Status delete_line(text_editor *ted, DLL **head, DLL **tail);

#endif
