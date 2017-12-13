#include "stdafx.h"
#include "stdlib.h"


struct member
{
	int len = -1;
	int sign = 1;
	int *num = NULL;
	void signChange()
	{
		sign *= -1;
	}
	void useSign()
	{
		for (int i = 0; i < len; i++)
		{
			num[i] *= sign;
		}
	}
};
struct plan
{
	int *line = NULL;
	int len = 0;
	void remove(int n)
	{
		len--;
		for (int i = n; i < len; i++)
			line[i] = line[i + 1];
	}
};
struct error
{
	enum er { Input = 1, FileOpen, Memory, Div0 };
	char List[5][100] = { "","Input","FileOpen","Memory","Div0" };
	int n = 0;
	void print(char *outputPath)
	{
		if (outputPath != NULL)
		{
			FILE *fp;
			fopen_s(&fp, outputPath, "w");
			fprintf(fp, "ERROR\t");
			fprintf(fp, "<%s>", List[n]);
			fclose(fp);
		}
		else
		{
			print(n);
		}
	}
	void print(int e)
	{
		printf("ERROR");
		printf("<{0}>", List[e]);
	}
	void print()
	{
		printf("ERROR");
		printf("<{0}>", List[n]);
	}
};
struct expression
{
	enum er { Input = 1, FileOpen, Memory, Div0 };
	plan outline;
	member *memb = NULL;
	error Error;
	char *op = NULL;
	int n = 0;

	expression(char *inputPath)
	{
		if (inputPath != NULL)
		{
			if (!(Error.n = (errorCheck(inputPath))))
				input(inputPath);
		}
		else
			Error.n = Input;
	}

	int errorCheck(char *inputPath)
	{
		FILE *fp;
		int br = 0;
		char curr = NULL;
		char prev = NULL;
		if (fopen_s(&fp, inputPath, "r") != NULL)
			return FileOpen;
		while (curr != EOF && curr != '\n')
		{
			curr = getc(fp);

			if ((curr > '9' || curr < '0') && curr != '*' && curr != '/' && curr != '+' && curr != '-' && curr != '(' && curr != ')' && curr != EOF && curr != '\n' && curr != '\0')
				return Input;
			else if ((curr == EOF || curr == '\n' || curr == '\0') && (prev == '+' || prev == '-' || prev == '*' || prev == '/'))
				return Input;
			else if ((curr == '+' || curr == '-' || curr == '*' || curr == '/') && (prev == '+' || prev == '-' || prev == '*' || prev == '/'))
				return Input;
			else if (curr >= '0' && curr <= '9' && prev == ')')
				return Input;
			else if (curr == ')' && prev == '(')
				return Input;
			else if (curr == '(' || curr == ')')
			{
				if (curr == '(')
					br++;
				else if (br > 0)
					br--;
				else return Input;
			}

			prev = curr;

		}
		if (br != 0)
			return Input;
		fclose(fp);
		return 0;
	}

	void input(char *inputPath)
	{
		FILE *fp;
		int start = -1;
		int i = 0;
		char prev = NULL;
		char curr = NULL;
		fopen_s(&fp, inputPath, "r");
		outline.len++;
		outline.line = (int*)realloc(outline.line, outline.len * sizeof(int));
		outline.line[outline.len - 1] = -1;
		while (curr != EOF)
		{
			curr = getc(fp);
			i++;

			if (curr <= '9' && curr >= '0' && (prev<'0' || prev>'9'))
			{
				n++;
				memb = (member*)realloc(memb, n * sizeof(member));
				memb[n - 1].sign = prev == '-' ? -1 : 1;
				memb[n - 1].num = NULL;

				op = (char*)realloc(op, n * sizeof(char));
				op[n - 1] = NULL;

				outline.len++;
				outline.line = (int*)realloc(outline.line, outline.len * sizeof(int));
				outline.line[outline.len - 1] = n - 1;

				start = i;
			}
			if (prev <= '9' && prev >= '0' && (curr<'0' || curr>'9'))
			{
				memb[n - 1].len = i - start;
				for (int j = 0; j < memb[n - 1].len / 2; j++)
				{
					start = memb[n - 1].num[j];
					memb[n - 1].num[j] = memb[n - 1].num[memb[n - 1].len - j - 1];
					memb[n - 1].num[memb[n - 1].len - j - 1] = start;
				}
				start = -1;
			}
			if (start != -1)
			{
				memb[n - 1].num = (int*)realloc(memb[n - 1].num, (i - start + 1) * sizeof(int));
				memb[n - 1].num[i - start] = curr - '0';
			}

			if (n > 0 && start == -1 && op[n - 1] == NULL && (curr == '+' || curr == '-' || curr == '*' || curr == '/'))
			{
				op[n - 1] = curr;
				curr = NULL;
			}
			if (curr == '(')
			{
				outline.len++;
				outline.line = (int*)realloc(outline.line, outline.len * sizeof(int));
				if (prev == '-')
				{
					outline.line[outline.len - 1] = -3;
				}
				else
				{
					outline.line[outline.len - 1] = -1;
				}
			}
			if (curr == ')')
			{
				outline.len++;
				outline.line = (int*)realloc(outline.line, outline.len * sizeof(int));
				outline.line[outline.len - 1] = -2;
			}

			prev = curr;
		}
		outline.len++;
		outline.line = (int*)realloc(outline.line, outline.len * sizeof(int));
		outline.line[outline.len - 1] = -2;
		fclose(fp);
	}

