use uo;
use unicode;
use math;
use cliloc;

include ":gumps:gumps";
include ":gumps:gumps_ex";
include "include/client";
include ":gumps:yesNo";
include ":datafile:datafile";
include "include/string";
include "util/bank";

var npcdesc := ReadConfigFile(":*:npcdesc");
var data_file := DFOpenDataFile(":questline:members", DF_CREATE);

var pid := getprocess(GetGlobalProperty("#QuestLinePID"));


function QuestGumpSelect(who)
    var gump := GFCreateGump ( 100, 100 );
    GFPage ( gump, 0);
    GFResizePic(gump, 0, 0, GetCfgConst("Defaults", "ForeGround"), 190, 140);
    GFTextLine(gump, 15, 15, 2100, "Quest line" );

    var paddingTop:=20;
    var stepTop:=20;


    GFTextLine(gump, 15, paddingTop + stepTop, 67, "Настройки");
    GFAddButton(gump, 140, paddingTop + stepTop, 4023, 4024, GF_CLOSE_BTN, 1);
    stepTop += 25;
    GFTextLine(gump, 15, paddingTop + stepTop, 67, "Участники");
    GFAddButton(gump, 140, paddingTop + stepTop, 4023, 4024, GF_CLOSE_BTN, 2);
    stepTop += 25;
    GFTextLine(gump, 15, paddingTop + stepTop, 67, "Просмотр");
    GFAddButton(gump, 140, paddingTop + stepTop, 4023, 4024, GF_CLOSE_BTN, 3);

    var input := GFSendGump ( who, gump);
    input := CInt(input[0]);

    case(input)
        1: QuestGumpControl(who);
        2: QuestGumpMembers(who);
        3: QuestGumpPlayer(who);
        default: SendSysMessage(who, "Cancelled.", 3, MESSAGE_COLOR_BAD);
    endcase

endfunction


function QuestGumpPlayer(who)
    var config := GetGlobalProperty("QuestLineConfig");

    var start := CInt(GetGlobalProperty("questline_start"));
    var expire := CInt(config.expire);
    if(!CInt(GetGlobalProperty("QuestLineEnabled")))
        SendSysMessageUC(who, "Квест не еще не стартовал", "RUS", 3, MESSAGE_COLOR_BAD);
        return;
    endif

    var gump := GFCreateGump ( 50, 50 );

    var width := 450;
    var height := 600;
    GFResizePic(gump, 50, 50, 9270, width, height);


//GFGumpPic(gump, 180, 0, 10462);

GFGumpPic(gump, 0, 0, 10400);


GFGumpPic(gump, width+18, 0, 10441);
//GFGumpPic(gump, width+18, 200, 10411);
//GFGumpPic(gump, width+18, 400, 10412);

