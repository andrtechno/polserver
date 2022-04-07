use uo;
use util;
include ":attributes:attributes";
include "include/buff";

function GetModAmount(magery)
  var mod_amount := CInt(RandomInt(3) +(magery/10));
  if(mod_amount > 10)
    mod_amount := 10;
  endif
  return mod_amount;
endfunction

function GetModDuration(magery)
  var duration := CInt(60 +((magery / 30) * 60));
  return(duration);
endfunction

function CanMod(who, mod)
    if(GetObjProperty(who, mod))
        return 1;
    else
        return 0;
    endif
endfunction

function DoTempMod(who, mod, amt, duration)

  var parms := {};
  parms[2] := who;
  parms[1] := mod;
  parms[3] := amt;
  parms[4] := duration;
  parms[5] := ReadGameClock() + duration;

  start_script("::/misc/unmod", parms);

endfunction


function undo_mod(who, mod, amt)
if(CanMod(who,mod))
  case(mod)
    "#int":
    "#p_int":
    "#cast_cunning":
    "#intbonus":
        SetIntelligenceMod(who, GetIntelligenceMod(who) - amt);

    "#p" :  who.setParalyzed(0);
    "#cast_bless":
        SetStrengthMod(who, GetStrengthMod(who) - amt);
        SetDexterityMod(who, GetDexterityMod(who) - amt);
        SetIntelligenceMod(who, GetIntelligenceMod(who) - amt);

    "#cast_agility":
    "#dexbonus":
    "#p_dex":
    "#dex":
        SetDexterityMod(who, GetDexterityMod(who) - amt);

    "#cast_strength":
    "#str":
    "#p_str":
    "#strbonus":
        SetStrengthMod(who, GetStrengthMod(who) - amt);

    "#cast_protection":
    "#ar":
        who.ar_mod := who.ar_mod - amt;

    "#cast_poly":
        SetStrengthMod(who, GetStrengthMod(who) - amt);
        SetDexterityMod(who, GetDexterityMod(who) - amt);
        SetIntelligenceMod(who, GetIntelligenceMod(who) - amt);
        who.ar_mod := who.ar_mod - amt;
        who.graphic := who.objtype;
        who.color := who.truecolor;
        EraseObjProperty(who,"poly");

  endcase

    var modd := GetObjProperty( who, mod );
    if(modd.buff_id)
       // BuffDebuff_removeIcon(who,modd.buff_id);
        who.delBuff(modd.buff_id);
    endif

  EraseObjProperty(who, mod);
  PlaySoundEffect(who, 0x01eb);
  PlayObjectCenteredEffect(who, 0x373a, 10,10);
endif
endfunction


function SpellCheck(who, modType, effect, duration, value, description)

	var i, x;

	var effectTypes := {};
	var effectValues := {};
	var effectExpirations := {};
	var effectDescriptions := {};
	var activeEffects := GetObjProperty(who, "ActiveEffects");
	if (!activeEffects)
		activeEffects := { };
	else
		for(i:=1; i<=len(activeEffects); i:=i+1)
			x := activeEffects[i];
			effectTypes[i] := x[1];
			effectValues[i] := x[2];
			effectExpirations[i] := x[3];
			effectDescriptions[i] := x[4];
		endfor
	endif

///////////////////////////
//
// EFFECTS START HERE
//
///////////////////////////

	case (effect)

// Bless - buffs Strength, Dexterity, and Inteligence.  Use "Bless" with "SUBTRACT" modType for
//         a curse effect.  Isn't a real mod per se, in that it activates 3 other mods.

	"Bless":
		var blessMod;
		var blessModList := {"Strength", "Dexterity", "Intelligence"};
		foreach blessMod in blessModList
			SpellCheck(who, modType, blessMod, duration, value, description);
		endforeach


