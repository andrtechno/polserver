use os;
use util;

function RecursiveCopy(who, item, copy)
//    var level := 1;
    Set_Critical(1);
    if (item.isA(POLCLASS_CONTAINER))
        var items := EnumerateItemsInContainer(item, ENUMERATE_ROOT_ONLY);
        foreach item in items
            if(!GetObjProperty(item, "LockID"))
                var newitem := CreateItemCopyAtLocation(who.x, who.y, who.z, item);
                MoveItemToContainer(newitem, copy, item.x, item.y);
                RecursiveCopy(who, item, newitem);
                LogStaff(who, "dupe/sub", newitem, newitem.amount);
                if(!newitem.stackable)
                    SetObjProperty(newitem, "Dupe", who.acctname+"/"+who.name);
                    SetObjProperty(newitem, "DupeAt", StrFormatTime("%Y-%d-%m %H:%M:%S",polcore().systime));
                    if(!GetObjProperty(newitem, "Created"))
                        SetObjProperty(newitem, "Created", "None");
                    endif
                    if(!GetObjProperty(newitem, "CreatedAt"))
                        SetObjProperty(newitem, "CreatedAt", "None");
                    endif
                endif

            endif
//level+=1;
        endforeach
    endif
//print(level);
    Set_Critical(0);
endfunction



function RecursiveFind(container, obj, byref list)

    Set_Critical(1);
    if (container.isA(POLCLASS_CONTAINER))
        var items := EnumerateItemsInContainer(container, ENUMERATE_ROOT_ONLY);
        foreach item in items
            RecursiveFind(item, obj, list);




        endforeach
    endif
    Set_Critical(0);
    return list;
endfunction
