use uo;
use os;
use cfgfile;

include ":attributes:attributes";
include ":gumps:confirmationSizable";
include "include/autoloop";
include ":alchemy:potionKegs";

var cfg := ReadConfigFile("alchemy");
var itemdesccfg := ReadConfigFile(":alchemy:itemdesc");
if(!cfg)
    syslog("Failed to open alchemy.cfg: " + cfg.errortext);
endif
if(!itemdesccfg)
    syslog("Failed to open :alchemy:itemdesc.cfg: " + itemdesccfg.errortext);
endif

function is_reagent(what)
    case(what.objtype)
        0xf7a: return 1;
        0xf7b: return 1;
        0xf84: return 1;
        0xf85: return 1;
        0xf86: return 1;
        0xf88: return 1;
        0xf8c: return 1;
        0xf8d: return 1;
        0xf82: return 1; //blood dragon
        0xF83: return 1; //Executioner's Caps
        0xF79: return 1; //Blackmoor
        0xF30: return 1; //diamond
        0xF0F: return 1; //star sapphire
        0xF7D: return 1; //
        0xF8F: return 1; //volcanic ash
        0xF8E: return 1; //serpent scale
        0xF78: return 1; //bat wing
        0xF90: return 1; //dead wood
        0xF7C: return 1; //blood spawn
    endcase
    return 0;
endfunction

function PourPotion(user,mortar)
    try_to_make_potion(user, "spacefill", 1, mortar, 0,0);
endfunction

function make_the_potion(user, mortar)
    var reg_on := Target( user );
    if(!reg_on)
        SendSysMessage(user, "Targetting Cancelled");
        return;
    endif
    if(!can_access(user, reg_on))
        return;
    endif
    if(is_reagent(reg_on))
        if(!cfg)
            SendSysMessage(user, "Alchemy config error. Please inform the staff.");
            return;
        endif

        var elem := cfg[reg_on.objtype];
        if(!elem)
            SendSysMessage(user, "Alchemy element error. Please inform the staff. "+elem.errortext);
            return;
        endif

        sleepms(300);
        SendSysMessage(user, "Select a empty bottle or keg.");
        var bottlekeg := Target( user );
        if(!bottlekeg)
            SendSysMessage(user, "Targetting Cancelled");
            return;
        endif

        if(bottlekeg.objtype == UOBJECT_EMPTY_BOTTLE || bottlekeg.objtype == 0x1940)

            var possible := array;
            var tmenu := CreateMenu("Select a potion to make");
            var num_possibilities := 0;
            possible := GetConfigStringArray( elem, "potion");

            foreach thingie in possible
                if(can_make(user, thingie))
                    var titem := cfg[thingie].objtype;

                    if(titem)
                        if(check_regs(user, thingie))
                            var elem := FindConfigElem( itemdesccfg, titem );
                            var color := CInt(GetConfigString(elem,"Color"));
                            var name := GetConfigString(elem,"Desc");
                            if(name.errortext)
                                thingie["_"] := " ";
                                name := thingie;
                            endif
                            if(color.errortext)
                                color:=0;
                            endif

                            var menuItem := AddMenuItem( tmenu, titem, name, color );
                            if(menuItem.errortext)
                                SendSysMessage(user, "Error::AddMenuItem -> "+menuItem.errortext);
                            endif
                            num_possibilities := num_possibilities + 1;
                        endif
                    endif
                endif
            endforeach
            if(num_possibilities == 0)
                SendSysMessage(user, "You can't make any potions!");
                return;
            endif

            var citem := SelectMenuItem2(user, tmenu);
            if(citem.errortext)
                SendSysMessage(user, "Error::SelectMenuItem2 -> "+citem.errortext);
            endif

            if(!citem.objtype)
                SendSysMessage(user, "Targetting canceled");
                return -1;
            endif

            var is_autoloop := SmallYesNo(user,"Autoloop?");


            if (bottlekeg.objtype == 0x1940)
                if(!GetObjProperty(bottlekeg, "PotionType"))
                    SetObjProperty(bottlekeg, "PotionType", cfg[possible[citem.index]].objtype);
                endif
                if(!GetObjProperty(bottlekeg, "OwnerSkill"))
                    SetObjProperty(bottlekeg, "OwnerSkill", 0);
                endif
                if(!GetObjProperty(bottlekeg, "OwnerLevel"))
                    SetObjProperty(bottlekeg, "OwnerLevel", 0);
                endif
            endif


            if(is_autoloop)
                AutoLoop_init(user, 100, 1);
                while(AutoLoop_more()>0)
                    try_to_make_potion(user, possible[citem.index], 0, mortar, reg_on, bottlekeg);
                endwhile
                AutoLoop_finish();
            else
                try_to_make_potion(user, possible[citem.index], 0, mortar, reg_on, bottlekeg);
            endif

        elseif(reg_on == mortar)
            var last := GetObjProperty(mortar, "LastMade");
            if(last)
                try_to_make_last(user, last, mortar);
            else
                SendSysMessage(user, "You cannot grind that.");
                return;
            endif
        else
            SendSysMessage(user, "That's not a reagent!");
        endif
    endif
