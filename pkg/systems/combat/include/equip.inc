
include ":damage:damage";
include ":itemUtils:itemtypes";

const APPRENTICE_SKILL := 1;
const JOURNEYMAN_SKILL := 2;
const EXPERT_SKILL     := 3;
const ADEPT_SKILL      := 4;
const MASTER_SKILL     := 5;
const GM_SKILL         := 6;


const SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL1	:= 75;		// In base value for apprentice items.
const SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL2	:= 90;		// In base value for journeyman items.
const SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL3	:= 110;	// In base value for expert items.
const SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL4	:= 125;	// In base value for adept items.
const SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL5	:= 140;	// In base value for master items.
const SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL6	:= 200;	// In base value for grandmaster items.


function HandleMods(who, item)
    set_critical(1);
    var itemdesc := ReadConfigFile(":*:itemdesc");
    var penalty  := CInt(itemdesc[item.objtype].DexPenalty);
    var restrict := CInt(itemdesc[item.objtype].GenderSpecific);
    var classid := CInt(itemdesc[item.objtype].classid);
    var disarm := CInt(GetObjProperty(who, "Disarm"));

    if(disarm)
        SendSysMessage(who, "You disarmed...", 3, 33);
        return 0;
    endif

    if((restrict == 1) and (who.graphic == 400))
        SendSysMessage(who, "Only females may wear that.");
        return 0;
    endif
    if(penalty)
        SetObjProperty(item, "#equipped", who.serial);
        AP_SetStatMod(who, DEXTERITY, AP_GetStatMod(who, DEXTERITY) - penalty);
    endif
    var owner := CInt(GetObjProperty(item, "owner"));
    if(owner)
        if(owner != who.serial)
            SendSysMessage(who, "You are not owner this item.");
            return 0;
        endif
    endif

    var who_classid := CInt(GetObjProperty(who,"classid"));
    if(classid && !who.isA(POLCLASS_NPC))
        if(IsRangerArmor (item))
            if(classid != who_classid)
                SendSysMessage(who, "Your class is not suitable for this subject");
                return 0;
            endif
        elseif(IsMedableArmor (item))
            if(classid != who_classid)
                SendSysMessage(who, "Your class is not suitable for this subject");
                return 0;
            endif
        elseif(IsWarriorArmor (item))
            if(classid != who_classid)
                SendSysMessage(who, "Your class is not suitable for this subject");
                return 0;
            endif
        endif
    endif

    /*var iDed := CInt(GetObjProperty(item, "IDed"));

    if(iDed)
        var skillid := CInt(GetObjProperty(item, "skilladv"));
        var skillAmount := CInt(GetObjProperty(item, "skilladvamount"));
        if(skillid && skillAmount)
            var skillName := AP_SkillIdToAttributeName(skillid);
            AP_SetSkillMod(who, skillName, AP_GetSkillMod(who, skillName) + skillAmount);
        endif
    endif*/

    DoStatMods(who, item);
    DoSkillAdvance(who, item);
    DoProt( who, item );
    DoArMods( who, item );
    DoImmunity( who, item );
    //DoHitChance( who, item );
    var type := GetObjProperty(item, "type");
    if(!type)
        return 1;
    endif
    var val := 0;
    if(!who.ip)
        return 1;
    endif

    case(type)
        "accuracy":     val := accuracy(who,item);
        "cursed":       val := cursed(who,item);
        "sight":        val := sight(who,item);
        "reflect":      val := reflect(who,item);
        "protection":   val := protection(who,item);
        "wizzohat":     val := wizzohat(who,item);
        "strength":     val := strengthBoost(who,item);
        "invisibility": val := invisibility(who,item);
        default:        val := 1;
    endcase

    if(item.resist_physical)
        ModifyResistance(who, RES_PHYSICAL_MOD, item.resist_physical);
    endif
    if(item.resist_fire)
        ModifyResistance(who, RES_FIRE_MOD, item.resist_fire);
    endif
    if(item.resist_cold)
        ModifyResistance(who, RES_COLD_MOD, item.resist_cold);
    endif
    if(item.resist_poison)
        ModifyResistance(who, RES_POISON_MOD, item.resist_poison);
    endif
    if(item.resist_energy)
        ModifyResistance(who, RES_ENERGY_MOD, item.resist_energy);
    endif

    if(item.damage_physical)
        ModifyDamage(who, DMG_PHYSICAL_MOD, item.damage_physical);
    endif
    if(item.damage_fire)
        ModifyDamage(who, DMG_FIRE_MOD, item.damage_fire);
    endif
    if(item.damage_cold)
        ModifyDamage(who, DMG_COLD_MOD, item.damage_cold);
    endif
    if(item.damage_poison)
        ModifyDamage(who, DMG_POISON_MOD, item.damage_poison);
    endif
    if(item.damage_energy)
        ModifyDamage(who, DMG_ENERGY_MOD, item.damage_energy);
    endif

    SetObjProperty( item, "EquippedAlready", 1 );

  return val;
  set_critical(0);
