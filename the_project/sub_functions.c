#define _CRT_SECURE_NO_WARNINGS
#include "apt.h"

// function sort ctime output according to our spec.[dd|mm|yyyy]
void date_sorting(char* c_time_string, short* res)
{
	char month_str[DATE_STR_SIZE];
	int day, year = 0, mult = 1;
	int i;

	for (i = 4; i < 7; i++) // ignores the first 4 letters (day of the week)
		month_str[i - 4] = c_time_string[i]; // gets the month string value

	month_str[3] = '\0';

	res[MONTH] = month_str_to_int(month_str);

	day = (int)((c_time_string[8] - '0')) * 10 + (c_time_string[9] - '0'); // gets the day value (2 digits)
	if (!(day > 0 && day < 32)) // checks rather there is only 1 digit in day and gets the day value
		day = (int)((c_time_string[9] - '0'));
	res[DAY] = day;

	for (int i = 23; i >= 20; i--)// gets the year value
	{

		year += (c_time_string[i] - '0')*mult;
		mult *= 10;
	}
	res[YEAR] = year;
}

// function converts month string to it integer number.
short int month_str_to_int(char* str)
{
	if (strcmp(str, "Jan") == 0)
		return 1;
	else if (strcmp(str, "Feb") == 0)
		return 2;
	else if (strcmp(str, "Mar") == 0)
		return 3;
	else if (strcmp(str, "Apr") == 0)
		return 4;
	else if (strcmp(str, "May") == 0)
		return 5;
	else if (strcmp(str, "Jun") == 0)
		return 6;
	else if (strcmp(str, "Jul") == 0)
		return 7;
	else if (strcmp(str, "Aug") == 0)
		return 8;
	else if (strcmp(str, "Sep") == 0)
		return 9;
	else if (strcmp(str, "Oct") == 0)
		return 10;
	else if (strcmp(str, "Nov") == 0)
		return 11;
	else if (strcmp(str, "Dec") == 0)
		return 12;
	else
		return 0;
}

// function makse new apt list.
AptList make_new_apt_list()
{
	AptList res_apt;
	res_apt.head = res_apt.tail = NULL;

	return res_apt;
}

// function makse new  apt list node.
AptListNode* make_new_apt_listnode()
{
	AptListNode* res_apt;
	res_apt = (AptListNode*)malloc(sizeof(AptListNode));
	res_apt->next = res_apt->prev = NULL;

	return res_apt;
}

// function inserts new node to tail.
void insert_node_to_tail(AptList * list, AptListNode * node)
{
	if (list->head == NULL) // if list is empty
	{
		node->apt.number = 1; // inserts the number or the first apartment
		list->head = list->tail = node;
	}
	else
	{
		node->apt.number = list->tail->apt.number + 1;
		list->tail->next = node;
		list->tail->next->prev = list->tail;
		list->tail = node;
	}
}

// function makes new History list.
HistoryList make_new_history_list()
{
	HistoryList res;
	res.head = res.tail = NULL;
	return res;
}

// function creates new history node.
HistoryNode* make_new_HistoryNode()
{
	HistoryNode* res = (HistoryNode*)malloc(sizeof(HistoryNode));
	res->next = res->prev = NULL;
	return res;
}

// function inserts history node to tail.
void insert_historyNode_to_tail(HistoryList* old_term_history, HistoryNode* p)
{
	if (old_term_history->head == NULL)//if empty list
		old_term_history->head = old_term_history->tail = p;
	else
	{
		old_term_history->tail->next = p;
		p->prev = old_term_history->tail;
	}
	old_term_history->tail = p;
}

// function prints apt details.
void print_apt(Apartment* apt)
{
	printf("Apt details:\nCode:%d \nAddress: %s \nPrice: %d \nNumber of rooms: %d \nEntry date: %d/%d/%d \nDatabase entry date: %d/%d/%d\n",
		apt->number,
		apt->address,
		apt->price,
		apt->room_num,
		apt->date[DAY], apt->date[MONTH], apt->date[YEAR],
		apt->input_apt_date[DAY], apt->input_apt_date[MONTH], apt->input_apt_date[YEAR]);
}

// function converts char to integer number.
int char_to_int(char* str)
{
	int num = 0;
	for (int i = 0; i < (int)strlen(str); i++)
	{
		num *= 10;
		num += str[i] - '0';
	}
	return num;
}

// function converts integer number to char.
char* int_to_char(int num)
{
	int p_size = 2, l_size = 0, i;
	char* res = (char*)malloc(p_size * sizeof(char));
	char temp;
	while (num != 0)
	{
		if (l_size == p_size)
		{
			p_size *= 2;
			res = (char*)realloc(res, p_size * sizeof(char));
		}
		res[l_size] = (char)('0' + (num % 10));
		num /= 10;
		l_size++;
	}
	if (l_size != p_size)
		res = (char*)realloc(res, l_size * sizeof(char));
	res[l_size] = '\0';
	for (i = 0; i < l_size / 2; i++)
	{
		temp = res[i];
		res[i] = res[l_size - i - 1];
		res[l_size - i - 1] = temp;
	}
	return res;
}

