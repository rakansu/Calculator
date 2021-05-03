#include "UltraEngine.h"
#include <iostream>
#include "Calculator.h"

using namespace UltraEngine;


const int WINDOW_WIDTH = 320;
const int WINDOW_HEIGHT = 460;

const int BUTTON_HORIZONTAL_COUNT = 4;
const int BUTTON_VERTICAL_COUNT = 6;



bool RescaleUI(const Event& event, shared_ptr<Object> extra)
{
	float dpiscale = float(event.data) / 100.0f;
	auto ui = dynamic_pointer_cast<Interface>(extra);
	ui->SetScale(dpiscale);
	auto window = dynamic_pointer_cast<Window>(event.source);
	window->SetShape(event.position.x, event.position.y, event.size.x, event.size.y);
	return true;
}



int main(int argc, const char* argv[])
{
	//Get displays
	auto displays = GetDisplays();
	if (displays.empty()) return 1;
	float dpiscale = displays[0]->scale;

	//Create window 
	auto style = WINDOW_CENTER | WINDOW_TITLEBAR;
	auto mainwindow = CreateWindow("Calculator", 0, 0, WINDOW_WIDTH * dpiscale, WINDOW_HEIGHT * dpiscale, displays[0], style);

	//Create user interface
	auto ui = CreateInterface(mainwindow);
	iVec2 sz = ui->root->ClientSize();


	//-------------------------------------------------------
	// Create Digit Panel
	//-------------------------------------------------------
	auto digitPanel = CreatePanel(0, 0, ui-> root -> size.x, ui->root->size.y * 0.2f,  ui->root);
	digitPanel->SetColor(0.2, 0.2, 0.2);

	auto digitText = CreateTextArea(5, 5, digitPanel->size.x - 10, digitPanel->size.y - 10, digitPanel, TEXTAREA_WORDWRAP);
	digitText->SetText("0" );
	digitText->SetFontScale(1.5f);


	//-------------------------------------------------------
	// Create Control Panel
	//-------------------------------------------------------
	auto controlPanel = CreatePanel(0, digitPanel->size.y, ui->root->size.x, ui->root->size.y - digitPanel->size.y, ui->root);
	controlPanel->SetColor(1, 0, 1, 0);

	//-------------------------------------------------------
	// Create Control Panel Buttons
	//-------------------------------------------------------
	int buttonWidth = controlPanel-> size.x / BUTTON_HORIZONTAL_COUNT;
	int buttonHeight = controlPanel-> size.y / BUTTON_VERTICAL_COUNT;
	char buttonInputs[BUTTON_VERTICAL_COUNT][BUTTON_HORIZONTAL_COUNT] =
	{
		{' ', ' ', ' ', ' '},
		{' ', ' ', ' ', '/'},
		{'7', '8', '9', '*'},
		{'4', '5', '6', '-'},
		{'1', '2', '3', '+'},
		{' ', '0', '.', '='},
	};
	auto buttonMatrix = new shared_ptr<UltraEngine::Widget>* [BUTTON_VERTICAL_COUNT];
	for (int i = 0; i < BUTTON_VERTICAL_COUNT; i++)
		buttonMatrix[i] = new shared_ptr<UltraEngine::Widget>[BUTTON_HORIZONTAL_COUNT];

	int bx = 0;
	int by = 0;
	for (int r = 0; r < BUTTON_VERTICAL_COUNT; r++)
	{
		for (int c = 0; c < BUTTON_HORIZONTAL_COUNT; c++)
		{
			buttonMatrix[r][c] = CreateButton(buttonInputs[r][c], bx, by, buttonWidth, buttonHeight, controlPanel, BUTTON_TOOLBAR);
			bx += buttonWidth;
		}
		bx = 0;
		by += buttonHeight;
	}


	//-------------------------------------------------------
	// Create Calculator
	//-------------------------------------------------------
	Calculator calculator;



	//Enable DPI scaling changes
	ui->SetScale(dpiscale);
	ListenEvent(EVENT_WINDOWDPICHANGED, mainwindow, RescaleUI, ui);
	
	//Show the window
	mainwindow->Show();
	mainwindow->Activate();


	bool isExitProgram = false;


	while (!isExitProgram)
	{
		const Event event = WaitEvent();
		switch (event.id)
		{
		case EVENT_WIDGETSELECT:
			break;
		case EVENT_WIDGETACTION:

			//-------------------------------------------------------
			// Process Input from Calculator Buttons
			//-------------------------------------------------------
			for (int r = 0; r < BUTTON_VERTICAL_COUNT; r++)
			{
				for (int c = 0; c < BUTTON_HORIZONTAL_COUNT; c++)
				{
					if (event.source == buttonMatrix[r][c])
					{
						calculator.Enter(buttonInputs[r][c]);
						digitText->SetText(calculator.GetOutput());
					}
				}
			}
			break;
		case EVENT_WINDOWCLOSE:
			if (event.source == mainwindow) isExitProgram = true;
			break;
		}
	}

	//-------------------------------------------------------
	// Garbage In -> Garbage Out
	//-------------------------------------------------------



	return 0;
}