#include <stdio.h>
#include <string.h>

#include <kernel/rescue_shell.h>
#include <arch/i386/keyboard.h>

#include <kernel/list.h>

typedef enum {
    SYMBOL,
    STRING_LITERAL
} token_type;

typedef struct token {
    token_type type;
    char *value;
} token_t;

struct Lexer {
    char *string;
    size_t length;
    size_t index;

    const char *symbols = "./";

    Lexer (char *string):
        string (strdup (string)),
        length (strlen (string)),
        index (0) { }

    bool EOF () {
        return this->index >= this->length;
    }

    static bool contains (const char *chars, char c) {
        return strchr (chars, c) != nullptr;
    }

    bool is_symbol (char c) {
        return contains (symbols, c);
    }

    List<token> *operator ()() {
        while (!this->EOF ()) {
            char c = string[this->index++];
            if (is_symbol (c))
                printf ("/%c\\", c);
            else
                printf ("%c", c);
        }
        return nullptr;
    }
};

void enter_rescue () {
    while (true) {
        printf ("$> ");
        char *str = gets ();
        printf ("%s", str);

        List<token> *tokens = Lexer(str)();

        delete str;
        delete tokens;
    }
}
