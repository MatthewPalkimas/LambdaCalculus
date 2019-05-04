#include <iostream>
#include <string>
#include <vector>

using namespace std;

class lambda
{
	public:
	virtual bool isVariable() { return false; }
	virtual bool isAbstraction() { return false; }
	virtual bool isApplication() { return false; }
	virtual void print() = 0;
	virtual char getName() { return '?'; }
	virtual int getNum() { return 0; }
	virtual	lambda * getVar() { return NULL; }
	virtual lambda * getBody() { return NULL; }
	virtual lambda * getFunction() { return NULL; }
	virtual lambda * getArgument() { return NULL; }
	virtual lambda * copy() = 0;
	virtual ~lambda() { }
};

class variable: public lambda
{
  public:
	char name;
	int number;
	variable(char c, int i = 0)
	{
		name = c;
		number = i;
	}
	virtual bool isVariable() { return true; }
	virtual char getName() { return name; }
	virtual int getNum() { return number; }	
	virtual void print()
	{
		cout << name;
		if (number != 0)
			cout << number;
	}
	virtual lambda * copy()
	{ return new variable(name, number); }
	virtual ~variable() { }
};

class abstraction: public lambda
{
  public:
	lambda * var, * body;
	abstraction(lambda * v, lambda * b)
	{
		var = v;
		body = b;
	}
	virtual bool isAbstraction() { return true; }
	virtual lambda * getVar() { return var; }
	virtual lambda * getBody() { return body; }
	virtual void print()
	{
		cout << "λ";
		var->print();
		cout << ".";
		body->print();
	}
	virtual lambda * copy()
	{ return new abstraction(var, body); }
	virtual ~abstraction()
	{
		delete var;
		delete body;
	}
};

class application: public lambda
{ 
  public:
    lambda * func;
    lambda * arg;
    application(lambda * f, lambda * a)
    { 
		func = f;
    	arg = a; 
	}
    virtual bool isApplication() { return true; }
    virtual lambda * getFunction() { return func; }
    virtual lambda * getArgument() { return arg; }
    virtual void print()
    { 
		cout << "(";
    	func->print();
      	cout << " ";
      	arg->print();
      	cout << ")"; 
	}
	virtual lambda * copy()
	{ return new application(func, arg); }
    virtual ~application()
    { 
		delete func;
    	delete arg; 
	}
};

struct input
{
	string line;
	int pos;
	void reset()
	{
		getline(cin, line);
		if (line == "exit")
		{
			cout << "|\n| EXITING!\n"; 
			cout << "|----------------------------------------------------------------------------------------\n";		
			exit(1);
		}	
		pos = 0;
	}
	char getch()
	{
		if (pos == line.length())
		{ return '\n'; }
		pos += 1;
		return line[pos-1];
	}	
	void backch()
	{
		if (pos > 0)
			pos -= 1;
	}
	void error(string msg)
	{ 
		cout << "|\n| " << msg << "| Occured at pos: " << pos << "\n|\n"; 
		cout << "|----------------------------------------------------------------------------------------\n";		
		exit(1);
	}
	void print()
	{
		int temp = pos;
		for(pos; pos < line.length(); pos++)
			cout << line[pos];
		cout << endl;
		pos = temp;
	}
};

struct token
{
	string type;
	char letter;
	int number;
	int size;
	token() {}
	token(string t)
	{
		type = t;
		size = 1;
	}
	token(string t, char l, int n, int s)
	{
		type = t;
		letter = l;
		number = n;
		size = s;
	}
};

struct lex
{
	input * inp; 
	token it;
	bool giveagain;
	int varsize;
	lex()
	{
		inp = new input;
		giveagain = false;
		varsize = 1;
		inp->reset();	
	}

	token back()
	{
		giveagain = false;
		return it;
	}
	token next()
	{
		if (giveagain)
		{
			giveagain = false;
			return it;
		}
		char c = inp->getch();
		while(c == ' ')
			c = inp->getch();
		//inp->print();
		switch (c)
		{
			case '#':
				return token("T_lambda");
			case '.':
				return token("T_dot");
			case '(':
				return token("T_openpar");
			case ')':
				return token("T_closepar");
			case'a':case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':case'k':case'l':case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':case'v':case'w':case'x':case'y':case'z':
			{
				char letter = c;
				c = inp->getch();
				int num = 0;
				varsize = 1;
				while(isdigit(c))
				{
					num = num*10 + (c - '0');
					c = inp->getch();
					varsize += 1;
				}
				//for(int i = 0; i < varsize; i++)
				inp->backch();
				return token("T_variable", letter, num, varsize);
			}
			case '\n':
				return token("T_end");
			default:
			{
				inp->error("| You inputed something that mismatched the lambda calculus language!\n");
				//cout << "character: " << c << " not allowed at that position!\n";
			}
		}
	}
};

