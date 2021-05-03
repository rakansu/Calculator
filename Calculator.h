#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <locale> 



class Calculator
{
public:
	Calculator();
	void Enter(char arg);
	std::string GetOutput();

private:
	enum State
	{
		LeftOp,
		Operator,
		RightOp,
		Result
	};

	State currentState;
	std::string leftOperand;
	std::string rightOperand;
	bool isCalculateResult;
	char mathOperator;
	void ProcessOperand(std::string& operand, char input);
	void ProcessOperator(char input);
	float CalculateResult();
};




#endif // !CALCULATOR_H
