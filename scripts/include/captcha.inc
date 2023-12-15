
include ":gumps:gumps";
include ":gumps:gumps_ex";

function captcha(who)
    var valueEntry := RandomDiceRoll("10d10+99");

    var gump := CInt(SendTextEntryGump(who, "Entry code: "+valueEntry+"", 1,1, 3,"code:"));
    if(gump != valueEntry)
        SendSysMessageUC(who, "Неверный код.", "RUS",3,33);
        return 0;
    endif;

    return 1;

endfunction

function captcha_v2Demo(who)
    var url := "https://placehold.co/600x400.png";
    var gump := GFCreateGump ( 50, 50 );
    GFPage ( gump, 0);


    GFResizePic(gump, 0, 0, GetCfgConst("Defaults", "ForeGround"), 300, 300);
    GFTextLine(gump, 15, 15, 2100, "Captcha v2" );

    GFHTMLArea(gump, 15, 15, 50, 50, "<img src=\""+url+"\" width=\"100\" height=\"100\" x=\"5\" y=\"5\">" );

    var input := GFSendGump ( who, gump);

    return 1;

endfunction
