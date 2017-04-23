#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_orig.h"

/* original version */
entry *findName(char lastName[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastName, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

entry *append_mp(char lastName[], entry *e, entry *next)
{
    e->pNext = next;
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

int Soundex(char *lastName)
{
    int size = strlen(lastName);
    int ans = 0;

    for (int i = 0; i < size; i++) {
        if (*(lastName + i) == 'b' || *(lastName + i) == 'f' || *(lastName + i) == 'p' || *(lastName + i) == 'v')
            ans += 1;
        else if (*(lastName + i) == 'c' || *(lastName + i) == 'g' || *(lastName + i) == 'j' || *(lastName + i) == 'k' || *(lastName + i) == 'q' || *(lastName + i) == 's' || *(lastName + i) == 'x' || *(lastName + i) == 'z')
            ans += 2;
        else if (*(lastName + i) == 'd' || *(lastName + i) == 'y')
            ans += 3;
        else if (*(lastName + i) == 'l')
            ans += 4;
        else if (*(lastName + i) == 'm' || *(lastName + i) == 'n')
            ans += 5;
        else if (*(lastName + i) == 'r')
            ans += 6;
    }

    return ans;
}

void fuzzy_search(char *lastName, entry *e)
{
    int tar_ans = Soundex(lastName);

    printf("The lastName similar to %s:\n", lastName);

    while (e != NULL) {
        int abs_val = abs(Soundex(e->lastName) - tar_ans);
        if (abs_val < 1 && *(e->lastName) == *(lastName))
            if(LevenshteinDistance(lastName, strlen(lastName), e->lastName, strlen(e->lastName)) <= 2)
                printf("%s\n", e->lastName);
        e = e->pNext;
    }
}

int minimum(int a, int b, int c)
{
    if (a <= b && a<= c)
        return a;
    else if (b <= a && b <= c)
        return b;
    else
        return c;

}

int LevenshteinDistance(const char *s, int len_s, const char *t, int len_t)
{
    int cost;

    /* base case: empty strings */
    if (len_s == 0) return len_t;
    if (len_t == 0) return len_s;

    /* test if last characters of the strings match */
    if (s[len_s-1] == t[len_t-1])
        cost = 0;
    else
        cost = 1;

    /* return minimum of delete char from s, delete char from t, and delete char from both */
    return minimum(LevenshteinDistance(s, len_s - 1, t, len_t    ) + 1,
                   LevenshteinDistance(s, len_s    , t, len_t - 1) + 1,
                   LevenshteinDistance(s, len_s - 1, t, len_t - 1) + cost);
}

void fuzzy_search_dis(char *lastName, entry *e)
{
    int ser_size = 2;

    printf("The lastName similar to %s:\n", lastName);

    while (e != NULL) {
        if (LevenshteinDistance(lastName, strlen(lastName), e->lastName, strlen(e->lastName)) <= ser_size)
            printf("%s\n", e->lastName);
        e = e->pNext;
    }
}

