use uo;
use os;
// Only in original map0 will this make mooingates

const DEFAULT_REALM := "britannia";

function CreateMoongates()
  var moongates := {
	{4467,	1283,	5, "Moonglow"},		// moonglow
    {1336,	1997,	5, "Britain"},		// britain
    {1499,	3771,	5, "Jhelom"},		// jehlom
	{771,	753,	5, "Yew"},		    // yew
    {2701,	693,	5, "Minoc"},		// minoc
    {1828,	2948, -20, "Trinsic"},	    // trinsic
    {643,	2067,	5, "Skara"},		// skara
    {3564,	2140,  34, "Magincia"} 	    // magincia
  };
  var moongate;
  syslog( "Creating moongates." );
  foreach gate in moongates
    foreach thing in ListItemsNearLocation(gate[1], gate[2], gate[3], 0, _DEFAULT_REALM);
      if(thing.objtype != 0x1887b)
        DestroyItem(thing);
      endif
    endforeach
	moongate := CreateItemAtLocation( gate[1], gate[2], gate[3], "systemmoongate", 1, _DEFAULT_REALM );
    SetObjProperty(moongate, "City", gate[4]);
    moongate.facing := 29;
    moongate.decayAt := 0;
    moongate.movable := 0;
  endforeach
endfunction


/*
 * MG_FindDestination(button_id)
 *
 * Purpose
 * Takes in a radiobox id value and finds the match for it.
 *
 * Parameters
 * button_id:	Radio box that was checked.
 *
 * Return Value
 *
 * Returns a struct with .x, .y, .z, .realm
 *
 */
function MG_FindDestination(button_id)
	// Not too happy with how I designed this part. - Austin
	// Should probably save a list in a datafile and directly access it by the 'select' variable.

	var config := ReadConfigFile(":moongates:moongate");
	var btn_id := 1;
	foreach realm in ( Realms() )
		realm := realm; // To avoid a compiler warning.
		var cfg_elem := config[_realm_iter];
		foreach entry in ( GetConfigStringDictionary(cfg_elem, "Town") )
			if ( btn_id == button_id )
				var coords := SplitWords(entry);
				var location := struct;
				location.+x := CInt(coords[1]);
				location.+y := CInt(coords[2]);
				location.+z := CInt(coords[3]);
				location.+realm := CStr(_realm_iter);
				return location;
			endif
			btn_id := btn_id+1;
			SleepMS(2);
		endforeach
		SleepMS(2);
	endforeach

endfunction