use uo;
use os;
use unicode;

include ":gumps:gumps";
include ":charclasses:classes";
include ":character:skillLocks";
include "include/client";
include ":attributes:advanceCheck";


var isdownskill := 0;

function SkillBookGump( mobile, viewTo, dskill := 0)
    isdownskill:=dskill;

    var classId := CInt(GetObjProperty(mobile, "classid"));
    var classLevel := CInt(GetObjProperty(mobile, "classlevel"));
    var cap_array := {};
    foreach class in ({ID_MAGE,ID_BARD,ID_WARRIOR,ID_RANGER,ID_CRAFTER,ID_THIEF})
        foreach s in ( GetClasseSkills( class ) )
            var skill_id := AP_AttributeNameToSkillId( AP_SkillIdToAttributeName(s) );
            if(class != 2)
                cap_array[skill_id+1] := 100;
            else
                cap_array[skill_id+1] := AP_GetSkillsCap();
            endif
        endforeach
    endforeach

	var s_gump := GFCreateGump(70, 70);

    GFGumpPic( s_gump, 0, 0, 500);
	GFPage( s_gump, 0 );

    var icon := GetClassGumpIcon(classId);
	for page_id := 1 to 4
        GFPage( s_gump, page_id );
		if( page_id == 1 )
		    renderGumpLine(s_gump, 0);
            GFTextLine(s_gump, 60, 12, 88, "Квалификация");
		    if(icon)
                GFGumpPic( s_gump, 80, 60, icon);
		    endif
            var textClass := GetClasseName(classId);
            if(classLevel)
                textClass := textClass+": "+classLevel+" ур.";
            endif
            GFTextLine(s_gump, 80, 130, 88, textClass);

        endif;
		if( page_id > 1 )
            GFAddButton(s_gump, 0, 0, 501, 501, GF_PAGE_BTN, (page_id-1));
		endif;
		if( page_id != 4 )
            GFAddButton(s_gump, 356, 0, 502, 502, GF_PAGE_BTN, (page_id+1));
		endif

        if(page_id == 1)
            renderData(s_gump, mobile, ID_MAGE, "right", 1);
        elseif(page_id==2)
            renderData(s_gump, mobile, ID_WARRIOR, "left", 2);
            renderData(s_gump, mobile, ID_CRAFTER, "right", 3);
        elseif(page_id==3)
            renderData(s_gump, mobile, ID_RANGER, "left", 4);
            renderData(s_gump, mobile, ID_THIEF, "right", 5);
        elseif(page_id==4)
            renderData(s_gump, mobile, ID_BARD, "left", 6);
        endif

    endfor

	//var input := GFSendGump( mobile, s_gump );


    var res := GFSendGump( viewTo, s_gump );
    res := CInt( res[0] );
    if( res )
        if((res >= 100) && (res < 200))
            toggleSkill(mobile, (res - 100));
        elseif((res >= 200) && (res < 300))
            downSkill(mobile, (res - 200));
        endif
    endif

endfunction

function getTotalSkill(mobile)

    var i;
    var total   := 0.0;
	var bard	:= 0.0;
	var crafter	:= 0.0;
	var mage	:= 0.0;
	var ranger	:= 0.0;
	var thief	:= 0.0;
	var warrior	:= 0.0;
	for(i:=0; i<=SKILLID__HIGHEST - 10; i:=i+1)
		var skill := GetEffectiveSkill(mobile,i);
		total	    := total + skill;
		if( i in GetClasseSkills( ID_BARD ) )
			bard := bard + skill;
		elseif( i in GetClasseSkills( ID_CRAFTER ) )
			crafter := crafter + skill;
		elseif( i in GetClasseSkills( ID_MAGE ) )
			mage := mage + skill;
		elseif( i in GetClasseSkills( ID_RANGER ) )
			ranger := ranger + skill;
		elseif( i in GetClasseSkills( ID_THIEF ) )
			thief := thief + skill;
		elseif( i in GetClasseSkills( ID_WARRIOR ) )
			warrior := warrior + skill;
		endif
	endfor
    return array{total,mage,warrior,crafter,ranger,thief,bard};
endfunction

