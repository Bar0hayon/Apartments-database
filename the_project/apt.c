#include "apt.h"
#define _CRT_SECURE_NO_WARNINGS 

// function add new apt to apt list.
void add_apt(AptList* apt_list, char* str)
{
	AptListNode* new_apt_node = make_new_apt_listnode();
	char *value;
	int short input_tmp_date[DATE_INT_SIZE]; // short int* to store to input date of the apt.
											 //puts user's input in the apartment node
	new_apt_node->apt.address = read_address(str);
	str += strlen(new_apt_node->apt.address) + 3;
	value = read_str(str);
	str += strlen(value);
	new_apt_node->apt.price = char_to_int(value);
	value = read_str(str);
	str += strlen(value) + 1;
	new_apt_node->apt.room_num = char_to_int(value);

	for (int i = 0; i < DATE_INT_SIZE; i++)
	{
		value = read_str(str);
		str += strlen(value) + 1;
		new_apt_node->apt.date[i] = char_to_int(value);
	}

	new_apt_node->apt.date[YEAR] += 2000; // set the current year according to the format.

	current_time(input_tmp_date); // insert formated input_apt_date values.
	new_apt_node->apt.input_apt_date[DAY] = input_tmp_date[DAY];
	new_apt_node->apt.input_apt_date[MONTH] = input_tmp_date[MONTH];
	new_apt_node->apt.input_apt_date[YEAR] = input_tmp_date[YEAR];

	insert_node_to_tail(apt_list, new_apt_node);
}

// function gets new apt.
void get_an_apt(AptList apt_list, char* str)
{
	Apartment *apt_arr;
	int max_price, max_room_num, min_room_num, arr_size;
	int s = FALSE, sr = FALSE;
	short int max_date[DATE_INT_SIZE], min_input_date[DATE_INT_SIZE];
	max_price = max_room_num = min_room_num = max_date[DAY] = max_date[MONTH] = max_date[YEAR] =
		min_input_date[DAY] = min_input_date[MONTH] = min_input_date[YEAR] = -1; // initialize all the variables.
	while (*str != '\0')
		str = str_translator(str, &max_price, &max_room_num, &min_room_num, &s, &sr, max_date, min_input_date);

	// build an array of apartments according to the user's demand.
	apt_arr = build_arr_from_list(apt_list, max_price, max_room_num, min_room_num, max_date, min_input_date, &arr_size);

	if (s) // checks rather its ascending / descending order.
		sort_low_to_high(apt_arr, arr_size);
	else if (sr)
		sort_high_to_low(apt_arr, arr_size);

	print_apt_arr(apt_arr, arr_size); // prints the apartments according to the given commands.
}

// function sorts an apartments array in descending order (highest to lowest price).
void sort_high_to_low(Apartment* apt_arr, int arr_size)
{
	Apartment temp;
	for (int i = 0; i<arr_size; i++)
		for (int j = 1; j<arr_size; j++)
			if (apt_arr[j - 1].price < apt_arr[j].price)
			{
				temp = apt_arr[j - 1];
				apt_arr[j - 1] = apt_arr[j];
				apt_arr[j] = temp;
			}
}

// function sorts an apartments array in ascending order (lowest to highest price).
void sort_low_to_high(Apartment* apt_arr, int arr_size)
{
	Apartment temp;
	for (int i = 0; i<arr_size; i++)
		for (int j = 1; j<arr_size; j++)
			if (apt_arr[j - 1].price > apt_arr[j].price)
			{
				temp = apt_arr[j - 1];
				apt_arr[j - 1] = apt_arr[j];
				apt_arr[j] = temp;
			}
}

// function prints an apartments array.
void print_apt_arr(Apartment* apt_arr, int arr_size)
{
	for (int i = 0; i < arr_size; i++)
		print_apt(&apt_arr[i]);
}

