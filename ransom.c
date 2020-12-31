#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_SIZE 6

char* readline();
char** split_string(char*);

/* Complete the checkMagazine function below */

/* nodes in maps
 * @param word is the word value 
 * @param word_count num of appearences
 * @param next pointer to the next node
 */
struct node {
    char word[WORD_SIZE]; 
    int word_count; 
    struct node *next;
};

struct node *initial_head (char **words_array);
int build_map (struct node *head_p,  struct node **tail_p, char **words_array ,
			   int count);
bool check_note_word (struct node *head_p_m, struct node *head_p_n);
void map_destroy(struct node *head);
int map_push_back (struct node *head_p, struct node **tail_p,char *check_word);

/*
 *check if we can build our note from the magazine
 * @param magazine_count number of words in magazine
 * @param magazine array of magazine words
 * @param note_count number of words in note
 * @param note array of note words
 * print "Yes" if we can build our note from the magazine and "No" otherwise
 */
void checkMagazine(int magazine_count, char **magazine,
		           int note_count, char **note) {
	/* initial head,tail for note */
    struct node *head_p_n;
    head_p_n = initial_head(note);
    struct node *tail_p_n = head_p_n;
	note++; /* we use the first word */
	note_count--; /* ignore the first word */

	/* initial head,tail for magazine */
	struct node *head_p_m;
	head_p_m = initial_head(magazine);
	struct node *tail_p_m = head_p_m;
	magazine++; /* we use the first word */
	magazine_count--; /* ignore the first word */

	if (!head_p_n || !head_p_m) { /* malloc fail */
	    map_destroy(head_p_n); /* destroy all nodes in note map */
		map_destroy(head_p_m); /* destroy all nodes in magazine map */
			exit (1);
	}
	/* build maps, free and exit if they fail */
	
	if (build_map (head_p_m,&tail_p_m,magazine,magazine_count)) { 
	    map_destroy(head_p_n); 
	    map_destroy(head_p_m); 
	    exit (1);
	}
    if (build_map(head_p_n,&tail_p_n,note,note_count)) {
	    map_destroy(head_p_n);
	    map_destroy(head_p_m); 
	    exit (1);
	}

	struct node *head_p_n_tmp = head_p_n;
	bool check = true;
	/* check every word note compare to magzine words */
	while (head_p_n_tmp) {
        if (!check_note_word(head_p_m,head_p_n_tmp)) {
	 	    check = false;
	 	    break;
        }
	 	head_p_n_tmp = head_p_n_tmp->next;
	}
	if (check) {
        printf("Yes");
	}  else {
		     printf("No");
	}
	/* free map nodes */
		map_destroy(head_p_n); 
		map_destroy(head_p_m); 
}

int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

/*
 * if word exist in map update word_count otherwise create new node
 * @param head_p pointer to first node in map
 * @param tail_p pointer to the tail 
 * @param check_word word to check in map
 * return 1 for error and 0 for success of function
 * note: tail will be update if necessary
 */
int map_push_back (struct node *head_p, struct node **tail_p, 
		           char *check_word) {
	if (!tail_p) {
        return 1; /* Invalid argument */
	}
	while (head_p) {
	    if (!strcmp(head_p->word,check_word)) {
			head_p->word_count++;
			return 0;
		}
		head_p = head_p->next;
	}
	struct node *new_node;
	new_node = (struct node*)malloc(sizeof(*new_node));
	if (!new_node) { 
	    return 1; /* Out of memory */
	}
	strcpy(new_node->word,check_word);
	new_node->word_count = 1;
	new_node->next = NULL;
	if (*tail_p) {
        (*tail_p)->next = new_node; /* Update tail */
		*tail_p = new_node; /* Change tail */
	}
	return 0;
}
/*
 * initialize map head with first word in words_array
 * @param words_array magazine/note array of words
 * return pointer for the map head node
 */
struct node *initial_head (char **words_array) {
	struct node *head_p;
	head_p = (struct node*)malloc(sizeof(*head_p));
	if (!head_p) {
	    return head_p; /* Out of memory */
	}
	strcpy(head_p->word,*words_array);
	head_p->word_count = 1;
	head_p->next = NULL;
	return head_p;
}
/*
 * build the map of magazine/note
 * @param head_p pointer to first node in map
 * @param tail_p pointer to the tail 
 * @param words_array magazine/note array of words
 * @param count num of words in array
 * return 1 for error and 0 for success of function
 * note: tail will be update if necessary
 */
int build_map (struct node *head_p, struct node **tail_p, char **words_array,
		       int count) {
	for (int i=0; i<count ;i++) {
		if (map_push_back(head_p,tail_p,words_array[i])) { /* Out of memory */
		    return 1;
        }
	}
	return 0;
}
/*
 * check if note_word can be taken from magazine map 
 * @param head_p_m pointer to first node in magazine map
 * @param head_p_n pointer to first node in note map
 * return true or false accordingly 
 */
bool check_note_word (struct node *head_p_m,struct node *head_p_n) {
 	while (head_p_m) {
 	    if (!strcmp(head_p_n->word,head_p_m->word)) {
 		    if ((head_p_n->word_count) > (head_p_m->word_count)) {
 				return false;
 			}
 			return true;
 		}
 		head_p_m = head_p_m->next;
 	}
 	return false;
}
/*
 * delete map 
 * @param head pointer to first node in map
 */
void map_destroy(struct node *head) {
    struct node *cursor; /* Points on the current node */
	struct node *cursor_copy; /* Used for deletion */
	cursor = head;
	while (cursor) {
	    cursor_copy = cursor;
		cursor = cursor->next;
		free(cursor_copy);
	}
}


