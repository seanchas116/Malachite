#include "mlblendmode.h"

namespace Malachite
{

BlendModeDictionary::BlendModeDictionary()
{
	nameHash.reserve(17);
	qtCompositionModeHash.reserve(17);
	
	nameHash.insert(Malachite::BlendModeNormal, "normal");
	nameHash.insert(Malachite::BlendModePlus, "plus");
	nameHash.insert(Malachite::BlendModeMultiply, "multiply");
	nameHash.insert(Malachite::BlendModeScreen, "screen");
	nameHash.insert(Malachite::BlendModeOverlay, "overlay");
	nameHash.insert(Malachite::BlendModeDarken, "darken");
	nameHash.insert(Malachite::BlendModeLighten, "lighten");
	nameHash.insert(Malachite::BlendModeColorDodge, "color-dodge");
	nameHash.insert(Malachite::BlendModeColorBurn, "color-burn");
	nameHash.insert(Malachite::BlendModeHardLight, "hard-light");
	nameHash.insert(Malachite::BlendModeSoftLight, "soft-light");
	nameHash.insert(Malachite::BlendModeDifference, "difference");
	nameHash.insert(Malachite::BlendModeExclusion, "exclusion");
	
	qtCompositionModeHash.insert(Malachite::BlendModeNormal, QPainter::CompositionMode_SourceOver);
	qtCompositionModeHash.insert(Malachite::BlendModePlus, QPainter::CompositionMode_Plus);
	qtCompositionModeHash.insert(Malachite::BlendModeMultiply, QPainter::CompositionMode_Multiply);
	qtCompositionModeHash.insert(Malachite::BlendModeScreen, QPainter::CompositionMode_Screen);
	qtCompositionModeHash.insert(Malachite::BlendModeOverlay, QPainter::CompositionMode_Overlay);
	qtCompositionModeHash.insert(Malachite::BlendModeDarken, QPainter::CompositionMode_Darken);
	qtCompositionModeHash.insert(Malachite::BlendModeLighten, QPainter::CompositionMode_Lighten);
	qtCompositionModeHash.insert(Malachite::BlendModeColorDodge, QPainter::CompositionMode_ColorDodge);
	qtCompositionModeHash.insert(Malachite::BlendModeColorBurn, QPainter::CompositionMode_ColorBurn);
	qtCompositionModeHash.insert(Malachite::BlendModeHardLight, QPainter::CompositionMode_HardLight);
	qtCompositionModeHash.insert(Malachite::BlendModeSoftLight, QPainter::CompositionMode_SoftLight);
	qtCompositionModeHash.insert(Malachite::BlendModeDifference, QPainter::CompositionMode_Difference);
	qtCompositionModeHash.insert(Malachite::BlendModeExclusion, QPainter::CompositionMode_Exclusion);
}

BlendModeDictionary BlendModeUtil::_dict;

}
