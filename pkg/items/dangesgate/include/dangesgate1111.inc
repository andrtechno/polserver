use uo;
use os;
// Only in original map0 will this make mooingates

const DEFAULT_REALM := "britannia";

function CreateDangesgates()
var gates := {
{2498,	916,	0, "Destard"},		    // destard enter 1
{2499,	916,	0, "Destard"},		    // destard enter 2
{2500,	916,	0, "Destard"},		    // destard enter 3
{5242,	1007,	0, "Exit Destard"},		// destard exit 1
{5243,	1007,	0, "Exit Destard"},		// destard exit 2
{5244,	1007,	0, "Exit Destard"}		// destard exit 3
};


var gates2 := {
{2498,	916,	0, "Destard"},		    // destard enter 1
{2499,	916,	0, "Destard"},		    // destard enter 2
{2500,	916,	0, "Destard"},		    // destard enter 3
{5242,	1007,	0, "Exit Destard"},		// destard exit 1
{5243,	1007,	0, "Exit Destard"},		// destard exit 2
{5244,	1007,	0, "Exit Destard"}		// destard exit 3
};

  var dgate;
  syslog( "Creating dangesgates." );
  foreach gate in gates
    foreach thing in ListItemsNearLocation(gate[1], gate[2], gate[3], 0, _DEFAULT_REALM);
      if(thing.objtype != 0x1887b)
        DestroyItem(thing);
      endif
    endforeach
	dgate := CreateItemAtLocation( gate[1], gate[2], gate[3], "systemdangesgate", 1, _DEFAULT_REALM );

SetObjProperty(dgate, "DistX", gate[1]);
SetObjProperty(dgate, "DistY", gate[2]);
SetObjProperty(dgate, "DistZ", gate[3]);
SetObjProperty(dgate, "City", gate[4]);
    dgate.facing := 29;
    dgate.decayAt := 0;
    dgate.movable := 0;
  endforeach
endfunction
