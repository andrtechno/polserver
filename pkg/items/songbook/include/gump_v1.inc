use uo;
use os;
use unicode;

include ":gumps:gumps";
include "include/client";
include ":magery:spells";

var item_cfg := ReadConfigFile(":*:itemdesc");


function GumpBook( mobile, item)
    var spell_id := 0;
    var spells := GetObjProperty(item, "spells");
    spells.Sort();

	var s_gump := GFCreateGump(70, 70);
    GFGumpPic( s_gump, 0, 0, 2200);
	GFPage( s_gump, 0 );

    var right := 195;
    var left := 35;
    var right_padding := 9;
    var left_padding := 9;

    //GFTextLine(s_gump, 75, 10, 68, "Songs");
    //GFTextLine(s_gump, 240, 10, 68, "Songs");


    var i:=1;
    foreach obj in ( spells )
        ////if(i % 8) or if(i <= 8)

        var item_cfg2 := item_cfg[obj];
        spell_id := item_cfg2.SpellId;
        var spell_elem := MS_GetSpellsCfgElem(spell_id);

        if(i <= 10)
            GFTextLine(s_gump, left + 17, left_padding, 0, spell_elem.Name); //+" "+spell_id+""
            GFAddButton(s_gump, left, (left_padding + 5), 2103, 2104, GF_CLOSE_BTN, spell_id);
            left_padding += 19;
        else
            GFTextLine(s_gump, right + 17, right_padding, 0, spell_elem.Name);
            GFAddButton(s_gump, right, (right_padding + 5), 2103, 2104, GF_CLOSE_BTN, spell_id);
            right_padding += 19;
        endif;
        i += 1;
    endforeach

    var res := GFSendGump( mobile, s_gump );
    res := CInt( res[0] );
    if( res )
        var script := Start_Script(":magery:spellStarter", {mobile, res});
    endif
endfunction