endfunction

function can_make(user, potion)
 // reg := reg; // stops ecompile "not used" warning
  var skillneeded:= CInt(cfg[potion].skill);
  if(skillneeded > 20)
    skillneeded := skillneeded - 20;
  else
    skillneeded := 0;
  endif
  if(AP_GetSkill(user, ALCHEMY) >= skillneeded)
    return 1;
  else
    return 0;
  endif
endfunction


function check_regs(user, potion)
    set_critical(1);
    potion[" "] := "_";

    var elem := FindConfigElem( cfg, potion );
    var ingredients := GetConfigStringArray(elem,"ingredient");

    var ings := array;
    foreach ingredient in ingredients
        var split := SplitWords( ingredient );
        var findItem := FindObjtypeInContainer( user.backpack, split[1]);
        if(!findItem)
            if(GetAmount(findItem) <= CInt(split[2]))
                set_critical(0);
                return 0;
            endif
        else
            ings.append({findItem,split[2]});
        endif
    endforeach


    if(ings)
        set_critical(0);
        return ings;
    endif

endfunction


function take_regs(user, potion)
    var ings := check_regs(user, potion);
    if(ings)
        foreach ingredient in ings
            SubtractAmount( ingredient[1], CInt(ingredient[2]));
        endforeach
        set_critical(0);
        return 1;
    endif

endfunction


function try_to_make_potion(user, potion, in_mortar, mortar, reg, bottle)
    if(!take_regs(user, potion))
        SendSysMessage(user, "You do not have enough reagents to make that.");
        return;
    endif
    var amt := CInt(GetObjProperty(bottle, "PotionAmount"));
    if (bottle.objtype == 0x1940)
        if(amt >= 100)
            SendSysMessage(user, "Keg is full.");
            return;
        endif
    endif
    if(!in_mortar)

        potion[" "] := "_";
        var difficulty := cfg[potion].skill;
        var points := cfg[potion].points;
        var product := cfg[potion].objtype;
        SetObjProperty(mortar, "LastMade", potion);
        var loop := 0;
        while(loop < 3)
            PerformAction( user, 0x0021 );
            PlaySoundEffect(user, SOUND_EFFECT_GRIND);
            PrintTextAbovePrivate(user, "You grind the reagent",user);
            sleep(2);
            loop := loop + 1;
        endwhile

        if(SkillCheck(user, ALCHEMY, difficulty, points))
            if(bottle.objtype == UOBJECT_EMPTY_BOTTLE)
                var thing := CreateItemInBackpack(user, product, 1);
                if ( !thing )
                    SendSysMessage(user, "Failed to create potion! Please report this to the staff (error logged)");
                    syslog( "Alchemy potion creation (1) failed: " + thing.errortext );
                    return;
                endif
                PlaySoundEffect(user, SOUND_EFFECT_POUR);
                SendSysMessage(user, "You create the potion and put it in your backpack");
                SubtractAmount(bottle,1);
                return;
            elseif (bottle.objtype == 0x1940)
                var thing := CreateItemInContainer(bottle, product, 1);
                if ( !thing )
                    SendSysMessage(user, "Failed to create potion! Please report this to the staff (error logged)");
                    syslog( "Alchemy potion creation (1) failed: " + thing.errortext );
                    return;
                endif
                var type := GetObjProperty(bottle, "PotionType");

                amt+=1;
                SetObjProperty(bottle, "PotionAmount", amt);
                PlaySoundEffect(user, SOUND_EFFECT_POUR);
                SendSysMessage(user, "You create the potion and put it in your keg");
rename_keg(bottle, product);
                return;

            endif;
        else
            PlaySoundEffect(user, SOUND_EFFECT_POUR);
            PrintTextAbovePrivate(user, "You pour out the useless mixture", user);
            return;
        endif
    else
        var hasbottle:= 0;
        var haskeg:= 0;
        set_critical(1);
        foreach item in EnumerateItemsInContainer(user.backpack)
            if(item.objtype == UOBJECT_EMPTY_BOTTLE)
                SubtractAmount(item, 1);
                hasbottle := 1;
                break;
            elseif(item.objtype == 0x1940)
                haskeg := 1;
                break;
            endif
        endforeach

        set_critical(0);
        if(hasbottle || haskeg)
            var product := GetObjProperty(mortar, "contents");
            var thing := CreateItemInBackpack(user, product, 1);
            if ( !thing )
                SendSysMessage(user, "Failed to create item! Please report this to the staff (error logged)");
                syslog( "Alchemy item creation failed: " + thing.errortext );
                return;
            endif
            PlaySoundEffect(user, SOUND_EFFECT_POUR);
            PrintTextAbovePrivate(user, "You create the item and put it in your backpack", user);
            EraseObjProperty( mortar, "contents" );
            return;
        else
            PrintTextAbovePrivate(user, "You dont have any bottles.", user);
            return;
        endif
    endif
