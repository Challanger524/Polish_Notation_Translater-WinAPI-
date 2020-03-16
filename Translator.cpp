#include "Precompiled.h"
#include "Translator.h"
#include <cassert>

//"Get operator's priority"
int OperChecker(const char c)
{
	if (c == '+' || c == '-') return 1;
	else if (c == '*' || c == '/' || c == '%') return 2;
	else if (c == '^') return 3;
	else if (c == '!') return-1; //for unar operators
	else return 0;
}

//Processing the input:
void Terminal_Single_Thread(string_view input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2)
{
	Timer t;
	size_t pos = 0;
	bool correct = false;

	cout << "Input :[" << input << ']';

	if (input[pos] == ' ') while (++pos < input.size() && input[pos] == ' ');
	if (input.size() < 3 || pos >= input.size()) {
		cout << " - Wrong input!";
		return;
	}
	//Define weather expression is Prefix or Postdix or Infix.
	if (OperChecker(input[pos])) {//if Prefix
		cout << " - Prefix";
		correct = PrefiSyntaxCheker(input);
		if (correct) {
			PrefToInf(input, res1);
			cout << "\nOutput:[" << res1 << "] - Infix";
			PrefToPost(input, res2);
			cout << "\nOutput:[" << res2 << "] - Postfix";
		}
	}
	else {
		pos = input.size() - 1;
		if (input[pos] == ' ' || OperChecker(input[pos]) < 0) while (--pos != 0 && (input[pos] == ' ' || OperChecker(input[pos]) < 0));//if unar operators at the end

		if (OperChecker(input[pos]) > 0) {//if Postfix
			cout << " - Postfix";
			correct = PostfSyntaxCheker(input);
			if (correct) {
				PostToPref(input, res1);
				cout << "\nOutput:[" << res1 << "] - Prefix";
				PostToInf(input, res2);
				cout << "\nOutput:[" << res2 << "] - Infix";
			}
		}
		else if (isalnum(input[pos]) || input[pos] == ')') {//if Infix
			cout << " - Infix";
			correct = InfixSyntaxCheker(input);
			if (correct) {
				InfToPref(input, res1);
				cout << "\nOutput:[" << res1 << "] - Prefix";
				InfToPost(input, res2);
				cout << "\nOutput:[" << res2 << "] - Postfix";
			}
		}
		else cout << "Wrong expression detected.";//if wrong
	}

	if (!correct) { cout << " wrong expression."; }
}
void Terminal_Double_Thread(string_view input, unique_ptr<char[]> &res1, unique_ptr<char[]> &res2)
{//works slower because of threading !
	Timer t;
	size_t pos = 0;
	bool correct = false;

	cout << "Input :[" << input << ']';

	if (input[pos] == ' ') while (++pos < input.size() && input[pos] == ' ');
	if (input.size() < 3 || pos >= input.size()) {
		cout << " - Wrong input!";
		return;
	}
	//Define weather expression is Prefix or Postdix or Infix.
	if (OperChecker(input[pos])) {//if Prefix
		cout << " - Prefix";
		correct = PrefiSyntaxCheker(input);
		if (correct) {
			auto handle = async(launch::async | launch::deferred, PrefToInf, ref(input), ref(res1));
			PrefToPost(input, res2);
			handle.wait();
			cout << "\nOutput:[" << res1 << "] - Infix";
			cout << "\nOutput:[" << res2 << "] - Postfix";
		}
	}
	else {
		pos = input.size() - 1;
		if (input[pos] == ' ' || OperChecker(input[pos]) < 0) while (--pos != 0 && (input[pos] == ' ' || OperChecker(input[pos]) < 0));//if unar operators at the end

		if (OperChecker(input[pos]) > 0) {//if Postfix
			cout << " - Postfix";
			correct = PostfSyntaxCheker(input);
			if (correct) {
				auto handle = async(launch::async | launch::deferred, PostToPref, ref(input), ref(res1));
				PostToInf(input, res2);
				handle.wait();
				cout << "\nOutput:[" << res1 << "] - Prefix";
				cout << "\nOutput:[" << res2 << "] - Infix";
			}
		}
		else if (isalnum(input[pos]) || input[pos] == ')') {//if Infix
			cout << " - Infix";
			correct = InfixSyntaxCheker(input);
			if (correct) {
				auto handle = async(launch::async | launch::deferred, InfToPref, ref(input), ref(res1));
				InfToPost(input, res2);
				handle.wait();
				cout << "\nOutput:[" << res1 << "] - Prefix";
				cout << "\nOutput:[" << res2 << "] - Postfix";
			}
		}
		else cout << "Wrong expression detected.";//if wrong
	}

	if (!correct) { cout << " wrong expression."; }
}

