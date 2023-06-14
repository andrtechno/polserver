use datafile;
use unicode;

include "include/client";
function TT_GetDataFile()
    var datafile := OpenDataFile( ":tournamentTeam:settings" );
    if( !datafile )
        CreateDataFile( ":tournamentTeam:settings" , DF_KEYTYPE_STRING );
        datafile := OpenDataFile( ":tournamentTeam:settings"  );
    endif
    return datafile;
endfunction


function TT_GetDatafileElem(elem_name )

    var datafile := TT_GetDataFile();
    var data_elem := datafile.FindElement( CStr( elem_name ) );
    if( !data_elem )
        data_elem := datafile.CreateElement( CStr( elem_name ) );
    endif

    if( data_elem.errortext )
        SysLog( "Error::TT_GetDatafileElem() - Unable to find elem ["+elem_name+"] -> "+data_elem.errortext );
        return error{"errortext":="Error::TT_GetDatafileElem() - Unable to find elem ["+elem_name+"] -> "+data_elem.errortext};
    endif

    return data_elem;
endfunction


function TT_Stone(team)
    var data_elem := TT_Elem(team);
    var stone_opponent := data_elem.GetProp( "StoneOpponent" );
    return stone_opponent;
endfunction

function TT_Players(team)
    var data_elem := TT_Elem(team);
    var players := data_elem.GetProp( "Players" );
    return players;
endfunction

function TT_Elem(team)
    var data_elem := TT_GetDatafileElem("Settings"+team);
    return data_elem;
endfunction

function TT_ElemGlobal()
    var data_elem := TT_GetDatafileElem("Global");
    return data_elem;
endfunction

function TT_GetGlobalScore()
    var data_elem := TT_ElemGlobal();
    var max := data_elem.GetProp( "MaxScore" );
    if(!max)
        data_elem.SetProp( "MaxScore", 0);
    endif
    return max;
endfunction

function TT_SetGlobalScore(value)
    var data_elem := TT_ElemGlobal();
    data_elem.SetProp( "MaxScore", value);
endfunction

function TT_GetGlobalTimer()
    var data_elem := TT_ElemGlobal();
    var max := data_elem.GetProp( "Timer" );
    if(!max)
        data_elem.SetProp( "Timer", 60);
    endif
    return max;
endfunction

function TT_SetGlobalTimer(value)
    var data_elem := TT_ElemGlobal();
    data_elem.SetProp( "Timer", value * 60);
endfunction


function addPlayer(who, team)
    SendSysMessage(who, "add player", color := 88);
    var targ := Target(who);
    if(!targ)
        SendSysMessage(who, "Cancelled");
        return;
    endif

    var type_list := TT_Players(team);
    if( !type_list )
        type_list := array;
    endif
    if(targ.serial in type_list)
        SendSysMessageUC(who, "Игрок уже присуствует в этой команде.", "RUS", color := MESSAGE_COLOR_BAD);
    endif

    if(!(targ.serial in type_list))
        if(YesNo(targ, "Участвовать в турнире за "+team+" team? ", "Да", "Нет", 30))
            type_list.Append( targ.serial );
            SendSysMessageUC(targ, "Вы были добавлены в команду", "RUS", color := MESSAGE_COLOR_GOOD);
        endif
    endif

    var data_elem := TT_Elem(team);
    data_elem.SetProp( "Players", type_list );

endfunction

function removePlayer(who, team)
    var type_list := TT_Players(team);
    if( !type_list )
        type_list := array;
    endif

    var index := 1;
    foreach player in type_list
        if(player == who)
            type_list.erase( index );
            SendSysMessageUC(who, "Вы бели удалены из команды.", "RUS", color := MESSAGE_COLOR_BAD);
        endif
        index += 1;
    endforeach

    var data_elem := TT_Elem(team);
    data_elem.SetProp( "Players", type_list );

endfunction

function addStone(who, team)

    SendSysMessageUC(who, "Укажите стоун", "RUS", 3 ,33);
    var targ := Target(who);
    if(!targ)
        SendSysMessage(who, "Cancelled");
        return;
    endif

    var data_elem := TT_Elem(team);

    // Clear out old cache
    //foreach prop_name in ( data_elem.PropNames() )
    //	data_elem.EraseProp( prop_name );
    //endforeach

    var tt := 0;
    //var SOpponent := data_elem.GetProp( "StoneOpponent" );
    //if( !SOpponent )
    tt := targ.serial;
    //endif

    data_elem.SetProp( "StoneOpponent", tt );


endfunction

function ClearSettings(who, team)
    var data_elem := TT_Elem(team);
    // Clear out old cache
    foreach prop_name in ( data_elem.PropNames() )
        data_elem.EraseProp( prop_name );
    endforeach
    //data_elem.SetProp( "StoneOpponent", tt );
    data_elem.SetProp( "Score", 0 );
endfunction


function TT_SetScore(team)
    var data_elem := TT_Elem(team);
    var score := CInt(data_elem.GetProp( "Score" ));
    data_elem.SetProp( "Score", score + 1 );

    var _blue := TT_Elem("Blue");
    var _red := TT_Elem("Red");
    var scoreBlue := CInt(_blue.GetProp( "Score" ));
    var scoreRed := CInt(_red.GetProp( "Score" ));

    BroadcastUC("Счет BlueTeam ("+scoreBlue+" - "+scoreRed+") RedTeam", "RUS", 3, 45);

endfunction

function TT_SetScoreBlue(value)
    var data_elem := TT_Elem("Blue");
    data_elem.SetProp( "Score", value );
endfunction

function TT_SetScoreRed(value)
    var data_elem := TT_Elem("Red");
    data_elem.SetProp( "Score", value );
endfunction

function TT_GetScore(team)
    var data_elem := TT_Elem(team);
    return data_elem.GetProp( "Score");
endfunction


function isOnline(mobile);
    if(mobile.connected)
        return "Online";
    else
        return "Offline";
    endif
endfunction




function GFGumpListPlayers(who, byref gump, x, y, list, index);
    if(Len(list))
        var onlineColor;
        var adminText;
        GFTextLine(gump, x + 235, y - 30, MESSAGE_COLOR_BAD, "Del." );
        foreach player in list
            var obj := SystemFindObjectBySerial(CInt(player));

            adminText:="/"+obj.acctname+"";
            if((isOnline(obj) == "Online"))
                onlineColor := MESSAGE_COLOR_GOOD;
            else
                onlineColor := MESSAGE_COLOR_BAD;
            endif

            GFTextCrop(gump, x, y, 50, 20, onlineColor, "("+isOnline(obj)+")");
            GFTextLine(gump, x + 50, y, 2100, obj.name+""+adminText );


            if(who.cmdlevel > 0)
                GFAddButton(gump, x + 230, y, 4017, 4018, GF_CLOSE_BTN, CInt(player + index)); //remove player
                //GFTextLine(gump,  x + 200 + 20, y - 3, 55, "Удалить" );
            elseif(who.serial == obj.serial)
                GFAddButton(gump, x + 240, y, 2117, 2118, GF_CLOSE_BTN, CInt(player + index)); //resign meself
                GFTextLine(gump,  x + 240 + 20, y - 3, 55, "Покинуть" );
            endif
            y := y + 20;
        endforeach
    endif
endfunction