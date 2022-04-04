use uo;
use os;
use cfgfile;
use basic;

const DEFAULT_REALM := "britannia";

var cfg := ReadConfigFile(":dangesgate:dangesgate");


function CreateDangesgates()
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

endfunction