// function builds an array of apartments from a list according to the user demand.
Apartment* build_arr_from_list(AptList apt_list, int max_price, int max_room_num, int min_room_num,
	short int* max_date, short int* min_input_date, int* arr_size)
{
	Apartment* temp_arr = (Apartment*)malloc(LIST_SIZE_SET * sizeof(Apartment));
	AptListNode* p = apt_list.head;
	int size = 0, p_size = LIST_SIZE_SET;
	int short cur_date[DATE_INT_SIZE];
	current_time(cur_date);
	//builds a temperory array with all of the apartments that posest one or more of the user's demand
	while (p)
	{
		if ((max_price == -1 || max_price >= p->apt.price) && (max_room_num == -1 || max_room_num >= p->apt.room_num)
			&& (min_room_num == -1 || min_room_num <= p->apt.room_num) && (max_date[DAY] == -1 || date_compare(max_date, p->apt.date))
			&& (min_input_date[DAY] == -1 || date_compare(p->apt.input_apt_date, min_input_date)))
		{
			if (size == p_size) // if the logical size of the array is getting bigger then he's physical size
			{
				p_size *= 2;
				temp_arr = (Apartment*)realloc(temp_arr, p_size * sizeof(Apartment));
			}
			temp_arr[size] = p->apt;
			size++;
		}
		p = p->next;
	}
	if (size != p_size)
		temp_arr = (Apartment*)realloc(temp_arr, size * sizeof(Apartment));
	*arr_size = size;
	return temp_arr;
}

// function reads a single demand from the user and changes the proper varriable according to the user's demand.
char* str_translator(char* str, int* max_price, int* max_room_num, int* min_room_num, int* s,
	int* sr, short int* max_date, short int* min_input_date)
{
	char *command, *value = NULL;
	int days;
	command = read_str(str);
	str += strlen(command);
	if (strcmp(command, "-MaximumPrice") == 0)
	{
		value = read_str(str);
		str += strlen(value) + 2;
		*max_price = char_to_int(value);
	}
	else if (strcmp(command, "-MinimumNumRooms") == 0)
	{
		value = read_str(str);
		str += strlen(value) + 1;
		*min_room_num = char_to_int(value);
	}
	else if (strcmp(command, "-MaximumNumRooms") == 0)
	{
		value = read_str(str);
		str += strlen(value) + 1;
		*max_room_num = char_to_int(value);
	}
	else if (strcmp(command, "-Date") == 0)
	{
		value = read_str(str);
		str += strlen(value) + 1;
		//transforming the date into int array formation
		max_date[DAY] = (value[0] - '0') * 10 + (value[1] - '0');
		max_date[MONTH] = (value[2] - '0') * 10 + (value[3] - '0');
		max_date[YEAR] = (value[4] - '0') * 1000 + (value[5] - '0') * 100 + (value[6] - '0') * 10 + (value[7] - '0');
	}
	else if (strcmp(command, "-Enter") == 0)
	{
		value = read_str(str);
		str += strlen(value) + 3;
		days = char_to_int(value);
		subtract_day(min_input_date, days);
	}
	else if (strcmp(command, "-s") == 0) // checks if user wants the apartments to be sorted in ascending method.
		*s = TRUE;
	else if (strcmp(command, "-sr") == 0) // checks if user wants the apartments to be sorted in descending method.
		*sr = TRUE;

	return (str);
}

// function deletes an apartment from the list according to the apartment number.
void buy_an_apt(AptList* apt_list, char* line)
{
	char* str = read_str(line);
	int del_number = char_to_int(str); // gets the requested apartment number
	AptListNode* p = apt_list->head;
	while (p)
	{
		if (p->apt.number == del_number)
		{
			if (p == apt_list->head)
			{
				apt_list->head = apt_list->head->next;
				if (apt_list->head) // if thers more then one apartment on the list
					apt_list->head->prev = NULL;
				free(p->apt.address);
				free(p);
			}
			else if (p == apt_list->tail)
			{
				apt_list->tail = p->prev;
				apt_list->tail->next = NULL;
				free(p->apt.address);
				free(p);
			}
			else
			{
				p->next->prev = p->prev;
				p->prev->next = p->next;
				free(p->apt.address);
				free(p);
			}
			break;
		}
		p = p->next;
	}
}

