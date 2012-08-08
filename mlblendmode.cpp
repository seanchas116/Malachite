#include "mlblendmode.h"

MLBlendModeDictionary::MLBlendModeDictionary()
{
	nameHash.reserve(17);
	qtCompositionModeHash.reserve(17);
	
	nameHash.insert(ML::BlendModeNormal, "normal");
	nameHash.insert(ML::BlendModePlus, "plus");
	nameHash.insert(ML::BlendModeMultiply, "multiply");
	nameHash.insert(ML::BlendModeScreen, "screen");
	nameHash.insert(ML::BlendModeOverlay, "overlay");
	nameHash.insert(ML::BlendModeDarken, "darken");
	nameHash.insert(ML::BlendModeLighten, "lighten");
	nameHash.insert(ML::BlendModeColorDodge, "color-dodge");
	nameHash.insert(ML::BlendModeColorBurn, "color-burn");
	nameHash.insert(ML::BlendModeHardLight, "hard-light");
	nameHash.insert(ML::BlendModeSoftLight, "soft-light");
	nameHash.insert(ML::BlendModeDifference, "difference");
	nameHash.insert(ML::BlendModeExclusion, "exclusion");
	
	qtCompositionModeHash.insert(ML::BlendModeNormal, QPainter::CompositionMode_SourceOver);
	qtCompositionModeHash.insert(ML::BlendModePlus, QPainter::CompositionMode_Plus);
	qtCompositionModeHash.insert(ML::BlendModeMultiply, QPainter::CompositionMode_Multiply);
	qtCompositionModeHash.insert(ML::BlendModeScreen, QPainter::CompositionMode_Screen);
	qtCompositionModeHash.insert(ML::BlendModeOverlay, QPainter::CompositionMode_Overlay);
	qtCompositionModeHash.insert(ML::BlendModeDarken, QPainter::CompositionMode_Darken);
	qtCompositionModeHash.insert(ML::BlendModeLighten, QPainter::CompositionMode_Lighten);
	qtCompositionModeHash.insert(ML::BlendModeColorDodge, QPainter::CompositionMode_ColorDodge);
	qtCompositionModeHash.insert(ML::BlendModeColorBurn, QPainter::CompositionMode_ColorBurn);
	qtCompositionModeHash.insert(ML::BlendModeHardLight, QPainter::CompositionMode_HardLight);
	qtCompositionModeHash.insert(ML::BlendModeSoftLight, QPainter::CompositionMode_SoftLight);
	qtCompositionModeHash.insert(ML::BlendModeDifference, QPainter::CompositionMode_Difference);
	qtCompositionModeHash.insert(ML::BlendModeExclusion, QPainter::CompositionMode_Exclusion);
}

MLBlendModeDictionary MLBlendMode::_dict;
