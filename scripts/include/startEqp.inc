use cfgfile;
use os;
use util;
include ":attributes:attributes";
include "include/client";
include "include/math";

include ":blacksmithy:craft";
var smith_cfg := ReadConfigFile("blacksmithy");
var craftername := "server";
//
//  See starteqp.cfg for a description of what we expect in the config file.
//

var unique_items_created := {};
var starteqp_cfg;


const WEAPON 	:= 1;
const ARMOR 	:= 2;
const SHIELD 	:= 3;
const CLOTHING 	:= 4;
const MISC 	    := 5;
const PROF 	    := 6;


var template, ammotype, ammoamount, theammo;
var magicspawnchance := 0;
var magicadjustment  := 0;
var making_starting_equipment := 0;

var magiccfg;
const MAX_COLOR	:= 1184;

var thecritter;

//New loot system
function MakeLoot(corpse, lootgroup)
    var npcdesc := ReadConfigFile("::npcdesc");

    var elem := FindConfigElem(npcdesc, GetObjProperty(corpse, "npctemplate"));
    var mybow := elem.missileweapon;
    var mprop, madj, has_backpack;
    if(elem)
        mprop := CInt(GetConfigInt(elem, "Magicitemchance"));
        madj  := CInt(GetConfigInt(elem, "MagicAdjustment"));
    else
        mprop := 1;
        if(corpse.objtype == 0x1A340)
            case(CInt(GetObjProperty(corpse, "level")))
                1: madj := 0;
                2: madj := 2;
                3: madj := 4;
                4: madj := 6;
                5: madj := 8;
            endcase
        else
            magicadjustment := 0;
        endif
    endif
    if(AP_GetVital(corpse, VITALID_LIFE))
        has_backpack := GetEquipmentByLayer(corpse, 0x15);
        if (!has_backpack)
            var newbackpack := CreateItemAtLocation(corpse.x, corpse.y, corpse.z, 0xe75, 1, corpse.realm);
            if (!EquipItem(corpse, newbackpack))
                return 0;
            endif
        endif

        foreach thing in ListEquippedItems(corpse)
            if(thing.objtype == 0xe75)
                corpse := thing;
                break;
            endif
        endforeach
    endif
    thecritter := corpse;
    if(mprop)
        magicspawnchance := 1;
    else
        if((corpse.objtype == 0x1A340) || (corpse.objtype == 0xe40))
            magicspawnchance := 1;
        else
            magicspawnchance := 0;
        endif
    endif
    magicadjustment  := madj;
    starteqp_cfg := ReadConfigFile("::nlootgroup");

    var last_damage := GetObjProperty(corpse, "LastDamage");
    var killer := last_damage.serial;
    var killerMobile := SystemFindObjectBySerial(killer, SYSFIND_SEARCH_OFFLINE_MOBILES);

    if(lootgroup)
        CreateFromGroup(corpse, lootgroup, killerMobile);

    //var test := makeItem2(0x1632b, "HeaterShield", corpse);

    else
        var lootgroups := GetConfigStringArray(elem, "lootgroup");
        foreach str in lootgroups
            CreateFromGroup(corpse, CInt(str), killerMobile);
        endforeach
    endif

    if (elem.ammotype)
        ammotype := elem.ammotype;
        if (ammotype)
                EquipFromTemplate(corpse, mybow);
                ammoamount := elem.ammoamount;
            if (!ammoamount)
                ammoamount := 30;
            endif
            theammo := CreateItemInBackpack(corpse, ammotype, ammoamount);
        endif
    endif

endfunction

function CreateStartingEquipment(who, skillids)
    making_starting_equipment := 1;
    starteqp_cfg := ReadConfigFile("starteqp");
    foreach skillid in skillids
        var skillname := GetSkillName(CInt(skillid));
        CreateFromGroup(who.backpack, skillname);
    endforeach
    CreateFromGroup(who.backpack, "all");
endfunction

function CreateFromGroup(who, group, killer := 0)
    if(!killer)
        killer := who;
    endif

    if(making_starting_equipment != 1)
        starteqp_cfg := ReadConfigFile("::nlootgroup");
    endif
    var elem := starteqp_cfg[group];
    foreach str in GetConfigStringArray(elem, "Stack")
        CreateFromStackString(who, str, killer);
    endforeach
    foreach str in GetConfigStringArray(elem, "Item")
        CreateFromItemString(who, str, killer);
    endforeach
    foreach str in GetConfigStringArray(elem, "Random")
        CreateFromRandomString(who, str, killer);
    endforeach
    foreach str in GetConfigStringArray(elem, "Group")
        CreateFromGroup(who, str, killer);
    endforeach
    foreach str in GetConfigStringArray(elem, "Unique")
        CreateFromUniqueString(who, str, killer);
    endforeach
endfunction

function GetSkillName(skillid)
  var cfg := ReadConfigFile(":attributes:uoskills");
  return cfg[skillid].attribute;
