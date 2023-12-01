use uo;
use os;

include "include/client";
include ":attributes:attributes";


var combatcfg := ReadConfigFile(":*:itemdesc");

//////////////////////////////////////////////////////////////
// Checks equipped items for the member "medloss".
//
// Parameters: mobile object reference
//
// Returned value: Either the string "nomed" or the numeric value zero
///////////////////////////////////////////////////////////////
function manaloss(who)
//  var medit := GetAttribute(who,ATTRIBUTEID_MEDITATION);
    var elem;
    var loss := 0;

    foreach thing in ListEquippedItems(who)
        elem := FindConfigElem(combatcfg, thing.objtype);
        loss := GetConfigString(elem, "medloss");
        if(loss == "nomed")
            return loss;
            break;
        endif
    endforeach
    return 0;
endfunction
