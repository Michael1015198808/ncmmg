#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int no;
void expr();

void a(int i) {
    if(i > 4) {
        printf("a%d5[%d]", no, i - 5);
    } else {
        printf("a%d%d", no, i);
    }
}

void ra() {
    a(1 + (rand()%6));
}

void singleton() {
    if((rand()&31) == 0) {
        printf("read()");
    } else 
    if(rand()&3) {
        ra();
    } else {
        if(no == 0 || rand()&15) {
            printf("%d", rand()&15);
        } else {
            printf("f%d(", rand()%no);
            expr();
            printf(",");
            expr();
            printf(")");
        }
    }
}
void parent_expr() {
    printf("(");
    expr();
    printf(")");
}
void expr() {
    if((rand()&7) == 0) {
        if(rand()&7) {
            expr();
        } else {
            parent_expr();
        }
    } else {
        singleton();
    }
    printf("%c", "+-*"[rand()%3]);
    if((rand()&7) == 0) {
        if(rand()&7) {
            expr();
        } else {
            parent_expr();
        }
    } else {
        singleton();
    }
}
void stmt() {
    switch(rand()&15) {
        case 0: {
                    int flag = rand()&3;
                    /*
                    if(flag & 2) {
                        printf("    while(");
                    } else {
                        printf("    if(");
                    }
                    */
                    printf("    if(");
                    parent_expr();
                    if((rand()&3) == 0) {
                        static const char* relops[] = {
                            "<=", ">=", "==", "!=", ">", "<"
                        };
                        printf(" %s ", relops[rand()%6]);
                        parent_expr();
                    }
                    printf(") {\n");
                    stmt();
                    printf("    }");
                    if(flag == 1) {
                        printf(" else {\n");
                        stmt();
                        printf("    }");
                    }
                    printf("\n");
                    break;
                }
        case 1: {
                    printf("    write(");
                    ra();
                    printf(");\n");
                }
        default:
                printf("    ");
                ra();
                printf(" = ");
                expr();
                printf(";\n");
    }
}
void gen_func() {
    printf("int f%d(int a%d1, int a%d2) {\n", no, no, no);
    printf("    int a%d3 = %d;\n", no, rand()&15);
    printf("    int a%d4 = %d;\n", no, rand()&15);
    printf("    int a%d5[2];\n", no);
    printf("    a%d5[0] = 17173;\n", no);
    printf("    a%d5[1] = 4399;\n", no);
    for(int i = 0; i < 5 || (rand() & 3); ++i) {
        stmt();
    }
    printf("    return "); expr(); printf(";\n");
    printf("}\n");
}
void gen_main() {
    printf("int main() {\n");
    printf("    int a%d1 = %d;\n", no, rand()&15);
    printf("    int a%d2 = %d;\n", no, rand()&15);
    printf("    int a%d3 = %d;\n", no, rand()&15);
    printf("    int a%d4 = %d;\n", no, rand()&15);
    printf("    int a%d5[2];\n", no);
    printf("    a%d5[0] = 10151;\n", no);
    printf("    a%d5[1] = 98808;\n", no);
    for(int i = 0; i < 5 || (rand() & 3); ++i) {
        stmt();
    }
    printf("    return "); expr(); printf(";\n");
    printf("}\n");
}
int main() {
    srand(time(NULL));
    for(no = 0; no < 5; ++no) {
        gen_func();
    }
    no = 6;
    gen_main();
    return 0;
}
