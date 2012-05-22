#include "mlblendmode.h"

MLBlendModeDictionary::MLBlendModeDictionary()
{
	nameHash.reserve(17);
	qtCompositionModeHash.reserve(17);
	
	nameHash.insert(MLGlobal::BlendModeNormal, "normal");
	nameHash.insert(MLGlobal::BlendModePlus, "plus");
	nameHash.insert(MLGlobal::BlendModeMultiply, "multiply");
	nameHash.insert(MLGlobal::BlendModeScreen, "screen");
	nameHash.insert(MLGlobal::BlendModeOverlay, "overlay");
	nameHash.insert(MLGlobal::BlendModeDarken, "darken");
	nameHash.insert(MLGlobal::BlendModeLighten, "lighten");
	nameHash.insert(MLGlobal::BlendModeColorDodge, "color-dodge");
	nameHash.insert(MLGlobal::BlendModeColorBurn, "color-burn");
	nameHash.insert(MLGlobal::BlendModeHardLight, "hard-light");
	nameHash.insert(MLGlobal::BlendModeSoftLight, "soft-light");
	nameHash.insert(MLGlobal::BlendModeDifference, "difference");
	nameHash.insert(MLGlobal::BlendModeExclusion, "exclusion");
	
	qtCompositionModeHash.insert(MLGlobal::BlendModeNormal, QPainter::CompositionMode_SourceOver);
	qtCompositionModeHash.insert(MLGlobal::BlendModePlus, QPainter::CompositionMode_Plus);
	qtCompositionModeHash.insert(MLGlobal::BlendModeMultiply, QPainter::CompositionMode_Multiply);
	qtCompositionModeHash.insert(MLGlobal::BlendModeScreen, QPainter::CompositionMode_Screen);
	qtCompositionModeHash.insert(MLGlobal::BlendModeOverlay, QPainter::CompositionMode_Overlay);
	qtCompositionModeHash.insert(MLGlobal::BlendModeDarken, QPainter::CompositionMode_Darken);
	qtCompositionModeHash.insert(MLGlobal::BlendModeLighten, QPainter::CompositionMode_Lighten);
	qtCompositionModeHash.insert(MLGlobal::BlendModeColorDodge, QPainter::CompositionMode_ColorDodge);
	qtCompositionModeHash.insert(MLGlobal::BlendModeColorBurn, QPainter::CompositionMode_ColorBurn);
	qtCompositionModeHash.insert(MLGlobal::BlendModeHardLight, QPainter::CompositionMode_HardLight);
	qtCompositionModeHash.insert(MLGlobal::BlendModeSoftLight, QPainter::CompositionMode_SoftLight);
	qtCompositionModeHash.insert(MLGlobal::BlendModeDifference, QPainter::CompositionMode_Difference);
	qtCompositionModeHash.insert(MLGlobal::BlendModeExclusion, QPainter::CompositionMode_Exclusion);
}

MLBlendModeDictionary MLBlendMode::_dict;
