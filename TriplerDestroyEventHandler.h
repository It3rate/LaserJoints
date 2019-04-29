#pragma once

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>

using namespace adsk::core;
using namespace adsk::fusion;

#include "C:\Users\Robin\AppData\Roaming\Autodesk\Autodesk Fusion 360\API\CPP\include\Core\UserInterface\CommandEventHandler.h"
class TriplerDestroyEventHandler :
	public adsk::core::CommandEventHandler
{
public:
	TriplerDestroyEventHandler();
	~TriplerDestroyEventHandler();
	void notify(const Ptr<CommandEventArgs>& eventArgs) override;
};

