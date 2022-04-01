
//horsebarn 5228 1182 0
const COPY_BACKPACK_X := 5858;
const COPY_BACKPACK_Y := 1158;
const COPY_BACKPACK_Z := 0;

const TIME_OPEN_CONTAINER := 30;

function freeHands(who)
	if(GetEquipmentByLayer(who, LAYER_HAND1) || GetEquipmentByLayer(who, LAYER_HAND2))
		SendSysMessage(who, "You need empty hands to perform this action!");
		return;
	endif
endfunction

function needGloves(who)
	var wearGloves:= 0;
    foreach item in ListEquippedItems(who)
    	if (item.objtype == 0x1300a || item.objtype == 0x1300b)
            wearGloves:= 1;
	    endif
	endforeach
	if(wearGloves == 0)
		SendSysMessage(who,"You need to equip thief gloves to do that");
		return 0;
	endif
	return 1;
endfunction

function tellplayers(who)
	var targets:=ListMobilesNearLocation(who.x, who.y, who.z, 10,who.realm);
	foreach mobile in targets
		if ((mobile.serial!=who.serial) && (mobile.dead==0) && (!mobile.criminal))
			SendSysMessage(mobile, "You notice "+who.name+" stealing!");
		//elseif(mobile.serial==who.serial)
		//	SendSysMessage(who, "You are caught stealing!");
		endif
	endforeach
endfunction

function callguard(who)
    var postarget:=ListMobilesNearLocation(who.x, who.y, who.z, 10,who.realm);
	foreach mobile in postarget
		if ((mobile.serial!=who.serial) && (mobile.npctemplate) && ((mobile.graphic == 400) || (mobile.graphic == 401)))
			PrintTextAbove(mobile, "Guards! Stop "+who.name+" you thief!");
		endif
	endforeach
endfunction

function GetDiff(who, victim)
    var skill := GetEffectiveSkill( who, SKILLID_SNOOPING );
    var whoskill := CInt(skill/4) + CInt(GetDexterity(who)/4);                     // max 60
    var victimskill := CInt(GetDexterity(victim)/6) + CInt(GetIntelligence(victim)/4) + 10; // max 60
    var chance := victimskill - whoskill + 50;
SendSysMessage(who, "chance: "+chance);
    return chance;
endfunction