//Verifying the correctness of expression
bool InfixSyntaxCheker(string_view view)
{
	bool expect_operator = false;
	int operators = 0;
	int operands = 0;
	int brackets = 0;

	for (size_t i = 0; i < view.size(); i++) {
		if (view[i] == ' ') continue;
		else if (OperChecker(view[i]) > 0 && expect_operator == true) {
			expect_operator = false;
			operators++;
		}
		else if (isalnum(view[i]) && expect_operator == false) {
			expect_operator = true;
			operands++;

			if (isdigit(view[i])) {
				while (++i < view.size() && isdigit(view[i]));
				i--;
			}
		}
		else if (view[i] == '(') brackets++;
		else if (view[i] == ')' && expect_operator == true) brackets--;
		else if (OperChecker(view[i]) < 0 && expect_operator == true) continue;

		else return false;
	}

	if (operands < 2) return false;
	if (operands != operators + 1) return false;
	if (brackets != 0) return false;

	return true;
}
bool PostfSyntaxCheker(string_view view)
{
	int operators = 0;
	int operands = 0;

	for (size_t i = 0; i < view.size(); i++) {
		if (view[i] == ' ') continue;
		else if (OperChecker(view[i]) > 0) operators++;
		else if (isalnum(view[i])) {
			operands++;
			if (isdigit(view[i])) {
				while (++i < view.size() && isdigit(view[i]));
				i--;
			}
		}
		else if (OperChecker(view[i]) < 0) continue;
		else return false;

		if (operands == operators) return false;
	}

	if (operands < 2) return false;
	if (operators != operands - 1) return false;

	return true;
}
bool PrefiSyntaxCheker(string_view view)
{
	bool stop = false;
	int operators = 0;
	int operands = 0;

	for (size_t i = 0; i < view.size(); i++)
	{
		if (view[i] == ' ') continue;
		else if (OperChecker(view[i]) > 0) operators++;
		else if (isalnum(view[i]))
		{
			operands++;
			if (isdigit(view[i])) {
				while (i < view.size() && isdigit(view[i])) i++;
				i--;
			}
		}
		else if (OperChecker(view[i]) < 0) continue;
		else return false;

		if (operators + 1 - operands == 0) stop = true;
		else if (stop == true) return false;
	}

	if (operands < 2) return false;
	if (operators != operands - 1) return false;

	return true;
}

//Translators(standart algorythms from the internet).
//Before the actual translating, the maximum possible size of result is being counting!