// function insert commands to history list.
void insert_old_history(HistoryList* old_term_history, char* new_command)
{
	HistoryNode* new_node = make_new_HistoryNode();
	new_node->data = (char*)malloc((strlen(new_command) + 1) * sizeof(char));
	strcpy(new_node->data, new_command);
	if (!(old_term_history->head)) // checks if list is empty
		old_term_history->head = old_term_history->tail = new_node;
	else // insert node to head
	{
		new_node->next = old_term_history->head;
		old_term_history->head->prev = new_node;
		old_term_history->head = new_node;
	}
}

// function insert last command to short (7 commands at max') history and saved list.
void insert_short_history(char** short_term_history, char* line, HistoryList *old_term_history)
{
	int i, number;
	char *cur_number, *new_number;
	if (*short_term_history == NULL) // checks if history list is empty
	{
		*short_term_history = (char*)malloc((strlen(line) + 3) * sizeof(char));
		short_term_history[0][0] = '1'; // set the number of the first command
		short_term_history[0][1] = ' ';
		strcpy(*short_term_history + 2, line);
	}
	else
	{
		if (short_term_history[N - 1]) // if short term history is full(7 commands)
			insert_old_history(old_term_history, short_term_history[N - 1]);
		for (i = N - 1; i > 0; i--) // shift the old commands
		{
			if (short_term_history[i - 1])
			{
				if (short_term_history[i])
				{
					short_term_history[i] = (char*)realloc(short_term_history[i], strlen(short_term_history[i - 1]) * sizeof(char) + 1);
					strcpy(short_term_history[i], short_term_history[i - 1]);
				}
				else
				{
					short_term_history[i] = (char*)realloc(short_term_history[i], strlen(short_term_history[i - 1]) * sizeof(char) + 1);
					strcpy(short_term_history[i], short_term_history[i - 1]);
				}
			}
		}
		cur_number = read_str(short_term_history[i]); // reads the previous number in the history
		number = char_to_int(cur_number);
		number++;
		new_number = int_to_char(number);
		*short_term_history = (char*)realloc(*short_term_history, (strlen(new_number)*strlen(line) + 1) * sizeof(char));
		strcpy(*short_term_history, new_number); // set the new command number
		short_term_history[0][strlen(new_number)] = ' ';
		strcpy(*short_term_history + strlen(new_number) + 1, line);
	}
}

// function deletes all the apartments which entered in the last "x" days to the data base.
void delete_an_apt(AptList* apt_list, char* str)
{
	char *temp = read_str(str);
	str += strlen(temp) + 1;
	temp = read_str(str);
	int days = char_to_int(temp);
	int short date[DATE_INT_SIZE];
	AptListNode* p = apt_list->tail;
	subtract_day(date, days); // gets the minimum date to stay in the list
	while (!date_compare(date, p->apt.input_apt_date)) // deletes all the apartments that entered after "date"
	{
		if (p == apt_list->head)
		{
			free(p->apt.address);
			free(p);
			apt_list->head = apt_list->tail = NULL;
		}
		else
		{
			p = p->prev;
			apt_list->tail = p;
			free(p->next->apt.address);
			free(p->next);
			p->next = NULL;
		}
	}
}

