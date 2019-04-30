#include "TriplerValidateEventHandler.h"

TriplerValidateEventHandler::TriplerValidateEventHandler()
{
}


TriplerValidateEventHandler::~TriplerValidateEventHandler()
{
}

void TriplerValidateEventHandler::notify(const Ptr<ValidateInputsEventArgs>& eventArgs)
{
	auto inputs = eventArgs->inputs();
	Ptr<SelectionCommandInput> femaleSel = inputs->itemById("selectionFemale");
	Ptr<SelectionCommandInput> maleSel = inputs->itemById("selectionMale");

	if (femaleSel && maleSel) 
	{
		eventArgs->areInputsValid(true);
	}
	else
	{
		eventArgs->areInputsValid(false);
	}
}