endfunction

/////////////////////////////////////////////////////////////////////////////
//
//  An 'Item' string can have one of the following forms:
//
//  FORM                                    EXAMPLE
//  --------------------------------------------------------------------
//  "Item [ObjTypeName]"                    "Item Mortar"
//  "Item [Count] [ObjTypeName]"            "Item 4 EmptyBottle"
//  "Item [ObjTypeName] [Colornum]"         "Item Robe 0xE9"
//  "Item [Count] [ObjTypeName] [Colornum]"  "Item 2 Robe 0xE9"
//  "item [ObjTypeName] [Colornum] [chancetospawn]  "item Longsword 0 100"
//  "Item [Count] [ObjTypeName] [Colornum] [chancetospawn]  "item 1 Longsword 0 100"
//
/////////////////////////////////////////////////////////////////////////////

function CreateFromItemString(who, str, killer)
    var params := SplitWords(str);
    var count, objname, color, chance;
    var colors_cfg := ReadConfigFile("::colors");
    var premium := killer.IsPremium();
    count := RandomDiceRoll(CStr(params[1]));
    params[1] := params[2];
    params[2] := params[3];
    params[3] := params[4];
    if (!count)
        count := 1;
    endif
    objname := params[1];
    if(CInt(params[2]))
        color := CInt(params[2]);
    elseif (params[2])
        color := 0;
        var colorg := FindConfigElem(colors_cfg, params[2]);
        if(colorg)
            var str := GetConfigStringArray(colorg, "color");
	        var ar := {};
	        var rndint := RandomInt(len(str)) + 1;
	        ar := SplitWords(str[rndint]);
	        if(len(ar) > 1)
	            if(CInt(ar[2]) > 0)
	                color := RandomInt(CInt(ar[2]) - CInt(ar[1]))+1;
	            else
	                color := CInt(str[RandomInt(len(str))+1]);
                endif
            else
                color := CInt(str[RandomInt(len(str))+1]);
            endif
            if(color == 1)
                color := 0;
            endif
        endif
    endif

    if(CInt(params[3]))
        chance := CInt(params[3]);
    else
        chance := 100;
    endif

    if(chance < 100)
        case(premium)
            1: chance := add_percent(chance, 10);
            2: chance := add_percent(chance, 20);
            3: chance := add_percent(chance, 30);
        endcase
    endif
    if(CInt(RandomInt(100)+1) <= CInt(chance))
        var i, canbemagic;
        for(i:=1; i<=CInt(count); i:=i+1)
            if (CInt(objname))
                objname := CInt(objname);
            else
                Lower(objname);
            endif

            var amount := 1;
            var item := CreateItemInContainer(who, objname, amount);
            if(!item)
                syslog("Unable to create " + objname + " on " + who.name + ". Error= " + item.errortext);
            endif
            item.color := color;
            if(making_starting_equipment)
                item.newbie := 1;
                continue;
            endif
            if( (item.graphic in array( 0xdf2, 0xdf3, 0xdf4, 0xdf5, 0x13f9 )) && (magicspawnchance == 1))
                ProcessWand(item);
            else
                if(magicspawnchance == 1)
                    canbemagic := MagicAllowedandType(objname);

//print("canbemagic "+ canbemagic);
ApplyMagic(who, item, magicadjustment, canbemagic);
/*                    case (canbemagic)
                        WEAPON:
                            if(enchant < 75)
                                ApplyHPMod( item );
                            elseif(enchant < 150)
                                ApplyWeapSkillMod( item );
                            elseif(enchant < 350)
                                ApplyDmgMod( item );
                            else
                                //ApplyWeapHitscript( who, item );
                            endif
                            AddName( item );
                            break;
                        ARMOR:
                            if(enchant < 75)
                                ApplyHPMod( item );
                            elseif(enchant < 150)
                                ApplyArSkillMod( item );
                            elseif(enchant < 350)
                                ApplyARMod( item );
                            else
                                //ApplyOnHitScript( who, item );
                            endif
                            AddName( item );
                            break;
                        SHIELD:
                            if(enchant < 100)
                                ApplyHPMod( item );
                            elseif(enchant < 200)
                                ApplyArSkillMod( item );
                            else
                                ApplyARMod( item );
                            endif
                            AddName( item );
                            break;
                        CLOTHING:
                            if(enchant < 250)
                                ApplyMiscSkillMod( item );
                            else
                                ApplyMiscArMod( item );
                            endif
                            AddName( item );
                            AddRandomColor( item );
                            break;
                        MISC:
                            if(enchant < 250)
                                ApplyMiscSkillMod( item );
                            elseif(enchant < 300 )
                                ApplyEnchant( who, item );
                            else
                                ApplyMiscArMod( item );
                            endif
                            AddName( item );
                            break;
                        PROF:
                            ApplyMiscSkillMod( item );
                            AddName( item );
                            break;
                        default:
                    endcase*/
                endif
            endif
        endfor
    endif
