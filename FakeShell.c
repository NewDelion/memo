#include <stdio.h>
#include <string.h>
#include <windows.h>

void shell();

typedef struct {
	char *cmd;
	char **args;
	int args_count;
} INPUT_DATA;

int main(void) {
	shell();
	
	return 0;
}

char* readline() {
	char *input = calloc(1, 1);
	char buff[2];

	while (fgets(buff, 2, stdin)) {
		if (strcmp(buff, "\n") == 0)
			break;
		input = realloc(input, strlen(input) + 2);
		strcat(input, buff);
	}
	
	return input;
}

char* get_command(char *input) {
	char *result;
	int size = 0, max = strlen(input);

	while (size < max && *(input + size) != ' ')
		size++;
	result = (char*)calloc(size + 1, 1);
	strncpy(result, input, size);

	return result;
}

INPUT_DATA read_input_data(char *input) {
	char *cmd = NULL, **args = NULL;
	char next_target = ' ';
	int args_count = 0;
	int cmd_size = 0, arg_size = 0, total_size = 0, max = strlen(input);
	char flg_esc = 0, flg_can_esc = 0;
	INPUT_DATA result;

	while (total_size < max && *(input + cmd_size) != ' ')
		total_size = ++cmd_size;
	cmd = (char*)calloc(cmd_size + 1, 1);
	strncpy(cmd, input, cmd_size);

	args_count = 0;
	while (total_size < max) {
		//スペース読み飛ばし
		while (total_size < max && *(input + total_size) == ' ')
			total_size++;
		if (total_size == max)
			break;
		args_count++;
		next_target = ' ';
		if (*(input + total_size) == '\"' || *(input + total_size) == '\'')
			next_target = *(input + total_size);
		if (!(arg_size = (next_target == ' '))) {
			total_size++;
			flg_can_esc = 1;
		}
		while (total_size + arg_size < max) {
			if (flg_can_esc && *(input + total_size + arg_size) == '\\' && !flg_esc)
				flg_esc = 1;
			else if (*(input + total_size + arg_size) == next_target && (!flg_can_esc || !flg_esc))
				break;
			else
				arg_size++;
		}
		if (args_count == 1)
			args = (char**)malloc(sizeof(char*));
		else
			args = realloc(args, args_count * sizeof(char*));
		*(args + args_count - 1) = calloc(arg_size + 1, 1);
		if (arg_size > 0)
			strncpy(*(args + args_count - 1), input + total_size, arg_size);
		total_size += arg_size + (next_target != ' ');
	}
	result.cmd = cmd;
	result.args = args;
	result.args_count = args_count;
	return result;
}

void command_ls(char *path) {
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	wchar_t *target;

	target = (wchar_t*)malloc((wcslen(path) + wcslen(L"\\*") + 1) * sizeof(wchar_t));
	wcscpy(target, path);
	wcscat(target, L"\\*");

	if ((hFind = FindFirstFile(target, &win32fd)) == INVALID_HANDLE_VALUE)
		return;
	free(target);
	target = NULL;

	do {
		if (wcscmp(L".", win32fd.cFileName) != 0 && wcscmp(L"..", win32fd.cFileName) != 0)
			printf("[%c] %ls\n", win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? 'D' : 'F', win32fd.cFileName);
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return;
}

void shell() {
	int i;
	char *input;
	char exit = 0;
	TCHAR current_dir[MAX_PATH];
	INPUT_DATA data;
	
	GetCurrentDirectory(MAX_PATH, current_dir);

	while (!exit) {
		printf(">> ");
		input = readline();
		data = read_input_data(input);

		if (strcmp(data.cmd, "exit") == 0)
			exit = 1;
		else if (strcmp(data.cmd, "pwd") == 0)
			printf("%ls\n", current_dir);
		else if (strcmp(data.cmd, "cd") == 0)
			;
		else if (strcmp(data.cmd, "ls") == 0)
			command_ls(current_dir);
		else if (strcmp(data.cmd, "test") == 0) {
			printf("Command: %s\n", data.cmd);
			for (i = 0; i < data.args_count; i++)
				printf("Args[%d]: %s\n", i, data.args[i]);
		}
		else if (strcmp(data.cmd, "") != 0)
			printf("不明なコマンド\n");

		free(input);
		input = NULL;
		free(data.cmd);
		data.cmd = NULL;
		for (i = 0; i < data.args_count; i++) {
			free(*(data.args + i));
			*(data.args + i) = NULL;
		}
		free(data.args);
		data.args = NULL;
	}

	printf("press any key...");
	getchar();
}
