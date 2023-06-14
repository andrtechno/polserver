

function captcha(who)
    var valueEntry:=RandomDiceRoll("10d10+99");

    var gump := CInt(SendTextEntryGump(who, "Entry code: "+valueEntry+"", 1,1, 3,"code:"));
    if(gump != valueEntry)
        SendSysMessageUC(who, "Неверный код.", "RUS",3,33);
        return 0;
    endif;

    return 1;

endfunction