endfunction






function ApplyMiscSkillMod( item )

	var chance := RandomInt(1000)+1;
	if (chance <= 5)
		ApplyStatMod( item );
		return;
	endif


	var skillid;
	var amount;

	var numb := (RandomInt(50)+1) * magicadjustment * 2;

	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( numb < 200 )
				numb := 200;
			endif
			break;

		2:	if( numb < 300 )
				numb := 300;
			endif
			break;
	endcase

   	if( numb < 200 )
		amount := 1;
   	elseif( numb < 300 )
		amount := 2;
   	elseif( numb < 400 )
		amount := 3;
   	elseif( numb < 500 )
		amount := 4;
   	elseif( numb < 600 )
		amount := 5;
	else
		amount := 6;
   	endif

	var skills := GetObjProperty( item, "AvailSkills" );
	if ( skills )
		skillid := skills[RandomDiceRoll("1d" + len(skills))];
	else
		skillid := RandomInt(SKILLID__HIGHEST + 1);
	endif

	SetObjProperty( item, "skilladv", skillid );
	SetObjProperty( item, "skilladvamount", amount );

endfunction


function ApplyWeapSkillMod( item )

	var chance := RandomInt(1000)+1;
	if( chance <= 5 )
		ApplyStatMod( item );
		return;
	endif

	var degree := (RandomInt(50)+1) * magicadjustment * 2;
	var amount;

	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( degree < 100 )
				degree := 100;
			endif
			break;

		2:	if( degree < 200 )
				degree := 200;
			endif
			break;
	endcase

   	if( degree < 100 )
		amount := 1;
   	elseif( degree < 200 )
		amount := 2;
   	elseif( degree < 350 )
		amount := 3;
   	elseif( degree < 450 )
		amount := 4;
   	elseif( degree < 550 )
		amount := 5;
	else
		amount := 6;
   	endif

	var skillid;
	case(RandomInt(2)+1)
		1:	skillid := item.skillid;
			break;

		default: skillid := SKILLID_TACTICS;
	endcase


	SetObjProperty( item, "skilladv", skillid );
	SetObjProperty( item, "skilladvamount", amount );

	//Check for another enchantment
	if ((RandomInt(100)+1) <= 5 * magicadjustment)
		ApplyHPMod( item );
	endif

endfunction

function ApplyMiscArMod( item )

	var numb := (RandomInt(50)+1) * magicadjustment * 2;
	var amount;

	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( numb < 200 )
				numb := 200;
			endif
			break;

		2:	if( numb < 350 )
				numb := 350;
			endif
			break;
	endcase

   	if( numb < 200 )
       		amount := 1;
   	elseif( numb < 350 )
       		amount := 2;
   	elseif( numb < 450 )
       		amount := 3;
   	elseif( numb < 550 )
       		amount := 4;
   	elseif( numb < 600 )
       		amount := 5;
	else
		amount := 6;
   	endif

   	SetObjProperty( item, "ArBonus", amount );

   	//Check for another enchantment
   	if( (RandomInt(100)+1) <= (8 * magicadjustment) )
		ApplyMiscSkillMod( item );
   	endif

endfunction


function ProcessWand(item)
    var amt;
    case(RandomInt(10) + 1)
        1: amt :=  15;   SetObjProperty(item, "SpellId",  1);   SetObjProperty(item, "damagemod", "of clumsiness");
        2: amt :=  15;   SetObjProperty(item, "SpellId",  3);   SetObjProperty(item, "damagemod", "of feeblemindedness");
        3: amt :=  15;   SetObjProperty(item, "SpellId",  4);   SetObjProperty(item, "damagemod", "of healing");
        4: amt :=  15;   SetObjProperty(item, "SpellId",  8);   SetObjProperty(item, "damagemod", "of weakness");
        5: amt :=  10;   SetObjProperty(item, "SpellId", 12);   SetObjProperty(item, "damagemod", "of harming");
        6: amt :=   5;   SetObjProperty(item, "SpellId", 18);   SetObjProperty(item, "damagemod", "of fireballs");
        7: amt :=   5;   SetObjProperty(item, "SpellId", 29);   SetObjProperty(item, "damagemod", "of greater healing");
        8: amt :=   3;   SetObjProperty(item, "SpellId", 30);   SetObjProperty(item, "damagemod", "of lightning");
        9: amt :=  10;   SetObjProperty(item, "SpellId", 31);   SetObjProperty(item, "damagemod", "of mana draining");
       10: amt := 100;   SetObjProperty(item, "SpellId", 65);   SetObjProperty(item, "damagemod", "of identification");
    endcase
    SetObjProperty(item, "charges", (amt + RandomInt(10)));
    item.usescript := ":wands:magicWands";
    var dsc;
    if(item.objtype == 0x13f9)
        dsc := "gnarled staff";
    else
        dsc := "wand";
    endif
    SetName(item, "a magic " + dsc);
