function MakeBleed(who)
    var bloodArray := array{0x122a, 0x122b, 0x122c, 0x122d, 0x122e, 0x122f};
    var blood := CreateItemAtLocation( who.x, who.y, who.z, bloodArray[RandomInt(6)+1], 1, who.realm );
    if ( blood.errortext )
        return;
    endif
    blood.movable := 0;
    blood.name := "blood";
    blood.decayat := ReadGameClock() + 5;
    blood.SetOwner(who);
    blood.SetDuration(30);
endfunction


function SetupInfo(attacker, defender, byref a_info, byref d_info)
	a_info := struct;
	a_info.+mobile := attacker;
	if ( attacker.IsA(POLCLASS_NPC) && (attacker.weapon).intrinsic )
		a_info.+prefix := "Attack";
		a_info.+cfginfo := NPC_GetNPCConfig(attacker.npctemplate);
	else
		a_info.+prefix := "";
		a_info.+cfginfo := item_cfg[(attacker.weapon).objtype];
	endif

	d_info := struct;
	d_info.+mobile := defender;
	if ( defender.IsA(POLCLASS_NPC) && (defender.weapon).intrinsic )
		d_info.+prefix := "Attack";
		d_info.+cfginfo := NPC_GetNPCConfig(defender.npctemplate);
	else
		d_info.+prefix := "";
		d_info.+cfginfo := item_cfg[(defender.weapon).objtype];
	endif

	return 1;
endfunction

function CombatMsg(mobile, text, type:="")
	// This is done just to prevent message spam on fast weapons.
	if ( CInt(GetObjProperty(mobile, "#CH-Msg"+type)) < ReadMillisecondClock() )
		SendSysMessage(mobile, text);
		SetObjProperty(mobile, "#CH-Msg"+type, ReadMillisecondClock()+800);
	endif

	return 1;
endfunction

function AmmoChecks(byref a_info, byref unused d_info)
	// Get ammo type from the weapon, if there.
	var ammo_type := CInt(GetObjProperty((a_info.mobile).weapon, "AmmoType"));
	// If the weapon has an ammo type CProp but that ammo type is not
	// in the character's backpack then switch to the default ammo type.
	if(ammo_type && !FindSubstance( (a_info.mobile).backpack, ammo_type, 1, 0, 0 ))
		ammo_type := 0;
	endif
	if(!ammo_type)
		ammo_type := CInt((a_info.cfginfo).ProjectileType);
	endif
	// If we are out of all ammo then return.
	if ( !ammo_type )
		return 1;
	endif
	// ammo_consumed is a flag used to determine whether to
	// play the projectile animation.
	ammo_consumed := 0;

    if(CInt((a_info.cfginfo).ProjectileType) != ammo_type) // add by panic
        if ( ConsumeSubstance((a_info.mobile).backpack, ammo_type, 1) )
            ammo_consumed := 1;
            return 1;
        else
            CombatMsg(a_info.mobile, "You do not have any "+GetObjTypeDesc(ammo_type, 1)+"!", "Ammo");
            return 0;
        endif
    endif
    return 0;
endfunction
