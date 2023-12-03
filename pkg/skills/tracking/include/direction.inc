use uo;
use os;

function Get_direction(who, critter)
    var ns_desc := "";
    var ew_desc := "";
    var thedesc;
    if(critter.y < who.y)
        ns_desc := "north";
    elseif(critter.y > who.y)
        ns_desc := "south";
    endif
    if(critter.x < who.x)
        ew_desc := "west";
    elseif(critter.x > who.x)
        ew_desc := "east";
    endif
    if(ns_desc == "" && ew_desc == "")
        thedesc := "nearby";
    else
        thedesc := ns_desc + ew_desc;
    endif
    return thedesc;
endfunction
