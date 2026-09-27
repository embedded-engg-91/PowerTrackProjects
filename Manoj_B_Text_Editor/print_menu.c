#include "text_editor.h"


void printMenu(void)
{
    printf("\n");
    printf("============================================\n");
    printf("              TEXT EDITOR MENU              \n");
    printf("============================================\n");

    printf("\nFile Operations:\n");
    printf("  open <filename>       - Open file\n");
    printf("  save <filename>       - Save file\n");
    printf("  close                 - Close file\n");

    printf("\nEditing:\n");
    printf("  insert <text>         - Insert text\n");
    printf("  delete <number>       - Delete specified number of characters\n");
    printf("  newline               - Start a new line\n");
    printf("  delete_line           - Delete entire current line\n");
    printf("  copy <number>         - Copy characters\n");
    printf("  cut <number>          - Cut characters\n");
    printf("  paste                 - Paste clipboard\n");

    printf("\nCursor Movement:\n");
    printf("  up                    - Move cursor up\n");
    printf("  down                  - Move cursor down\n");
    printf("  left                  - Move cursor left\n");
    printf("  right                 - Move cursor right\n");
    printf("  home                  - Move to start of line\n");
    printf("  end                   - Move to end of line\n");
    printf("  start                 - Move to start of file\n");
    printf("  finish                - Move to end of file\n");

    printf("\nSearch & Replace:\n");
    printf("  find <text>           - Search text\n");
    printf("  replace <old> <new>   - Replace text\n");

    printf("\nHistory:\n");
    printf("  undo                  - Undo last operation\n");
    printf("  redo                  - Redo last operation\n");

    printf("\nOther:\n");
    printf("  print                 - Display text\n");
    printf("  help                  - Show this menu\n");
    printf("  exit                  - Exit editor\n");

    printf("\n============================================\n");
}
