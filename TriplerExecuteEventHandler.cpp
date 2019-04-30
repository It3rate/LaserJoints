#include "TriplerExecuteEventHandler.h"

const std::vector<std::string> TriplerExecuteEventHandler::colorNames = std::vector<std::string>({ "Yellow", "Red", "Blue", "White" });

TriplerExecuteEventHandler::TriplerExecuteEventHandler()
{
}

TriplerExecuteEventHandler::~TriplerExecuteEventHandler()
{
}

bool TriplerExecuteEventHandler::setLinkages()
{
	app = Application::get();
	if (!app)
		return false;

	ui = app->userInterface();
	if (!ui)
		return false;

	product = app->activeProduct();
	if (!product)
		return false;

	design = product;
	if (!design)
		return false;

	params = design->userParameters();
	if (!params)
		return false;

	return true;
}

void TriplerExecuteEventHandler::notify(const Ptr<CommandEventArgs>& eventArgs)
{
	if (!setLinkages()) {
		return;
	}

	auto inputs = eventArgs->command()->commandInputs();
	ensureParams();

	Ptr<SelectionCommandInput> femaleSel = inputs->itemById("selectionFemale");
	Ptr<SelectionCommandInput> maleSel = inputs->itemById("selectionMale");
	Ptr<Profile> profile0;
	int extrudeDir0;
	Ptr<Profile> profile1;
	int extrudeDir1;
	if (femaleSel)
	{
		profile0 = femaleSel->selection(0)->entity();
		Ptr<ButtonRowCommandInput> extendDirectionInput = inputs->itemById("femaleDirection");
		extrudeDir0 = extendDirectionInput->selectedItem()->index(); // 0 Pos, 1 Cent, 2 Neg
		extrudeDir0 = (extrudeDir0 - 1) * -1; // 1 Pos, 0 Cent, -1 Neg
	}

	if (maleSel)
	{
		profile1 = maleSel->selection(0)->entity();
		Ptr<ButtonRowCommandInput> extendDirectionInput = inputs->itemById("maleDirection");
		extrudeDir1 = extendDirectionInput->selectedItem()->index(); // 0 Pos, 1 Cent, 2 Neg
		extrudeDir1 = (extrudeDir1 - 1) * -1; // 1 Pos, 0 Cent, -1 Neg
	}

	if (femaleSel)
	{
		tripleProfile(profile0, extrudeDir0);
	}
	if (maleSel)
	{
		tripleProfile(profile1, extrudeDir1);
	}
}

void TriplerExecuteEventHandler::ensureParams()
{
	Ptr<adsk::fusion::UserParameter> outerExpr = addOrGetParam(thicknessOuter, "2 mm");
	Ptr<adsk::fusion::UserParameter> innerExpr = addOrGetParam(thicknessInner, "3 mm");
}

void TriplerExecuteEventHandler::tripleProfile(Ptr<Profile> profile, int extrudeDir)
{
	design->activateRootComponent();
	auto rootComponent = design->rootComponent();
	auto ident = Matrix3D::create();
	auto newComponent = rootComponent->occurrences()->addNewComponent(ident);
	newComponent->activate();

	std::string start = (extrudeDir == 0) ? "-(" + thicknessOuter + " + " + thicknessInner + " / 2)" : "0";
	std::string dist = (extrudeDir == -1) ? "-" + thicknessOuter : thicknessOuter;
	auto extrude0 = this->extrudeBody(newComponent, profile, start, dist, 0);

	start = (extrudeDir == -1) ? "-" + thicknessOuter : 
		(extrudeDir == 0) ? "-" + thicknessInner + " / 2" : thicknessOuter;
	dist = (extrudeDir == -1) ? "-" + thicknessInner : thicknessInner;
	auto extrude1 = this->extrudeBody(newComponent, profile, start, dist, 1);

	start = (extrudeDir == -1) ? "-(" + thicknessOuter + " + " + thicknessInner + ")" : 
		(extrudeDir == 0) ? thicknessInner + " / 2" : thicknessOuter + " + " + thicknessInner;
	dist = (extrudeDir == -1) ? "-" + thicknessOuter : thicknessOuter;
	auto extrude2 = this->extrudeBody(newComponent, profile, start, dist, 2);

	design->activateRootComponent();
}
Ptr<adsk::fusion::UserParameter> TriplerExecuteEventHandler::addOrGetParam(
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

Ptr<adsk::fusion::ExtrudeFeature> TriplerExecuteEventHandler::extrudeBody(
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

Ptr<adsk::core::Appearance> TriplerExecuteEventHandler::addAppearance(
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