	void output()
	{
		if ((memb[n - 1].sign == -1) && (memb[n - 1].len != 1 || memb[n - 1].num[0] != 0))
			printf("-");
		for (int i = memb[n - 1].len - 1; i >= 0; i--)
			printf("%d", memb[n - 1].num[i]);
	}

	void output(char* path)
	{
		if (path != NULL)
		{
			FILE *fp;
			fopen_s(&fp, path, "w");
			if ((memb[n - 1].sign == -1) && (memb[n - 1].len != 1 || memb[n - 1].num[0] != 0))
				fprintf(fp, "-");
			for (int i = memb[n - 1].len - 1; i >= 0; i--)
				fprintf(fp, "%d", memb[n - 1].num[i]);
			fclose(fp);
		}
		else
		{
			output();
		}
	}

	void math()
	{
		void mFunc(member, member, member*, char);
		int i;
		int end;
		int start;
		while (outline.len > 1)
		{
			end = 0;
			while (outline.line[end] != -2)
			{
				if (outline.line[end] == -1 || outline.line[end] == -3)
					start = end;
				end++;
			}
			if (end - start > 2)
			{
				for (i = start + 1; i < end - 1; i++)
					if (op[outline.line[i]] == '*' || op[outline.line[i]] == '/')
					{
						mFunc(memb[outline.line[i]], memb[outline.line[i + 1]], &memb[outline.line[i + 1]], op[outline.line[i]]);
						if (memb[outline.line[i + 1]].len == -1)
						{
							Error.n = Div0;
							return;
						}
						outline.remove(i);
						end--;
						i--;
					}
				for (i = start + 1; i < end - 1; i++)
					if (op[outline.line[i]] == '+' || op[outline.line[i]] == '-')
					{
						mFunc(memb[outline.line[i]], memb[outline.line[i + 1]], &memb[outline.line[i + 1]], op[outline.line[i]]);
						outline.remove(i);
						end--;
						i--;
					}
			}
			if (outline.line[start] == -3)
				memb[outline.line[start + 1]].signChange();
			outline.remove(end);
			outline.remove(start);
		}
	}
};


void sum(member, member, member*);
void div(member, member, member*);
void mult(member, member, member*);

int main(int argc, char **argv)
{
	expression expr(argv[1]);
	if (expr.Error.n)
		expr.Error.print(argv[2]);
	expr.math();
	if (expr.Error.n)
		expr.Error.print(argv[2]);
	expr.output(argv[2]);
}

void mFunc(member a, member b, member *res, char op)
{
	switch (op)
	{
	case '+':
	{
		for (int i = 0; i < b.len; i++)
			b.num[i] *= b.sign;
		for (int i = 0; i < a.len; i++)
			a.num[i] *= a.sign;
		if (a.len > b.len)
			sum(a, b, res);
		else sum(b, a, res);
		break;
	}
	case '-':
	{
		for (int i = 0; i < b.len; i++)
			b.num[i] *= (b.sign*(-1));
		for (int i = 0; i < a.len; i++)
			a.num[i] *= a.sign;
		if (a.len > b.len)
			sum(a, b, res);
		else sum(b, a, res);
		break;
	}
	case '*':
	{
		res->sign = a.sign*b.sign;
		if (a.len > b.len)
			mult(a, b, res);
		else mult(b, a, res);
		break;
	}
	case '/':
	{
		if (b.len == 1 && b.num[0] == 0)
		{
			res->len = -1;
			break;
		}
		res->sign = a.sign*b.sign;
		div(a, b, res);
		break;
	}
	default:
		break;
	}
}

