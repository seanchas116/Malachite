#include "blendmode.h"

namespace Malachite
{

BlendModeDictionary::BlendModeDictionary()
{
	nameHash.reserve(27);
	qtCompositionModeHash.reserve(27);
	
	nameHash[BlendModeNormal] = "normal";
	nameHash[BlendModePlus] = "plus";
	nameHash[BlendModeMultiply] = "multiply";
	nameHash[BlendModeScreen] = "screen";
	nameHash[BlendModeOverlay] = "overlay";
	nameHash[BlendModeDarken] = "darken";
	nameHash[BlendModeLighten] = "lighten";
	nameHash[BlendModeColorDodge] = "color-dodge";
	nameHash[BlendModeColorBurn] = "color-burn";
	nameHash[BlendModeHardLight] = "hard-light";
	nameHash[BlendModeSoftLight] = "soft-light";
	nameHash[BlendModeDifference] = "difference";
	nameHash[BlendModeExclusion] = "exclusion";
	
	nameHash[BlendModeClear] = "clear";
	nameHash[BlendModeSource] = "source";
	nameHash[BlendModeDestination] = "destination";
	nameHash[BlendModeSourceOver] = "source-over";
	nameHash[BlendModeDestinationOver] = "destination-over";
	nameHash[BlendModeSourceIn] = "source-in";
	nameHash[BlendModeDestinationIn] = "destination-in";
	nameHash[BlendModeSourceOut] = "source-out";
	nameHash[BlendModeDestinationOut] = "destination-out";
	nameHash[BlendModeSourceAtop] = "source-atop";
	nameHash[BlendModeDestinationAtop] = "destination-atop";
	nameHash[BlendModeXor] = "xor";
	
	qtCompositionModeHash[BlendModeNormal] = QPainter::CompositionMode_SourceOver;
	qtCompositionModeHash[BlendModePlus] = QPainter::CompositionMode_Plus;
	qtCompositionModeHash[BlendModeMultiply] = QPainter::CompositionMode_Multiply;
	qtCompositionModeHash[BlendModeScreen] = QPainter::CompositionMode_Screen;
	qtCompositionModeHash[BlendModeOverlay] = QPainter::CompositionMode_Overlay;
	qtCompositionModeHash[BlendModeDarken] = QPainter::CompositionMode_Darken;
	qtCompositionModeHash[BlendModeLighten] = QPainter::CompositionMode_Lighten;
	qtCompositionModeHash[BlendModeColorDodge] = QPainter::CompositionMode_ColorDodge;
	qtCompositionModeHash[BlendModeColorBurn] = QPainter::CompositionMode_ColorBurn;
	qtCompositionModeHash[BlendModeHardLight] = QPainter::CompositionMode_HardLight;
	qtCompositionModeHash[BlendModeSoftLight] = QPainter::CompositionMode_SoftLight;
	qtCompositionModeHash[BlendModeDifference] = QPainter::CompositionMode_Difference;
	qtCompositionModeHash[BlendModeExclusion] = QPainter::CompositionMode_Exclusion;
	
	qtCompositionModeHash[BlendModeClear] = QPainter::CompositionMode_Clear;
	qtCompositionModeHash[BlendModeSource] = QPainter::CompositionMode_Source;
	qtCompositionModeHash[BlendModeDestination] = QPainter::CompositionMode_Destination;
	qtCompositionModeHash[BlendModeSourceOver] = QPainter::CompositionMode_SourceOver;
	qtCompositionModeHash[BlendModeDestinationOver] = QPainter::CompositionMode_DestinationOver;
	qtCompositionModeHash[BlendModeSourceIn] = QPainter::CompositionMode_SourceIn;
	qtCompositionModeHash[BlendModeDestinationIn] = QPainter::CompositionMode_DestinationIn;
	qtCompositionModeHash[BlendModeSourceOut] = QPainter::CompositionMode_SourceOut;
	qtCompositionModeHash[BlendModeDestinationOut] = QPainter::CompositionMode_DestinationOut;
	qtCompositionModeHash[BlendModeSourceAtop] = QPainter::CompositionMode_SourceAtop;
	qtCompositionModeHash[BlendModeDestinationAtop] = QPainter::CompositionMode_DestinationAtop;
	qtCompositionModeHash[BlendModeXor] = QPainter::CompositionMode_Xor;
}

BlendModeDictionary BlendModeUtil::_dict;

}