// Dexterity - buffs Dex.  Agility, Bless, Curse, Clumsy, etc. should all use this effect.

	"Dexterity":
		if (modType == "ADD")
			if (effect in effectTypes)
				for(i:=1; i<=len(effectTypes); i:=i+1)
					if (effectTypes[i] == effect)
						if (value <= effectValues[i])
							SendSysMessage(who, "Nothing happens: the " + description + " is weaker than the " + CStr(effectDescriptions[i]) + " that is already affecting you.");
						elseif (effectValues[i] < 0)
							SendSysMessage(who, "The " + description + " has countered the effects of the " + CStr(effectDescriptions[i]) + " that was affecting you.");
							SetDexterityMod(who, GetDexterityMod(who) - effectValues[i]);
							effectValues[i] := 0;
							effectExpirations[i] := (ReadGameClock() + duration);
						else
							SetDexterityMod(who, GetDexterityMod(who) + (value - effectValues[i]) );
							SendSysMessage(who, "You sense that you are quicker and more agile.");
							effectValues[i] := value;
							effectExpirations[i] := (ReadGameClock() + duration);
							effectDescriptions[i] := description;
							Start_Script("::/misc/cleareffect", {who, "Dexterity", duration});
						endif
					endif
				endfor
			else
				SetDexterityMod(who, GetDexterityMod(who) + value);
				SendSysMessage(who, "You sense that you are quicker and more agile.");
				i := len(effectTypes)+1;
				effectTypes[i] := effect;
				effectValues[i] := value;
				effectExpirations[i] := (ReadGameClock() + duration);
				effectDescriptions[i] := description;
				Start_Script("::/misc/cleareffect", {who, "Dexterity", duration});
			endif
		elseif (modType == "SUBTRACT")
			if (effect in effectTypes)
				for(i:=1; i<=len(effectTypes); i:=i+1)
					if (effectTypes[i] == effect)
						if (value >= effectValues[i])
							SendSysMessage(who, "Nothing happens: the " + description + " is weaker than the " + CStr(effectDescriptions[i]) + " that is already affecting you.");
						elseif (effectValues[i] > 0)
							SendSysMessage(who, "The " + description + " has countered the effects of the " + CStr(effectDescriptions[i]) + " that was affecting you.");
							SetDexterityMod(who, GetDexterityMod(who) - effectValues[i]);
							effectValues[i] := 0;
							effectExpirations[i] := (ReadGameClock() + duration);
						else
							SetDexterityMod(who, GetDexterityMod(who) + (value - effectValues[i]) );
							SendSysMessage(who, "You sense that you are slower and less agile.");
							effectValues[i] := value;
							effectExpirations[i] := (ReadGameClock() + duration);
							effectDescriptions[i] := description;
							Start_Script("::/misc/cleareffect", {who, "Dexterity", duration});
						endif
					endif
				endfor
			else
				SetDexterityMod(who, GetDexterityMod(who) + value);
				SendSysMessage(who, "You sense that you are slower and less agile.");
				i := len(effectTypes)+1;
				effectTypes[i] := effect;
				effectValues[i] := value;
				effectExpirations[i] := (ReadGameClock() + duration);
				effectDescriptions[i] := description;
				Start_Script("::/misc/cleareffect", {who, "Dexterity", duration});
			endif

		elseif (modType == "REMOVE")
			for(i:=1; i<=len(effectTypes); i:=i+1)
				if (effectTypes[i] == effect)
					if (effectExpirations[i] <= ReadGameClock() )
						SetDexterityMod(who, GetDexterityMod(who) - effectValues[i]);
						if (effectValues[i] > 0)
							SendSysMessage(who, "You feel a bit slower as the " + CStr(effectDescriptions[i]) + " wear off.");
						elseif (effectValues[i] < 0)
							SendSysMessage(who, "You feel a bit faster as the " + CStr(effectDescriptions[i]) + " wear off.");
						else
							SendSysMessage(who, "The " + CStr(effectDescriptions[i]) + " has worn off.");
						endif
						effectTypes.erase(i);
						effectValues.erase(i);
						effectExpirations.erase(i);
						effectDescriptions.erase(i);
					endif
				endif
			endfor
		endif

