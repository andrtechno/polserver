use uo;
use unicode;

include ":gumps:gumps";
include ":gumps:gumps_ex";


function GetQuestListGump(who)
    var gump := GFCreateGump ( 100, 100 );
    GFPage ( gump, 0);
    GFResizePic(gump, 0, 0, GetCfgConst("Defaults", "ForeGround"), 450, 500);
    GFTextLine(gump, 15, 15, 2100, "Название квеста" );
    GFTextLine(gump, 380, 15, 2100, "Пройти" );

    var paddingTop:=30;
    var stepTop:=25;
    var i := 1;
    foreach questItem in (GetQuestList())
        GFTextLine(gump, 15, paddingTop + stepTop * i, 500, questItem[1]);
        if(GetActiveQuest(who) == i)
            //GFAddButton(gump, 320, paddingTop + stepTop * i, 2074, 2075, GF_CLOSE_BTN, 0);
            GFTextLine(gump, 270, paddingTop + stepTop * i, 67, "Выполняется");
            GFAddButton(gump, 380, paddingTop + stepTop * i, 2071, 2072, GF_CLOSE_BTN, CInt(200 + i));
        else
            if(GetActiveQuest(who))
                GFTextLine(gump, 340, paddingTop + stepTop * i, 37, "Не доступно");
            else
                GFAddButton(gump, 390, paddingTop + stepTop * i, 2074, 2075, GF_CLOSE_BTN, CInt(100 + i));
            endif
        endif
//GFGumpPic(gump, 190, paddingTop + 3 + stepTop * i, 1469, 0);
        i:=i+1;
    endforeach

    var input := GFSendGump ( who, gump, 2);
    input := CInt(input[0]);

    case(input)
        101:
        102:
        103:
            QuestStart(who, input);

        201:
        202:
        203:
            QuestCancel(who);
            SendSysMessage(who, "Quest Cancelled.", 3, 37);
    endcase

endfunction


function QuestCancel(who)
    EraseObjProperty(who, "QuestMissionActions");
    EraseObjProperty(who, "QuestId");
    EraseObjProperty(who, "QuestAction");
    EraseObjProperty(who, "QuestMission");
    EraseObjProperty(who, "isTalk");
    EraseObjProperty(who, "Quest_TakeItems");
    EraseObjProperty(who, "QuestLastTalk");
    EraseObjProperty(who, "QuestActionKill");
    EraseObjProperty(who, "QuestCurrentActionKill");
    RemoveQuestArrow(who);

endfunction


function RemoveQuestArrow(who)
    var qr := GetObjProperty(who,"QuestArrow");
    if(qr)
        SendQuestArrow(who, -1, -1, qr[1]);
        EraseObjProperty(who, "QuestArrow");
    endif
endfunction


function QuestStart(who, input)
    var index := input - 100;
    SetObjProperty(who,"QuestId",index);
    SetMission(who,index, 1);
    SendSysMessage(who, "Quest started.");
endfunction


function GetActiveQuest(who)
    if(GetObjProperty(who,"QuestId"))
        return CInt(GetObjProperty(who,"QuestId"));
    endif
    return 0;
endfunction


function GetQuestList()
    var test:={};
    test.append({"Разговор по душам"});
    test.append({"Дыхание дракона"});
    return test;
endfunction


function GetMission(who, missionParams)
    var activeQuest := GetActiveQuest(who);
    if(activeQuest)

        var questMission := GetObjProperty(who,"QuestMission");
        var action := ReadConfigFile(":quest_"+activeQuest+":action");

        var	missionElem := FindConfigElem( action, questMission );
        var missionScript := GetConfigString( missionElem, "Script" );

        var script := start_script(":quests:"+missionScript, {who, missionParams, action, missionElem});
        if ( script.errortext )
            SysLog(script.errortext);
        else
            Print("Starting :quests:"+missionScript+" OK.");
        endif

    else
        SendSysMessage(who, "You don't have an active quest.", 3, 67);
    endif
endfunction


function SetMission(who, questId, missionIndex)
    SetObjProperty(who, "QuestMission", missionIndex);
    var mission := ReadConfigFile(":quest_"+questId+":action");
    var	missionElem := FindConfigElem( mission, missionIndex );
    var	mActionKills := GetConfigStringArray( missionElem, "ActionKill" );
    var missionScript := GetConfigString( missionElem, "Script" );

    AddQuestActions(who);
    var	mArrow := GetConfigString( missionElem, "Arrow" );
    var xy := struct;
    if(mArrow)
        var mArrowSplit := SplitWords(mArrow);
        xy.+x := mArrowSplit[1];
        xy.+y := mArrowSplit[2];
    //else
   //     var npcSerial := GetConfigString( missionElem, "TalkNpc" );
   //     var npc := SystemFindObjectBySerial(CInt(npcSerial));
   //     xy.+x := npc.x;
   //     xy.+y := npc.y;
    endif

    RemoveQuestArrow(who);
    if(xy.x && xy.y)
        AddQuestArrow(who, CInt(xy.x), CInt(xy.y));
    endif