// function reads commands from prompt.
void functions_reader(AptList* apt_list, char** short_term_history, HistoryList *old_term_history)
{
	char *command, *line, *history_line=NULL;
	printf(">>");
	line = get_line();
	command = read_str(line);
	while (strcmp(command, "exit"))
	{
		if (!strcmp(command, "add-an-apt"))
		{
			insert_short_history(short_term_history, line, old_term_history);
			line += strlen(command) + 1;
			add_apt(apt_list, line);
		}
		else if (!strcmp(command, "get-an-apt"))
		{
			insert_short_history(short_term_history, line, old_term_history);
			line += strlen(command) + 1;
			get_an_apt(*apt_list, line);
		}
		else if (!strcmp(command, "buy-an-apt"))
		{
			insert_short_history(short_term_history, line, old_term_history);
			line += strlen(command) + 1;
			buy_an_apt(apt_list, line);
		}
		else if (!strcmp(command, "delete-an-apt"))
		{
			insert_short_history(short_term_history, line, old_term_history);
			line += strlen(command) + 1;
			delete_an_apt(apt_list, line);
		}
		else if (!strcmp(command, "short_history"))
			print_short_history(short_term_history);
		else if (!strcmp(command, "history"))
			print_all_history(short_term_history, *old_term_history);

		if (!strcmp(command, "!!"))
		{//gets the last command in the history
			line = (char*)realloc(line, strlen(*short_term_history) * sizeof(char));
			command = read_str(*short_term_history);//gets the number at the begginnig of the string
			strcpy(line, *short_term_history + strlen(command) + 1);
			command = read_str(line);
		}
		else if (line[0] == '!')
		{
			if (strstr(line, "^") == NULL)//if theres 
			{
				line += 1;
				command = read_str(line);//gets the command number in the history as a string
				line = find_in_history(short_term_history, *old_term_history, command);
				command = read_str(line);
			}
			else
			{
				line += 1;
				command = read_str(line);
				history_line = find_in_history(short_term_history, *old_term_history, command);
				history_line = history_string_convertor(history_line, line);
				strcpy(line, history_line);
				command = read_str(history_line);
			}
		}
		else
		{
			printf(">>");
			line = get_line();
			command = read_str(line);
		}
	}
	free(command);
	free(line);
	free(history_line);
	printf("Good Bye!\n");
}

// function saves the commands that were called in program, in "commands_history.txt".
void save_commands_history(char** short_term_history, HistoryList old_term_history)
{
	FILE* f = fopen("commands_history.txt", "w");
	int i, last_cmd_number, size;
	char* temp = read_str(*short_term_history);
	HistoryNode* p = old_term_history.head;
	last_cmd_number = char_to_int(temp);
	if (last_cmd_number <= N)
		for (i = 0; i < last_cmd_number; i++)
		{
			size = strlen(short_term_history[i]);
			fprintf(f, "%d %s", size, short_term_history[i]);
		}
	else
	{
		for (i = 0; i < N; i++)
		{
			size = strlen(short_term_history[i]);
			fprintf(f, "%d %s", size, short_term_history[i]);
		}
		while (p)
		{
			size = strlen(p->data);
			fprintf(f, "%d %s", size, p->data);
			p = p->next;
		}
	}
	fclose(f);
}

// function gets the commands from the txt file.
void get_commands_from_txt(char** short_term_history, HistoryList *old_term_history)
{
	FILE* f = fopen("commands_history.txt", "r");
	int i, size, command_num = 0;
	char eat_spaces, *command;
	HistoryNode* p = make_new_HistoryNode();
	long int fsize = fileSize(f);
	//if (!f)
	//	return;

	for (i = 0; i < N && (ftell(f) < fsize); i++)//input data to short history
	{
		fscanf(f, "%d%c", &size, &eat_spaces);
		short_term_history[i] = (char*)malloc((size + 1) * sizeof(char));
		fgets(short_term_history[i], (size + 1) * sizeof(char), f);
		short_term_history[i][size] = '\0';
	}
	if ((ftell(f) < fsize))
	{
		fscanf(f, "%d%c", &size, &eat_spaces);
		p->data = (char*)malloc((size + 1) * sizeof(char));
		fgets(p->data, (size + 1) * sizeof(char), f);
		p->data[size] = '\0';
		insert_historyNode_to_tail(old_term_history, p);
		command = read_str(p->data);
		command_num = char_to_int(command);
	}
	while ((ftell(f) < fsize) && command_num != 1)
	{
		p->next = make_new_HistoryNode();
		fscanf(f, "%d%c", &size, &eat_spaces);
		p->next->data = (char*)malloc((size + 1) * sizeof(char));
		fgets(p->next->data, (size + 1) * sizeof(char), f);
		p->next->data[size] = '\0';
		insert_historyNode_to_tail(old_term_history, p->next);
		p = p->next;
		command = read_str(p->data);
		command_num = char_to_int(command);
	}

	fclose(f);
}

