// COMBAT ABILITY INCLUDE FILE
// This include file contains the function to disable the combat ability again.

use uo;
use os;
use polsys;
use util;

include "include/client";

function RemoveAbility(who)
    var ability_reset := CreatePacket(0xBF,5);
	ability_reset.SetInt8(2,0x05);
	ability_reset.SetInt8(4,0x21);
	ability_reset.SendPacket(who);
    EraseObjProperty(who, "Ability");
    var options := GetObjProperty(who, "AbilityOptions");
    EraseObjProperty(who, "AbilityOptions");


    var regen := AP_GetVitalRegenRateMod(who, options.vital);
    if(regen)
        AP_ModifyVitalRegenRateMod( who, options.vital, CInt(regen) - 500 );
    //else
    //    AP_SetVitalRegenRateMod( who, options.vital, 0 );
    endif

    //SendSysMessage(who, "You Disable Ability Attempt", 3, MESSAGE_COLOR_BAD);
endfunction


function killPID(who)
    SendSysMessage(who, "You Disable Ability Attempt 11", 3, MESSAGE_COLOR_BAD);
    var process := GetProcess(GetObjProperty(who, "#AbilityPID"));
    process.kill();
    EraseObjProperty( who, "#AbilityPID");
    EraseObjProperty( who, "Ability");
    EraseObjProperty( who, "AbilityOptions");
    return 1;
endfunction


function AbilityChecks(attacker, defender, damage)
    ability := CInt(GetObjProperty(attacker, "Ability"));
    if(ability)
        var timer := CInt(GetObjProperty(defender, "#AbilityTimer"));
        if(timer)
            if(timer <= ReadGameClock())
                return 0;
            endif
            EraseObjProperty(defender, "#AbilityTimer");
        endif

        var elem := FindConfigElem(g_ability_cfg, ability);
        var chance := GetConfigReal(elem, "Chance");
        var luck := GetConfigInt(elem, "Luck");
        if(attacker.luck >= luck)
            var rfloat := CDbl(RandomFloat(99)+1);

            if(chance >= rfloat)
                print("ability chance");
                var script := GetConfigString(elem, "Script");
                /*var mana := GetConfigInt(elem, "ManaCost");
                var curr := CInt(AP_GetVital(attacker, MANA));
                if ( curr < mana )
                    SendSysMessage(attacker, "low mana", 3, MESSAGE_COLOR_BAD);
                    //RemoveAbility(attacker);
                    return 0;
                endif*/
                var result := Run_Script(script, {attacker, defender, damage});
                if(result)
                    attacker.luck_mod -= 1;
                    //AP_SetVital(attacker, MANA, (curr-mana));
                    return result;
                endif
            endif
        endif
    endif
    return 0;
endfunction
