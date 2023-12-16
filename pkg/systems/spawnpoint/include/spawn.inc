use uo;
use os;
use cfgfile;
use basic;


include "include/utility";
include ":datafile:datafile";

const DEFAULT_REALM := "britannia";

var cfg := ReadConfigFile(":spawnpoint:setup-britannia");
var data_file := DFOpenDataFile(":spawnpoint:spawn", DF_CREATE);

function CreateNpcSpawn()
    var gateList := GetConfigIntKeys(cfg);
    foreach g in gateList
        var elem := FindConfigElem(cfg, g);
        var realm_elem := DFFindElement(data_file, g, DF_CREATE);
        createSpawn(realm_elem, elem);
    endforeach
endfunction



function createSpawn(realm_elem, elem)
    var coord := SplitWords(GetConfigString(elem, "Coord"));
    var merchants := GetConfigStringArray(elem, "Merchant");
var template := GetConfigString(elem, "Spawn");
var minDelay := GetConfigInt(elem, "MinDelay");
var maxDelay := GetConfigInt(elem, "MaxDelay");
var range := GetConfigInt(elem, "Range");
//print(template);
//return 1;
    var obj := 0x1A401;
    if(!template)
        template:=1;
    endif
    var node := CreateItemAtLocation( CInt(coord[1]), CInt(coord[2]), CInt(coord[3]), obj, 1, DEFAULT_REALM );
    //SetObjProperty(node, "sleep", 1);
    if(!node)
        syslog("error create spawnpoint");
    endif
    SetObjProperty(node, "Spawn", template);
SetObjProperty(node, "MinDelay", minDelay);
SetObjProperty(node, "MaxDelay", maxDelay);
SetObjProperty(node, "Range", range);
SetObjProperty(node, "Amount", 1);
SetObjProperty(node, "Initialized", 1);
SetObjProperty(node, "SpawnQueue", array);
SetObjProperty(node, "Children", array);
SetObjProperty(node, "ResetMe", 1);

    //EraseObjProperty(node, "sleep");
    //node.invisible := 1;
endfunction