endfunction

function strengthBoost(who,item)
    var gain := GetObjProperty(item, "gain");
    if(!gain)
        gain := 5;
    endif
    SetObjProperty(item, "#equipped", who.serial);
    var parms := {who, item};
    start_script("chargeUpkeep", parms);
    return 1;
endfunction

function wizzohat(who,item)
    AP_SetStatMod(who, INTELLIGENCE, AP_GetStatMod(who, INTELLIGENCE) + 10);
    AP_SetStatMod(who, STRENGTH, AP_GetStatMod(who, STRENGTH) - 5);
    AP_SetStatMod(who, DEXTERITY, AP_GetStatMod(who, DEXTERITY) - 5);
    SetObjProperty(item, "#equipped", who.serial);
    return 1;
endfunction

function accuracy(who,item)
  var amt := CInt(GetObjProperty(item,"gain"));
  if((!amt) || (amt == 0))
    amt := 5;
  endif
  var cfg := ReadConfigFile(":combat:itemdesc");
  var attrib := cfg[item.objtype].Attribute;
  case(attrib)
    "Wrestling": ModifyAttributeTemporaryMod(who, WRESTLING,     amt);
    "Fencing":   ModifyAttributeTemporaryMod(who, FENCING,       amt);
    "Archery":   ModifyAttributeTemporaryMod(who, ARCHERY,       amt);
    "Swords":    ModifyAttributeTemporaryMod(who, SWORDSMANSHIP, amt);
    "Mace":      ModifyAttributeTemporaryMod(who, MACEFIGHTING,  amt);
  endcase
  SetObjProperty(item, "#equipped", who.serial);
  return 1;
endfunction

function cursed(who,item)
  PlaySoundEffect(who, 0x246);
  if(!GetObjProperty(item,"named"))
    item.name := item.name + " [Cursed]";
    SetObjProperty(item,"named", 1);
  endif
  PrintTextAbovePrivate(who,"you have been cursed!", who);
  SetObjProperty(item, "#equipped", who.serial);
  return 1;
endfunction

function invisibility(who,item)
  var gain := GetObjProperty(item, "gain");
  if(!gain)
    gain := 5;
  endif
  SetObjProperty(item, "#equipped", who.serial);
  var parms := {who, item};
  start_script("chargeUpkeep", parms);
  return 1;
endfunction

function sight(who,item)
  var gain := GetObjProperty(item, "gain");
  if(!gain)
    gain := 5;
  endif
  SetObjProperty(item, "#equipped", who.serial);
  var parms := {who, item};
  start_script("chargeUpkeep", parms);
  return 1;
endfunction

function protection(who,item)
  var gain := GetObjProperty(item, "gain");
  if(!gain)
    gain := 5;
  endif
  SetObjProperty(item, "#equipped", who.serial);
  var parms := {who, item};
  start_script("chargeUpkeep", parms);
  return 1;
endfunction

function reflect(who,item)
  var gain := GetObjProperty(item, "gain");
  if(!gain)
    gain := 5;
  endif
  SetObjProperty(item, "#equipped", who.serial);
  var parms := {who, item};
  start_script("chargeUpkeep", parms);
  return 1;
endfunction