void InfToPost(string_view _string, unique_ptr<char[]> &ptr)
{
	stack<char> OperStack;
	size_t pos = 0;//iterator
	long long new_size = 0;
	int priority = -1;

	//counting amount of -odd places from '(', ')' and needed for ' ' between numbers
	new_size = count_num(_string.begin(), _string.end());
	new_size -= 2 * count(_string.begin(), _string.end(), '(');

	ptr = make_unique<char[]>(static_cast<size_t>(new_size + _string.size() + 1));

	for (size_t i = 0; i < _string.size(); i++)//Infix to Postfix converter
	{
		if (isalnum(_string[i])) {
			if (isdigit(_string[i])) {
				if (pos != 0 && isdigit(ptr[pos - 1])) ptr[pos++] = ' ';//make 123' '															  
				while (isdigit(_string[i])) ptr[pos++] = _string[i++];//make 123' '456
				i--;
			}
			else ptr[pos++] = _string[i];
		}
		else if (OperChecker(_string[i])) {
			int t_priority = OperChecker(_string[i]);

			if (t_priority < 0) ptr[pos++] = _string[i];
			else if (t_priority > priority) {
				OperStack.push(_string[i]);
				priority = t_priority;
			}
			else if (t_priority <= priority) {
				ptr[pos++] = OperStack.top();
				OperStack.pop();
				OperStack.push(_string[i]);
				priority = t_priority;
			}
		}
		else if (_string[i] == '(') {
			OperStack.push('(');
			priority = -1;
		}
		else if (_string[i] == ')') {
			while (OperStack.top() != '(') {
				ptr[pos++] = OperStack.top();
				OperStack.pop();
			}
			OperStack.pop();
			if (!OperStack.empty()) priority = OperChecker(OperStack.top());
			else priority = -1;
		}
	}//Infix to Postfix coverter//end

	while (!OperStack.empty()) {
		ptr[pos++] = OperStack.top();
		OperStack.pop();
	}
	ptr[pos] = '\0';
}
void InfToPref(string_view _string, unique_ptr<char[]> &ptr)
{
	char copy_str[G_SIZER];
	size_t pos = _string.size();
	copy_str[pos--] = '\0';

	//Here is recursive lambda expression instead of simple function - because "I can" and it is some sort of incapsulation.
	auto InnerReverse = [] (char *copy, size_t begin, size_t end, const auto &Lambda) -> void {
		char temp;
		size_t it;
		end--;

		while (begin < end)
		{
			it = end;

			if (OperChecker(copy[end]) < 0) {
				while (OperChecker(copy[--it]) < 0);
				if (begin >= it) return;
			}
			else if (OperChecker(copy[end]) > 0) {
				temp = copy[end];
				size_t i = end - 1;
				do { copy[i + 1] = copy[i]; } while (i-- != begin);
				copy[begin++] = temp;
				continue;
			}

			if (isalpha(copy[it])) {
				while (it <= end) {
					temp = copy[it];
					size_t i = it - 1;
					do { copy[i + 1] = copy[i]; } while (i-- != begin);
					copy[begin++] = temp;
					it++;
				}
			}
			else if (isdigit(copy[it])) {
				while (begin < it && isdigit(copy[it])) {
					temp = copy[it];
					size_t i = it - 1;
					do { copy[i + 1] = copy[i]; } while (i-- != begin);
					copy[begin++] = temp;
				}
				while (begin < it && ++it <= end) {//If (it != end)
					temp = copy[it];
					size_t i = it - 1;
					do { copy[i + 1] = copy[i]; } while (i-- != begin);
					copy[begin++] = temp;
				}
			}
			else if (copy[it] == ')') {
				size_t l_begin = begin + 1;
				size_t l_end = it;//last bracket

				int bracket = 1;
				while (bracket != 0) {
					it--;
					if (copy[it] == ')') bracket++;
					else if (copy[it] == '(') bracket--;
				}//it == first bracket

				l_end = l_end - it + begin;//first bracket + distance between brackets

				if (begin < it) while (it <= end) {
					temp = copy[it];
					size_t i = it - 1;
					do { copy[i + 1] = copy[i]; } while (i-- != begin);
					copy[begin++] = temp;
					it++;
				}
				else begin = end;

				Lambda(copy, l_begin, l_end, Lambda);
			}
			else if (copy[it] == ' ') end--;
			else terminate();
		}
	};//InnerReverse(Lambda) end//

#if 1
	//memcpy(copy_str, _string., _string.size());
	strcpy_s(copy_str, _string.data());
	InnerReverse(copy_str, 0, _string.size(), InnerReverse);

#else //Reversing while copying part(a bit faster but less recursive way).
	for (size_t i = 0; i < _string.size(); i++, pos--) {
		if (OperChecker(_string[pos]) < 0) {
			size_t unar_end = pos + 1;
			while (OperChecker(_string[--pos]) < 0);

			if (isalpha(_string[pos])) {
				copy_str[i] = _string[pos];
				for (size_t it = pos + 1; it < unar_end; it++) copy_str[++i] = _string[it];
			}
			else if (isdigit(_string[pos])) {
				size_t it = pos + 1;
				while (pos < _string.size() && isdigit(_string[pos])) copy_str[i++] = _string[pos--];
				for (it; it < unar_end; it++) copy_str[i++] = _string[it];
				i--;
				pos++;
			}
			else if (_string[pos] == ')') {
				int brackets = 1;
				size_t l_begin = i + 1;
				size_t l_end = pos + 1;

				while (brackets != 0) {
					pos--;
					if (_string[pos] == ')') brackets++;
					else if (_string[pos] == '(') brackets--;
				}//pos - will point to first bracket

				size_t j = pos;
				while (j < l_end) copy_str[i++] = _string[j++];
				l_end = i - 1;
				while (j < unar_end) copy_str[i++] = _string[j++];
				i--;

				InnerReverse(copy_str, l_begin, l_end, InnerReverse);
			}
		}
		else if (_string[pos] == '(') copy_str[i] = ')';
		else if (_string[pos] == ')') copy_str[i] = '(';
		else    copy_str[i] = _string[pos];
	}
#endif

	if (!InfixSyntaxCheker(_string)) {
		cout << "\n[ERROR] Infix reverse failed\n";
		return;
		}

	InfToPost(copy_str, ptr);//InfToPost call

	pos = strlen(ptr.get()) - 1;
	for (size_t i = 0; pos > i; pos--, i++) swap(ptr[i], ptr[pos]);//postfix string reverse
	}

