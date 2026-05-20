#include <stdio.h>

int main() {

    int score1, score2, score3, score4, score5;

    scanf("%dEnter Score 1/5:\n", &score1);
    scanf("\n%dEnter Score 2/5\n", &score2);
    scanf("\n%dEnter Score 3/5\n", &score3);
    scanf("\n%dEnter Score 4/5\n", &score4);
    scanf("\n%dEnter Score 5/5\n", &score5);
    
    printf("\n%d Score1:", score1);
    printf("\n%d Score2:", score2);
    printf("\n%d Score3:", score3);
    printf("\n%d Score4:", score4);
    printf("\n%d Score5:", score5);
    printf("\n\n%d Total:", score1 + score2 + score3 + score4 + score5);
    
    return 0;
}