endfunction

function ClothTable(item)
    var val := RandomInt(19) + 1;
    var type := RandomInt(10) + 1;
    if(type >= 1)
        var charges := RandomDiceRoll("1d30") + 10;
        SkillMod(item);
        SetObjProperty(item, "type", "skill");
    else


        if(val >= 14)
            var charges := RandomDiceRoll("1d30") + 10;
            SetObjProperty(item, "charges", charges);
            SetObjProperty(item, "type", "sight");
            SetObjProperty(item, "damagemod", " of nightsight");
        elseif(val >= 11)
            var charges := RandomDiceRoll("1d30") + 10;
            SetObjProperty(item, "charges", charges);
            SetObjProperty(item, "type", "protection");
            SetObjProperty(item, "damagemod", " of protection");
        elseif(val <= 8)
            var charges := RandomDiceRoll("1d30") + 10;
            SetObjProperty(item, "charges", charges);
            SetObjProperty(item, "type", "strength");
            SetObjProperty(item, "damagemod", " of strength");
        elseif(val > 5)
            var charges := RandomDiceRoll("1d30") + 10;
            SetObjProperty(item, "charges", charges);
            SetObjProperty(item, "type", "invisibility");
            SetObjProperty(item, "damagemod", " of invisibility");
        elseif(val == 3)
            var charges := RandomDiceRoll("1d30") + 10;
            SetObjProperty(item, "charges", charges);
            SetObjProperty(item, "type", "blessing");
            SetObjProperty(item, "damagemod", " of blessing");
        else
            var charges := RandomDiceRoll("1d30") + 10;
            SetObjProperty(item, "charges", charges);
            SetObjProperty(item, "type", "reflect");
            SetObjProperty(item, "damagemod", " of magic reflection");
        endif
    endif
    AddName(item);
    AddRandomColor( item );
endfunction


function JewelryTable(item)
    var val := RandomInt(19) + 1;
    var type := RandomInt(10) + 1;
    //panic scoro...
    ApplyStatMod(item);
    AddName(item);

endfunction

function ArmorTable(item)
    var newarray := {1, 2};
    var count := RandomInt(99) + 1;
    var counter;
    if(count >= 40)
        counter := 1;
    else
        counter := 2;
    endif
    count := 0;
    var num;
    while(count <= counter)
        num := RandomInt(99) + 1;
        if(num >=  50)
            num := 1;
        else
            num := 2;
        endif
        case(num)
            1: ApplyHPMod(item);
            2: ApplyARMod(item);
        endcase
        newarray.erase(num);
        count := count + 1;
    endwhile
    if((RandomInt(10)+1) <= 1)
        ExtraArmorTable(item);
    endif
    AddName(item);
endfunction

function ExtraArmorTable(item)
  var val := RandomInt(4) + 1;
  case(val)
    1: var charges := RandomDiceRoll("1d30") + 10;
       SetObjProperty(item, "charges", charges);
       SetObjProperty(item, "type", "protection");
       var dmgmod := GetObjProperty(item, "damagemod");
       if(dmgmod)
         dmgmod := dmgmod + " and protection";
         SetObjProperty(item, "damagemod", dmgmod);
       else
         dmgmod := " of protection";
         SetObjProperty(item, "damagemod", dmgmod);
       endif
    2: var charges := RandomDiceRoll("1d30") + 10;
       SetObjProperty(item, "charges", charges);
       SetObjProperty(item, "type", "strength");
       var dmgmod := GetObjProperty(item, "damagemod");
       if(dmgmod)
         dmgmod := dmgmod + " and strength";
         SetObjProperty(item, "damagemod", dmgmod);
       else
         dmgmod := " of strength";
         SetObjProperty(item, "damagemod", dmgmod);
       endif
    3:  var charges := RandomDiceRoll("1d30") + 10;
       SetObjProperty(item, "charges", charges);
       SetObjProperty(item, "type", "blessing");
       var dmgmod := GetObjProperty(item, "damagemod");
       if(dmgmod)
         dmgmod := dmgmod + " and blessing";
         SetObjProperty(item, "damagemod", dmgmod);
       else
         dmgmod := " of blessing";
         SetObjProperty(item, "damagemod", dmgmod);
       endif
    4: var charges := RandomDiceRoll("1d30") + 10;
       SetObjProperty(item, "charges", charges);
       SetObjProperty(item, "type", "reflect");
       var dmgmod := GetObjProperty(item, "damagemod");
       if(dmgmod)
         dmgmod := dmgmod + " and magic reflection";
         SetObjProperty(item, "damagemod", dmgmod);
       else
         dmgmod := " of magic reflection";
         SetObjProperty(item, "damagemod", dmgmod);
       endif
  endcase
endfunction