void PostToInf(string_view _string, unique_ptr<char[]> &ptr)
{
	int priority;//for operator priority
	size_t new_size;
	size_t oper_num;
	size_t binar_num;
	size_t
		up = 0,//stack level iterators
		low = 0,
		j = 1,//iterators
		k = 1;

	oper_num = count_if(_string.begin(), _string.end(), OperChecker);
	binar_num = count_if(_string.begin(), _string.end(), [] (char c) {return OperChecker(c) > 0; });
	oper_num++;

	new_size = _string.size() + oper_num * 2;// the maximum possible amount of chars in result

	vector<vector<char>> mas(oper_num, vector<char>(new_size));//matrix as a stack
	ptr = make_unique<char[]>(new_size);

	for (size_t pos = 0; pos < _string.size(); pos++)
	{
		if (isalpha(_string[pos]))//if we meet a letter
		{
			j = 2;
			mas[up][0] = '0';//'0' is right
			mas[up][1] = _string[pos];
			if (pos + 1 < _string.size() && _string[pos + 1] == ' ') {
				while (++pos < _string.size() && _string[pos] == ' ');
				pos--;
			}
			if (pos + 1 < _string.size() && OperChecker(_string[pos + 1]) < 0) {
				while (++pos < _string.size() && OperChecker(_string[pos]) < 0) mas[up][j++] = _string[pos];
				pos--;
			}
			mas[up][j] = '\0';
			up++;
		}
		else if (isdigit(_string[pos]))//if we meet a number
		{
			j = 1;
			mas[up][0] = '0';//'0' is right
			while (isdigit(_string[pos])) mas[up][j++] = _string[pos++];
			pos--;
			if (pos + 1 < _string.size() && _string[pos + 1] == ' ') {
				while (++pos < _string.size() && _string[pos] == ' ');
				pos--;
			}
			if (pos + 1 < _string.size() && OperChecker(_string[pos + 1]) < 0) {
				while (++pos < _string.size() && OperChecker(_string[pos]) < 0) mas[up][j++] = _string[pos];
				pos--;
			}
			mas[up][j] = '\0';
			up++;
		}
		else if (OperChecker(_string[pos]) < 0) {//if we meet an unar operator
			j = 1;
			while (mas[up][j++] != '\0');
			mas[up][j++] = _string[pos];
			mas[up][j] = '\0';
		}
		else if (OperChecker(_string[pos]) > 0)//if we meet a binary operator
		{//we make one string out of two with operator in between
			size_t n_pos = pos + 1;
			up--;
			low = up - 1;
			j = 1;
			k = 1;

			ptr[0] = mas[low][0];

			if (n_pos < _string.size() && _string[n_pos] == ' ') while (++n_pos < _string.size() && _string[n_pos] == ' ');
			if (n_pos < _string.size() && OperChecker(_string[n_pos]) < 0) ptr[j++] = '(';

			while (mas[low][k] != '\0') ptr[j++] = mas[low][k++];//copy from stack to buff
			ptr[j] = '\0';

			priority = OperChecker(_string[pos]);
			j = 1;

			if ((ptr[0] != '0') && (priority > ptr[0] - 48)) {//if first operands have lower priority we put '(' and ')'
				mas[low][j++] = '(';
				k = 1;
				while (ptr[k] != '\0') mas[low][j++] = ptr[k++];
				mas[low][j++] = ')';
			}
			else while (ptr[j] != '\0') {
				mas[low][j] = ptr[j]; //without brackets '(')'
				j++;
			}
			mas[low][0] = static_cast<char>(priority + 48);
			mas[low][j++] = _string[pos];

			if ((mas[up][0] != '0') && (priority > mas[up][0] - 48)) {//if second operands have lower priority
				mas[low][j++] = '(';
				k = 1;
				while (mas[up][k] != '\0') mas[low][j++] = mas[up][k++];
				mas[low][j++] = ')';
			}
			else {
				k = 1;
				while (mas[up][k] != '\0') {
					mas[low][j] = mas[up][k++];
					j++;
				}
			}

			if (n_pos < _string.size() && OperChecker(_string[n_pos]) < 0) {
				pos = n_pos - 1;
				mas[low][0] = '0';
				mas[low][j++] = ')';
				while (pos + 1 < _string.size() && OperChecker(_string[++pos]) < 0) mas[low][j++] = _string[pos];
				pos--;
			}
			mas[low][j] = '\0';
		}//if we meet a binary operator//end
	}
	j = 1;
	k = 0;
	while (mas[0][j] != '\0') ptr[k++] = mas[0][j++];//copy result from stack to 'ptr'
	ptr[k] = '\0';
}
void PostToPref(string_view _string, unique_ptr<char[]> &ptr)
{
	stack<char> OperStack;
	stack<int> PairCondition;
	size_t space_count;//' '//
	size_t num_count;
	size_t j;//iterator

	space_count = count(_string.begin(), _string.end(), ' ');
	num_count = count_num(_string.begin(), _string.end());

	j = _string.size() + num_count - space_count + 1;

	ptr = make_unique<char[]>(j);

	ptr[--j] = '\0';
	j--;
	if (num_count)memset(ptr.get(), '$', num_count);

	for (size_t i = _string.size() - 1; i < _string.size(); )
	{
		if (_string[i] == ' ') i--;
		else if (OperChecker(_string[i]) > 0) {//|+^
			OperStack.push(_string[i--]);
			PairCondition.push(0);
		}
		else if (OperChecker(_string[i]) < 0) OperStack.push(_string[i--]);
		else if (isalpha(_string[i]))
		{
			ptr[j--] = _string[i--];

			if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a| -> \..+\ |...a!!!!|
				while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
					ptr[j--] = OperStack.top();
					OperStack.pop();
				}

			if (!PairCondition.empty()) PairCondition.top()++;

			while (!PairCondition.empty() && PairCondition.top() == 2) {
				ptr[j--] = OperStack.top();
				OperStack.pop();

				if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a+| -> |...a+!!!!|
					while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
						ptr[j--] = OperStack.top();
						OperStack.pop();
					}

				PairCondition.pop();
				if (!PairCondition.empty()) PairCondition.top()++;
			}
		}
		else if (isdigit(_string[i]))
		{
			if (isdigit(ptr[j + 1])) ptr[j--] = ' ';

			while (i < _string.size() && isdigit(_string[i])) ptr[j--] = _string[i--];

			if (!OperStack.empty() && OperChecker(OperStack.top()) < 0) //|+!!!!|a| -> |
				while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
					ptr[j--] = OperStack.top();
					OperStack.pop();
				}

			if (!PairCondition.empty()) PairCondition.top()++;

			while (!PairCondition.empty() && PairCondition.top() == 2) {
				ptr[j--] = OperStack.top();
				OperStack.pop();

				if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a+| -> |...a+!!!!|
					while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
						ptr[j--] = OperStack.top();
						OperStack.pop();
					}

				PairCondition.pop();
				if (!PairCondition.empty()) PairCondition.top()++;
			}
		}
	}

	while (!OperStack.empty()) {
		ptr[j--] = OperStack.top();
		OperStack.pop();
	}

	while (ptr[0] == '$') {
		j = 0;
		while (ptr[j] != '\0') {
			ptr[j] = ptr[j + 1];
			j++;
		}
		ptr[--j] = '\0';
	}
}