lambda * parse_application(lex & L);
lambda * parse_abstraction(lex & L);
lambda * parse_variable(lex & L);
	lambda * parse_lambda(lex & L)
	{
		token t = L.next();;
		//cout << "pos: " << L.inp->pos << endl;
		for(int i = 0; i < t.size; i++)		
			L.inp->backch();
		//L.inp->backch();
		//cout << "pos: " << L.inp->pos << endl;
		if(t.type == "T_variable")
			return parse_variable(L);
		else if(t.type == "T_lambda")
			return parse_abstraction(L);
		else if(t.type == "T_openpar")
			return parse_application(L);
		cout << "Your input mismatched the lambda calculus language!\n";
		exit(0);
	}
	lambda * parse_variable(lex & L)
	{
		//cout << "pos: " << L.inp->pos << endl;
		token t = L.next();
		//cout << "pos: " << L.inp->pos << endl;
		if(t.type != "T_variable")
			L.inp->error("variable expected of notation ex: 'a5', letter than any numbers\n");
		//cout << "letter: " << t.letter << " num: " << t.number << endl;
		return new variable(t.letter, t.number);
	}
	lambda * parse_abstraction(lex & L)
	{
		token t = L.next();
		if(t.type != "T_lambda")
			L.inp->error("lambda '#' expected to start an abstraction\n");
		lambda * v = parse_variable(L);
		t = L.next();
		if (t.type != "T_dot")
			L.inp->error("dot '.' expected for abstraction\n");
		lambda * b = parse_lambda(L);
		return new abstraction(v, b);
	}
	lambda * parse_application(lex & L)
	{
		token t = L.next();
		if(t.type != "T_openpar")
			L.inp->error("open parenthesis '(' expected for applicaton\n");
		//L.inp->print();
		lambda * a = parse_lambda(L);
		//a->print();
		lambda * b = parse_lambda(L);
		t = L.next();
		if(t.type != "T_closepar")
			L.inp->error("close parenthesis ')' expected for application end\n");
		return new application(a,b);
	}	

lambda * reduce(lambda * what, lambda * bereplace, lambda * replacer)
{
	if(bereplace->isVariable())
	{
		if(bereplace->getName() == what->getName())
			return replacer;
		else
			return bereplace;
	}
	else if(bereplace->isApplication())
	{
		lambda * t1 = reduce(what, bereplace->getFunction(), replacer);
		lambda * t2 = reduce(what, bereplace->getArgument(), replacer);
		return new application(t1, t2);
	}
	else if(bereplace->isAbstraction())
	{
		return new abstraction(bereplace->getVar(), reduce(what, bereplace->getBody(), replacer));
	}
	//return bereplace;
}
int counter = 0;
lambda * evaluate(lambda * yeet)
{
	counter++;			//this is only for if a function goes infinite. I am assuming 100 evals would be an infinite function. Can be taken out to really see it crash
	if(counter == 100)	// part of it
		return yeet;	// the return to just print it
	lambda * beingreturned = yeet;
	if(yeet->isApplication())
	{
		lambda * f = yeet->getFunction();
		lambda * a = yeet->getArgument();
		if(f->isAbstraction())
		{
			lambda * v = f->getVar();
			lambda * l = f->getBody();
			lambda * t = reduce(v, l, a);
			beingreturned = evaluate(t);
			return beingreturned;
		}
		else if(f->isApplication())
		{
			lambda * r =  new application(evaluate(f), evaluate(a));
			return evaluate(r);
		}
		else if(f->isVariable())
			return new application(f, evaluate(a));	
	}
	else if(yeet->isVariable())
	{
		return yeet;		
	}
	else if(yeet->isAbstraction())
	{
		return new abstraction(yeet->getVar(), evaluate(yeet->getBody()));		
	}
}

void test(lambda * f)	//a way to print based on answer, TRUE or FALSE
{
	if(f->isAbstraction())			//I never usually have functions in this manner of bracketing but so many brackets and if statements so decided better like this O_o
	{	lambda * x = f->getBody();
		if(x->isAbstraction())
		{	lambda * y = x->getBody();
			if(y->isVariable())
			{	if(y->getName() == 'y')
					cout << "\nSTATEMENT WAS FALSE!\n";
				else if(y->getName() == 'x')
					cout << "\nSTATEMENT WAS TRUE!\n";	} } } }

int main()
{ 
	cout << "|----------------------------------------------------------------------------------------\n";
	cout << "| Hello! Welcome to Palkimas Lambda 'λ' Epression Reducer!\n";
	cout << "| In this program you can enter a lambda 'λ' expression and this will beta 'β' reduce it.\n";
	cout << "| When entering please use '#' as your 'λ'\n";
	cout << "| To exit enter just \"exit\"\n|\n";  
	cout << "| This Lambda 'λ' Calculus is defined as below:\n|\n";
	cout << "| variable    ::= letter{number}\n";
	cout << "| abstraction ::= #variable.lambda\n";
	cout << "| application ::= (lambda lambda)\n";
	cout << "| lambda      ::= variable\n";
	cout << "|                 |abstraction\n";
	cout << "|                 |application\n|\n";
	while(true)
	{
		cout << "| > ";
		lex L;		
		lambda * e = parse_lambda(L);
		lambda * finished = evaluate(e); 	
 		cout << "|\n| Result:\n|\n";
 		cout << "| ";
		finished->print();
		test(finished);
		cout << "\n|\n"; 
		cout << "|----------------------------------------------------------------------------------------\n|\n";
	}
}
