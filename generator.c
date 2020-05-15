#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

int no = 0, indent = 0, is_main = 0;
void expr();
void stmt();
void stmts(int);

unsigned int randuint(){
  static FILE *f = NULL;
  if(!f){
    f = fopen("/dev/urandom", "r");
    assert(f);
  }
  unsigned int ret;
  fread(&ret, sizeof(unsigned int), 1, f);
  return ret;
}

static void builtin_funcs() {
    printf("int neg(int neg_i) {\n");
    printf("    return -(neg_i);\n");
    printf("}\n");
    printf("int abs(int abs_i) {\n");
    printf("    if(abs_i > 0) {\n");
    printf("        return abs_i;\n");
    printf("    }\n");
    printf("    return neg(abs_i);\n");
    printf("}\n");
}

void a(int i) {
    if(i > 6) {
        printf("a%d6[%d]", no, i - 7);
    } else if(i > 4) {
        printf("a%d5[%d]", no, i - 5);
    } else {
        printf("a%d%d", no, i);
    }
}
int output(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    return printf("%*s", indent, "") + vprintf(fmt, va);
}

void ra() {
    a(1 + (randuint()&7));
}

void singleton() {
    if((randuint()&31) == 0) {
        printf("read()");
    } else {
        ra();
    }
}
void parent_expr() {
    printf("(");
    expr();
    printf(")");
}
void raw_expr() {
    if((randuint()&7) == 0) {
        if(randuint()&7) {
            expr();
        } else {
            parent_expr();
        }
    } else {
        singleton();
    }
    printf("%c", "+-*"[randuint()%3]);
    if((randuint()&7) == 0) {
        if(randuint()&7) {
            expr();
        } else {
            parent_expr();
        }
    } else {
        singleton();
    }
}
void expr() {
    switch(randuint()&31) {
        case 0:
            printf("abs(");
            raw_expr();
            printf(")");
            break;
        case 1:
            printf("neg(");
            raw_expr();
            printf(")");
            break;
        case 2:
            printf("!(");
            raw_expr();
            printf(")");
            break;
        default:
            raw_expr();
            break;
    }
}
void stmt_return() {
    if(is_main) {
        output("return 0;\n");
    } else {
        output("return "); expr(); printf(";\n");
    }
}
void stmt_if() {
    output("if(");
    parent_expr();
    if((randuint()&3) == 0) {
        static const char* relops[] = {
            "<=", ">=", "==", "!=", ">", "<"
        };
        printf(" %s ", relops[randuint()%6]);
        parent_expr();
    }
    printf(") {\n");
    indent += 4;
    stmts(2);
    indent -=4;
    output("}");
    if(randuint()&1) {
        printf(" else {\n");
        indent += 4;
        stmt(2);
        indent -= 4;
        output("}");
    }
    printf("\n");
}
void stmt_while() {
    static int dep = 0;
    if(dep < 4) {
        output("i%d[%d] = 0;\n", no, dep);
        output("while(i%d[%d] < %d) {\n", no, dep, 1 + (randuint()&3));
        indent += 4;
        output("i%d[%d] = i%d[%d] + 1;\n", no, dep, no, dep);
        ++dep;
        stmts(2);
        --dep;
        indent -= 4;
        output("}\n");
    } else {
        stmt();
    }
}
void stmt() {
    switch(randuint()&15) {
        case 0:
            stmt_if();
            return;
        case 1:
            output("write(");
            expr();
            printf(");\n");
            return;
        case 2:
                stmt_while();
                return;
        case 3:
                if(no == 0 || rand()&15) {
                    printf("%d", rand()&15);
                } else {
                    printf("f%d(", rand()%no);
                    expr();
                    printf(", ");
                    expr();
                    printf(", ");
                    printf("a%d%d", no, 5 + (rand()&1));
                    printf(")");
                }
        case 14:
                if((randuint()&3) == 0) {
                    stmt_return();
                    return;
                }
                //fall
        default:
                output("");
                ra();
                printf(" = ");
                expr();
                printf(";\n");
    }
}
void stmts(int n) {
    for(int i = 0; i < n || ((randuint() & 1) && i < 2 * n); ++i) {
        stmt();
    }
}
void gen_func() {
    output("int f%d(int a%d1, int a%d2, int a%d6[2]) {\n", no, no, no, no);
    indent += 4;
    output("int a%d3 = %d;\n", no, randuint()&15);
    output("int a%d4 = %d;\n", no, randuint()&15);
    output("int a%d5[2];\n", no);
    output("int i%d[4];\n", no);
    output("a%d5[0] = 17173;\n", no);
    output("a%d5[1] = 4399;\n", no);
    stmts(5);
    stmt_return();
    indent -= 4;
    output("}\n");
}
void gen_main() {
    is_main = 1;
    output("int main() {\n");
    indent += 4;
    output("int a%d1 = %d;\n", no, randuint()&15);
    output("int a%d2 = %d;\n", no, randuint()&15);
    output("int a%d3 = %d;\n", no, randuint()&15);
    output("int a%d4 = %d;\n", no, randuint()&15);
    output("int a%d5[2];\n", no);
    output("int a%d6[2];\n", no);
    output("int i%d[4];\n", no);
    output("a%d5[0] = 10151;\n", no);
    output("a%d5[1] = 98808;\n", no);
    output("a%d6[0] = 114;\n", no);
    output("a%d6[1] = 514;\n", no);
    stmts(5);
    stmt_return();
    indent -= 4;
    output("}\n");
}
int main() {
    builtin_funcs();
    for(no = 0; no <= 5; ++no) {
        gen_func();
    }
    gen_main();
    return 0;
}
