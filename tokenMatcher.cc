#include "tokenMatcher.h"
#include "tokens.h"

// see tokens.txt for more guidance on tokenizing
bool matchToken (const char* str, Style style, TokenMatch* ret) {
    ret->length = 1;
    switch (*str) {
        case '(':
            ret->style = Default;
            ret->type = OpenParen;
            return true;
        case '[':
            ret->style = Boolean;
            ret->type = OpenParen;
            return true;
        case '{':
            ret->style = CStyle;
            ret->type = OpenParen;
            return true;
        case ')':
            ret->style = Default;
            ret->type = CloseParen;
            return true;
        case ']':
            ret->style = Boolean;
            ret->type = CloseParen;
            return true;
        case '}':
            ret->style = CStyle;
            ret->type = CloseParen;
            return true;
    }
    switch (style) {
    case Logical:
        ret->style = Logical;
        switch (*str) {
        case '-':
            if (str[1] == '>') {
                ret->length = 2;
                ret->type = Implication;
            } else {
                ret->type = Not;
            }
            return true;
        case '^':
            ret->type = And;
            return true;
        case 'v':
        case 'V':
            ret->type = Or;
            return true;
        case '+':
            ret->type = Xor;
            return true;
        case '<':
            if (str[1] == '-' && str[2] == '>') {
                ret->length = 3;
                ret->type = Biconditional;
                return true;
            }
            break;
        }
        break;
    case Boolean:
        ret->style = Boolean;
        switch (*str) {
        case '-':
            ret->type = Not;
            return true;
        case '*':
            ret->type = And;
            return true;
        case '+':
            ret->type = Or;
            return true;
        case '^':
            ret->type = Xor;
            return true;
        case '=':
            ret->type = Biconditional;
            return true;
        }
        break;
    case CStyle:
        ret->style = CStyle;
        switch (*str) {
        case '!':
            ret->style = Default;
        case '~':
            ret->type = Not;
            return true;
        case '&':
            ret->type = And;
            if (str[1] == '&') {
                ret->length = 2;
                ret->style = Default;
            }
            return true;
        case '|':
            ret->type = Or;
            if (str[1] == '|') {
                ret->length = 2;
                ret->style = Default;
            }
            return true;
        case '^':
            ret->type = Xor;
            return true;
        case '=':
            ret->type = Biconditional;
            return true;
        }
        break;
    default:
        ret->length = 1;
        switch (*str) {
        case '!':
            ret->style = Default;
            ret->type = Not;
            return true;
        case '~':
            ret->style = CStyle;
            ret->type = Not;
            return true;
        case '-':
            ret->style = Logical;
            if (str[1] == '>') {
                ret->length = 2;
                ret->type = Implication;
            } else {
                ret->type = Not;
            }
            return true;
        case '&':
            ret->type = And;
            if (str[1] == '&') {
                ret->length = 2;
                ret->style = Default;
            } else {
                ret->style = CStyle;
            }
            return true;
        case '*':
            ret->type = And;
            ret->style = Boolean;
            return true;
        case '|':
            ret->type = Or;
            if (str[1] == '|') {
                ret->length = 2;
                ret->style = Default;
            } else {
                ret->style = CStyle;
            }
            return true;
        case '+':
            ret->type = Or;
            ret->style = Boolean;
            return true;
        case '^':
            ret->type = Xor;
            ret->style = Default;
            return true;
        case '=':
            ret->type = Biconditional;
            ret->style = Default;
            return true;
        case '<':
            if (str[1] == '-' && str[2] == '>') {
                ret->length = 3;
                ret->type = Biconditional;
                ret->style = Logical;
                return true;
            }
            break;
        }
    }
    return false;
}