void PrefToInf(string_view _string, unique_ptr<char[]> &ptr)
{
	int priority;//for operator priority
	size_t new_size;
	size_t oper_num;
	size_t binar_num;
	size_t
		up = 0,//stack level iterators
		low = 0,
		j = 1,//iterators
		k = 1;

	oper_num = count_if(_string.begin(), _string.end(), OperChecker);
	binar_num = count_if(_string.begin(), _string.end(), [] (char c) {return OperChecker(c) > 0; });
	oper_num++;

	new_size = _string.size() + oper_num * 2;// the maximum possible amount of chars in result 

	vector<vector<char>> mas(oper_num, vector<char>(new_size));//matrix as a stack
	ptr = make_unique<char[]>(new_size);

	for (size_t pos = _string.size() - 1; pos < _string.size(); pos--)
	{
		if (_string[pos] == ' ') continue;
		if (isalpha(_string[pos]))//if we meet a letter
		{
			j = 2;
			mas[up][0] = '0';//'0' is right
			mas[up][1] = _string[pos];

			if (pos - 1 < _string.size() && _string[pos - 1] == ' ') {
				while (--pos < _string.size() && _string[pos] == ' ');
				pos++;
			}
			if (pos - 1 < _string.size() && OperChecker(_string[pos - 1]) < 0) {
				while (--pos < _string.size() && OperChecker(_string[pos]) < 0) mas[up][j++] = _string[pos];
				pos++;
			}
			mas[up][j] = '\0';
			up++;
		}
		else if (isdigit(_string[pos]))//if we meet a number
		{
			size_t it = pos;
			size_t j_it;

			while (isdigit(_string[--it]));
			j_it = pos - it;
			j = j_it + 1;

			while (it < pos)  mas[up][j_it--] = _string[pos--];

			pos++;

			if (pos - 1 < _string.size() && _string[pos - 1] == ' ') {
				while (--pos < _string.size() && _string[pos] == ' ');
				pos++;
			}
			if (pos - 1 < _string.size() && OperChecker(_string[pos - 1]) < 0) {
				while (--pos < _string.size() && OperChecker(_string[pos]) < 0) mas[up][j++] = _string[pos];
				pos++;
			}

			mas[up][0] = '0';//'0' is right
			mas[up][j] = '\0';
			up++;
		}
		else if (OperChecker(_string[pos]) < 0) {//if we meet an unar operator
			j = 1;
			while (mas[up][j++] != '\0');
			mas[up][j++] = _string[pos];
			mas[up][j] = '\0';
		}
		else if (OperChecker(_string[pos]) > 0)//if we meet a binary operator
		{//we make one string out of two with operator in between
			size_t n_pos = pos - 1;
			up--;
			low = up - 1;
			j = 1;
			k = 1;

			priority = OperChecker(_string[pos]);

			ptr[0] = mas[low][0];
			while (mas[low][k] != '\0') {
				ptr[k] = mas[low][k];//copy from stack to buff
				k++;
			}
			ptr[k] = '\0';
			k = 1;

			if (n_pos < _string.size() && _string[n_pos] == ' ') while (--n_pos < _string.size() && _string[n_pos] == ' ');
			if (n_pos < _string.size() && OperChecker(_string[n_pos]) < 0) mas[low][j++] = '(';

			if ((mas[up][0] != '0') && (priority > mas[up][0] - 48)) {//if upper operands have lower priority
				mas[low][j++] = '(';
				while (mas[up][k] != '\0') mas[low][j++] = mas[up][k++];
				mas[low][j++] = ')';
			}
			else {
				while (mas[up][k] != '\0') mas[low][j++] = mas[up][k++];
			}
			//
			mas[low][0] = static_cast<char>(priority + 48);
			mas[low][j++] = _string[pos];
			//
			k = 1;
			if ((ptr[0] != '0') && (priority > ptr[0] - 48)) {//if //lower operands have lower priority we put '(' and ')'
				mas[low][j++] = '(';
				while (ptr[k] != '\0') mas[low][j++] = ptr[k++];
				mas[low][j++] = ')';
			}
			else {
				while (ptr[k] != '\0') mas[low][j++] = ptr[k++]; //without brackets
			}

			if (n_pos < _string.size() && OperChecker(_string[n_pos]) < 0) {
				pos = n_pos + 1;
				mas[low][0] = '0';
				mas[low][j++] = ')';
				while (pos - 1 < _string.size() && OperChecker(_string[--pos]) < 0) mas[low][j++] = _string[pos];
				pos++;
			}
			mas[low][j] = '\0';
		}//if we meet a binary operator//end
	}
	j = 1;
	k = 0;
	while (mas[0][j] != '\0') ptr[k++] = mas[0][j++];//copy result from stack to 'ptr'
	ptr[k] = '\0';
}
void PrefToPost(string_view _string, unique_ptr<char[]> &ptr)
{
	stack<char> OperStack;
	stack<int> PairCondition;
	size_t space_count;//' '//
	size_t num_count;
	size_t j = 0;//iterator

	space_count = count(_string.begin(), _string.end(), ' ');
	num_count = count_num(_string.begin(), _string.end());

	ptr = make_unique<char[]>(_string.size() + num_count - space_count + 1);

	for (size_t i = 0; i < _string.size(); )
	{
		if (_string[i] == ' ') i++;
		else if (OperChecker(_string[i]) > 0) {
			OperStack.push(_string[i++]);
			PairCondition.push(0);
		}
		else if (OperChecker(_string[i]) < 0) OperStack.push(_string[i++]);
		else if (isalpha(_string[i]))
		{
			ptr[j++] = _string[i++];

			if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a| -> \..+\ |...a!!!!|
				while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
					ptr[j++] = OperStack.top();
					OperStack.pop();
				}

			PairCondition.top()++;

			while (!PairCondition.empty() && PairCondition.top() == 2) {
				ptr[j++] = OperStack.top();
				OperStack.pop();

				if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a+| -> |...a+!!!!|
					while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
						ptr[j++] = OperStack.top();
						OperStack.pop();
					}

				PairCondition.pop();
				if (!PairCondition.empty()) PairCondition.top()++;
			}
		}
		else if (isdigit(_string[i]))
		{
			if (j > 0 && isdigit(ptr[j - 1])) ptr[j++] = ' ';
			while (i < _string.size() && isdigit(_string[i])) ptr[j++] = _string[i++];

			if (!OperStack.empty() && OperChecker(OperStack.top()) < 0) //|+!!!!|a| -> |
				while ((OperChecker(OperStack.top()) < 0)) {
					ptr[j++] = OperStack.top();
					OperStack.pop();
				}

			PairCondition.top()++;

			while (!PairCondition.empty() && PairCondition.top() == 2) {
				ptr[j++] = OperStack.top();
				OperStack.pop();

				if (!OperStack.empty() && OperChecker(OperStack.top()) < 0)//\...+!!!!\ |...a+| -> |...a+!!!!|
					while (!OperStack.empty() && OperChecker(OperStack.top()) < 0) {
						ptr[j++] = OperStack.top();
						OperStack.pop();
					}

				PairCondition.pop();
				if (!PairCondition.empty()) PairCondition.top()++;
			}
		}
	}

	while (!OperStack.empty()) {
		ptr[j++] = OperStack.top();
		OperStack.pop();
	}
	ptr[j] = '\0';
}