GFGumpPic(gump, 200, 15, 2440);
    GFTextLine(gump, 200+20, 15+2, 2100, "QUEST LINE" );


    if(!config.npcs)
        config.npcs := array;
    endif;


    var offsetLeft := 60;
    GFTextLine(gump, offsetLeft+15, 85, 55, "НПЦ");
    GFTextLine(gump, offsetLeft+200, 85, 55, "Убито");
    GFTextRight(gump, offsetLeft+width-100, 85, 55, "Награда");

    var dataMembers := GetObjProperty(who, "questline_npcs");

    var timerPadding := 150;
    var timerPaddingTop := 470;

    var mem := DFFindElement(data_file, who.serial);

    var endOf := start + expire * 60;
    var timeleft := (endOf - ReadGameClock()) / 60;


    var totalCost := CInt(mem.GetProp("total"));


    GFTextLine(gump, offsetLeft+15, height-50, MESSAGE_COLOR_BAD, "Ваша награда: "+totalCost+" gp.");


    if(timeleft >= 0)

        if(CInt(GetObjProperty(who,"QuestLinePID")) == CInt(GetGlobalProperty("#QuestLinePID")) && mem.GetProp("active"))
            GFTextLine(gump, offsetLeft+15+35, height, MESSAGE_COLOR_BAD, "Отменить квест");
            GFAddButton(gump, offsetLeft+15, height, 4005, 4006, GF_CLOSE_BTN, 101);
        else
            GFTextLine(gump, offsetLeft+15+35, height, MESSAGE_COLOR_GOOD, "Участвовать");
            GFAddButton(gump, offsetLeft+15, height, 4005, 4006, GF_CLOSE_BTN, 100);
        endif

        //timeleft := expire;
        GFTextLine(gump, offsetLeft+302, height-10, 2100, "Осталось "+timeleft+" мин.");
    endif

    GFTextLine(gump, offsetLeft+270, height+15, 2100, "Прохождение "+expire+" мин.");




    GFPage ( gump, 1);
    var paddingTop:=120;
    var stepTop:=40;
    var limit := 10;


    var num_options := config.npcs.Size();
    foreach questElem in (config.npcs)
        var npc2 := FindConfigElem(npcdesc, questElem.npc);
        GFResizePic(gump, offsetLeft+5, paddingTop, 3000, width-30, 35);
        GFTextLine(gump, offsetLeft+40, paddingTop-1, 500, npc2.Name);
        GFTextLine(gump, offsetLeft+40, paddingTop+12, 2100, questElem.npc);
        //GFGumpPic(gump, offsetLeft+5, paddingTop-3, 9721);

        GFTextLine(gump, offsetLeft+15, paddingTop+7, 0, _questElem_iter);
        var cur := 0;
        if(dataMembers[_questElem_iter])
            cur := dataMembers[_questElem_iter];
        endif

        if(dataMembers[_questElem_iter] >= CInt(questElem.amount))
            GFTextLine(gump, offsetLeft+195+8, paddingTop+7, MESSAGE_COLOR_GOOD, cur+"/"+questElem.amount);
        else
            GFTextLine(gump, offsetLeft+195+8, paddingTop+7, MESSAGE_COLOR_BAD, cur+"/"+questElem.amount);
        endif
        GFTextRight(gump, offsetLeft+width-75, paddingTop+7, 2100, AddCommas(questElem.cost));
        GFTilePic(gump, offsetLeft+width-75, paddingTop+3, 0x0eef, 0);

        paddingTop := paddingTop + stepTop;

        if( _questElem_iter % limit == 0 && _questElem_iter < num_options )
            GFAddButton( gump, width, height-80, 4007, 4006, GF_PAGE_BTN, gump.cur_page+1 );
            GFPage( gump, gump.cur_page+1 );
            GFAddButton( gump, width-30, height-80, 4014, 4015, GF_PAGE_BTN, gump.cur_page-1 );

            paddingTop := 120;
            stepTop := 40;
        endif
        if(num_options > limit)
            GFTextRight(gump, width-95, height-80, 2100, "страница: "+gump.cur_page+"/"+(ceil (num_options / limit)+1)+" ");
        endif
    endforeach


    var input := GFSendGump ( who, gump);
    input := CInt(input[0]);

    case(input)
        100:

            dataMembers := array;
            foreach questElem in (config.npcs)
                dataMembers.insert(_questElem_iter,0);
            endforeach

            SetObjProperty(who, "questline_npcs", dataMembers);
            SetObjProperty(who, "QuestLinePID", GetGlobalProperty("#QuestLinePID"));

            var mem := DFFindElement(data_file, who.serial, DF_CREATE);

            if(CInt(mem.GetProp("reward")))
                SendSysMessageUC(who, "Вы уже усчаствовали!!!", "RUS", 3, MESSAGE_COLOR_BAD);
                return 0;
            endif


            mem.SetProp("active", 1);
            mem.SetProp("total", 0);
            mem.SetProp("reward", 0);

            var member_name := who.name;
            if(GetObjProperty(who, "Incognito"))
                member_name := GetObjProperty(who, "Incognito").name;
            endif
            BroadcastUC( member_name+" стал участником Quest line", "RUS", 3, MESSAGE_COLOR_GOOD );

            //SendSysMessageUC(who, "Вы стали участником Quest Line", "RUS", 3, MESSAGE_COLOR_GOOD);

        101:
            if(YesNo(who, "Вы уверены что хотите покинуть Quest Line? Вы не получите свою награду.","Покинуть","Отменить"))
                EraseObjProperty(who, "questline_npcs");

                SendSysMessageUC(who, "Вы покинули Quest Line", "RUS", 3, MESSAGE_COLOR_BAD);


                var member_name := who.name;
                if(GetObjProperty(who, "Incognito"))
                    member_name := GetObjProperty(who, "Incognito").name;
                endif

                var mem := DFFindElement(data_file, who.serial);
                mem.SetProp("active", 0);
                var total := CInt(mem.GetProp("total"));
                var sum := total;
                BroadcastUC( member_name+" покинул Quest line", "RUS", 3, 33 );
                if(total > 0)
                    var bankbox := FindBankBox( who );
                    while(sum >= 60000)
                        sum := sum - 60000;
                        CreateItemInContainer(bankbox, 0xeed, 60000);
                    endwhile
                    CreateItemInContainer(bankbox, 0xeed, sum);
                    EraseObjProperty(who, "QuestLinePID");
                    SendSysMessageUC(who, "Вам начислена награда "+AddCommas(total)+" gp.", "RUS", 3, 55);
                endif
                //mem.SetProp("total", 0);
                mem.SetProp("reward", 1);

            else
                SendSysMessageCL(who, 1042023, color := 33); // Cancelled.
            endif

    endcase