// function save apt to binary file ("apt_history.bin") according to spec.
void save_apt_history(AptList apt_list)
{
	unsigned char byte1;//4 bits to num of rooms and 4 bits to day(4 out of 5)
	unsigned char byte2;//1 remaining bit of day,4 bits of month,3 of year(3 out of 7)
	unsigned char byte3;//4 remainin bits from the year(at the left 4 bits)
	unsigned char byte4;//5 bits for input day and 3 bits for input month(3 out of 4)
	unsigned char byte5;//1 remaining bite for input month and 7 bits for input year

	AptListNode *p = apt_list.head;
	short int addressLen;
	FILE* file = fopen("apt_history.bin", "wb");

	while (p)
	{
		p->apt.date[YEAR] -= 2000;
		p->apt.input_apt_date[YEAR] -= 2000;
		addressLen = (short int)strlen(p->apt.address);
		fwrite(&(p->apt.number), sizeof(short int), 1, file);
		fwrite(&addressLen, sizeof(short int), 1, file);
		fwrite(p->apt.address, sizeof(char), addressLen, file);
		fwrite(&(p->apt.price), sizeof(int), 1, file);
		bytes_saving_helper(p, &byte1, &byte2, &byte3, &byte4, &byte5);
		fwrite(&byte1, sizeof(unsigned char), 1, file);
		fwrite(&byte2, sizeof(unsigned char), 1, file);
		fwrite(&byte3, sizeof(unsigned char), 1, file);
		fwrite(&byte4, sizeof(unsigned char), 1, file);
		fwrite(&byte5, sizeof(unsigned char), 1, file);
		p = p->next;
	}

	fclose(file);
}

// function gets apt from binary file and arange the apt's in a list.
void get_apt_from_bin(AptList *apt_list)
{
	FILE* file = fopen("apt_history.bin", "rb");
	AptListNode *p = make_new_apt_listnode();
	short int addressLen;
	int flag = FALSE; // flag to cheack rather file is empty
	unsigned char temp;
	long int fsize = fileSize(file);
	if (!file)
		return;
	while (ftell(file) < fsize)
	{
		fread(&(p->apt.number), sizeof(short int), 1, file);
		fread(&addressLen, sizeof(short int), 1, file);
		p->apt.address = (char*)calloc(addressLen + 1, sizeof(char));
		fread(p->apt.address, sizeof(char), addressLen, file);
		fread(&(p->apt.price), sizeof(int), 1, file);
		fread(&temp, sizeof(unsigned char), 1, file);
		p->apt.room_num = (temp >> 4);
		temp = temp << 4;
		temp = temp >> 3;
		p->apt.date[DAY] = temp;
		fread(&temp, sizeof(unsigned char), 1, file);
		p->apt.date[DAY] = p->apt.date[DAY] | (temp >> 7);
		temp = temp << 1;
		p->apt.date[MONTH] = temp >> 4;
		temp = temp << 4;
		p->apt.date[YEAR] = temp >> 1;
		fread(&temp, sizeof(unsigned char), 1, file);
		p->apt.date[YEAR] = p->apt.date[YEAR] | (temp >> 4);

		fread(&temp, sizeof(unsigned char), 1, file);
		p->apt.input_apt_date[DAY] = temp >> 3;
		temp = temp << 5;
		p->apt.input_apt_date[MONTH] = temp >> 4;
		fread(&temp, sizeof(unsigned char), 1, file);
		p->apt.input_apt_date[MONTH] = p->apt.input_apt_date[MONTH] | (temp >> 7);
		temp = temp << 1;
		p->apt.input_apt_date[YEAR] = temp >> 1;

		p->apt.date[YEAR] += 2000;
		p->apt.input_apt_date[YEAR] += 2000;
		insert_node_to_tail(apt_list, p);
		p->next = make_new_apt_listnode();
		p = p->next;
		flag = TRUE;
	}
	if (flag)
	{
		free(p);
		apt_list->tail->next = NULL;
	}
	fclose(file);
}

// function free all allocation existed.
void free_all(char * short_term_history[], HistoryList * old_term_history, AptList * apt_list)
{
	free_apt_list(apt_list);
	free_old_history(old_term_history);
	free_short_term_history(short_term_history);
}

