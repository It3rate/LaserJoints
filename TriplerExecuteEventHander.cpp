#include "TriplerExecuteEventHander.h"

const std::vector<std::string> TriplerExecuteEventHander::colorNames = std::vector<std::string>({ "Yellow", "Red", "Blue", "White" });
const std::string outer = "thickness_outer";
const std::string inner = "thickness_inner";

TriplerExecuteEventHander::TriplerExecuteEventHander()
{
}


TriplerExecuteEventHander::~TriplerExecuteEventHander()
{
}

void TriplerExecuteEventHander::notify(const Ptr<CommandEventArgs>& eventArgs)
{
	app = Application::get();
	if (!app)
		return;

	ui = app->userInterface();
	if (!ui)
		return;

	product = app->activeProduct();
	if (!product)
		return;

	design = product;
	if (!design)
		return;

	params = design->userParameters();
	if (!params)
		return;

	//ui->activeSelections()->clear();
	while (ui->activeSelections()->count() < 2)
	{
		auto sel = ui->selectEntity("Select a Profile.", "Profiles");
		if (sel)
		{
			ui->activeSelections()->add(sel);
		}
	}

	//Ptr<Selection> sel;
	//if (ui->activeSelections()->count() > 0 &&
	//	ui->activeSelections()->item(0)->entity()->objectType() == adsk::fusion::Profile::classType())
	//{
	//	sel = ui->activeSelections()->item(0);
	//}
	//else
	//{
	//	sel = ui->selectEntity("Select a Profile.", "Profiles");
	//}

	if (ui->activeSelections()->count() > 1)
	{
		ensureParams();
		Ptr<Profile> profile0 = ui->activeSelections()->item(0)->entity();
		Ptr<Profile> profile1 = ui->activeSelections()->item(1)->entity();
		//Ptr<Profile> profile = sel->entity();
		tripleProfile(profile0);
		tripleProfile(profile1);
	}
	else
	{
		ui->messageBox("no selection");
	}
	//adsk::terminate();
}

void TriplerExecuteEventHander::ensureParams()
{
	Ptr<adsk::fusion::UserParameter> outerExpr = addOrGetParam(outer, "2 mm");
	Ptr<adsk::fusion::UserParameter> innerExpr = addOrGetParam(inner, "3 mm");
}

void TriplerExecuteEventHander::tripleProfile(Ptr<Profile> profile)
{
	design->activateRootComponent();
	auto rootComponent = design->rootComponent();
	auto ident = Matrix3D::create();
	auto newComponent = rootComponent->occurrences()->addNewComponent(ident);
	newComponent->activate();

	auto extrude0 = this->extrudeBody(newComponent, profile, "0", outer, 0);
	auto extrude1 = this->extrudeBody(newComponent, profile, outer, inner, 1);
	auto extrude2 = this->extrudeBody(newComponent, profile, outer + " + " + inner, outer, 2);

	design->activateRootComponent();
}
Ptr<adsk::fusion::UserParameter> TriplerExecuteEventHander::addOrGetParam(
	std::string name, 
	std::string defaultExpression)
{
	Ptr<adsk::fusion::UserParameter> param = params->itemByName(name);
	if (!param)
	{
		param = params->add(name, adsk::core::ValueInput::createByString(defaultExpression), "mm", "");
	}
	return param;
}

Ptr<adsk::fusion::ExtrudeFeature> TriplerExecuteEventHander::extrudeBody(
	Ptr<adsk::fusion::Occurrence> occurance,
	Ptr<adsk::fusion::Profile> profile,
	std::string start,
	std::string distance,
	int colorIndex)
{
	auto extrudes = occurance->component()->features()->extrudeFeatures();
	auto extrudeInput = extrudes->createInput(profile, FeatureOperations::NewBodyFeatureOperation);

	auto startExpr = ValueInput::createByString(start);
	auto distanceExpr = ValueInput::createByString(distance);
	auto zeroExpr = ValueInput::createByReal(0.0);
	Ptr<ExtentDefinition> startExtent = OffsetStartDefinition::create(startExpr);
	auto distExtent = DistanceExtentDefinition::create(distanceExpr);
	auto zeroExtent = DistanceExtentDefinition::create(zeroExpr);

	// BUG: Setting only one side extent evaluates the expression rather than recording it.
	//extrudeInput->setOneSideExtent(distExtent, ExtentDirections::PositiveExtentDirection, zeroExpr);
	extrudeInput->setTwoSidesExtent(distExtent, zeroExtent);
	extrudeInput->startExtent(startExtent);

	extrudeInput->operation(FeatureOperations::NewBodyFeatureOperation);
	auto extrude = extrudes->add(extrudeInput);
	if (extrude->bodies()->count() > 0)
	{
		auto body = extrude->bodies()->item(0);
		body->name("b_" + colorNames[colorIndex]);
		addAppearance(body, colorIndex);
	}
	return extrude;
}

Ptr<adsk::core::Appearance> TriplerExecuteEventHander::addAppearance(
	Ptr<BRepBody> body,
	int colorIndex)
{
	Ptr<MaterialLibrary> lib = app->materialLibraries()->itemByName("Fusion 360 Appearance Library");
	auto colorName = "Plastic - Matte (" + colorNames[colorIndex] + ")";
	if (!design->appearances()->itemByName(colorName)) {
		auto appearance = lib->appearances()->itemByName(colorName);
		appearance->copyTo(design);
	}
	auto localAppearance = design->appearances()->itemByName(colorName);
	body->appearance(localAppearance);
	return localAppearance;
}


