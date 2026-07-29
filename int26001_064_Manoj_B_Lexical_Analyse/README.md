
# Lexical Analyser Documentation

## Project Overview

The Lexical Analyser is designed to scan a C source file and divide it into meaningful lexical tokens. It reads the source program line by line and classifies each token into its corresponding category such as reserved keywords, identifiers, operators, delimiters, braces, constants, and preprocessor directives.

---

# Workflow

```
Input C File
      │
      ▼
Validate Input
      │
      ▼
Open File
      │
      ▼
Read One Line (fgets)
      │
      ▼
Tokeniser()
      │
      ▼
Character-by-character Scanning
      │
      ▼
Extract Token
      │
      ▼
Classify Token
      │
      ▼
Print Token Type
```

---

# Modules Implemented

## 1. Input Validation

### Purpose

Validates the command-line arguments provided by the user.

### Checks Performed

* Number of arguments
* File extension (`.c`)

### Functions Used

* `strrchr()`
* `strcmp()`

---

## 2. File Handling

### Purpose

Opens the given C source file.

### Functions Used

```c
fopen()
fclose()
```

### Return Status

* File Found
* File Not Found

---

## 3. Reading Source File

The source file is read one complete line at a time.

```c
fgets(buffer,sizeof(buffer),fptr);
```

Each line is passed to the tokenizer.

---

# Tokeniser Module

## Purpose

Scans one line character by character and extracts individual tokens.

---

## Processing Logic

### Ignore Whitespaces

```
' '
'\t'
'\n'
```

These characters are skipped.

---

## Identifier / Keyword Recognition

The tokenizer identifies tokens beginning with

```
A-Z
a-z
_
```

and continues reading until a non-alphanumeric character is encountered.

Example

```
count
_temp
main
printf
```

After extraction, the token is classified as

* Reserved Keyword
* Non Reserved Keyword
* Identifier

using `token_type()`.

---

## Constant Recognition

Numeric constants are recognised by checking

```
0-9
```

Digits are read continuously until a non-digit character appears.

Example

```
25
100
4567
```

---

## Operator Recognition

Single-character operators are currently recognised.

Examples

```
+
-
*
/
%
=
<
>
&
|
!
^
~
```

---

## Brace Recognition

Recognised braces

```
(
)
{
}
[
]
```

---

## Delimiter Recognition

Currently recognised delimiters

```
;
```


---

## Preprocessor Directive Recognition

Implemented support for

```
#include
```

The tokenizer

1. Reads the complete directive beginning with `#`
2. Validates it against the directive table
3. Reads the associated header file

Example

```
#include <stdio.h>

Output

#include → Preprocessor Directive
stdio.h → Header
```

Also supports

```
#include "header.h"
```

---

# Token Classification

The extracted token is compared against predefined tables.

Current tables implemented are

## Reserved Keywords

Examples

```
int
char
float
double
struct
union
typedef
const
volatile
```

---

## Non Reserved Keywords

Examples

```
if
else
for
while
switch
case
return
goto
break
continue
```

---

## Operators

Stored in an operator table.

Example

```
+
-
*
/
%
<
>
=
&
|
!
^
~
```

---

## Braces

```
(
)
{
}
[
]
```

---

## Delimiters

```
;
```

---

## Preprocessor Directives

Examples

```
#include
#define
#ifdef
#ifndef
#endif
```

---

# Functions Implemented

## validate()

Validates command-line arguments.

---

## file_handler()

Checks file existence and opens the file.

---

## tokeniser()

Main lexical analysis function.

Responsibilities

* Skip whitespace
* Extract tokens
* Identify constants
* Detect operators
* Detect braces
* Detect delimiters
* Detect preprocessor directives
* Print token classifications

---

## token_type()

Compares extracted tokens with keyword/operator tables and returns the token category.

---

## Helper Functions

```
is_operator()
is_brace()
is_delim()
is_directives()
```

---


# C Library Functions Used

```
strlen()
strcmp()
isalnum()
isdigit()
isalpha()
isspace()
fgets()
fopen()
fclose()
printf()
```

---

# Sample Execution

Input

```c
#include <stdio.h>

void test()
{
    int count, i;
    i < count;
}
```

Output

```
#include is a Preprocessor Directive
stdio.h is a Header

void is a Reserved Keyword
test is an Identifier

( is a Brace
) is a Brace
{ is a Brace

int is a Reserved Keyword
count is an Identifier
, is a Delimiter
i is an Identifier
; is a Delimiter

i is an Identifier
< is an Operator
count is an Identifier
; is a Delimiter

} is a Brace
```

---