int compare(member a, member b)
{
	if (a.len > b.len)
		return 1;
	else if (b.len > a.len)
		return 0;
	else for (int i = a.len - 1; i >= 0; i--)
		if (a.num[i] > b.num[i])
			return 1;
		else if (b.num[i] > a.num[i])
			return 0;
	return 2;
}

void sum(member a, member b, member *res)
{
	res->len = a.len + 1;
	res->num = (int*)malloc((res->len + 1) * sizeof(int));
	res->sign = 1;
	memset(res->num, 0, res->len * sizeof(int));
	for (int i = 0; i < b.len; i++)
	{
		res->num[i] = a.num[i] + b.num[i];
		if (res->num[i] < 0)
		{
			res->num[i] += 10;
			res->num[i + 1]--;
		}
		if (res->num[i] > 9)
		{
			res->num[i] -= 10;
			res->num[i + 1]++;
		}
	}
	for (int i = b.len; i < a.len; i++)
	{
		res->num[i] = a.num[i];
		if (res->num[i] < 0)
		{
			res->num[i] += 10;
			res->num[i + 1]--;
		}
		if (res->num[i] > 9)
		{
			res->num[i] -= 10;
			res->num[i + 1]++;
		}
	}
	if (res->num[res->len - 1] < 0)
	{
		if (res->num[res->len - 1] == -1)
		{
			res->len--;
			res->num[res->len] = 0;
		}
		else
		{
			res->num[res->len - 1] += 10;
		}
		res->sign = -1;
		res->num[0] = 10 - res->num[0];
		for (int i = 1; i < res->len; i++)
		{
			res->num[i] = 9 - res->num[i];
		}
		int i = 0;
		while (res->num[i] == 10)
		{
			i++;
			res->num[i - 1] = 0;
			res->num[i]++;
		}
		if (res->num[res->len] != 0)
			res->len++;
	}
	while (res->num[res->len - 1] == 0 && res->len > 1)
		res->len--;
}

void mult(member a, member b, member *res)
{
	res->len = a.len + b.len;
	res->num = (int*)malloc(res->len * sizeof(int));
	memset(res->num, 0, res->len * sizeof(int));
	for (int i = 1; i <= b.len; i++)
		for (int j = 1; j <= a.len; j++)
		{
			res->num[i + j - 2] += (a.num[j - 1] * b.num[i - 1]);
		}
	for (int i = 0; i < res->len; i++)
	{
		res->num[i + 1] += (res->num[i] / 10);
		res->num[i] %= 10;
	}
	while (res->num[res->len - 1] == 0 && res->len > 1)
		res->len--;
}

void div(member a, member b, member *res)
{
	member c;
	member t;
	res->len = a.len - b.len + 1;
	res->len = res->len < 1 ? 1 : res->len;
	res->num = (int*)malloc(res->len * sizeof(int));
	memset(res->num, 0, res->len * sizeof(int));
	if (compare(b, a))
	{
		res->len = 1;
		if (res->sign == -1 && (a.len != 1 || a.num[0] != 0))
			res->num[0] = 1;
		return;
	}
	else
	{
		for (int i = a.len - b.len; i >= 0; i--)
		{
			c.len = a.len - i;
			c.num = &a.num[i];
			if (compare(c, b))
			{
				t.len = c.len + 1;
				t.num = (int*)malloc(t.len * sizeof(int));
				memset(t.num, 0, t.len * sizeof(int));
				t.len = 1;
				int j = 0;
				while (compare(c, t))
				{
					j++;
					if (t.len > b.len)
						sum(t, b, &t);
					else
						sum(b, t, &t);
				}
				j--;
				res->num[i] = j;
				memset(t.num, 0, t.len * sizeof(int));
				t.len = 1;
				for (int g = 0; g < j; g++)
					if (t.len > b.len)
						sum(t, b, &t);
					else
						sum(b, t, &t);
				for (int g = 0; g < t.len; g++)
					t.num[g] *= -1;
				sum(c, t, &c);
				for (int g = i; g < a.len; g++)
					a.num[g] = c.num[g - i];
				while (a.num[a.len - 1] == 0 && a.len > 1)
					a.len--;
			}
		}
		if (res->sign == -1 && (a.len != 1 || a.num[0] != 0))
		{
			int i = 0;
			res->num[0]++;
			while (res->num[i++] > 9)
			{
				res->num[i - 1] %= 10;
				res->num[i]++;
			}
		}
		while (res->num[res->len - 1] == 0 && res->len > 1)
			res->len--;
	}

}
