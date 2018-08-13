#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STR_MAX_SIZE 40
#define MAX_SIZE_ADDRESS 100
#define DATE_INT_SIZE 3
#define DATE_STR_SIZE 4
#define DAY 0
#define MONTH 1
#define YEAR 2
#define FALSE 0
#define TRUE 1
#define LIST_SIZE_SET 10
#define LINE_MAX_SIZE 256
#define N 7

typedef struct apartment {
	int number;
	char* address;
	int price;
	short int room_num;
	short int date[DATE_INT_SIZE];
	short int input_apt_date[DATE_INT_SIZE];
}Apartment;

typedef struct aptListNode {
	Apartment apt;
	struct aptListNode *next, *prev;

} AptListNode;

typedef struct aptList {
	AptListNode *head, *tail;
} AptList;

typedef struct historynode {
	char* data;
	struct historynode *next, *prev;
}HistoryNode;

typedef struct historylist {
	HistoryNode *head, *tail;
}HistoryList;


AptList make_new_apt_list();
AptListNode* make_new_apt_listnode();
Apartment* build_arr_from_list(AptList apt_list, int max_price, int max_room_num, int min_room_num,
	short int* max_date, short int* min_input_date, int* arr_size);
HistoryList make_new_history_list();
HistoryNode* make_new_HistoryNode();


int date_compare(short int* max_date, short int* date);
int char_to_int(char* str);

short int month_str_to_int(char* str);
long int fileSize(FILE * f);

void bytes_saving_helper(AptListNode *p, unsigned char *byte1, unsigned char *byte2, unsigned char *byte3,
	unsigned char *byte4, unsigned char *byte5);
void current_time(short int *res);
void insert_node_to_tail(AptList* list, AptListNode* node);
void print_apt(Apartment* apt);
void date_sorting(char* c_time_string, short * res);
void print_apt_arr(Apartment* apt_arr, int arr_size);
void sort_low_to_high(Apartment* apt_arr, int arr_size);
void sort_high_to_low(Apartment* apt_arr, int arr_size);
void get_an_apt(AptList apt_list, char* str);
void add_apt(AptList* apt_list, char* str);
void buy_an_apt(AptList* apt_list, char* line);
void functions_reader(AptList* apt_list, char** short_term_history, HistoryList *old_term_history);
void print_short_history(char** short_term_history);
void insert_short_history(char** short_term_history, char* line, HistoryList *old_term_history);
void insert_old_history(HistoryList* old_term_history, char* new_command);
void print_all_history(char** short_term_history, HistoryList old_term_history);
void subtract_day(short int* date, int dayToSubtract);
void delete_an_apt(AptList* apt_list, char* str);
void save_commands_history(char** short_term_history, HistoryList old_term_history);
void insert_historyNode_to_tail(HistoryList* old_term_history, HistoryNode* p);
void get_commands_from_txt(char** short_term_history, HistoryList *old_term_history);
void save_apt_history(AptList apt_list);
void get_apt_from_bin(AptList *apt_list);
void free_short_term_history(char* short_history_arr[]);
void free_apt_list(AptList* apt_list);
void free_old_history(HistoryList* old_term_history);
void free_all(char* short_term_history[], HistoryList* old_term_history, AptList* apt_list);

char* str_translator(char* str, int* max_price, int* max_room_num, int* min_room_num, int* s,
	int* sr, short int* max_date, short int* min_input_date);
char* read_str(char* line);
char* get_line();
char* read_address(char* line);
char* int_to_char(int num);
char* find_in_history(char** short_term_history, HistoryList old_term_history, char* command);
char* history_string_convertor(char* line, char* command);
char* read_str_to_convert(char* line);
char *replaceWord(const char *s, const char *oldW, const char *newW);


