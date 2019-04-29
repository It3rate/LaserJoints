#pragma once

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include "TriplerExecuteEventHander.h"
#include "TriplerDestroyEventHandler.h"
#include "TriplerInputChangedEventHander.h"

using namespace adsk::core;
using namespace adsk::fusion;

class TriplerCreatedEventHandler :
	public adsk::core::CommandCreatedEventHandler
{
public:
	TriplerCreatedEventHandler();
	~TriplerCreatedEventHandler();

	void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override;
private:
	TriplerExecuteEventHander onExecuteHandler;
	TriplerDestroyEventHandler onDestroyHandler;
	TriplerInputChangedEventHander onInputChangedHandler;
};

