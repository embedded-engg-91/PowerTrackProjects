#include "text_editor.h"

void initialise(text_editor *ted)

{
    ted->curs.line_no = 0;
    ted->curs.pos = 0;

    ted->doc.firstline = NULL;
    ted->doc.lastline = NULL;

    ted->doc.linecount = 0;

    ted->undo_stack = NULL;
    ted->redo_stack = NULL;
}