function renderData(byref gump, mobile, classid, side, counter)
    var start_padding:=47;
    var p:=0;
    var tp:=0;
    if(side == "left")
        p:=38;
        tp:=38;
    else
        p:=228;
        tp:=228;
    endif
    var getSkillList := GetSkillListId()[classid];
    var getSkillName := GetSkillListName()[classid];
    var i;
    GFTextLine(gump, (tp + 30), 12, 88, GetClasseName(classId));

    var skill:=0;
    var skillData := getTotalSkill(mobile);

    for(i:=1; i<=getSkillList.Size(); i:=i+1)
        var skill := GetEffectiveSkill(mobile,i);

        GFTextLine(gump, p, start_padding, 0, getSkillName[i]);
        GFTextLine(gump, (p + 122), start_padding, 0, GetBaseSkill(mobile, getSkillList[i]));

        if(isdownskill)
            GFAddButton(gump, (p + 148), (start_padding + 2), 2117, 2118, GF_CLOSE_BTN, (200 + getSkillList[i]));
        else
            var skillstate := SL_GetLockState(mobile, getSkillList[i]);
            if(skillstate >= 1)
                GFAddButton(gump, (p + 148), (start_padding + 2), 2092, 2435, GF_CLOSE_BTN, (100 + getSkillList[i]));
            else
                GFAddButton(gump, (p + 148), (start_padding + 2), 2435, 2436, GF_CLOSE_BTN, (100 + getSkillList[i]));
            endif
        endif
        start_padding += 17;
    endfor


    GFTextLine(gump, p, 195, 0, "Процент");
    GFTextLine(gump, (p + 122), 195, 0, GivePercent(skillData[classid+1],skillData[1]) + "%");


    var id:=0;
    if(counter % 2)
        id:=1;
    endif

    renderGumpLine(gump, id);
    return gump;
endfunction


function renderGumpLine(byref gump, id)
    var h;
    var ttop := 30;

    var xOffset := 35 + ( id * 190 );
    GFGumpPic( gump, xOffset, ttop, 57);
    xOffset += 20;
    for ( h:=0; h<8; h+=1 )
        GFGumpPic( gump, xOffset, ttop, 58);
        xOffset += 15;
    endfor
    GFGumpPic( gump, (xOffset - 5), ttop, 59);

    return gump;
endfunction

function toggleSkill(mobile, skill_id)
    var skillstate := SL_GetLockState(mobile, skill_id);
    if(skillstate == 2)
        SL_SaveLockState(mobile, skill_id, 0);
    elseif(skillstate == 0)
        SL_SaveLockState(mobile, skill_id, 2);
    endif
endfunction

function downSkill(mobile, skill_id)
    var value := CInt(SendTextEntryGump( mobile, "Set value: ", TE_CANCEL_ENABLE, TE_STYLE_NORMAL, 3));

    var newValue := GetBaseSkill(mobile, skill_id) - value;

    if(newValue >= 0)
        SetBaseSkill(mobile, skill_id, newValue);
    else
        SendSysMessageUC(mobile, "Вы не можете урезать больше навыка, чем у вас есть.", "RUS", color := MESSAGE_COLOR_BAD);
    endif

endfunction

function GivePercent( skill, total )

	var percent	:= CDbl( skill / total * 100.0 );
	var floating	:= CInt( percent * 10 );
	var cuted	:= CInt( percent ) * 10;
	var diffe	:= CInt( floating - cuted );

	percent	:= CInt(percent) + "." + diffe;

	return percent;

endfunction


function UC(strs)
    var result := array {};
    if (TypeOf(strs) != "Array")
    result := CAscZ(CStr(strs));
    else
    foreach str in (strs)
    if (TypeOf(str) != "Array")
    str := CAscZ(CStr(str));
    endif
    result := result + str;     // POL 2003-06-19 or later required for this operation
    /*      foreach elem in (str)
    result.append(elem);
    endforeach */
    endforeach
    endif
    return result;
endfunction

function UC2(strs)
  var result := array {};
  if (TypeOf(strs) != "Array")
    result := CAscZ(CStr(strs));
  else
    foreach str in (strs)
      if (TypeOf(str) != "Array")
        str := CAscZ(CStr(str));
      endif
      result := result + str;     // POL 2003-06-19 or later required for this operation
      foreach elem in (str)
        result.append(elem);
      endforeach
      //
    endforeach
  endif
  return result;
endfunction