// function gets new line from prompt.
char* get_line()
{
	char *str = (char*)malloc(LINE_MAX_SIZE * sizeof(char));
	char ch = getchar();
	int i = 0;
	while (ch != '\n')
	{
		str[i] = ch;
		ch = getchar();
		i++;
	}
	str[i] = '\0';
	i++;
	str = (char*)realloc(str, i * sizeof(char));
	return str;
}

// function gets string and returns new string until the first break.
char* read_str(char* line)
{
	char *str = (char*)malloc(STR_MAX_SIZE * sizeof(char));
	int i = 0;
	while (*line == ' ' || *line == '^')//getting to the command
		line++;
	while (line[i] != ' '&&line[i] != '\0'&&line[i] != '^')
	{
		str[i] = line[i];
		i++;
	}
	str[i] = '\0';
	i++;
	str = (char*)realloc(str, i * sizeof(char));

	return str;
}

// function gets string and returns the string that found in the quates.
char* read_address(char* line)
{
	char *str = (char*)malloc(MAX_SIZE_ADDRESS * sizeof(char));
	int i = 0;
	while (*line == ' ' || *line == '"' || *(line + 1) == '^' || *line == '^') // getting to the address
		line++;
	while (line[i] != '"')
	{
		str[i] = line[i];
		i++;
	}
	str[i] = '\0';
	i++;
	str = (char*)realloc(str, i * sizeof(char));

	return str;
}

// funtion prints short history list.
void print_short_history(char** short_term_history)
{
	for (int i = N - 1; i >= 0; i--)
		if (short_term_history[i])
			printf("%s\n", short_term_history[i]);
}

// function ptints both short and old history lists.
void print_all_history(char** short_term_history, HistoryList old_term_history)
{
	HistoryNode *p = old_term_history.tail;
	while (p)
	{
		printf("%s\n", p->data);
		p = p->prev;
	}
	print_short_history(short_term_history);
}

// function subtracts "x" days from the current date.
void subtract_day(short int* date, int dayToSubtract)
{
	time_t now = time(NULL);
	struct tm now_tm = *localtime(&now);
	struct tm then_tm = now_tm;
	dayToSubtract *= 60 * 60 * 24; // change the number of days to seconds value
	then_tm.tm_sec -= dayToSubtract;   // subtract the days

	mktime(&then_tm);      // normalize it
	date[DAY] = then_tm.tm_mday;
	date[MONTH] = then_tm.tm_mon + 1;
	date[YEAR] = then_tm.tm_year + 1900;
}

// function gets string to convert.
char* read_str_to_convert(char* line)
{
	char *str = (char*)malloc(STR_MAX_SIZE * sizeof(char));
	int i = 0;
	while (*line == '^')
		line++;
	while (line[i] != '\0'&&line[i] != '^')
	{
		str[i] = line[i];
		i++;
	}
	str[i] = '\0';
	i++;
	str = (char*)realloc(str, i * sizeof(char));

	return str;
}

// function checks and returns size of given file.
long int fileSize(FILE * f)
{
	long int res, temp;
	temp = ftell(f);
	fseek(f, 0, SEEK_END);
	res = ftell(f);
	fseek(f, temp, SEEK_SET);
	return res;
}

// function free short term history.
void free_short_term_history(char *short_history_arr[])
{
	for (int i = 0; i < N; i++)
		free(short_history_arr[i]);
}

// function free apt list.
void free_apt_list(AptList* apt_list)
{
	AptListNode* p = apt_list->head;
	while (p&&p->next)
	{
		p = p->next;
		free(p->prev->apt.address);
		free(p->prev);
	}
	if (p)
	{
		free(p->apt.address);
		free(p);
	}
	apt_list->head = apt_list->tail = NULL;

}

// function free old history.
void free_old_history(HistoryList* old_term_history)
{
	HistoryNode *p = old_term_history->head, *to_del;
	while (p&&p->next)
	{
		to_del = p;
		p = p->next;
		free(to_del->data);
		free(to_del);
	}
	if (p)
	{
		free(p->data);
		free(p);
	}
}

// function gets the current time using ctime function. output format: string.
void current_time(short int *res)
{
	time_t current_time;
	char* c_time_string;

	current_time = time(&current_time);	// Obtain current time.
	c_time_string = ctime(&current_time); // Convert to local time format.

	if (c_time_string == NULL)
	{
		(void)fprintf(stderr, "Failure to convert the current time.\n");
		exit(EXIT_FAILURE);
	}

	date_sorting(c_time_string, res);
}

