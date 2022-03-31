use uo;
use os;

function IsToolEquipped(me,tool)
    if((!Accessible(me,tool)) || (Distance(me, tool) > 1))
        return 0;
    endif
    foreach item in ListEquippedItems(me)
        if (tool.serial == item.serial )
        return 1;
        endif
    endforeach
    return EquipItem(me, tool );
endfunction


function HasToolEquipped(me,tool)
    if((!Accessible(me,tool)) || (Distance(me, tool) > 1))
        return 0;
    endif
    foreach item in ListEquippedItems(me)
        if (tool.serial == item.serial )
        return 1;
        endif
    endforeach
endfunction