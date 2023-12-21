use uo;
use os;
use cfgfile;
use basic;


include "include/utility";
include ":datafile:datafile";

const DEFAULT_REALM := "britannia";

var cfg := ReadConfigFile(":merchantSpawn:settings_britannia");
var data_file := DFOpenDataFile(":merchantSpawn:merchantSpawn", DF_CREATE);

function CreateMerchantSpawn()
    var gateList := GetConfigIntKeys(cfg);
    foreach g in gateList
        var elem := FindConfigElem(cfg, g);
        var realm_elem := DFFindElement(data_file, g, DF_CREATE);
        var serial := realm_elem.GetProp("serial");
        if(serial)
            var item := SystemFindObjectBySerial(CInt(serial));
            if(!item)
                //print("createSpawn 1");
                createSpawn(realm_elem, elem);
            endif
        else
            //print("createSpawn 2");
            createSpawn(realm_elem, elem);
        endif

    endforeach

endfunction



function createSpawn(realm_elem, elem)
    var coord := SplitWords(GetConfigString(elem, "Coord"));
    var merchants := GetConfigStringArray(elem, "Merchant");
    var template := GetConfigInt(elem, "Template");
    var obj := 0x1887a;
    if(!template)
        template:=1;
    endif
    var node := CreateItemAtLocation( CInt(coord[1]), CInt(coord[2]), CInt(coord[3]), obj, 1, DEFAULT_REALM );
    SetObjProperty(node, "sleep", 1);
    if(!node)
        syslog("error create merchantspawn");
    endif
    SetObjProperty(node, "Template", template);
    SetObjProperty(node, "Restock", 30);
    realm_elem.SetProp("serial", node.serial);
    foreach merch in merchants
        var split := SplitWords(merch);
        node.Insert(split[1], struct{x:=CInt(split[2]), y:=CInt(split[3]), z:=CInt(split[4])});
    endforeach
    EraseObjProperty(node, "sleep");
    node.invisible := 1;
endfunction