// Free Action - player can move while frozen, etc.
	"FreeAction":
		if (modType == "ADD")
			if (effect in effectTypes)
				for(i:=1; i<=len(effectTypes); i:=i+1)
					if (effectTypes[i] == effect)
						effectExpirations[i] := ReadGameClock() + duration;
						GrantPrivilege(who, "freemove");
						who.enable("freemove");
						if (effectDescriptions[i] != description)
							SendSysMessage(who, "The " + description + " has extended the duration of your previous " + CStr(effectDescriptions[i]) + ".");
						else
							SendSysMessage(who, "You have refreshed the effects of the " + description + ".");
						endif
						effectExpirations[i] := description;
						Start_Script("::/misc/cleareffect", {who, "FreeAction", duration});
					endif
				endfor
			else
				GrantPrivilege(who, "freemove");
				who.enable("freemove");
				i := len(effectTypes)+1;
				effectTypes[i] := effect;
				effectValues[i] := "freemove";
				effectExpirations[i] := (ReadGameClock() + duration);
				effectDescriptions[i] := description;
				SendSysMessage(who, "You are now moving freely.");
				Start_Script("::/misc/cleareffect", {who, "FreeAction", duration});
			endif
		elseif (modType == "REMOVE")
			for(i:=1; i<=len(effectTypes); i:=i+1)
				if (effectTypes[i] == effect)
					if (effectExpirations[i] <= ReadGameClock() )
						who.disable("freemove");
						RevokePrivilege(who, "freemove");
						SendSysMessage(who, "The " + CStr(effectDescriptions[i]) + " has worn off.");
						effectTypes.erase(i);
						effectValues.erase(i);
						effectExpirations.erase(i);
						effectDescriptions.erase(i);
					endif
				endif
			endfor
		endif

// Regeneration - increases heal rate.
	"Regeneration":
		if (modType == "ADD")
			if (effect in effectTypes)
				for(i:=1; i<=len(effectTypes); i:=i+1)
					if (effectTypes[i] == effect)
						if (value <= effectValues[i])
							SendSysMessage(who, "Nothing happens: the " + description + " is weaker than the " + CStr(effectDescriptions[i]) + " that is already effecting you.");
						else
							SetObjProperty(who, (CPROP_NAME_PREFIX_REGEN_RATE + VITALID_LIFE), value);
							RecalcVitals(who);
							if (effectDescriptions[i] != description)
								SendSysMessage(who, "The " + description + " has extended the potency of your previous " + CStr(effectDescriptions[i]) + ".");
							else
								SendSysMessage(who, "You have refreshed the effects of the " + description + ".");
							endif
							effectExpirations[i] := (ReadGameClock() + duration);
							effectDescriptions[i] := description;
							Start_Script("::/misc/cleareffect", {who, "Regeneration", duration});
						endif
					endif
				endfor
			else
				SetObjProperty(who, (CPROP_NAME_PREFIX_REGEN_RATE + VITALID_LIFE), value);
				RecalcVitals(who);
				i := len(effectTypes)+1;
				effectTypes[i] := effect;
				effectValues[i] := value;
				effectExpirations[i] := (ReadGameClock() + duration);
				effectDescriptions[i] := description;
				SendSysMessage(who, "Your body begins to regenerate more rapidly.");
				Start_Script("::/misc/cleareffect", {who, "Regeneration", duration});
			endif
		elseif (modType == "REMOVE")
			for(i:=1; i<=len(effectTypes); i:=i+1)
				if (effectTypes[i] == effect)
					if (effectExpirations[i] <= ReadGameClock() )
						EraseObjProperty(who, (CPROP_NAME_PREFIX_REGEN_RATE + VITALID_LIFE));
						RecalcVitals(who);
						SendSysMessage(who, "The " + CStr(effectDescriptions[i]) + " has worn off.");
						effectTypes.erase(i);
						effectValues.erase(i);
						effectExpirations.erase(i);
						effectDescriptions.erase(i);
					endif
				endif
			endfor
		endif

	endcase
// END OF EFFECTS


	activeEffects := {};
	x := {};
	for(i:=1; i<=len(effectTypes); i:=i+1)
		x := {};
		x[1] := effectTypes[i];
		x[2] := effectValues[i];
		x[3] := effectExpirations[i];
		x[4] := effectDescriptions[i];
		activeEffects[i] := x;
	endfor

	SetObjProperty(who, "ActiveEffects", activeEffects);

endfunction