endfunction




function QuestGumpControl(who)
    var gump := GFCreateGump ( 50, 50 );

    var width := 500;
    var height := 600;
    GFResizePic(gump, 0, 0, GetCfgConst("Defaults", "ForeGround"), width, height);
    GFTextLine(gump, 15, 15, 3, "Настройки Quest line" );


    //var dataQuest := GetGlobalProperty("questline");
    //if(!dataQuest)
    //    dataQuest := array;
    //endif;

    var config := GetGlobalProperty("QuestLineConfig");
    if(!config.npcs)
        config.npcs := array;
    endif;

    GFTextLine(gump, width-160, 15, 67, "Добавить НПЦ");
    GFAddButton(gump, width-50, 15, 4005, 4006, GF_CLOSE_BTN, 400);

    var enabled := CInt(GetGlobalProperty("QuestLineEnabled"));



    GFTextLine(gump, 15, 55, 55, "НПЦ");
    GFTextLine(gump, 200, 55, 55, "Кол.");
    GFTextLine(gump, width-160, 55, 55, "Награда");

    if(Len(config.npcs))

        GFAddButton(gump, 15, height-90, 4005, 4006, GF_CLOSE_BTN, 2000);
        GFTextLine(gump, 15+35, height-90, MESSAGE_COLOR_BAD, "Удалить все");

        var timerPadding := 150;
        var timerPaddingTop := 470;



        var timer := CInt(config.timer);
        var expire := CInt(config.expire);
        var start := CInt(GetGlobalProperty("questline_start"));
        if(!enabled)
                GFGumpPic(gump, timerPadding, height-30, 2443);
                GFTextEntry(gump, timerPadding+10, height-30+1, 135, 20, 2100, timer, 1001);
                GFTextRight(gump, timerPadding-50, height-30, 2100, "через");
                GFTextLine(gump, timerPadding+68, height-30, 2100, "мни.");
                GFTextLine(gump, 15+35, height-30, 67, "Старт");
                GFAddButton(gump, 15, height-30, 4005, 4006, GF_CLOSE_BTN, 1000);



                GFTextLine(gump, 330, height-55, 2100, "Прохождение");
                GFGumpPic(gump, 330, height-30, 2443);
                GFTextEntry(gump, 330+10, height-30+1, 135, 20, 2100, expire, 1002);
                GFTextLine(gump, 400, height-30, 2100, "мин");
        else
            GFAddButton(gump, 15, height-30, 4005, 4006, GF_CLOSE_BTN, 1000);
            GFTextLine(gump, 15+35, height-30, 67, "Квест запущен! (отменить?)");
        endif
    endif
    GFTextLine(gump, 15+35, height-60, 10, "Сохранить изменения");
    GFAddButton(gump, 15, height-60, 4005, 4006, GF_CLOSE_BTN, 500);


    GFPage ( gump, 1);
    var paddingTop:=80;
    var stepTop:=40;
    var limit := 10;
    var num_options := config.npcs.Size();
    foreach questElem in (config.npcs)
        var npc2 := FindConfigElem(npcdesc, questElem.npc);
        GFResizePic(gump, 15, paddingTop, 3000, width-30, 35);
        GFTextLine(gump, 15+5, paddingTop+7, 0, _questElem_iter);

        GFTextLine(gump, 15+25, paddingTop - 1, 500, npc2.Name);
        GFTextLine(gump, 15+25, paddingTop + 12, 2100, questElem.npc);

        GFGumpPic(gump, 185, paddingTop+6, 2443);
        GFTextEntry(gump, 185+8, paddingTop + 7, 135, 20, 2100, questElem.amount, 200+_questElem_iter);


        GFGumpPic(gump, width-160, paddingTop+6, 2443);
        GFTextEntry(gump, width-160+8, paddingTop + 7, 135, 20, 2100, questElem.cost, 100+_questElem_iter);
        GFTextLine(gump, width-160+60+5, paddingTop + 7, 2100, "gp.");

        GFAddButton(gump, width-55, paddingTop+6, 4017, 4018, GF_CLOSE_BTN, 600+_questElem_iter); //remove



        paddingTop := paddingTop + stepTop;


        if( _questElem_iter % limit == 0 && _questElem_iter < num_options )
            GFAddButton( gump, width-55, height-120, 4007, 4006, GF_PAGE_BTN, gump.cur_page+1 );
            GFPage( gump, gump.cur_page+1 );
            GFAddButton( gump, width-70, height-120, 4014, 4015, GF_PAGE_BTN, gump.cur_page-1 );

            paddingTop := 80;
            stepTop := 40;
        endif
        if(num_options > limit)
            GFTextRight(gump, width-135, height-120, 2100, "страница: "+gump.cur_page+"/"+(ceil (num_options / limit)+1)+" ");
        endif
    endforeach

    var input := GFSendGump ( who, gump);

    var newQuestData := array;
    foreach key in ( input.keys )
        if(key > 100 && key < 200)
            var mob := struct;
            mob.npc := config.npcs[key-100].npc;
            mob.cost := CInt(GFExtractData(input, key));
            mob.amount := CInt(GFExtractData(input, key+100));

            if(mob.cost > 60000)
                SendSysMessageUC(who, "Максимальная награда для '"+config.npcs[key-100].npc+"' должна быть не больше 60000", "RUS", 3, MESSAGE_COLOR_BAD);
                return QuestGumpControl(who);
            endif;

            newQuestData.append(mob);
        endif
        if(key > 600 && key < 700) //remove npc
