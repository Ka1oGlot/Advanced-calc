#include <iostream>
#include <iomanip>

const int MAX_LEN = 100;

struct OperStack {
    char data[MAX_LEN];
    int top = -1;

    void push(char c) {
        if (top < MAX_LEN - 1) {
            top++;
            data[top] = c;
        }
    }

    char pop() {
        if (top >= 0) {
            return data[top--];
        }
        return '\0';
    }

    char peek() {
        if (top >= 0) {
            return data[top];
        }
        return '\0';
    }

    bool isEmpty() {
        return top == -1;
    }
};

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    if (op == '!') return 4;
    return 0;
}

long double factorial(int n)
{
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

long double power(double base, double exp) {
    double result = 1;
    for (int i = 0; i < static_cast<int>(exp); i++) {
        result *= base;
    }
    return result;
}

void toPostfix(const char* expression, char* output) {
    OperStack operators;
    int j = 0;

    for (int i = 0; expression[i] != '\0'; i++) {
        char c = expression[i];

        if (isDigit(c)) {
            while (isDigit(expression[i]) || expression[i] == '.') {
                output[j++] = expression[i++];
            }
            output[j++] = ' ';
            i--;
        } else if (c == '(') {
            operators.push(c);
        } else if (c == ')') {
            while (!operators.isEmpty() && operators.peek() != '(') {
                output[j++] = operators.pop();
                output[j++] = ' ';
            }
            operators.pop();
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '!') {
            while (!operators.isEmpty() && precedence(operators.peek()) >= precedence(c)) {
                output[j++] = operators.pop();
                output[j++] = ' ';
            }
            operators.push(c);
        }
    }

    while (!operators.isEmpty()) {
        output[j++] = operators.pop();
        output[j++] = ' ';
    }

    output[j] = '\0';
}

double evaluatePostfix(const char* postfix) {
    double stack[MAX_LEN];
    int top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        if (isDigit(c)) {
            double num = 0;
            while (isDigit(postfix[i])) {
                num = num * 10 + (postfix[i++] - '0');
            }
            if (postfix[i] == '.') {
                i++;
                double fraction = 0.1;
                while (isDigit(postfix[i])) {
                    num += (postfix[i++] - '0') * fraction;
                    fraction /= 10;
                }
            }
            stack[++top] = num;
        } else if (c == '+') {
            double b = stack[top--];
            double a = stack[top--];
            stack[++top] = a + b;
        } else if (c == '-') {
            double b = stack[top--];
            double a = stack[top--];
            stack[++top] = a - b;
        } else if (c == '*') {
            double b = stack[top--];
            double a = stack[top--];
            stack[++top] = a * b;
        } else if (c == '/') {
            double b = stack[top--];
            double a = stack[top--];
	    if (b == 0) {
                std::cerr << "Помилка: на 0 ділити не можна!" << std::endl;
                exit(EXIT_FAILURE);
            }
            stack[++top] = a / b;
        } else if (c == '^') {
            double b = stack[top--];
            double a = stack[top--];
            stack[++top] = power(a, b);
        } else if (c == '!') {
            double a = stack[top--];
	    if (a < 0) {
		    std::cerr << "Помилка: факторіал не визначений для від'ємних чисел!" << std::endl;
		    exit(EXIT_FAILURE);
	    }
            stack[++top] = factorial(a);
        }
    }

    return stack[top];
}

int main() {
    char expression[MAX_LEN];
    char postfix[MAX_LEN];
    std::cout << "'+' - додавання\n'-' - віднімання\n'*' - множення\n'/' - ділення\n'^' - зведення в степінь\n'!' - знайти факторіал\n" << "Введіть математичний вираз: ";
    std::cin >> expression;

    toPostfix(expression, postfix);
    std::cout << "Зворотна польська нотація: " << postfix << std::endl;

    double result = evaluatePostfix(postfix);
    std::cout << std::fixed << std::setprecision(2) << "Результат: " << result << std::endl;

    return 0;
}
