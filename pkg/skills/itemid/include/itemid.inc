
include "include/namingbyenchant";

function itemd_id(who, the_item)
var ncharges;
    var msg := "It appears to be";

    var name := "";
    var itsname := the_item.name;
    if (!itsname)
        var cfgfile := ReadConfigFile(":*:itemdesc");
        var elem := FindConfigElem(cfgfile, the_item.objtype);
        itsname := elem.desc;
    endif
    if(GetObjProperty(the_item, "silvermod"))
        name :=  "silver" + name;
    endif
    if(GetObjProperty(the_item, "durability"))
        name := name + " " + GetObjProperty(the_item, "durability");
    endif
    if(GetObjProperty(the_item, "accuracy"))
        name := name + " " + GetObjProperty(the_item, "accuracy");
    endif
    itsname["a "] := "";
    itsname["an "] := "";
    itsname["magic "] := "";
    name := name + " " + itsname;
    if(GetObjProperty(the_item, "damagemod"))
        name := name + " " + GetObjProperty(the_item, "damagemod");
    endif
    name.erase(1);
    if((!GetObjProperty(the_item, "IDed")) and (name != ""))
        SetName(the_item, name);
    endif

    itsname := the_item.desc;
    if(itsname[1] == " ")
        if(CInt(itsname[2]) == 0)
            case(CStr(itsname[2]))
            "a": msg := msg + " an";
            "e": msg := msg + " an";
            "i": msg := msg + " an";
            "o": msg := msg + " an";
            "u": msg := msg + " an";
            endcase
        endif
    else
        msg := msg + " ";
        if(Cint(itsname[1]) == 0)
            case(Cstr(itsname[1]))
            "a": msg := msg + " an ";
            "e": msg := msg + " an ";
            "i": msg := msg + " an ";
            "o": msg := msg + " an ";
            "u": msg := msg + " an ";
            endcase
        endif
    endif
    //msg := msg + itsname;

    SetObjProperty( the_item, "IDed", 1 );
    if(GetObjProperty(the_item, "charges") )
        ncharges := CStr(GetObjProperty(the_item, "charges") );
        if(ncharges)
            SendSysMessage(who, "It has " + ncharges + " charges.");
        endif
    endif


    SetNameByEnchant( the_item );


endfunction
