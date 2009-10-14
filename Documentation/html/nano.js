//============================================================================
//		Window load
//----------------------------------------------------------------------------
window.onload = function()
{


	// Apply the corners
	settings = {	tl: { radius: 6 },
					tr: { radius: 6 },
					bl: { radius: 6 },
					br: { radius: 6 },
					antiAlias: true,
					autoPad:   true };

	theCorners = new curvyCorners(settings, "box");
	theCorners.applyCornersToAll();
		
	theCorners = new curvyCorners(settings, "box code");
	theCorners.applyCornersToAll();
}