function WeaponTable(item)
  var newarray := array(1, 2, 3);
  var count := RandomInt(100) + 1;
  var counter;
  if(count >= 60)
    counter := 1;
  elseif(count >= 40)
    counter := 2;
  elseif(count >= 10)
    counter := 3;
  else
    counter := 4;
  endif
  if(counter == 1)
    DestroyItem(item);
    return;
  endif
  count := 0;
  var num, val;
  while(count <= counter)
    num := RandomInt(len(newarray)) + 1;
    val := newarray[num];
    case(val)
      1: ApplyAccyMod(item);
      2: ApplyHPMod(item);
      3: ApplyDmgMod(item);
    endcase
    newarray.erase(num);
    count := count + 1;
  endwhile
  AddName(item);
endfunction

/////////////////////////////////////////////////////////////////////////////
//
//  A 'Random' string follows the following form:
//
//	FORM											sEXAMPLE
//	--------------------------------------------------------------------
//  "Random [Count] [GroupName]"					"Random 4 Reagents"
//  "Random [Count] [GroupName] [chancetospawn]"	"Random 4 Reagents 50"
//
//  'Count' entries from 'GroupName' are chosen randomly.  The same entry
//  will not be chosen twice.
//
/////////////////////////////////////////////////////////////////////////////
function CreateFromRandomString(who, desc, killer)
var premium := killer.IsPremium();
  var params := SplitWords(desc);
  var amt := Cint(params[1]);
  var count := 1;
  if(amt != 1)
    count := RandomDiceRoll(Cstr(amt + 1));
  endif



  var group := params[2];
  var chance := params[4];
  var elem := FindConfigElem(starteqp_cfg, group);
  if(elem == error)
    syslog("Unable to find group " + group);
    return;
  endif
  var items := GetConfigStringArray(elem, "Item");
  if(items == error)
    syslog("Group " + group + " does not contain items!");
    return;
  endif
  if(!chance)
    chance := 100;
  endif

case(premium)
1: chance := add_percent(chance, 10);
2: chance := add_percent(chance, 20);
3: chance := add_percent(chance, 30);
endcase
  var rand := RandomInt(99)+1;
  if(CInt(rand) <= CInt(chance))
    if(count > len(items))
      count := len(items);
    endif


    while(count)
      var n := RandomInt(len(items)) + 1;

      if(items[n])
        CreateFromItemString(who, items[n], killer);
        if(making_starting_equipment)
          items[n] := "";
        endif
        count := count - 1;
      endif
    endwhile
  endif
endfunction

/////////////////////////////////////////////////////////////////////////////
//
//  A 'Unique' string has the following form:
//
//  FORM                                    EXAMPLE
//  --------------------------------------------------------------------
//  "Unique [ObjTypeName]"                  "Unique Spellbook"
//
//  Only one of a unique item will be created as starting equipment.
//
/////////////////////////////////////////////////////////////////////////////

function CreateFromUniqueString(who, str, killer)
    var params := SplitWords(str);
    var objtype;
    if (CInt(params[1]))
        objtype := params[1];
    else
        objtype := GetObjtypeByName(params[1]);
    endif
    if(!objtype)
        syslog("Don't know what a " + str + " is");
        return;
    endif
    if(objtype in unique_items_created)
        return;
    endif
    unique_items_created[len(unique_items_created) + 1] := objtype;
    var newstr := "1 " + str;
    CreateFromItemString(who, newstr, killer);
endfunction

/////////////////////////////////////////////////////////////////////////////
//
//  A 'Stack' string is just like "Item" but stacks the item instead of making
//  count number of individual items.
//
/////////////////////////////////////////////////////////////////////////////

function CreateFromStackString(who, str, killer)
    var params := SplitWords(str);
    var count;
    var objname;
    var color;
    var chance;
    count := RandomDiceRoll(CStr(params[1]));
    params[1] := params[2];
    params[2] := params[3];
    params[3] := params[4];
    if (!count)
        count := 1;
    endif
    objname := params[1];
    objname := Lower(objname);
    if(CInt(params[2]))
        color := CInt(params[2]);
    else
        color := 0;
    endif
    var premium := killer.IsPremium();
    if (CInt(params[3]))
        chance := CInt(params[3]);

        case(premium)
            1: chance := add_percent(chance, 10);
            2: chance := add_percent(chance, 20);
            3: chance := add_percent(chance, 30);
        endcase
    else
        chance := 100;
    endif
    var rand := RandomInt(100)+1;
    case(premium)
        1: count := add_percent(count, 10);
        2: count := add_percent(count, 20);
        3: count := add_percent(count, 30);
    endcase
    if(count > 60000)
        count := 60000;
    endif
    if(CInt(rand) <= CInt(chance))
        if (CInt(objname))
            objname := CInt(objname);
        else
            Lower(objname);
        endif

        var item := CreateItemInContainer(who, objname, count);
        if(!item)
            syslog("Unable to create " + objname + " on " + who.name + ". Error= " + item.errortext);
        endif
        item.color := color;
        if(making_starting_equipment)
            item.newbie := 1;
        endif
    endif
