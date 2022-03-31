use uo;
use os;
use cfgfile;
use basic;

const DEFAULT_REALM := "britannia";

var cfg := ReadConfigFile(":dangesgate:dangesgate");


function CreateDangesgates()

    //var gates := {
    //{2498,      916,    0,      5242,       1007,       0,  "Destard"}
   // {2499,	916,	0, "Destard"},
   // {2500,	916,	0, "Destard"},
    //{5242,	1007,	0, "Exit Destard"},
   // {5243,	1007,	0, "Exit Destard"},
   // {5244,	1007,	0, "Exit Destard"}
  //  };
syslog( "Creating dangesgates." );

var gateList :=GetConfigIntKeys(cfg);


foreach g in gateList


var elem :=FindConfigElem(cfg, g);
var enterPoint:=SplitWords(GetConfigString(elem, "PointEnter"));
var exitPoint:= SplitWords(GetConfigString(elem, "PointExit"));
var nameEnter:=CStr(GetConfigString(elem, "NameEnter"));
var nameExit:= GetConfigString(elem, "NameExit");
var invisible:= GetConfigInt(elem, "Invisible");
if(invisible.errortext)
    invisible:=1;
endif;
//syslog(exitPoint);





foreach thing in ListItemsNearLocation(CInt(enterPoint[1]), CInt(enterPoint[2]), CInt(enterPoint[3]), 0, _DEFAULT_REALM);
    if(thing.objtype != 0x1887b)
    DestroyItem(thing);
    endif
endforeach
foreach thing in ListItemsNearLocation(CInt(exitPoint[1]), CInt(exitPoint[2]), CInt(exitPoint[3]), 0, _DEFAULT_REALM);
    if(thing.objtype != 0x1887b)
    DestroyItem(thing);
    endif
endforeach

var dgate := CreateItemAtLocation( CInt(enterPoint[1]), CInt(enterPoint[2]), CInt(enterPoint[3]), "systemdangesgate", 1, _DEFAULT_REALM );
if(!dgate)
syslog("error create dangesgate");
endif
SetObjProperty(dgate, "DistX", exitPoint[1]);
SetObjProperty(dgate, "DistY", exitPoint[2]);
SetObjProperty(dgate, "DistZ", exitPoint[3]);
SetObjProperty(dgate, "City", nameEnter);
SetObjProperty(dgate, "Type", "enter");
// dgate.facing := 29;
dgate.decayAt := 0;
dgate.movable := 0;
dgate.invisible := invisible;

var dgate2 := CreateItemAtLocation( CInt(exitPoint[1]), CInt(exitPoint[2]), CInt(exitPoint[3]), "systemdangesgate", 1, _DEFAULT_REALM );
if(!dgate2)
syslog("error create dangesgate");
endif
SetObjProperty(dgate2, "DistX", enterPoint[1]);
SetObjProperty(dgate2, "DistY", enterPoint[2]);
SetObjProperty(dgate2, "DistZ", enterPoint[3]);
SetObjProperty(dgate2, "City", nameExit);
SetObjProperty(dgate2, "Type", "exit");
//dgate2.facing := 29;
dgate2.decayAt := 0;
dgate2.movable := 0;
dgate2.invisible := invisible;


endforeach

/*

var dgate;
var dgate2;

  foreach gate in gates
    foreach thing in ListItemsNearLocation(gate[1], gate[2], gate[3], 0, _DEFAULT_REALM);
      if(thing.objtype != 0x1887b)
        DestroyItem(thing);
      endif
    endforeach
    foreach thing in ListItemsNearLocation(gate[4], gate[5], gate[6], 0, _DEFAULT_REALM);
    if(thing.objtype != 0x1887b)
    DestroyItem(thing);
    endif
    endforeach

	dgate := CreateItemAtLocation( gate[1], gate[2], gate[3], "systemdangesgate", 1, _DEFAULT_REALM );
    SetObjProperty(dgate, "DistX", gate[4]);
    SetObjProperty(dgate, "DistY", gate[5]);
    SetObjProperty(dgate, "DistZ", gate[6]);
    SetObjProperty(dgate, "City", gate[7]);
    SetObjProperty(dgate, "Type", "enter");
   // dgate.facing := 29;
    dgate.decayAt := 0;
    dgate.movable := 0;
    dgate.invisible := 0;

    dgate2 := CreateItemAtLocation( gate[4], gate[5], gate[6], "systemdangesgate", 1, _DEFAULT_REALM );
    SetObjProperty(dgate2, "DistX", gate[1]);
    SetObjProperty(dgate2, "DistY", gate[2]);
    SetObjProperty(dgate2, "DistZ", gate[3]);
    SetObjProperty(dgate2, "City", gate[7]);
    SetObjProperty(dgate2, "Type", "exit");
    //dgate2.facing := 29;
    dgate2.decayAt := 0;
    dgate2.movable := 0;
    dgate2.invisible := 0;
  endforeach*/
endfunction
