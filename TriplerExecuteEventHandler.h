#pragma once

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>

using namespace adsk::core;
using namespace adsk::fusion;

const std::string thicknessOuter = "thickness_outer";
const std::string thicknessInner = "thickness_inner";

class TriplerExecuteEventHandler :
	public adsk::core::CommandEventHandler
{
public:
	//const static char* colorNames[];
	static const std::vector<std::string> colorNames;
	TriplerExecuteEventHandler();
	~TriplerExecuteEventHandler();
	void notify(const Ptr<CommandEventArgs>& eventArgs) override;

protected:

	Ptr<Application> app;
	Ptr<UserInterface> ui;
	Ptr<Product> product;
	Ptr<Design> design;
	Ptr<adsk::fusion::UserParameters> params;

	bool setLinkages();
	void ensureParams();

	void tripleProfile(Ptr<Profile> profile, int extrudeDir);

	Ptr<adsk::fusion::UserParameter> addOrGetParam(
		std::string name,
		std::string defaultExpression);

	Ptr<adsk::fusion::ExtrudeFeature> extrudeBody(
		Ptr<adsk::fusion::Occurrence> occurance,
		Ptr<adsk::fusion::Profile> profile,
		std::string start,
		std::string distance,
		int colorIndex);

	Ptr<adsk::core::Appearance> addAppearance(
		Ptr<BRepBody> body,
		int colorIndex);
};