//print(config);
//var config := GetGlobalProperty("QuestLineConfig");
            SendSysMessageUC(who, "НПЦ "+config.npcs[key-600].npc+" удалено", "RUS", 3, MESSAGE_COLOR_GOOD);

            config.npcs.Erase(key-600);
            SetGlobalProperty("QuestLineConfig", config);

            return QuestGumpControl(who);
        endif
        if(key == 500) //save chnages
            var timer := CInt(GFExtractData(input, 1001));
            var expire := CInt(GFExtractData(input, 1002));

            var config := struct;
            if(expire)
                config.expire := expire;
            endif
            if(timer)
                config.timer := timer;
            endif
            config.npcs := newQuestData;
            SetGlobalProperty("QuestLineConfig", config);


            SendSysMessageUC(who, "Изменения сохранены.", "RUS", 3, MESSAGE_COLOR_GOOD);
            return QuestGumpControl(who);

        elseif(key == 1000) // start/cancel quest
            var time := CInt(GFExtractData(input, 1001));
            var expire := CInt(GFExtractData(input, 1002));
            var start := ReadGameClock() + time * 60;

            var starter := Cint(GetGlobalProperty("questline_start"));
            var enabled := CInt(GetGlobalProperty("QuestLineEnabled"));

            var pid_starter := GetProcess(GetGlobalProperty("#QuestLineStaterPID"));
            if(!pid_starter)
                SetGlobalProperty("QuestLineEnabled", 1);
                var config := struct;
                config.expire := expire;
                config.timer := time;
                config.npcs := newQuestData;
                SetGlobalProperty("QuestLineConfig", config);

                var starter := start_script(":questline:starter", {time, expire});
                SetGlobalProperty( "#QuestLineStarterPID", starter.pid );
            else
                EraseGlobalProperty("QuestLineEnabled");
                pid_starter.kill();
            endif
