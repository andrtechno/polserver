use uo;
use os;
use cfgfile;
use basic;

include "include/utility";
include ":datafile:datafile";

const DEFAULT_REALM := "britannia";

var cfg := ReadConfigFile(":lifegate:settings_britannia");
var data_file := DFOpenDataFile(":lifegate:gateinfo", DF_CREATE);

function CreateLifeGates()
    var gateList :=GetConfigIntKeys(cfg);
    foreach g in gateList
        var elem :=FindConfigElem(cfg, g);
        var coord :=SplitWords(GetConfigString(elem, "Coord"));
        var realm_elem := DFFindElement(data_file, g, DF_CREATE);
        var serial := realm_elem.GetProp("serial");
        if(serial)
            var item := SystemFindObjectBySerial(serial);
            if(!item)
                createGate(realm_elem, coord);
            endif
        else
            createGate(realm_elem, coord);
        endif

    endforeach

endfunction

function createGate(realm_elem, coord)
    var gate := CreateItemAtLocation( CInt(coord[1]), CInt(coord[2]), CInt(coord[3]), "lifegate", 1, _DEFAULT_REALM );
    if(!gate)
        syslog("error create life gate");
    endif
    var prop := realm_elem.SetProp("serial", gate.serial);
endfunction

function CleanupOldLifes()

	foreach elem_name in ( DFGetElemNames(data_file) )
		var serial := CInt(elem_name);
		var item := SystemFindObjectBySerial(serial);
		if ( !item )
			continue;
		elseif ( item.IsSystemMoongate() )
			DestroyItem(item);
		endif
		data_file.DeleteElement(elem_name);
		SleepMS(2);
	endforeach

	return 1;
endfunction