endfunction


function QuestMissionAward(who, action, missionElem)
    var	awards := GetConfigStringArray( missionElem, "Award" );
    var missionsCount := GetConfigMaxIntKey(action);
    var questMission := CInt(GetObjProperty(who, "QuestMission"));
    var awardString := "";
    foreach item in (awards)
        var awardSplit := SplitWords(item,";");
        var it := CreateItemInBackpack(who, awardSplit[2],CInt(awardSplit[3]));
        awardString := it.name+" ("+CInt(awardSplit[3])+"), "+awardString;
    endforeach
    SendSysMessage(who, "Mission complete", 3, 67);
    SendSysMessage(who, awardString, 3, 67);

    if(missionsCount == questMission)
        QuestCancel(who);
        SendSysMessage(who, "Quest complete.", 3, 67);
    elseif(questMission <= missionsCount)
        SendSysMessage(who, "New mission.", 3, 67);
        SetMission(who, GetObjProperty(who,"QuestId"), questMission + 1);
    endif

endfunction

function QuestTalkNpc(who, npc)
    var activeQuest := GetActiveQuest(who);
    CloseGump(who, 1, 0);

    var mission := ReadConfigFile(":quest_"+activeQuest+":action");
    var	missionElem := FindConfigElem( mission, GetObjProperty(who, "QuestMission") );
    var	talknpccc := GetConfigString( missionElem, "talkNpc" );




    var	TalkNpc := GetObjProperty( who, "QuestLastTalk" );

    //print(TalkNpc);
    //print(CInt(talknpccc));
    //print(npc.serial);


    if(CInt(TalkNpc) == npc.serial)
        SendSysMessageUC(who, "Вы уже поговорили.", "RUS",3, 67);
        return;
    endif




    if(activeQuest)
        var	talkText := GetConfigString( missionElem, "talkText" );

        var gump := GFCreateGump ( 100, 100 );

        GFPage ( gump, 0);
        GFGumpPic(gump, 0, 0, 1249);
        GFHTMLArea(gump, 50, 40, 335, 205, talkText, 0, 1);
        GFAddButton(gump, 160, 260, 247, 248, GF_CLOSE_BTN, 1);

        var input := GFSendGump ( who, gump , 1);
        input := CInt(input[0]);
        return input;
    endif
return;
endfunction

function AddQuestArrow(who,x,y)
    if(GetObjProperty(who, "QuestArrow"))
        SendQuestArrow(who, -1, -1, 1);
    endif


    SetObjProperty(who, "QuestArrow",{1,x,y});
    SendQuestArrow(who, x, y, 1);
endfunction


//Збарать предметы из бекпака игрока
function QuestTakeItem(who)
    var takeItems := GetObjProperty(who, "Quest_TakeItems");
    if(takeItems)
        foreach item in (takeItems)

        endforeach
    endif
endfunction


function AddQuestActions(who)
    var activeQuest := GetActiveQuest(who);
    var list := {};
    var currentList := {};
    if(activeQuest)
        var mission := ReadConfigFile(":quest_"+activeQuest+":action");
        var	missionElem := FindConfigElem( mission, GetObjProperty(who,"QuestMission") );

        var	actionKills := GetConfigStringArray( missionElem, "ActionKill" );
        foreach kill in actionKills
            var killSplit := SplitWords(kill);
            list.append({killSplit[1],killSplit[2]});
            currentList.append({killSplit[1],0});
        endforeach

        if(list.Size())
            SetObjProperty(who, "QuestActionKill", list);
            SetObjProperty(who, "QuestCurrentActionKill", currentList);
        endif

    endif
endfunction







function QuestAction(who, script, params)
    var activeQuest := GetActiveQuest(who);
    if(activeQuest)
        var questMission := GetObjProperty(who,"QuestMission");
        var action := ReadConfigFile(":quest_"+activeQuest+":action");
        var	missionElem := FindConfigElem( action, questMission );

        var script_start := start_script(script, {who, params, action, missionElem});
        if ( script_start.errortext )
            SysLog(script_start.errortext);
        else
            Print("Starting "+script+" OK.");
        endif
    else
        SendSysMessage(who, "You don't have an active quest.", 3, 67);
    endif
endfunction





