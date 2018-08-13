

#include "apt.h"
#define _CRT_SECURE_NO_WARNINGS 


void main()
{
	AptList apt_list = make_new_apt_list();
	char *short_term_history[N] = { '\0' };
	HistoryList old_term_history = make_new_history_list();
	get_commands_from_txt(short_term_history, &old_term_history);
	get_apt_from_bin(&apt_list);

	printf("Please enter one of the following commands:\n"
		"add-an-apt, get-an-apt, buy-an-apt or delete-an-apt\n"
		"For reconstruction commands, please enter:\n"
		"!!, !num, history, short_history or !num^str1^str2\n"
		"To exit, enter exit.\n");

	functions_reader(&apt_list, short_term_history, &old_term_history);
	save_commands_history(short_term_history, old_term_history);
	save_apt_history(apt_list);
	free_all(short_term_history, &old_term_history, &apt_list);
}


