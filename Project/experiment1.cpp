#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct soldier {
    int health;
    int strength;
} Soldier;

typedef struct stack {
    Soldier soldiers[100];
    int top;
} Stack;

int isEmpty(Stack* s); 

void push(Stack *s, Soldier soldier) {
    if(s->top < 99) {  
        s->top++;
        s->soldiers[s->top] = soldier;
    } else {
        printf("Stack is full, can't add soldier.\n");
    }
}



Soldier pop(Stack *s) {
    if(!isEmpty(s)) {
        return s->soldiers[s->top--];
    } else {
        printf("Stack is empty, can't pop soldier.\n");
        Soldier emptySoldier = {-1, -1}; 
        return emptySoldier;
    }
}

Soldier top(Stack *s) {
    return s->soldiers[s->top];
}



int isEmpty(Stack* s) {
    return s->top == -1;
}

int damageCalculation(int strength1, int strength2) {
    return (strength1 - strength2) * 0.05 + 50;
}

Soldier randomSoldier() {
    Soldier s;
    s.health = (rand() % 100) + 1;
    s.strength = (rand() % 100) + 1;
    return s;
}

void criticalShot(Stack *s1, Stack *s2, int *turn) {
    if (*turn == 1) {
        printf("Critical shot by Side 1\nSide 2 is completely wiped out!\n");
        while (!isEmpty(s2)) {
            pop(s2);
        }
        *turn = 2; 
    } else {
        printf("Critical shot by Side 2\nSide 1 is completely wiped out!\n");
        while (!isEmpty(s1)) {
            pop(s1);
        }
        *turn = 1; 
    }
}



void gameSummary(Stack *s1, Stack *s2) {
    printf("End Game Summary\n");
    if (s1->top > s2->top) {
        printf("Side 1 wins with %d soldiers remaining\n", s1->top + 1);
    } else {
        printf("Side 2 wins with %d soldiers remaining\n", s2->top + 1);
    }
}


int main() {
    srand(time(NULL));
    Stack side1, side2;
    side1.top = -1;
    side2.top = -1;
    int battleOccurred = 0;

    int turn = 1;  // 1 for side 1, 2 for side 2

    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char command[100];

    while (fscanf(file, "%s", command) != EOF) {
        if (strcmp(command, "A") == 0) {
            int side;
            Soldier soldier;
            fscanf(file, " %d %d,%d", &side, &soldier.health, &soldier.strength);
            if (side == 1) {
                push(&side1, soldier);
                printf("Add soldiers to side 1\n");
                printf("S- H:%d S:%d\n", soldier.health, soldier.strength);
            } else {
                push(&side2, soldier);
                printf("Add soldiers to side 2\n");
                printf("S- H:%d S:%d\n", soldier.health, soldier.strength);
            }
        } 
        else if (strcmp(command, "F") == 0) {
            if (isEmpty(&side1) || isEmpty(&side2)) {
                printf("One of the sides has no soldiers left. Can't fight.\n");
                continue;
            }
			battleOccurred = 1;
            if (turn == 1) {
                Soldier s1 = top(&side1);
                Soldier s2 = top(&side2);
                int damage = damageCalculation(s1.strength, s2.strength);
                printf("Side 1 hit Side 2 with %d damage\n", damage);
                s2.health -= damage;
                if (s2.health <= 0) {
                    printf("Soldier from side 2 has fallen\n");
                    pop(&side2);
                }
                turn = 2;
            } else {
                Soldier s1 = top(&side1);
                Soldier s2 = top(&side2);
                int damage = damageCalculation(s2.strength, s1.strength);
                printf("Side 2 hit Side 1 with %d damage\n", damage);
                s1.health -= damage;
                if (s1.health <= 0) {
                    printf("Soldier from side 1 has fallen\n");
                    pop(&side1);
                }
                turn = 1;
            }
        } 
        else if (strcmp(command, "R") == 0) {
            int side;
            fscanf(file, " %d", &side);
            Soldier s = randomSoldier();
            if (side == 1) {
                printf("Called reinforcements to side 1\n");
                printf("S- H:%d S:%d\n", s.health, s.strength);
                push(&side1, s);
            } else {
                printf("Called reinforcements to side 2\n");
                printf("S- H:%d S:%d\n", s.health, s.strength);
                push(&side2, s);
            }
        }
        else if (strcmp(command, "C") == 0) {
    		criticalShot(&side1, &side2, &turn);
		}
        else if (strcmp(command, "Invalid Input") == 0) {
            printf("Error: Invalid command\n");
        } 
        else {
            printf("Unrecognized command: %s\n", command);
        }
    }

    if (battleOccurred) {
        gameSummary(&side1, &side2);
    } else {
        printf("End\n");
    }
    fclose(file);
    return 0;
}

