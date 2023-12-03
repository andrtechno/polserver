use polsys;

include "include/objtype";

function makeItem(metal, weapon, backpack)
    var tname;
    var nme;
    var itemname;
    var ar_mod := 0;
    var dmg_mod := 0;
    var maxhp_mod := 0;
    var quality_mod := 0;

    var theitem := CreateItemInContainer(backpack, weapon, 1);
    itemname := smith_cfg[theitem.objtype].name;
    case(metal.objtype)
            UOBJ_INGOT_COPPER:     nme := ""              + itemname;
            UOBJ_INGOT_IRON:       nme := "iron "         + itemname; ar_mod:=1; dmg_mod:=1; maxhp_mod:=1; quality_mod:=1;
            UOBJ_INGOT_STEEL:      nme := "steel "        + itemname; ar_mod:=2; dmg_mod:=2; maxhp_mod:=2; quality_mod:=2;
            UOBJ_INGOT_ANRA:       nme := "anra "         + itemname; ar_mod:=3; dmg_mod:=3; maxhp_mod:=3; quality_mod:=3;
            UOBJ_INGOT_VALORITE:   nme := "valorite "     + itemname; ar_mod:=4; dmg_mod:=4; maxhp_mod:=4; quality_mod:=4;
            UOBJ_INGOT_LAVAROCK:   nme := "lavarock "     + itemname; ar_mod:=5; dmg_mod:=5; maxhp_mod:=5; quality_mod:=5;
            UOBJ_INGOT_ICEROCK:    nme := "icerock "      + itemname; ar_mod:=6; dmg_mod:=6; maxhp_mod:=6; quality_mod:=6;
            UOBJ_INGOT_SHADOW:     nme := "shadow "       + itemname; ar_mod:=7; dmg_mod:=7; maxhp_mod:=7; quality_mod:=7;
            UOBJ_INGOT_AZURITE:    nme := "azurite "      + itemname; ar_mod:=8; dmg_mod:=8; maxhp_mod:=8; quality_mod:=8;
            UOBJ_INGOT_DOOM:       nme := "doom "         + itemname; ar_mod:=9; dmg_mod:=9; maxhp_mod:=9; quality_mod:=9;
            UOBJ_INGOT_BLUESTEEL:  nme := "bluesteel "    + itemname; ar_mod:=10; dmg_mod:=10; maxhp_mod:=10; quality_mod:=10;
            UOBJ_INGOT_DARKRUBY:   nme := "darkruby "     + itemname; ar_mod:=11; dmg_mod:=11; maxhp_mod:=11; quality_mod:=11;
            UOBJ_INGOT_CRYSTALIT:  nme := "crystalit "    + itemname; ar_mod:=12; dmg_mod:=12; maxhp_mod:=12; quality_mod:=12;
            UOBJ_INGOT_ONYX:       nme := "onyx "         + itemname; ar_mod:=13; dmg_mod:=13; maxhp_mod:=13; quality_mod:=13;
            UOBJ_INGOT_MIFRIL:     nme := "mifril "       + itemname; ar_mod:=14; dmg_mod:=14; maxhp_mod:=14; quality_mod:=14;
    endcase
    theitem.color := metal.color;
    //theitem.quality := 0.26 * quality_mod; //old
    theitem.quality := 0.20 * quality_mod;
    theitem.hp          := theitem.maxhp;
    theitem.ar_mod      := (ar_mod * 2);

    if(!theitem)
        //theitem.hit_chance := 5;
        //theitem.hit_chance_mod := 10;
    endif;


    //weapon
    //theitem.dmg_mod := dmg_mod;

    tname := "an exceptional " + nme;
    SetObjProperty(theitem, "craftername", craftername);
    SetObjProperty(theitem, "type", "armor_"+nme);
    SetName(theitem, tname);

endfunction



function makeItem2(metal2, weapon, backpack)
    var tname;
    var ar_mod := 0;
    var dmg_mod := 0;
    var maxhp_mod := 0;
    var quality_mod := 0;

    var theitem := CreateItemInContainer(backpack, weapon, 1);
    var metal := GetItemDescriptor(metal2);
    var itemInfo := GetItemDescriptor(weapon);
    var itemname := metal.name;

    case(metal.objtype)
            UOBJ_INGOT_COPPER:
            UOBJ_INGOT_IRON:       ar_mod:=1; dmg_mod:=1; maxhp_mod:=1; quality_mod:=1;
            UOBJ_INGOT_STEEL:      ar_mod:=2; dmg_mod:=2; maxhp_mod:=2; quality_mod:=2;
            UOBJ_INGOT_ANRA:       ar_mod:=3; dmg_mod:=3; maxhp_mod:=3; quality_mod:=3;
            UOBJ_INGOT_VALORITE:   ar_mod:=4; dmg_mod:=4; maxhp_mod:=4; quality_mod:=4;
            UOBJ_INGOT_LAVAROCK:   ar_mod:=5; dmg_mod:=5; maxhp_mod:=5; quality_mod:=5;
            UOBJ_INGOT_ICEROCK:    ar_mod:=6; dmg_mod:=6; maxhp_mod:=6; quality_mod:=6;
            UOBJ_INGOT_SHADOW:     ar_mod:=7; dmg_mod:=7; maxhp_mod:=7; quality_mod:=7;
            UOBJ_INGOT_AZURITE:    ar_mod:=8; dmg_mod:=8; maxhp_mod:=8; quality_mod:=8;
            UOBJ_INGOT_DOOM:       ar_mod:=9; dmg_mod:=9; maxhp_mod:=9; quality_mod:=9;
            UOBJ_INGOT_BLUESTEEL:  ar_mod:=10; dmg_mod:=10; maxhp_mod:=10; quality_mod:=10;
            UOBJ_INGOT_DARKRUBY:   ar_mod:=11; dmg_mod:=11; maxhp_mod:=11; quality_mod:=11;
            UOBJ_INGOT_CRYSTALIT:  ar_mod:=12; dmg_mod:=12; maxhp_mod:=12; quality_mod:=12;
            UOBJ_INGOT_ONYX:       ar_mod:=13; dmg_mod:=13; maxhp_mod:=13; quality_mod:=13;
            UOBJ_INGOT_MIFRIL:     ar_mod:=14; dmg_mod:=14; maxhp_mod:=14; quality_mod:=14;
    endcase
    theitem.color := metal.color;
    //theitem.quality := 0.26 * quality_mod; //old
    theitem.quality := 0.20 * quality_mod;
    theitem.hp          := theitem.maxhp;
    theitem.ar_mod      := (ar_mod * 2);
    if(!theitem)
        //theitem.hit_chance := 5;
        //theitem.hit_chance_mod := 10;
    endif;


    //weapon
    //theitem.dmg_mod := dmg_mod;

    itemname["Ingot"] := "";
    tname := "an exceptional " + itemname + " "+itemInfo.desc;
    SetName(theitem, tname);

    return theitem;

endfunction
