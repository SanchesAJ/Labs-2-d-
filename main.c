#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define ADD '+'
#define SUB '-'
#define MUL '*'
#define DIV '/'
// '@'-- условный стоп-символ на всём протяжении кода.

//ERROR-коды
#define SYNTAXIX_ERROR {printf ("syntax error");}
#define ZERO {printf ("division by zero");}
#define OPER {(('+')||('-')||('*')||('/'))}
//STACK

typedef struct _List {
	char val;
	struct _List *next;
} List;
typedef struct _Stack {
	List *head;
} Stack;
typedef enum ErrorCode {
	GOOD,
	MEMORY_ERROR,
	VOID,
	NO
} Error;
List *createList(char value) {
	List *l = malloc(sizeof(List));
	if (l == NULL) {
		printf("Error");
		return NULL;
	}
	l->val = value;
	l->next = NULL;
	return l;
}
Error push(Stack *s, char el) {
	List *l = createList(el);
	if (l == NULL)
		return MEMORY_ERROR;
	l->next = s->head;
	s->head = l;
	return GOOD;
}
int isEmpty(Stack *f) {
	return f->head == NULL ? 1 : 0;
}
char peek(Stack *s) {
	if ((s->head->val) == NULL)
		return NULL;
	else
		return s->head->val;
}
Error pop(Stack *s, char *a) {
	if (s->head == NULL)
		return VOID;
	*a = s->head->val;
	List *oldHead = s->head;
	s->head = s->head->next;
	free(oldHead);
	return GOOD;
}
//Я знаю что его нужно вынести в библиотеку. Но не могу это седелать сейчас.
int  convert(char *str, int num) {
	int f = 0;
	num -= 1;
	int r = num;
	for (int i = 0; i < r; i++) {
		char d = str[i];
		str[i] = str[r];
		str[r] = d;
		r--;
	}
	int i = 0;
	r = 1;
	while ((str[num] <= '9') && (str[num] >= '0')) {
		f += (str[num] - '0')*r;
		r *= 10;
		num--;
	}


	return f;
}
Error chek(char *e) {
	int i = 0; char proxy = '@'; int count = 0;
	Stack *s = malloc(sizeof(Stack));
	push(s, proxy);
	while (e[i] != NULL) {
		if (!((e[i] == '+') || (e[i] == '-') || (e[i] == '*') || (e[i] == '/') || (e[i] == '(') || (e[i] == ')') || ((e[i] <= '9') && (e[i] >= '0')))) {
			SYNTAXIX_ERROR
				return VOID;
		}
		if ((e[i] == '+') || (e[i] == '-') || (e[i] == '*') || (e[i] == '/')) {
			if ((e[i - 1] == '+') || (e[i - 1] == '-') || (e[i - 1] == '*') || (e[i - 1] == '/')) {
				SYNTAXIX_ERROR
					return VOID;
			}

		}


		if (e[i] == '.') {
			SYNTAXIX_ERROR
				return VOID;
		}
		if (e[i] == '(') {
			push(s, e[i]);
		}
		if (e[i] == ')') {
			if (peek(s) == '@') {
				SYNTAXIX_ERROR
					return VOID;
			}
			pop(s, &proxy);
		}
		if ((e[i - 1] == '(') && (e[i] == ')')) {
			SYNTAXIX_ERROR
				return VOID;
		}



		i++;

	}

	if (peek(s) != '@') {
		SYNTAXIX_ERROR
			return VOID;
	}
	return GOOD;
	free(s);
}
//предварительная проверка
int prior(char a) {
	if ((a == '+') || (a == '-'))
		return 0;
	if ((a == '*') || (a == '/'))
		return 1;
}
// Польская запись
Stack  *toPoland(char *ex) {
	int  i = 0;
	char proxy = '@';
	Stack *s = malloc(sizeof(Stack));
	Stack *node = malloc(sizeof(Stack));
	push(s, proxy);
	push(node, proxy); //костыли, но без них никак
	while (ex[i - 1] != '@') {
		char red = ex[i];

		//считали токен
		if ((red != '+') && (red != '-') && (red != '*') && (red != '/') && (red != '(') && (red != ')') && (red != '@')) {
			push(s, red);
		}
		//первый этап

		if ((red == '+') || (red == '-') || (red == '*') || (red == '/')) {

			while (((peek(node) == '+') || (peek(node) == '-') || (peek(node) == '*') || (peek(node) == '/'))) {
				pop(node, &proxy);
				if (prior(proxy) >= prior(red)) {
					push(s, proxy);
				}
				else {
					push(node, proxy);
					push(node, red);
					break;
				}
			}
			if ((peek(node) == '@') || (peek(node) == '('))
				push(node, red);

		}
		if ((red == '(')) {
			push(node, red);
		}
		if ((red == ')')) {
			if ((peek(node) == '(')&&(peek(s)!=' '))
			{
				SYNTAXIX_ERROR
					return NULL;
			}

			while (peek(node) != '(') {
				pop(node, &proxy);
				push(s, proxy);
				if (peek(node) == '@') {
					SYNTAXIX_ERROR
						return NULL;
				}

			}
			pop(node, &proxy);
		}
		if (ex[i] == '@') {

			while (peek(node) != '@') {
				if ((red == '('))
				{
					SYNTAXIX_ERROR
						return NULL;
				}

				pop(node, &proxy);
				push(s, proxy);

			}
		}
		i++;


	}
	return s;

}
//Перевод в Польскую нотацию
Stack *Regroup(Stack *s) {
	char proxy = '@';
	Stack *node = malloc(sizeof(Stack));
	push(node, proxy);

	while (peek(s) != '@') {
		pop(s, &proxy);
		push(node, proxy);
	}
	return node;
}
//Переворот стека
int toint(Stack *s) {
	char proxy;
	if (peek(s) == ' ')
		pop(s, &proxy);

	int count = 0;

	char str[10];
	while ((peek(s) != ' ') && (s->head->val != '@')) {
		pop(s, &str[count]);
		count++;
	}
	if (str[0]!='-')
	return convert(str, count);
	else {
		return -1*convert(&str[1], count - 1);
	}

}

