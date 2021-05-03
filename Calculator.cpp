#include "UltraEngine.h"
#include "Calculator.h"

Calculator::Calculator()
{
	leftOperand = ' ';
	rightOperand = ' ';
	mathOperator = ' ';
	currentState = Calculator::LeftOp;
	isCalculateResult = false;
}

void Calculator::Enter(char input)
{
	switch (currentState)
	{
	case Calculator::LeftOp:
		ProcessOperand(leftOperand, input);
		break;
	case Calculator::Operator: 
		ProcessOperator(input);
		break;
	case Calculator::RightOp:
		ProcessOperand(rightOperand, input);
		break;
	case Calculator::Result:
		break;
	}
}

std::string Calculator::GetOutput()
{
	std::string result = "";
	if (isCalculateResult) result = " = " + to_string((int)CalculateResult());
	return leftOperand + " " + mathOperator + '\n' + rightOperand + result;
}

void Calculator::ProcessOperand(std::string& operand, char input)
{
	if (operand[0] == ' ' || operand[0] == '0')
	{
		if (isdigit(input)) operand = input;
		else operand = '0';
		return;
	}

	switch (input)
	{
	case ' ':
		break;
	case '=':
		if (currentState == Calculator::RightOp) isCalculateResult = true;
		break;
	case '*':
	case '-':
	case '+':
	case '/':
		currentState = Calculator::Operator;
		mathOperator = input;
		break;
	default:
		if (isdigit(input)) operand += input;
		break;
	}

}

void Calculator::ProcessOperator(char input)
{
	switch (input)
	{
	case '*':
	case '-':
	case '+':
	case '/':
		currentState = Calculator::Operator;
		mathOperator = input;
		break;
	default:
		if (isdigit(input))
		{
			currentState = Calculator::RightOp;
			ProcessOperand(rightOperand, input);
		}
		break;
	}
}

float Calculator::CalculateResult()
{
	float left = stof(leftOperand);
	float right = stof(rightOperand);
	float result = 0;
	switch (mathOperator)
	{
	case '*': result = left * right; break;
	case '-': result = left - right; break;
	case '+': result = left + right; break;
	case '/': result = left / right; break;
	}
	return result;
}