endfunction

function try_to_make_last(user, potion, mortar)
  var reg;
  case(potion)
    "Refresh":            reg := FindRegs(user, 0xf7a);
    "Total_Refresh":      reg := FindRegs(user, 0xf7a);
    "Agility":            reg := FindRegs(user, 0xf7b);
    "Greater_Agility":    reg := FindRegs(user, 0xf7b);
    "Lesser_Cure":        reg := FindRegs(user, 0xf84);
    "Cure":               reg := FindRegs(user, 0xf84);
    "Greater_Cure":       reg := FindRegs(user, 0xf84);
    "Lesser_Heal":        reg := FindRegs(user, 0xf85);
    "Heal":               reg := FindRegs(user, 0xf85);
    "Greater_Heal":       reg := FindRegs(user, 0xf85);
    "Strength":           reg := FindRegs(user, 0xf86);
    "Greater_Strength":   reg := FindRegs(user, 0xf86);
    "Lesser_Poison":      reg := FindRegs(user, 0xf88);
    "Poison":             reg := FindRegs(user, 0xf88);
    "Greater_Poison":     reg := FindRegs(user, 0xf88);
    "Deadly_Poison":      reg := FindRegs(user, 0xf88);
    "Lesser_Explosion":   reg := FindRegs(user, 0xf8c);
    "Explosion":          reg := FindRegs(user, 0xf8c);
    "Greater_Explosion":  reg := FindRegs(user, 0xf8c);
    "Nightsight":         reg := FindRegs(user, 0xf8d);
    "Homeric":            reg := FindRegs(user, 0x0F8D);
    "Homeric_Greater":    reg := FindRegs(user, 0x0F8D);
    "Invisibility":       reg := FindRegs(user, 0x0F83);
    "Fine_Intellect":     reg := FindRegs(user, 0x0f84);
    "Fabulous_Intellect": reg := FindRegs(user, 0x0f84);
    //"Lesser_Mego_Invulnerability":reg := FindRegs(user, 0xF8E);
    //"Mego_Invulnerability":reg := FindRegs(user, 0xF8E);
    //"Greater_Mego_Invulnerability":reg := FindRegs(user, 0xF8E);


  endcase
    if(reg == "none")
        SendSysMessage(user, "You do not have enough reagents to make that.");
        return;
    endif
    if(!take_regs(user, potion))
        SendSysMessage(user, "You do not have enough reagents to make that.3");
        return;
    endif
  potion[" "] := "_";
  var difficulty := cfg[potion].skill;
  var points := cfg[potion].points;
  var product := cfg[potion].objtype;
  /*PlaySoundEffect(user, SOUND_EFFECT_GRIND);
  PrintTextAbovePrivate(user, "You grind the reagent",user);
  sleep(2);
  PlaySoundEffect(user, SOUND_EFFECT_GRIND);
  PrintTextAbovePrivate(user, "You grind the reagent",user);
  sleep(2);*/



    var loops:=3;
    var delay:=1;
	if( loops )
		for i := 3 to loops
			Sleep(delay);
			PerformAction( user, 0x0021 );
			PlaySoundEffect(user, 0x248);
		endfor
	else
		Sleep(delay);
        PerformAction( user, 0x0021 );
        PlaySoundEffect(user, 0x248);

		Sleep(delay);
	endif







    if(SkillCheck(user, ALCHEMY, difficulty, points))
        var hasbottle:= 0;
        set_critical(1);
        foreach item in EnumerateItemsInContainer(user.backpack)
            if(item.objtype == UOBJECT_EMPTY_BOTTLE)
                if(SubtractAmount(item, 1))
                    hasbottle := 1;
                    break;
                endif
            endif
        endforeach
        set_critical(0);
        if(hasbottle)
            var thing := CreateItemInBackpack(user, product, 1);
            if ( !thing )
                SendSysMessage(user, "Failed to create potion! Please report this to the staff (error logged)");
                syslog( "Alchemy potion creation (2) failed: " + thing.errortext );
                return;
            endif
            PlaySoundEffect(user, SOUND_EFFECT_POUR);
            SetObjProperty(mortar, "LastMade", potion);
            PrintTextAbovePrivate(user, "You create the potion and put it in your backpack", user);
            return;
        else
            PrintTextAbovePrivate(user, "You dont have any bottles.", user);
            SetObjProperty(mortar, "contents", product);
            return;
        endif
    else
        PlaySoundEffect(user, SOUND_EFFECT_POUR);
        PrintTextAbovePrivate(user, "You pour out the useless mixture", user);
        return;
    endif
endfunction

function FindRegs(who, type)
    var holder := "none";
    foreach thing in EnumerateItemsInContainer(who.backpack)
        if(thing.objtype == type)
            holder := thing;
            break;
        endif
    endforeach
    return holder;
endfunction