endfunction

////
//Checks to see if that item is allowed to be magic and returns the type
////
function MagicAllowedandType( objname )

	if( !magiccfg )
		magiccfg := ReadConfigFile( ":combat:enchantableitems" );
	endif

   	var elem1 := FindConfigElem( magiccfg, WEAPON );
   	var data1 := GetConfigInt( elem1, objname );
   	if( data1 )
        return WEAPON;
   	endif

   	var elem2 := FindConfigElem( magiccfg, ARMOR );
   	var data2 := GetConfigInt( elem2, objname );
   	if( data2 )
        return ARMOR;
   	endif

   	var elem3 := FindConfigElem( magiccfg, SHIELD );
   	var data3 := GetConfigInt( elem3, objname );
   	if( data3 )
        return SHIELD;
   	endif

	var elem4 := FindConfigElem( magiccfg, CLOTHING );
	var data4 := GetConfigInt( elem4, objname );
	if( data4 )
		return CLOTHING;
	endif

	var elem5 := FindConfigElem( magiccfg, MISC );
	var data5 := GetConfigInt( elem5, objname );
	if( data5 )
		return MISC;
	endif

	var elem6 := FindConfigElem( magiccfg, PROF );
	var data6 := GetConfigInt( elem6, objname );
	if( data6 )
		return PROF;
	endif

   	return 0;

endfunction



function ApplyArSkillMod( item )

	var chance := RandomInt(1000)+1;
	if (chance <= 5)
		ApplyStatMod( item );
		return;
	endif

	var skillid;
	var amount;
	var numb := (RandomInt(50)+1) * magicadjustment * 2;

	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( numb < 200 )
				numb := 200;
			endif
			break;

		2:	if( numb < 300 )
				numb := 300;
			endif
			break;
	endcase

   	if( numb < 200 )
		amount := 1;
   	elseif( numb < 300 )
		amount := 2;
   	elseif( numb < 400 )
		amount := 3;
   	elseif( numb < 500 )
		amount := 4;
   	elseif( numb < 600 )
		amount := 5;
	else
		amount := 6;
   	endif

	case( RandomInt(2)+1 )
		1:	skillid := SKILLID_RESISTING_SPELLS;
			break;

		default: skillid := SKILLID_HIDING;
	endcase


	SetObjProperty( item, "skilladv", skillid );
	SetObjProperty( item, "skilladvamount", amount );

	//Check for another enchantment
	if ((RandomInt(100)+1) <= 5 * magicadjustment)
		ApplyHPMod( item );
	endif

endfunction


////
//Picks and assigns a damagemod and a suffix for the item
////
function ApplyDmgMod( item )

   	var numb := (RandomInt(50)+1) * magicadjustment * 2;
	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( numb < 150 )
				numb := 150;
			endif
			break;

		2:	if( numb < 300 )
				numb := 300;
			endif
			break;
	endcase

   	if( numb < 150 )
        item.dmg_mod := 5;
   	elseif( numb < 300 )
        item.dmg_mod := 10;
   	elseif( numb < 400 )
        item.dmg_mod := 15;
   	elseif( numb < 500 )
        item.dmg_mod := 20;
   	elseif( numb < 600 )
        item.dmg_mod := 25;
	else
		item.dmg_mod := 30;
   	endif


   	if( (RandomInt(100)+1) <= (10 * magicadjustment) )
        if ((RandomInt(100)+1) <= 75)
            ApplyHPMod( item );
		else
			ApplyWeapSkillMod( item );
		endif
   	endif

endfunction


//no used
function ApplyWeapHitscript( who, item )

	CountAvailHitscripts( );
	var scripttype := (Random(94)+1) + (magicadjustment * 2);
	if (scripttype <= 40)
		ApplySpellHitscript( item );
	elseif (scripttype <= 80)
		ApplySlayerHitscript( item );
	elseif (scripttype <= 100)
		ApplyEffectHitscript( item );
	elseif (scripttype <= 105)
		DestroyItem( item );
		CreateFromRandomString( who, "GMWeapon" );
	else
		ApplyGreaterHitscript( item );
	endif

endfunction


function ApplyAccyMod(item)
  var numb := ((RandomInt(100)+1) - magicadjustment);
  if(numb < 1)
    numb := 1;
  endif
  var modifier := 0;
  if(numb >= 50)
    modifier := 5;
    SetObjProperty(item, "accuracy", "accurate");
  elseif(numb >= 25)
    modifier := 10;
    SetObjProperty(item, "accuracy", "surpassingly accurate");
  elseif(numb >= 15)
    modifier := 15;
    SetObjProperty(item, "accuracy", "eminently accurate");
  elseif(numb >= 5)
    modifier := 20;
    SetObjProperty(item, "accuracy", "exceedingly accurate");
  elseif(numb >= 1)
    modifier := 25;
    SetObjProperty(item, "accuracy", "supremely accurate");
  endif
  SetObjProperty(item, "type", "accuracy");
  SetObjProperty(item, "gain", modifier);