void tochar(int Num, Stack *s) {
	char str[10];
	int i = 0;
	int r = 10;
	
	if (Num < 0) {
		str[0] = '-';
		i++;
		Num *= -1;
	}
	while (Num != 0) {
		str[i] = Num % r + '0';
		i++;
		Num /= 10;
	}

	str[i] = '@';
	for (int j = 0; j < i - 1; j++) {
		char d = str[j];
		str[j] = str[i - 1];
		str[i - 1] = d;
		i--;
	}

	i = 0;
	while (str[i] != '@') {
		push(s, str[i]);
		i++;
	}
	push(s, ' ');
}
void Calculator(Stack *s) {
	char proxy = '@';
	Stack *node = malloc(sizeof(Stack));
	push(node, proxy);
	char red;

	while (peek(s) != '@')
	{

		pop(s, &red);

		if ((red != '+') && (red != '-') && (red != '*') && (red != '/'))
			push(node, red);

		if ((red == '+') || (red == '-') || (red == '*') || (red == '/')) {
			if (peek(node) == '@') {
				SYNTAXIX_ERROR
					return NULL;
			}
			switch (red)
			{
			case ADD: {
				int one = toint(node);
				if ((one == '@') || (peek(node) == '@')) {
					SYNTAXIX_ERROR
						return NULL;
				}
				int two = toint(node);

				
				int R = one + two;

				tochar(R, node);
				break;
			}
			case SUB: {
				int one = toint(node);
				if ((one == '@') || (peek(node) == '@')) {
					SYNTAXIX_ERROR
						return NULL;
				}

				int two = toint(node);

				

				int R = two - one;

				tochar(R, node);
				break;
			}
			case MUL: {
				int one = toint(node);
				if ((one == '@') || (peek(node) == '@')) {
					SYNTAXIX_ERROR
						return NULL;
				}
				int two = toint(node);

				


				int R = one * two;

				tochar(R, node);
				break;
			}
			case DIV: {
				int one = toint(node);
				if ((one == '@') || (peek(node) == '@')) {
					SYNTAXIX_ERROR
						return NULL;
				}

				int two = toint(node);

				

				if (one == 0) {
					ZERO
						return NULL;
				}
				int R = two / one;
				tochar(R, node);
				break;
			}

			default:
				break;
			}

		}
	}





	printf("%d", toint(node));
	
	//Отладить вычисления. Но оно работет... Через раз... 
	return;

}
//Счёт

void Correct(char *e) {
	int i = 0; int count = 0;
	int len = 0;
	while (e[i] != '@') {
		len++;
		i++;
	}//немного корявый счёт
	i = 0;
	while (e[i] != '@') {
		count = 0;
		if ((e[i] <= '9') && (e[i] >= '0')) {
			while ((e[i] <= '9') && (e[i] >= '0'))
				i++;
			for (int r = len; i - 1 < r; r--) {
				e[r + 1] = e[r];
			}
			e[i] = ' ';
			len++;
		}
		i++;
	}
	return;
}
//Корректировка чисел для удобства 
int NoOPS(char *e) {
	int count = 0;
	char real[100]; int i = 0;
	while (e[i] != '@') {
		if ((e[i] == '+') || (e[i] == '-') || (e[i] == '*') || (e[i] == '/') /*|| (e[i] == '(') || (e[i] == ')')*/)
			count++;
		i++;
	}
	i = 0; int j = 0;
	if (count == 0) {
		while (e[i] != '@') {
			if (((e[i] <= '9') && (e[i] >= '0'))) {
				real[j] = e[i];
				j++;
			}
			i++;
		}
		real[j] = '\0';
	}
	else return 1;

	printf("%s", real);
	return 0;
}
int main() {

	char example[2048];
	gets_s(example, 1024);

	if (chek(example) == VOID) {
		return 0;
	}

	example[strlen(example)] = '@';
	if (example[0] == '@') {
		SYNTAXIX_ERROR
			return 0;
	}

	if (!NoOPS(example))
		return 0;

	Correct(&example);
	//размечаем знаки  
	Stack *s = toPoland(example);
	/*Обратная Польская запись */



	if (s == NULL)
		return 0;
	s = Regroup(s);
	Calculator(s);
	//Подсчёт


	free(s);
	return 0;
}