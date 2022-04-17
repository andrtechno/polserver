//use uo;
//use os;

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
        GFTextLine(gump, 15, paddingTop + stepTop * i, 500, questItem[2]);
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




//2031 help buttn




    var input := GFSendGump ( who, gump );
    input := CInt(input[0]);
    print(input);


    case(input)
        101: QuestStart(who, input);
        102: QuestStart(who, input);
        103: QuestStart(who, input);

        201: QuestCancel(who);
        202: QuestCancel(who);
        203: QuestCancel(who);
    endcase

endfunction


function QuestCancel(who)
    EraseObjProperty(who, "QuestMissionActions");
    EraseObjProperty(who, "QuestId");
    EraseObjProperty(who, "QuestAction");
    EraseObjProperty(who, "QuestMission");
    EraseObjProperty(who, "QuestStatus");
    SendSysMessage(who, "Quest Cancelled.");
endfunction

function QuestStart(who, input)
    var index := input - 100;

    SetObjProperty(who,"QuestId",index);
    SetObjProperty(who,"QuestStatus","active");

    SetMission(who,index, 0);
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
    test.append({"qid1","Убей драконов","action"});
    test.append({"qid2","Дыхание дракона","action"});
    //test.append({"qid3","Убей драконов3","action"});
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
    endif
endfunction


function SetMission(who, questId, missionIndex)
    SetObjProperty(who, "QuestMission", missionIndex);
    var mission := ReadConfigFile(":quest_"+questId+":action");
    var	missionElem := FindConfigElem( mission, missionIndex );
    var	mActions := GetConfigStringArray( missionElem, "action" );
    var list := {};
    if(mActions)
        foreach a in (mActions)
            var split := SplitWords(a);
            list.append(array{split[1],split[2]});
        endforeach
        SetObjProperty(who, "QuestMissionActions",list);
    endif
endfunction


function QuestMissionAward(who, action, missionElem)
    var	awards := GetConfigStringArray( missionElem, "Award" );
var missionsCount := GetConfigMaxIntKey(action);
var questMission := CInt(GetObjProperty(who, "QuestMission"));
var awardString := "";
    foreach item in (awards)
        var awardSplit := SplitWords(item);
        var it := CreateItemInBackpack(who, awardSplit[2],CInt(awardSplit[3]));
        awardString := it.name+" ("+CInt(awardSplit[3])+"), "+awardString;
    endforeach
    SendSysMessage(who, "Mission complete", 3, 67);
    SendSysMessage(who, awardString, 3, 67);

    if(questMission <= missionsCount)
        SetMission(who, GetObjProperty(who,"QuestId"), questMission + 1);
    elseif(missionsCount == CInt(GetObjProperty(who, "QuestMission")))

    //complate quest!!!
    endif



endfunction