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


//////////////////////////////////////////////////
// Checks for items equipped in the hands that might interfere with meditation.
//
// Legal items are: Black Staff, Shepherd's Crook, Gnarled Staff, Quarter Staff,
// 									Spellbook, Paladin Spellbook, Codex Damnorum (necro tome), Book of Arms and
//									Book of Spellweaving.
//
// Parameters: Mobile object reference
//
// Returned value: zero if no interferring items are in a hand or 1 if the item interferes.
//
//////////////////////////////////////////////////
function IsThingOnHand(who)
    var hand1 := (GetEquipmentByLayer(who, 1));
    var hand2 := (GetEquipmentByLayer(who, 2));
    if(hand1)
        if ((hand1.graphic == 0x0df0) || (hand1.graphic == 0x0e81) || (hand1.graphic == 0x13f9) || (hand1.graphic == 0x13f8)
        || (hand1.graphic == 0x0e8a) || (hand1.graphic == 0x0efa) || (hand1.graphic == 0x2252) || (hand1.graphic == 0x2253)
        || (hand1.graphic == 0x2254) || (hand1.graphic == 0x2D50))
        else
            SendSysMessage(who,"Your equipment prevents meditation.", color := 33);
            return 1;
        endif
    elseif (hand2)
        if ((hand1.graphic == 0x0df0) || (hand1.graphic == 0x0e81) || (hand1.graphic == 0x13f9) || (hand1.graphic == 0x13f8)
            || (hand1.graphic == 0x0e8a) || (hand1.graphic == 0x0efa) || (hand1.graphic == 0x2252) || (hand1.graphic == 0x2253)
            || (hand1.graphic == 0x2254) || (hand1.graphic == 0x2D50))
        else
            SendSysMessage(who,"Your equipment prevents meditation.", color := 33);
            return 1;
        endif
    else
        return 0;
    endif
endfunction