//if(!pid)
//    var ended:= start_script(":questline:ended");
//    SetGlobalProperty( "#QuestLinePID", ended.pid );
//else
//    pid.kill();
//endif
            /*if(enabled)
                EraseGlobalProperty("QuestLineEnabled");
                EraseGlobalProperty("questline_start");
                //SetGlobalProperty("#QuestLinePID", getpid());


                if(pid_starter)
                    pid_starter.kill();
                endif
                if(pid)
                    pid.kill();
                endif
                //if(!pid)

                //var ended:= start_script(":questline:ended");
                //SetGlobalProperty( "#QuestLinePID", ended.pid );
                //endif
            else
                SetGlobalProperty("QuestLineEnabled", 1);
                if(expire <= 1)
                    SendSysMessageUC(who, "Время прохождения слишком маленькое.", "RUS", 3, MESSAGE_COLOR_BAD);
                    return QuestGumpControl(who);
                endif
                //SetGlobalProperty("questline_expire", expire);
                //SetGlobalProperty("questline_timer", time);


                var config := struct;
                config.expire := expire;
                config.timer := time;
                SetGlobalProperty("QuestLineConfig", config);

                //SetGlobalProperty("questline_start", start);
                if(!pid_starter)
                    var starter := start_script(":questline:starter", {time, expire});
                    SetGlobalProperty( "#QuestLineStarterPID", starter.pid );
                endif


            endif*/


            return QuestGumpControl(who);

        elseif(key == 2000) //remove all
            EraseGlobalProperty("questline_start");
            EraseGlobalProperty("QuestLineConfig");
            SendSysMessageUC(who, "Все удалено.", "RUS", 3, MESSAGE_COLOR_GOOD);
            return QuestGumpControl(who);

        elseif(key == 400) //add npc
            var config := GetGlobalProperty("QuestLineConfig");
            if(!config.npcs)
                config.npcs := array;
            endif
            var gump_npc := SendTextEntryGump(who, "Entry npc:");
            if(!gump_npc)
                QuestGumpControl(who);
            endif
            var npc := FindConfigElem(npcdesc, gump_npc);
            if(npc)
                if(npc.Privs != "invul")
                    foreach fi in (config.npcs)
                        if(fi.npc == gump_npc)
                            SendSysMessageUC(who, fi.npc+" уже есть в списке.", "RUS", 3, MESSAGE_COLOR_BAD);
                            return QuestGumpControl(who);
                        endif
                    endforeach
                    var mob := struct;
                    mob.npc := gump_npc;
                    mob.amount := 1;
                    mob.cost := 1;
                    config.npcs.append(mob);
                    SetGlobalProperty("QuestLineConfig", config);

                    SendSysMessageUC(who, "НПЦ "+gump_npc+" добавлено", "RUS", 3, MESSAGE_COLOR_GOOD);
                    return QuestGumpControl(who);
                else
                    SendSysMessageUC(who, "НПЦ "+gump_npc+" запрещен.", "RUS", 3, MESSAGE_COLOR_BAD);
                    return QuestGumpControl(who);
                endif
            else
                SendSysMessageUC(who, "НПЦ "+gump_npc+" не найдено.", "RUS", 3, MESSAGE_COLOR_BAD);
                return QuestGumpControl(who);
            endif
        endif
    endforeach