//Translator made by myself (deprecated)
//void PrefToInfMyOwn(string_view _string, unique_ptr<char[]> &ptr, const size_t _string.size())
#if 0
void PrefToInfMyOwn(string_view _string, unique_ptr<char[]> &ptr, const size_t _string.size())
{
	bool block_ready = 0;
	bool one_operand = 0;
	size_t oper_num = 0;
	size_t
		s = 0, //stack iterators
		t = 0,
		j = 1,//iterators
		k = 1;

	for (size_t i = 0; i < _string.size(); i++) if (OperChecker(_string[i])) oper_num++;//counting nuber of binary operators(determine stack depth)
	oper_num++;
	size_t p_size = _string.size() + oper_num * 2;// the maximum possible amount of chars in result 

	ptr = new char[p_size];

	char** mas = new char*[oper_num];//stack from massives            
	for (size_t i = 0; i < oper_num; i++) mas[i] = new char[p_size];


	for (size_t pos = 0; pos < _string.size(); pos++)//Prefix to Infix converter
	{
		if (_string[pos] == ' ') continue;//!?
		else if (OperChecker(_string[pos]) > 0) {//if we meet binary operator
			mas[s][0] = '0';//'0' is right
			mas[s][1] = _string[pos];
			mas[s][2] = '\0';
			s++;
		}
		else if (OperChecker(_string[pos]) < 0) {//if we meet an unar operator make/!!!.../ -> |0!3|
			int  unar_num = 1;
			char unar_temp = _string[pos];

			while (_string[++pos] == unar_temp) unar_num++;
			if (unar_num > 9) { unar_num = 9; cout << "ATTENTION:too many similar unar operators\n"; }
			mas[s][0] = '0';//'0' is right
			mas[s][1] = unar_temp;
			mas[s][2] = unar_num + 48;
			mas[s][3] = '\0';

			pos--;
			s++;
		}
		else if (isalpha(_string[pos]))//if we meet a letter
		{
			s--;
			if (OperChecker(mas[s][1]) < 0) {//if empty level of stack only with UNARY operator make|0!3|/a.../ -> |0a!!!|
				char unar_temp = mas[s][1];//=! if |0!3|
				int  unar_num = mas[s][2] - 48;//=3 if |0!3|
				j = 2;
				t = s - 1;

				mas[s][0] = '0';
				mas[s][1] = _string[pos];
				while (unar_num--) mas[s][j++] = unar_temp;//make|0a!!!|					
				mas[s][j] = '\0';

				//if program accepts more than 1 unar operators:sin,cos etc.
				/*while (OperChecker(mas[s - 1][1]) < 0) {
				t = s - 1;
				char unar_temp = mas[t][1];//=! if |0!3|
				int unar_num = mas[t][2] - 48;//=3 if |0!3|
				j = 1;

				mas[t][0] = '0';
				while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];
				//mas[t][j] = string[pos];
				while (unar_num--) mas[s][j++] = unar_temp;//make|0a!!!sinsinsin|
				mas[s][j] = '\0';
				s--;
				}*/

				if (OperChecker(mas[t][1]) > 0) {//if empty level of stack only with BINARY operator make|0+|/a!!!.../ -> |#a!!!+|

					j = 1;
					char oper_temp = mas[t][1];

					mas[t][0] = '#';//'#' means partically filled stack level
					while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];
					mas[t][j++] = oper_temp;
					mas[t][j] = '\0';

					s--;
				}
				else if (mas[t][0] == '#') {//if partically filld stack level convert from|#b+|a!!!| -> |1b+a!!!|
					j = 0;
					k = 1;

					while (mas[t][++j] != '\0');
					mas[t][0] = OperChecker(mas[t][j - 1]) + 48;
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
					mas[t][j] = '\0';

					s--;
					block_ready = true;
				}
			}
			else if (OperChecker(mas[s][1]) > 0) {//if empty level of stack only with BINARY operator make|0+|/a.../ -> |#a+|
				mas[s][0] = '#';//'#' means partically filled stack level
				mas[s][2] = mas[s][1];
				mas[s][1] = _string[pos];
				mas[s][3] = '\0';
			}
			else if (mas[s][0] == '#') {//if partically filld stack level convert from|#b+|/a.../ -> |b+a|
				j = 0;
				while (mas[s][++j] != '\0');
				mas[s][0] = OperChecker(mas[s][j - 1]) + 48;
				mas[s][j++] = _string[pos];
				mas[s][j] = '\0';

				block_ready = true;
			}
			s++;
		}//else if (isalpha(string[pos]))//end
		else if (isdigit(_string[pos]))//if we meet a number
		{
			s--;
			if (OperChecker(mas[s][1]) < 0) {//if empty level of stack only with UNARY operator make|0!3|/a.../ -> |0a!!!|
				char unar_temp = mas[s][1];//=! if |0!3|
				int  unar_num = mas[s][2] - 48;//=3 if |0!3|
				j = 1;
				t = s - 1;

				mas[s][0] = '0';
				while (isdigit(_string[pos])) mas[s][j++] = _string[pos++];//
				while (unar_num--) mas[s][j++] = unar_temp;//make|0a!!!|					
				mas[s][j] = '\0';

				pos--;
				//if program accepts more than 1 unar operators:sin,cos etc.
				/*while (OperChecker(mas[s - 1][1]) < 0) {
				t = s - 1;
				char unar_temp = mas[t][1];//=! if |0!3|
				int unar_num = mas[t][2] - 48;//=3 if |0!3|
				j = 1;

				mas[t][0] = '0';
				while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];
				//mas[t][j] = string[pos];
				while (unar_num--) mas[s][j++] = unar_temp;//make|0a!!!sinsinsin|
				mas[s][j] = '\0';
				s--;
				}*/

				if (OperChecker(mas[t][1]) > 0) {//if empty level of stack only with BINARY operator make|0+|/a!!!.../ -> |#a!!!+|

					j = 1;
					char oper_temp = mas[t][1];

					mas[t][0] = '#';//'#' means partically filled stack level
					while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];
					mas[t][j++] = oper_temp;
					mas[t][j] = '\0';

					s--;
				}
				else if (mas[t][0] == '#') {//if partically filld stack level convert from|#b+|123!!!| -> |1b+123!!!|
					j = 0;
					k = 1;

					while (mas[t][++j] != '\0');
					mas[t][0] = OperChecker(mas[t][j - 1]) + 48;
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
					mas[t][j] = '\0';

					s--;
					block_ready = true;
				}
			}
			else if (OperChecker(mas[s][1]) > 0) {//if empty level of stack only with BINARY operator make|0+|/a.../ -> |#a+|
				char oper_temp = mas[s][1];
				j = 1;

				mas[s][0] = '#';//'#' means partically filled stack level
				while (isdigit(_string[pos])) mas[s][j++] = _string[pos++];//				
				mas[s][j++] = oper_temp;
				mas[s][j] = '\0';

				pos--;
			}
			else if (mas[s][0] == '#') {//if partically filld stack level convert from|#b+|/a.../ -> |b+a|
				j = 0;
				while (mas[s][++j] != '\0');
				mas[s][0] = OperChecker(mas[s][j - 1]) + 48;
				while (isdigit(_string[pos])) mas[s][j++] = _string[pos++];//				
				mas[s][j] = '\0';

				pos--;
				block_ready = true;
			}
			s++;
		}//else if (isdigit(string[pos]))//if we meet a number//end

		while (block_ready && s > 1)//we recive |0!2| or |0+| or |#c+| with |1a+b| -> |(a!+b!!)!| \\ |a!+b!!| \\ |#a*(a!+b!!)|
		{
			s--;
			t = s - 1;

			if (mas[t][0] == '#' && mas[s][0] != '#') {//if |#d^|#c*|1a+b| -> |#d^|2c*(a+b)| -> |3d^(c*(a+b))|
				j = 0;
				k = 1;

				while (mas[t][++j] != '\0');

				if (mas[s][0] - 48 < OperChecker(mas[t][j - 1])) {//if operands have lower priority				
					mas[t][0] = OperChecker(mas[t][j - 1]);
					mas[t][j++] = '(';
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
					mas[t][j++] = ')';
				}
				else {
					mas[t][0] = mas[s][0];
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
				}

				mas[t][j] = '\0';
				s--;
			}
			else if (mas[s][0] != '#' && OperChecker(mas[t][1]) > 0) {//if |0*|1a+b| -> |#(a+b)*|
				j = 1;
				k = 1;
				char oper_temp = mas[t][1];

				if (mas[s][0] - 48 < OperChecker(mas[t][1])) {//if operands have lower priority				
					mas[t][j++] = '(';
					while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
					mas[t][j++] = ')';
				}
				else while (mas[s][j] != '\0') mas[t][j] = mas[s][j++];

				mas[t][0] = '#';
				mas[t][j++] = oper_temp;
				mas[t][j] = '\0';
				s--;
			}
			else if (mas[s][0] != '0' && mas[s][0] != '#' && OperChecker(mas[t][1]) < 0) {//|0!N|1a+b| -> |1(a+b)!/N/| :N-quantity
				j = 2;
				k = 1;

				char unar_temp = mas[t][1];
				int  unar_num = mas[t][2] - 48;

				mas[t][0] = '1';
				mas[t][1] = '(';
				while (mas[s][k] != '\0') mas[t][j++] = mas[s][k++];
				mas[t][j++] = ')';
				while (unar_num--) mas[t][j++] = unar_temp;//make |?(a+b)!!!|
				mas[t][j] = '\0';

				s--;
			}
			else block_ready = false;
			s++;
		}//while (block_ready && s > 1)//end
		block_ready = false;
		}//for (size_t pos = 0; pos < _string.size(); pos++)//end

	j = 1;
	k = 0;
	while (mas[0][j] != '\0') ptr[k++] = mas[0][j++];//copy result from stack to 'ptr'
	ptr[k] = '\0';

	for (size_t i = 0; i < oper_num; i++)
		delete[] mas[i];
	delete[] mas;
	//PrefToInf//end
	}
#endif

[[nodiscard]] int count_num(string_view::const_iterator First, string_view::const_iterator Last) {
	assert(First <= Last && "iterator First no bigger than Last");
	unsigned int nums = 0;
	for (string_view::const_iterator It = First; It != Last; ) {
		if (isdigit(*It)) {
			nums++;
			while (++It != Last && isdigit(*(It)));
			continue;
		}
		It++;
	}
	return nums;
}
