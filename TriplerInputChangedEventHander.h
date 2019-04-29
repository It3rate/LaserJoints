#pragma once

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>

using namespace adsk::core;
using namespace adsk::fusion;

class TriplerInputChangedEventHander :
	public adsk::core::InputChangedEventHandler
{
public:
	TriplerInputChangedEventHander();
	~TriplerInputChangedEventHander();
	void notify(const Ptr<InputChangedEventArgs>& eventArgs) override;
};