endfunction

function ApplyHPMod( item )

   	var numb := (RandomInt(50)+1) * magicadjustment * 2;
   	var hp;

   	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( numb < 75 )
				numb := 75;
			endif
			break;

		2:	if( numb < 150 )
				numb := 150;
			endif
			break;
	endcase

   	if( numb < 75 )
		hp := 10;
   	elseif( numb < 150 )
		hp := 20;
   	elseif( numb < 300 )
		hp := 30;
   	elseif( numb < 400 )
		hp := 40;
   	elseif( numb < 550 )
		hp := 50;
	else
		hp := 60;
   	endif

   	item.maxhp_mod := hp;
   	item.hp := item.maxhp;

endfunction

function ApplyARMod( item )

   	var numb := (RandomInt(50)+1) * magicadjustment * 2;

	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( numb < 150 )
				numb := 150;
			endif
			break;

		2:	if( numb < 300 )
				numb := 300;
			endif
			break;
	endcase

   	if( numb < 150 )
       		item.ar_mod := 5;
   	elseif( numb < 300 )
       		item.ar_mod := 10;
   	elseif( numb < 400 )
       		item.ar_mod := 15;
   	elseif( numb < 500 )
       		item.ar_mod := 20;
   	elseif( numb < 600 )
       		item.ar_mod := 25;
	else
		item.ar_mod := 30;
   	endif


	//Check for another enchantment
   	if( (RandomInt(100)+1) <= (10 * magicadjustment) )
       		if ((RandomInt(100)+1) <= 75)
       			ApplyHPMod( item );
		else
			ApplyArSkillMod( item );
		endif
   	endif


endfunction

function ApplyStatMod( item )

	var numb := (RandomInt(100)+1) * magicadjustment;
	var amount;
   	if( numb < 200 )
		amount := 5;
   	elseif( numb < 300 )
		amount := 10;
   	elseif( numb < 400 )
		amount := 15;
   	elseif( numb < 500 )
		amount := 200;
   	elseif( numb < 600 )
		amount := 25;
	else
		amount := 30;
   	endif
    if(amount > 30)
        amount := 30;
    endif
	var id := RandomInt(3)+1;
	if( id == 1 )
		SetObjProperty( item, "str", amount );
	elseif( id == 2 )
		SetObjProperty( item, "int", amount );
	else
		SetObjProperty( item, "dex", amount );
	endif

	//Check for another enchantment
	if ((RandomInt(100)+1) <= 2 * magicadjustment)
		ApplyHPMod( item );
	endif

endfunction

function SilverMod(item)
  var numb := (RandomInt(10) + 1);
  if(numb == 1)
    var hits := GetObjProperty(item, "OnHit");
    if(!hits)
      hits := {};
    endif
    hits.append("silverHit");
    SetObjProperty(item, "OnHit", hits);
    SetObjProperty(item, "silvermod", 1);
  endif
endfunction

function ApplyEnchant( who, item )

	var numb := (RandomInt(100)+1) * magicadjustment;
	if( numb < 300 )
		ApplyProtection( item );
	elseif( numb < 500 )
		ApplyElementalImmunity( item );
	elseif( numb < 550 )
		DestroyItem( item );
		//CreateFromRandomString( who, "MiscMagicItem" );
		return;
	else
		ApplyImmunity( item );
	endif

	//Check for another enchantment
	if ((RandomInt(100)+1) <= (5 * magicadjustment))
		var secchance := RandomInt(100)+1;
		if (secchance < 75)
            ApplyMiscSkillMod( item );
		else
			ApplyMiscArMod( item );
		endif
	endif

endfunction



function ApplyImmunity( item )

	var n;
	var itype;
	var power := (RandomInt(100)+1) * (magicadjustment - 3);

	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( power < 170 )
				power := 170;
			endif
			break;

		2:	if( power < 200 )
				power := 200;
			endif
			break;
	endcase

	if (power < 170)
		n := 1;
	elseif (power < 200)
		n := 2;
	elseif (power < 230)
		n := 3;
	elseif (power < 270)
		n := 4;
	elseif (power < 300)
		n := 5;
	else
		n := 6;
	endif

	case(RandomInt(3)+1)
		1:	itype := "PermPoisonImmunity";
			break;

		2:	itype := "PermMagicImmunity";
			break;

		default: itype := "Permmr";
	endcase

	SetObjProperty( item, itype, n );

endfunction

