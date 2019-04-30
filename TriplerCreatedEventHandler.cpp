#include "triplerCreatedEventHandler.h"

TriplerCreatedEventHandler::TriplerCreatedEventHandler()
{
}
TriplerCreatedEventHandler::~TriplerCreatedEventHandler()
{
}

void TriplerCreatedEventHandler::notify(const Ptr<CommandCreatedEventArgs>& eventArgs)
{
	if (eventArgs)
	{
		Ptr<UserInterface> ui = Application::get()->userInterface();
		if (!ui)
			return;
		Ptr<Command> command = eventArgs->command();

		Ptr<CommandEvent> onExecute = command->execute();
		if (!onExecute)
			return;
		bool isOk = onExecute->add(&onExecuteHandler);
		if (!isOk)
			return;

		Ptr<CommandEvent> onDestroy = command->destroy();
		if (!onDestroy)
			return;
		isOk = onDestroy->add(&onDestroyHandler);
		if (!isOk)
			return;

		Ptr<InputChangedEvent> onInputChanged = command->inputChanged();
		if (!onInputChanged)
			return;
		isOk = onInputChanged->add(&onInputChangedHandler);
		if (!isOk)
			return;

		Ptr<CommandInputs> inputs = command->commandInputs();
		if (!inputs)
			return;

		createUI(inputs);
	}
}

void TriplerCreatedEventHandler::createUI(Ptr<CommandInputs> inputs)
{
	// Female
	auto selectionInputFemale = inputs->addSelectionInput("selectionFemale", "Female Profile", "Select target side.");
	selectionInputFemale->addSelectionFilter("Profiles");

	Ptr<ButtonRowCommandInput> femaleDirectionInput = inputs->addButtonRowCommandInput("femaleDirection", "Extrude Direction", false);
	if (!femaleDirectionInput)
		return;
	Ptr<ListItems> femaleDirectionItems = femaleDirectionInput->listItems();
	if (!femaleDirectionItems)
		return;
	auto btnPos = femaleDirectionItems->add("Positive", false, "Resources/Positive");
	auto btnCen = femaleDirectionItems->add("Centered", false, "Resources/Centered");
	auto btnNeg = femaleDirectionItems->add("Negative", false, "Resources/Negative");
	btnPos->isSelected(true);

	// Male
	auto selectionInputMale = inputs->addSelectionInput("selectionMale", "Male Profile", "Select source side.");
	selectionInputMale->addSelectionFilter("Profiles");

	Ptr<ButtonRowCommandInput> maleDirectionInput = inputs->addButtonRowCommandInput("maleDirection", "Extrude Direction", false);
	if (!maleDirectionInput)
		return;
	Ptr<ListItems> maleDirectionItems = maleDirectionInput->listItems();
	if (!maleDirectionItems)
		return;
	btnPos = maleDirectionItems->add("Positive", false, "Resources/Positive");
	btnCen = maleDirectionItems->add("Centered", false, "Resources/Centered");
	btnNeg = maleDirectionItems->add("Negative", false, "Resources/Negative");
	btnPos->isSelected(true);



	auto slotSize = inputs->addIntegerSpinnerCommandInput("slotSize", "Slot Size (mm)", 1, 500, 1, 50);
}


