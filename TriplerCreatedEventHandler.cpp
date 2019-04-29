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
		//ui->messageBox("created");
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
	}
}
