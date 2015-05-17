
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cctype>

std::map<char, int> precedences;

struct ast {

	ast(int var) : type(VAR), variable(var) {}
	ast(std::string f) : type(OP), func(f) {}

	enum type_t { VAR, OP } type;
	std::string func;
	std::vector<ast> operands;
	int variable;

	
};

std::ostream& operator<<(std::ostream& os, const ast& a) {
	if ( a.type == ast::VAR ) {
		os << a.variable;
	} else {
		os << a.func << "(";
		if ( a.operands.size() > 0 ) {
			os << a.operands.front();
		}
		for ( int i = 1; i < a.operands.size(); ++i ) {
			os << ", " << a.operands[i];
		}
		os << ")";
	}
	return os;
}

struct token {
	enum type_t { NUM, OPERATOR } type;

	token(char op) : type(OPERATOR), op(op) {}
	token(int n) : type(NUM), number(n) {}

	int number;
	char op;
};

std::ostream& operator<<(std::ostream& os, const token& t) {
#if 0
	if ( t.type == token::NUM ) {
		os << "num(" << t.number << ")";
	} else {
		os << "op(" << t.op << ")";
	}
#else
	if ( t.type == token::NUM ) {
		os <<  t.number;
	} else {
		os << t.op;
	}
#endif
	return os;
}

void parse(std::string line) {


	std::vector<token> tokens;

	for ( int i = 0; i < line.size(); ++i ) {
		char ch = line[i];
		if ( std::isdigit(ch) ) {
			int num = 0;
			while ( i < line.size() && std::isdigit(line[i]) ) {
				num *= 10;
				num += (line[i] - '0');
				++i;
			}
			tokens.push_back(token(num));
			--i;
		} else {
			tokens.push_back(token(ch));
		}
	}

	
	std::vector<token> rpm;

	std::vector<char> opstack;

	for ( int i = 0; i < tokens.size(); ++i ) {
		const token& cur = tokens[i];

		if ( cur.type == token::NUM ) {
			rpm.push_back(cur);
		} else { //OP
			if ( cur.op == '-' && ( i == 0 || tokens[i-1].type == token::OPERATOR ) ) {
				opstack.push_back('m');
			} else if ( cur.op == '+' && ( i == 0 || tokens[i-1].type == token::OPERATOR ) ) {
				opstack.push_back('p');
			} else if ( cur.op == '(' ) {
				opstack.push_back(cur.op);
			} else if ( cur.op == ')' ) {
				while ( opstack.back() != '(' ) {
					rpm.push_back(opstack.back());
					opstack.pop_back();
				}
				opstack.pop_back();
			} else {
				while ( !opstack.empty() && opstack.back() != '(' && precedences[opstack.back()] < precedences[cur.op] ) {
					rpm.push_back(opstack.back());
					opstack.pop_back();
				}
				opstack.push_back(cur.op);
			}
		}
	}

	while ( !opstack.empty() ) {
		rpm.push_back(opstack.back());
		opstack.pop_back();
	}
	
	//for ( int i = 0; i < rpm.size(); ++i ) {
		//std::cout << rpm[i] << " ";
	//}
	
	std::vector<ast> stack;
	
	for ( int i = 0; i < rpm.size(); ++i ) {
		if ( rpm[i].type == token::NUM ) {
			stack.push_back( ast(rpm[i].number) );
		} else {
			if ( rpm[i].op == 'm' ) {
				ast arg = stack.back();
				stack.pop_back();

				ast r("minus");
				r.operands.push_back(arg);

				stack.push_back(r);
			} else if ( rpm[i].op == 'p' ) {
				ast arg = stack.back();
				stack.pop_back();

				ast r("plus");
				r.operands.push_back(arg);

				stack.push_back(r);
			} else if ( rpm[i].op == '-' ) {
				ast lhs = stack.back();
				stack.pop_back();
				ast rhs = stack.back();
				stack.pop_back();

				if ( lhs.type == ast::OP && lhs.func == "sub" ) {
					lhs.operands.insert(lhs.operands.begin(), rhs);
					stack.push_back(lhs);
				} else {
					ast r("sub");
					r.operands.push_back(rhs);
					r.operands.push_back(lhs);
					
					stack.push_back(r);
				}				
			} else if ( rpm[i].op == '+' ) {
				ast lhs = stack.back();
				stack.pop_back();
				ast rhs = stack.back();
				stack.pop_back();

				if ( lhs.type == ast::OP && lhs.func == "add" ) {
					lhs.operands.insert(lhs.operands.begin(), rhs);
					stack.push_back(lhs);
				} else {
					ast r("add");
					r.operands.push_back(rhs);
					r.operands.push_back(lhs);
					
					stack.push_back(r);
				}				
			} else if ( rpm[i].op == '/' ) {
				ast lhs = stack.back();
				stack.pop_back();
				ast rhs = stack.back();
				stack.pop_back();

				if ( lhs.type == ast::OP && lhs.func == "div" ) {
					lhs.operands.insert(lhs.operands.begin(), rhs);
					stack.push_back(lhs);
				} else {
					ast r("div");
					r.operands.push_back(rhs);
					r.operands.push_back(lhs);
					
					stack.push_back(r);
				}				
			} else if ( rpm[i].op == '*' ) {
				ast lhs = stack.back();
				stack.pop_back();
				ast rhs = stack.back();
				stack.pop_back();

				if ( lhs.type == ast::OP && lhs.func == "mul" ) {
					lhs.operands.insert(lhs.operands.begin(), rhs);
					stack.push_back(lhs);
				} else {
					ast r("mul");
					r.operands.push_back(rhs);
					r.operands.push_back(lhs);
					
					stack.push_back(r);
				}				
			} else if ( rpm[i].op == '^' ) {
				ast lhs = stack.back();
				stack.pop_back();
				ast rhs = stack.back();
				stack.pop_back();

				if ( false && lhs.type == ast::OP && lhs.func == "pow" ) {
					lhs.operands.insert(lhs.operands.begin(), rhs);
					stack.push_back(lhs);
				} else {
					ast r("pow");
					r.operands.push_back(lhs);
					r.operands.push_back(rhs);
					
					
					stack.push_back(r);
				}				
			}
		}
	}

	std::cout << stack.back() << std::endl;

}







int main() {
	precedences['+'] = 5;
	precedences['-'] = 5;
	precedences['p'] = 3;
	precedences['m'] = 3;
	precedences['*'] = 3;
	precedences['/'] = 3;
	precedences['^'] = 1;


	std::ifstream in("h.in");
	
	int tcs;
	in >> tcs;
	in.ignore();

	while ( tcs-- ) {
		std::string line;
		std::getline(in, line);

		parse(line);
	}

}