// function checks rather "max_date" is bigger then "date". 
int date_compare(short int* max_date, short int* date)
{
	if (max_date[YEAR] > date[YEAR])
		return TRUE;
	else if (max_date[YEAR] == date[YEAR])
	{
		if (max_date[MONTH] > date[MONTH])
			return TRUE;
		else if ((max_date[MONTH] == date[MONTH]) && (max_date[DAY] >= date[DAY]))
			return TRUE;
	}
	return FALSE;
}

// function finds a specific command from history according to the command number.
char* find_in_history(char** short_term_history, HistoryList old_term_history, char* command)
{
	char* res, *temp;
	int i, command_num, cur_num;
	HistoryNode* p = old_term_history.head;
	command_num = char_to_int(command);
	temp = read_str(*short_term_history);
	cur_num = char_to_int(temp);
	if (command_num > cur_num - 7) // checks if command located in short history list
	{
		for (i = 0; i < N; i++)
		{
			temp = read_str(short_term_history[i]); // reads the command number
			cur_num = char_to_int(temp); // transforms the command number to int
			if (command_num == cur_num) // checks if it's the requested command
			{
				res = (char*)malloc(strlen(short_term_history[i]) * sizeof(char));
				strcpy(res, short_term_history[i] + strlen(temp) + 1); // inserts the string (without command number) to history list
				return res;
			}
		}
	}
	else
	{
		while (p)
		{
			temp = read_str(p->data); // reads the command number
			cur_num = char_to_int(temp); // transforms the command number to int
			if (command_num == cur_num) // checks if it's the requested command
			{
				res = (char*)malloc(strlen(p->data) * sizeof(char));
				strcpy(res, p->data + strlen(temp) + 1);//enters the string without the command number in the history
				return res;
			}
			p = p->next;
		}
	}
	return NULL;
}

// function converts regEx history commands to known command.
char* history_string_convertor(char* line, char* command)
{
	char *res, *tmp_command1, *tmp_command2, *tmp;
	int result_size = 0;


	tmp_command2 = read_str(command);
	command += strlen(tmp_command2);
	tmp_command2 = read_str_to_convert(command);

	tmp = (char*)malloc(strlen(tmp_command2) * sizeof(char));
	strcpy(tmp, tmp_command2);

	command += strlen(tmp_command2) + 1;
	tmp_command1 = read_str_to_convert(command);

	if (strlen(tmp_command2) < strlen(tmp_command1))
	{
		tmp = (char*)realloc(tmp, strlen(tmp_command1) * sizeof(char));
		strcpy(tmp, tmp_command1);
		result_size = strlen(line) + strlen(tmp_command1) - strlen(tmp_command2) + 1;
	}
	else
	{
		tmp = (char*)realloc(tmp, strlen(tmp_command2) * sizeof(char));
		strcpy(tmp, tmp_command1);
		result_size = strlen(line) + strlen(tmp_command2) - strlen(tmp_command1) + 1;
	}


	res = replaceWord(line, tmp_command2, tmp);
	return res;
	free(res);
}

// function replace specific substring in a given string.
char *replaceWord(const char *s, const char *oldW, const char *newW)
{
	char *result;
	int i, cnt = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);

	// Counting the number of times old word occurs in the string
	for (i = 0; s[i] != '\0'; i++)
		if (strstr(&s[i], oldW) == &s[i])
		{
			cnt++;
			// Jumping to index after the old word.
			i += oldWlen - 1;
		}

	// Making new string of enough length
	result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

	i = 0;
	while (*s)
		if (strstr(s, oldW) == s) // compare the substring with the result
		{
			strcpy(&result[i], newW);
			i += newWlen;
			s += oldWlen;
		}
		else
			result[i++] = *s++;

	result[i] = '\0';
	return result;
}

// function helps to convert decimal to binary according to specific spec.
void bytes_saving_helper(AptListNode *p, unsigned char *byte1, unsigned char *byte2, unsigned char *byte3,
	unsigned char *byte4, unsigned char *byte5)
{
	unsigned char temp;
	*byte1 = p->apt.room_num << 4;
	temp = p->apt.date[DAY] >> 1;
	*byte1 = *byte1 | temp;
	*byte2 = p->apt.date[DAY] << 7;
	temp = p->apt.date[MONTH] << 3;
	*byte2 = *byte2 | temp;

	temp = p->apt.date[YEAR] >> 4;

	*byte2 = *byte2 | temp;
	*byte3 = p->apt.date[YEAR] << 4;

	*byte4 = p->apt.input_apt_date[DAY] << 3;
	temp = p->apt.input_apt_date[MONTH] >> 1;
	*byte4 = *byte4 | temp;
	*byte5 = p->apt.input_apt_date[MONTH] << 7;
	*byte5 = *byte5 | p->apt.input_apt_date[YEAR];
}