function ApplyProtection( item )

	var numb := (RandomInt(50)+1) * magicadjustment * 2;
	var ptype;
	var charges;

	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( numb < 75 )
				numb := 75;
			endif
			break;

		2:	if( numb < 150 )
				numb := 150;
			endif
			break;
	endcase

	if( numb < 75 )
		charges := 5;
   	elseif( numb < 150 )
		charges := 10;
   	elseif( numb < 300 )
		charges := 15;
   	elseif( numb < 400 )
		charges := 20;
   	elseif( numb < 550 )
		charges := 25;
	else
		charges := 30;
   	endif


	case( RandomDiceRoll( "1d3" ) )
		1:	ptype := "PoisonImmunity";
			break;

		2:	ptype := "MagicImmunity";
			break;

		3:	ptype := "SpellReflection";
			break;

		default: ptype := "SpellReflection";
	endcase

	SetObjProperty( item, ptype, charges );

endfunction

function ApplyElementalImmunity( item )

	var element;
	var power;
	var numb := (RandomInt(100)+1) * magicadjustment;

	case( CInt(magicadjustment / 3) )
		0:	break;
		1:	if( numb < 150 )
				numb := 150;
			endif
			break;

		2:	if( numb < 300 )
				numb := 300;
			endif
			break;
	endcase

	if( numb < 150 )
       		power := 1;
   	elseif( numb < 300 )
       		power := 2;
   	elseif( numb < 450 )
       		power := 3;
   	elseif( numb < 550 )
       		power := 4;
   	elseif( numb < 600 )
       		power := 5;
	else
		power := 6;
   	endif

	case(RandomInt(8)+1)
		1:	element := "FireProtection";
			break;

		2:	element := "WaterProtection";
			break;

		3:	element := "AirProtection";
			break;

		4:	element := "EarthProtection";
			break;

		5:	element := "FreeAction";
			power := 1;
			break;

		6:	element := "NecroProtection";
			break;

//		7:	element := "HolyProtection";
//			break;

		7:	element := "HealingIncrease";
			break;

		default: element := "PhysicalProtection";
	endcase

	SetObjProperty( item, element, power );

endfunction

function AddName(item)
    var currdesc := item.desc;
    if(currdesc[1] == "a")
        var space := find(item.desc, " ", 1);
        var newname := currdesc[space+1,len(item.desc)];
        SetName(item,"a magic " + newname);
    else
        SetName(item, "magic " + currdesc);
    endif
    SetObjProperty( item, "UnIDedMagicalItem", 1 );

    if( (item.objtype < 0x1eff || item.objtype > 0x1f04) && (RandomInt(100)+1) <= 5 )
        //SetObjProperty(item, "Cursed", 1 );
        SetObjProperty(item, "type", "cursed" );
    endif
endfunction


function AddRandomColor( item )

	SetObjProperty( item, "Undyeable", 1 );
	if( !GetObjProperty( item, "EnchantColor" ) )
		var color := RandomDiceRoll( "1d"+MAX_COLOR );
		while( color > 999 && color < 1152 )
			color := RandomDiceRoll( "1d"+MAX_COLOR );
		endwhile

		SetObjProperty( item, "EnchantColor", color );
	endif

endfunction


function ApplyMagic(who, item, magicadjustment, canbemagic)
    //print("magicadjustment "+magicadjustment);
    var enchant := (RandomInt(75)+26) * CInt(magicadjustment/2 + 1);
    if (magicadjustment == 5)
        enchant := enchant + 51;
    endif
    //print("enchant "+enchant);

    case (canbemagic)
        WEAPON:
            if(enchant < 75)
                ApplyHPMod( item );
            elseif(enchant < 150)
                ApplyWeapSkillMod( item );
            elseif(enchant < 350)
                ApplyDmgMod( item );
            else
                //ApplyWeapHitscript( who, item );
            endif
            AddName( item );
            break;

        ARMOR:
            if(enchant < 75)
                ApplyHPMod( item );
            elseif(enchant < 150)
                ApplyArSkillMod( item );
            elseif(enchant < 350)
                ApplyARMod( item );
            else
                //ApplyOnHitScript( who, item );
            endif
            AddName( item );
            break;

        SHIELD:
            if(enchant < 100)
                ApplyHPMod( item );
            elseif(enchant < 200)
                ApplyArSkillMod( item );
            else
                ApplyARMod( item );
            endif
            AddName( item );
            break;

        CLOTHING:
            if(enchant < 250)
                ApplyMiscSkillMod( item );
            else
                ApplyMiscArMod( item );
            endif
            AddName( item );
            AddRandomColor( item );
            break;

        MISC:
            if(enchant < 250)
                ApplyMiscSkillMod( item );
            elseif(enchant < 300 )
                ApplyEnchant( who, item );
            else
                ApplyMiscArMod( item );
            endif
            AddName( item );
            break;

        PROF:
            ApplyMiscSkillMod( item );
            AddName( item );
            break;
        default:
    endcase
endfunction