function DoImmunity( who, item )

	if( GetObjProperty( item, "EquippedAlready" ) )
    	return 1;
	endif

	var possibles := { "PermPoisonImmunity",
			     "PermMagicImmunity",
			     "Permmr",
			     "FireProtection",
			     "WaterProtection",
			     "AirProtection",
			     "EarthProtection",
			     "FreeAction",
			     "NecroProtection",
			     "HolyProtection",
			     "PhysicalProtection",
			     "HealingIncrease" };



	var level;

	foreach prot in possibles
		level := CInt( GetObjProperty( item, prot ) );
        var v := CInt(GetObjProperty( who, prot ));
		if( level )

			if( CInt(GetObjProperty( who, prot )) < level )
				SetObjProperty( who, prot, level + v );
			endif
		endif
	endforeach

	return 1;

endfunction

function DoStatMods(who,it)

	if ( GetObjProperty(it,"str") )
	    SetStrengthMod(who,  CInt(GetStrengthMod(who)) + CInt(GetObjProperty(it,"str")));
	endif
	if ( GetObjProperty(it,"int") )
		SetIntelligenceMod(who,  CInt(GetIntelligenceMod(who)) + CInt(GetObjProperty(it,"int")));
	endif
	if ( GetObjProperty(it,"dex") )
		SetDexterityMod(who,  CInt(GetDexterityMod(who)) + CInt(GetObjProperty(it,"dex")));
	endif
	//if ( GetObjProperty(it,"DefaultDex") )
	//	SetDexterityMod(who,  CInt(GetDexterityMod(who)) + CInt(GetObjProperty(it,"DefaultDex")));
	//endif

endfunction

function DoSkillAdvance(who,it)
	var skillid := Cint(GetObjProperty(it,"skilladv"));
    var skillnum := AP_SkillIdToAttributeName(skillid);

    var iDed := CInt(GetObjProperty(it, "IDed"));
    if(!iDed)

        return;
    endif

	if( GetObjProperty( it, "EquippedAlready" ))
		//if(!GetSkillTemporaryMod(who, skillnum))
		//	return;
		//endif
        if(!GetAttributeTemporaryMod(who, skillnum))
            print("GetAttributeTemporaryMod");
            return;
        endif

	endif


	if ( (skillnum) || (skillnum == 0) )

		var power    := CInt(GetObjProperty( it, "skilladvamount" ));

		var skillval := GetEffectiveSkill(who, skillid);

		/*var skillcap;
        case( power )
			APPRENTICE_SKILL:
				skillcap := SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL1;
				break;
			JOURNEYMAN_SKILL:
				skillcap := SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL2;
				break;
			EXPERT_SKILL:
				skillcap := SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL3;
				break;
			ADEPT_SKILL:
				skillcap := SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL4;
				break;
			MASTER_SKILL:
				skillcap := SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL5;
				break;
			GM_SKILL:
				skillcap := SETTING_MAX_SKILL_FOR_SKILL_ITEM_LEVEL6;
				break;
			default:
				skillcap := 0;
				power    := 0;
		endcase
		skillcap := skillcap;*/

		// Apply Multipliers
		var multiplier := CInt( GetObjProperty( it, "SpecMult" ) );
		if( multiplier )
			power := CInt( power * multiplier );
		elseif(it.isa(POLCLASS_WEAPON))
			power := power * 5;
		endif

		//if((power + skillval) > skillcap)
		//	power := skillcap - skillval;
		//endif
		if(power < 0)
			power := 0;
		endif

		//AlterSkillTemporaryMod( who, skillnum, power);

        AP_SetSkillMod(who, skillnum, AP_GetSkillMod(who, skillnum) + power);
		//SetObjProperty(who, ("currsk" + Cstr(it.serial)), power);

	endif

endfunction

function DoArMods( who, item )

	var ar := CInt(GetObjProperty( item, "ArBonus" ));
	if( ar )
		who.ar_mod := who.ar_mod + ar;
	endif

endfunction


function DoHitChance(who, item)
who.hit_chance_mod := 100;
endfunction


function DoProt( who, item )

	if( GetObjProperty( item, "EquippedAlready" ) )
		return 1;
	endif

	var possibles := {
        "PoisonImmunity",
        "MagicImmunity",
        "SpellReflection"
    };


	foreach prot in possibles
		if( GetObjProperty( item, prot ) )
			SetObjProperty( who, prot, item.serial );
		endif
	endforeach

	return 1;

endfunction