endfunction



function QuestGumpMembers(who)
    var gump := GFCreateGump ( 50, 50 );

    var width := 500;
    var height := 600;
    GFResizePic(gump, 50, 50, 9270, width, height);
    GFGumpPic(gump, 0, 0, 10400);
    GFGumpPic(gump, width+18, 0, 10441);
    GFGumpPic(gump, 200, 15, 2440);
    GFTextLine(gump, 200+20, 15+2, 2100, "QUEST LINE" );


    var offsetLeft := 60;
    GFTextLine(gump, offsetLeft+15, 85, 55, "Участник");
    GFTextLine(gump, offsetLeft+200, 85, 55, "Акт.");
    GFTextLine(gump, offsetLeft+270, 85, 55, "Награжден");
    GFTextRight(gump, offsetLeft+width-80, 85, 55, "Сумма");


    var timerPadding := 150;
    var timerPaddingTop := 470;



    var mem := DFFindElement(data_file, who.serial);

    GFPage ( gump, 1);
    var paddingTop:=120;
    var stepTop:=40;
    var limit := 10;

    var num_options := DFGetElemNames(data_file).Size();

    if(!num_options)
        GFTextLine(gump, offsetLeft+15, paddingTop+6, 2100, "Список участников пуст.");
    endif
	foreach char in ( DFGetElemNames(data_file) )
	    var mem := DFFindElement(data_file, char);
        var mobile := SystemFindObjectBySerial(CInt(char), SYSFIND_SEARCH_OFFLINE_MOBILES);
        var member_name := mobile.name;
        var active := CInt(mem.GetProp("active"));
        var reward := CInt(mem.GetProp("reward"));

        if(GetObjProperty(mobile, "Incognito"))
            member_name := GetObjProperty(mobile, "Incognito").name;
        endif

        GFResizePic(gump, offsetLeft+5, paddingTop, 3000, width-30, 35);
        GFTextLine(gump, offsetLeft+15, paddingTop+7, 0, _char_iter);
        GFTextLine(gump, offsetLeft+35, paddingTop+7, 500, member_name +" ["+mobile.acctname+"]");

        if(active)
            GFTextLine(gump, offsetLeft+200, paddingTop+7, MESSAGE_COLOR_GOOD, "Да");
        else
            GFTextLine(gump, offsetLeft+200, paddingTop+7, MESSAGE_COLOR_BAD, "Нет");
        endif


        if(reward)
            GFTextLine(gump, offsetLeft+270, paddingTop+7, MESSAGE_COLOR_GOOD, "Да");
        else
            GFTextLine(gump, offsetLeft+270, paddingTop+7, MESSAGE_COLOR_BAD, "Нет");
        endif


        GFTextRight(gump, offsetLeft+width-75, paddingTop+7, 2100, AddCommas(CInt(mem.GetProp("total"))));
        GFTilePic(gump, offsetLeft+width-75, paddingTop+3, 0x0eef, 0);
        paddingTop := paddingTop + stepTop;
        if( _char_iter % limit == 0 && _char_iter < num_options )
            GFAddButton( gump, width, height-80, 4007, 4006, GF_PAGE_BTN, gump.cur_page+1 );
            GFPage( gump, gump.cur_page+1 );
            GFAddButton( gump, width-30, height-80, 4014, 4015, GF_PAGE_BTN, gump.cur_page-1 );
            paddingTop := 120;
            stepTop := 40;
        endif
		SleepMS(2);
	endforeach



var input := GFSendGump ( who, gump);
endfunction
