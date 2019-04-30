#include "TriplerInputChangedEventHandler.h"



TriplerInputChangedEventHandler::TriplerInputChangedEventHandler()
{
}


TriplerInputChangedEventHandler::~TriplerInputChangedEventHandler()
{
}

void TriplerInputChangedEventHandler::notify(const Ptr<InputChangedEventArgs>& eventArgs)
{

	Ptr<UserInterface> ui = Application::get()->userInterface();
	if (!ui)
		return;

	Ptr<CommandInputs> inputs = eventArgs->inputs();
	if (!inputs)
		return;

	Ptr<CommandInput> cmdInput = eventArgs->input();
	if (!cmdInput)
		return;

	Ptr<TableCommandInput> tableInput = inputs->itemById("table");
	if (!tableInput)
		return;

	if (cmdInput->id() == "tableAdd") {
		//addRowToTable(tableInput);
	}
	else if (cmdInput->id() == "tableDelete") {
		if (tableInput->selectedRow() == -1) {
			ui->messageBox("Select one row to delete.");
		}
		else {
			tableInput->deleteRow(tableInput->selectedRow());
		}
	}
}