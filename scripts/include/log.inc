use file;
use uo;
use polsys;
use util;

include ":timeutils:time";

var dmy := FormatTime(GetTimeStruct(GetTime()), "<DAY>-<MON>-<YY>");
var hms := FormatTime(GetTimeStruct(GetTime()), "<HH>:<MM>:<SS>");
var month := FormatTime(GetTimeStruct(GetTime()), "<MON>-<YY>");

function LogStaff(who, text, targ, amount := 1, bcast := 0)



    var objText := "";
    if(targ.isA(POLCLASS_MOBILE))
        if(targ.isA(POLCLASS_NPC))
            objText := "[NPC: "+targ.npctemplate+"/"+targ.name+"]";
        else
            objText := "["+targ.acctname+"/"+targ.name+"]";
        endif

    elseif(targ.isA(POLCLASS_ITEM))
        objText := Hex(targ.objtype)+" - "+FormatItemDescription(targ.name, amount)+" ["+targ.serial+"]";
    endif

    LogToFile("::log/acc_"+who.acctname+"-"+month+".log", "["+who.name+"]: "+ text + ": "+ objText, LOG_DATETIME);

    if(bcast)
        foreach character in EnumerateOnlineCharacters()
            if( who.cmdlevel >= character.cmdlevel)
                SendSysMessage( character, "You receive a notification:", 3, 55 );
            endif
        endforeach
    endif
endfunction

function LogLine(who)
    LogToFile("::log/acc_"+who.acctname+".log", "===================", LOG_DATETIME);
endfunction

function LogStaff2(who, text, targ)

    var objText := "";
    if(targ.isA(POLCLASS_MOBILE))
        objText := "["+targ.acctname+"/"+targ.name+"]";
    elseif(targ.isA(POLCLASS_ITEM))
        objText := "["+targ.serial+"/"+targ.name+"]";
    endif

    LogToFile("::log/staff.log", "["+who.acctname+"/"+who.name+"]: "+ text + " > "+ objText, LOG_DATETIME);
endfunction

function LogDonate(who, am)
    LogToFile("::log/donate.log", who.client.acctname+"/"+who.name+" - "+am+" donate coins", LOG_DATETIME);
endfunction
